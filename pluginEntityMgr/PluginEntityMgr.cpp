/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2008 The Boeing Company
 *  
 *  This component is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; specifically 
 *  version 2.1 of the License.
 *  
 *  This software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *  
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this software; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *  
 *  
 *  FILENAME:   PluginEntityMgr.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  maintain state for CIGI entities.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  04/04/2004 Greg Basler
 *      Initial Release.
 *
 *  2007-07-01 Andrew Sampson
 *      Ported to new plugin API
 *
 *  2008-07-07 Andrew Sampson
 *      Rewrote entity manager.  Mostly based on symbology mgr plugin.
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#include <iostream>

#include <CigiArtPartCtrlV3.h>
#include <CigiShortArtPartCtrlV3.h>
#include <CigiCompCtrlV3_3.h>
#include <CigiShortCompCtrlV3_3.h>

#include "AllCigi.h"

#include "PluginEntityMgr.h"

#ifdef WIN32
#define isnan _isnan
#endif


EXPORT_DYNAMIC_CLASS( PluginEntityMgr )

// ================================================
// PluginEntityMgr
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginEntityMgr::PluginEntityMgr() : Plugin(),
	allEntities( new EntityContainer ),
	topLevelEntities( new EntityContainer ),
	conversionObserver( new EntityCoordinateConversionObserver ),
	animStopNotificationListener( new AnimStopNotificationListener( allEntities.get() ) ),
	entityCtrlProc( this ),
	artPartProc( allEntities.get() ),
	shortArtPartProc( allEntities.get() ),
	compCtrlProc( allEntities.get() ),
	shortCompCtrlProc( allEntities.get() )
{
	name_ = "PluginEntityMgr";

	licenseInfo_.setLicense( LicenseInfo::LicenseLGPL );
	licenseInfo_.setOrigin( "Boeing+AndrewSampson" );

	dependencies_.push_back( "PluginDefFileReader" );
	dependencies_.push_back( "PluginCoordinateConversionMgr" );

	ImsgPtr = NULL;
	OmsgPtr = NULL;
	DefFileData = NULL;
	timeElapsedLastFrame = NULL;
	coordinateConverter = NULL;
	
	// coordinate conversion only needs to be performed on the top-level ents
	conversionObserver->startObservingContainer( topLevelEntities.get() );
}


// ================================================
// ~PluginEntityMgr
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginEntityMgr::~PluginEntityMgr() throw()
{
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginEntityMgr::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{
	case SystemState::BlackboardPost:
		// This state is for posting things to the blackboard and other 
		// initialization tasks
		
		// post the entities
		bb_->put( "AllEntities", allEntities.get() );
		bb_->put( "TopLevelEntities", topLevelEntities.get() );
		
		break;

	case SystemState::BlackboardRetrieve:
		// This state is for retrieving things from the blackboard

		bb_->get( "CigiIncomingMsg", ImsgPtr );
		bb_->get( "CigiOutgoingMsg", OmsgPtr );
		bb_->get( "DefinitionData", DefFileData );
		bb_->get( "TimeElapsedLastFrame", timeElapsedLastFrame );
		bb_->get( "CoordinateConverterProxy", coordinateConverter );

		if( coordinateConverter != NULL )
		{
			conversionObserver->setCoordinateConverter( coordinateConverter );
		}
		
		if( ImsgPtr != NULL )
		{
			ImsgPtr->RegisterEventProcessor( CIGI_ENTITY_CTRL_PACKET_ID_V3, &entityCtrlProc );
			ImsgPtr->RegisterEventProcessor( CIGI_ART_PART_CTRL_PACKET_ID_V3, &artPartProc );
			ImsgPtr->RegisterEventProcessor( CIGI_SHORT_ART_PART_CTRL_PACKET_ID_V3, &shortArtPartProc );
			ImsgPtr->RegisterEventProcessor( CIGI_COMP_CTRL_PACKET_ID_V3_3, &compCtrlProc );
			ImsgPtr->RegisterEventProcessor( CIGI_SHORT_COMP_CTRL_PACKET_ID_V3_3, &shortCompCtrlProc );
		}
		
		animStopNotificationListener->setOutgoingMessageBuffer( OmsgPtr );
		break;

	case SystemState::ConfigurationProcess:
		processConfigData();
		break;

	case SystemState::Operate:
	case SystemState::Debug:
		operate();
		break;

	case SystemState::Reset:
	case SystemState::Shutdown:
		allEntities->flagAllEntitiesAsDestroyed();
		break;

	default:
		break;
	}
	
}


// ================================================
// operate
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginEntityMgr::operate()
{

	EntityContainer::EntityIteratorPair iterators = allEntities->getEntities();
	for( ; iterators.first != iterators.second; iterators.first++ )
	{
		Entity *entity = (*iterators.first).second.get();

		// skip over entities that do not exist
		if( entity == NULL )
			continue;
		
		
		processEntity( entity );
		
	}
}


// ================================================
// processConfigData
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginEntityMgr::processConfigData()
{
	DefFileGroup *root = *DefFileData;

	if(root == NULL)
	{
		return;
	}

	factory.init( root );
}


// ================================================
// processEntity
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginEntityMgr::processEntity( Entity *entity )
{
	entity->update( *timeElapsedLastFrame );
}


// ================================================
// createEntity
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Entity *PluginEntityMgr::createEntity( int id, int type )
{
	// Sanity checking - someone asked to create an entity that already 
	// exists...
	Entity *result = allEntities->findEntity( id );
	if( result == NULL )
	{
		result = factory.createEntity( id, type );
		allEntities->addEntity( result );
	}

	return result;
}


// ================================================
// reparentEntity
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
bool PluginEntityMgr::reparentEntity( int entityID, bool isChild, int newParentID )
{
	// find the entity, its old parent (if applicable), and its new parent 
	// (if applicable)
	
	Entity *entity = allEntities->findEntity( entityID );
	if( entity == NULL )
	{
		// error - entity not found
		return false;
	}
	
	// detach the entity from its old parent
	if( entity->getIsChild() )
	{
		// get entity's old parent
		Entity *oldParent = entity->getParent();
		if( oldParent == NULL )
		{
			// error - inconsistent entity state
		}
		else
		{
			// remove the entity from oldParent
			oldParent->removeEntity( entity );
		}
	}
	// remove the entity from the list of top-level entities
	else
	{
		// get entity's entry in the top-level entities container
		Entity *topLevel = topLevelEntities->findEntity( entityID );
		if( topLevel == NULL || topLevel != entity )
		{
			// error - inconsistent entity state
		}
		else
		{
			// remove the entity from the list of top-level entities
			topLevelEntities->removeEntity( entity );
		}
	}
	
	// if entity is now a child, attach it to its new parent
	if( isChild )
	{
		// find the new parent
		Entity *newParent = allEntities->findEntity( newParentID );
		if( newParent == NULL )
		{
			// error - new parent not found
			return false;
		}
		
		entity->setParent( isChild, newParentID, newParent );
		newParent->addEntity( entity );
	}
	// if the entity is now top-level, clear its parent and add it to 
	// the top-level container
	else
	{
		entity->setParent( isChild, 0xffff, NULL );
		topLevelEntities->addEntity( entity );
	}
	return true;
}


