/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2004 The Boeing Company
 *  
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *  
 *  
 *  FILENAME:   PluginViewMgr.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  listen for ViewControl and ViewDefinition packets and make their 
 *  information available to other plugins via the blackboard.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  04/06/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 *
 *  2007-07-21 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#include <iostream>
#include <string.h> // for memset

#include "PluginViewMgr.h"
#include "MPVExceptions.h"

using namespace mpv;

EXPORT_DYNAMIC_CLASS( PluginViewMgr )

// ================================================
// PluginViewMgr
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginViewMgr::PluginViewMgr() : Plugin() 
{
	name_ = "PluginViewMgr";
	licenseInfo_.setLicense( LicenseInfo::LicenseGPL );
	licenseInfo_.setOrigin( "Boeing" );
	dependencies_.push_back( "PluginDefFileReader" );
	dependencies_.push_back( "PluginEntityMgr" );

	ImsgPtr = NULL;
	allEntities = NULL;
	
	standbyView = new View();
	databaseLoadingView = new View();
	
	initAllViewParams();
}


// ================================================
// ~PluginViewMgr() throw() 
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginViewMgr::~PluginViewMgr() throw() 
{
	// Note - contents of viewMap will be deleted when the map passes out of 
	// scope, and the references it contains are released.

	// delete contents of viewGroupParamsMap
	std::map< int, ViewGroupParams * >::iterator vgpmIter;
	for( vgpmIter = viewGroupParamsMap.begin(); 
		vgpmIter != viewGroupParamsMap.end(); vgpmIter++ )
	{
		delete (*vgpmIter).second;
	}
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginViewMgr::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{

	case SystemState::BlackboardPost:
		// This state is for posting things to the blackboard and other 
		// initialization tasks
		
		bb_->put( "ViewMap", &viewMap );
		bb_->put( "ViewGroupParamsMap", &viewGroupParamsMap );
		bb_->put( "StandbyView", standbyView.get() );
		bb_->put( "DatabaseLoadingView", databaseLoadingView.get() );

		break;

	case SystemState::BlackboardRetrieve:
		// This state is for retrieving things from the blackboard

		bb_->get( "DefinitionData", DefFileData );

		bb_->get( "AllEntities", allEntities );

		bb_->get( "CigiIncomingMsg", ImsgPtr );

		if( ImsgPtr != NULL ) {

			ImsgPtr->RegisterEventProcessor(
				CIGI_VIEW_CTRL_PACKET_ID_V3,
				(CigiBaseEventProcessor *) &viewCtrlP);
			ImsgPtr->RegisterEventProcessor(
				CIGI_VIEW_DEF_PACKET_ID_V3,
				(CigiBaseEventProcessor *) &viewDefP);
			
			viewCtrlP.init( &viewMap, &viewGroupParamsMap );
			viewDefP.init( &viewMap, &viewGroupParamsMap );

		}
		break;

	case SystemState::ConfigurationProcess:
		initAllViewParams();
		getConfigData();
		break;

	case SystemState::Operate:
	case SystemState::Debug:
		{
		ViewMap::iterator viewIter;
		for( viewIter = viewMap.begin(); 
			viewIter != viewMap.end(); viewIter++ )
		{
			sanityCheckView( viewIter->second.get() );
		}
		}
		break;

	default:
		break;
	}
	
}


// ================================================
// getConfigData
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginViewMgr::getConfigData( void )
{
	DefFileGroup *root = *DefFileData;

	if(root == NULL)
	{
		initAllViewParams();
		return;
	}

	for( std::list< DefFileGroup * >::iterator iter = root->children.begin();
		iter != root->children.end(); iter++ )
	{
		DefFileGroup *group = (*iter);
		if( group->getName() == "view" ) 
		{
			RefPtr<View> view = NULL;
			int id;

			DefFileAttrib *attr = group->getAttribute( "id" );
			if( attr == NULL )
				continue;
			else
				id = attr->asInt();
			
			view = viewMap[id];
			if( !view.valid() )
			{
				// we need to create a new ViewParams and add it to the map
				view = new View;
				view->setID( id );
				viewMap[id] = view;
			}
			
			attr = group->getAttribute( "group" );
			if( attr != NULL )
			{
				view->setGroupID( attr->asInt() );
			}
			
			attr = group->getAttribute( "type" );
			if( attr != NULL )
			{
				view->setType( attr->asInt() );
			}
			
			attr = group->getAttribute( "entity_id" );
			if( attr != NULL )
			{
				view->setEntityID( attr->asInt() );
			}
			
			attr = group->getAttribute( "parallel_projection" );
			if( attr != NULL )
			{
				view->setParallelProjection( attr->asInt() );
			}
			
			attr = group->getAttribute( "mirror" );
			if( attr != NULL )
			{
				view->setMirrorMode( attr->asInt() );
			}
			
			// FOV ///////////////////////////////////////////////////
			float fovLeft = view->getFovLeft();
			float fovRight = view->getFovRight();
			float fovTop = view->getFovTop();
			float fovBottom = view->getFovBottom();
			attr = group->getAttribute( "fov_left" );
			if( attr != NULL )
				fovLeft = attr->asFloat();
			attr = group->getAttribute( "fov_right" );
			if( attr != NULL )
				fovRight = attr->asFloat();
			attr = group->getAttribute( "fov_top" );
			if( attr != NULL )
				fovTop = attr->asFloat();
			attr = group->getAttribute( "fov_bottom" );
			if( attr != NULL )
				fovBottom = attr->asFloat();
			view->setFov( fovLeft, fovRight, fovTop, fovBottom );
			
			// Near/Far Planes ///////////////////////////////////////
			attr = group->getAttribute( "skip_minimum_nearplane_check" );
			if( attr != NULL )
				view->setSkipMinimumNearPlaneCheck( attr->asInt() );
			float near = view->getNearPlane();
			float far = view->getFarPlane();
			attr = group->getAttribute( "near_plane" );
			if( attr != NULL )
				near = attr->asFloat();
			attr = group->getAttribute( "far_plane" );
			if( attr != NULL )
				far = attr->asFloat();
			view->setNearFarPlanes( near, far );
			
			// Viewport Dimensions ///////////////////////////////////
			float viewportLeft = view->getViewportLeft();
			float viewportTop = view->getViewportTop();
			float viewportWidth = view->getViewportWidth();
			float viewportHeight = view->getViewportHeight();
			attr = group->getAttribute( "viewport_left" );
			if( attr != NULL )
				viewportLeft = attr->asFloat();
			attr = group->getAttribute( "viewport_top" );
			if( attr != NULL )
				viewportTop = attr->asFloat();
			attr = group->getAttribute( "viewport_width" );
			if( attr != NULL )
				viewportWidth = attr->asFloat();
			attr = group->getAttribute( "viewport_height" );
			if( attr != NULL )
				viewportHeight = attr->asFloat();
			view->setViewport( 
				viewportLeft, viewportTop,
				viewportWidth, viewportHeight );
			
			attr = group->getAttribute( "offset" );
			if( attr != NULL )
			{
				std::vector<float> vals = attr->asFloats();
				if( vals.size() >= 3 )
				{
					// these offsets are in +x=north, +y=east, -z=up, 
					// so we need to swap things around
					view->setViewOffset( Vect3( 
						vals[1], 
						vals[0], 
						-1.0 * vals[2] ) );
				}
			}
			
			attr = group->getAttribute( "rotation" );
			if( attr != NULL )
			{
				std::vector<float> vals = attr->asFloats();
				if( vals.size() >= 3 )
				{
					view->setViewRotate( Vect3( vals[0], vals[1], vals[2] ) );
				}
			}
			
			sanityCheckView( view.get() );
		}
		else
		{
			// ignore everything else
		}
	}
	
}


// ================================================
// initAllViewParams() 
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginViewMgr::initAllViewParams() 
{
	ViewMap::iterator viewIter;
	for( viewIter = viewMap.begin(); 
		viewIter != viewMap.end(); viewIter++ )
	{
		if( viewIter->second.valid() )
			viewIter->second->initialize();
	}
}


// ================================================
// sanityCheckView
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginViewMgr::sanityCheckView( View *view )
{
	int entityID = view->getEntityID();
	
	// Check to see if the target entity for this view is valid.  If not, 
	// set the target entity to the default (entity ID 0).
	Entity *targetEntity = allEntities->findEntity( entityID );
	if( targetEntity == NULL || 
		targetEntity->getState() == Entity::Remove )
	{
		if( entityID != 0 )
			std::cerr << "Warning - The target entity for view " << view->getID() 
				<< " (entity " << entityID 
				<< ") no longer exists.\n"
				<< "\tThe target entity has been reset to 0\n";
		// default to entity 0 if the target entity no longer exists
		view->setEntityID( 0 );
	}
}
