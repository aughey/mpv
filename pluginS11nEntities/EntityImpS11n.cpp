/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2004 The Boeing Company
 *  Copyright (c) 2009 Andrew Sampson
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
 *  2009-06-19 Andrew Sampson
 *      Copied file from EntityImpOSG to EntityImpS11n.  Adapted for use in 
 *      S11n subsystem.
 *  
 *  </pre>
 */

#include <stdio.h>
#include <iostream>

#ifdef WIN32
#define snprintf _snprintf
#endif

#include "EntityImpS11n.h"


using namespace mpv;
using namespace mpvs11n;


// ================================================
// EntityImpS11n
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
EntityImpS11n::EntityImpS11n( Entity *_entity ) : EntityImp( _entity )
{
	topGroup = new mpvs11n::GroupNode( "entity" );
	
	id = new mpvs11n::IntNode( "id" );
	id->set( entity->getID() );
	name = new mpvs11n::StringNode( "name" );
	type = new mpvs11n::IntNode( "type" );
	state = new mpvs11n::StringNode( "state" );
	parentID = new mpvs11n::IntNode( "parent_id" );
	alpha = new mpvs11n::IntNode( "alpha" );
	inheritAlpha = new mpvs11n::IntNode( "inherit_alpha" );
	collisionDetectionEnabled = new mpvs11n::IntNode( "collision_detection_enabled" );
	groundClampState = new mpvs11n::StringNode( "ground_clamp_state" );
	positionGDC = new mpvs11n::DoubleVectorNode( "position_geodetic" );
	orientationGDC = new mpvs11n::DoubleVectorNode( "orientation_geodetic" );
	positionDB = new mpvs11n::DoubleVectorNode( "position_database" );
	orientationDB = new mpvs11n::DoubleVectorNode( "orientation_database" );
	childEntityGroup = new mpvs11n::GroupNode( "child_entities" );
	symbolSurfaceGroup = new mpvs11n::GroupNode( "symbol_surfaces" );

	topGroup->addChild( id.get() );
	topGroup->addChild( name.get() );
	topGroup->addChild( type.get() );
	topGroup->addChild( state.get() );
	topGroup->addChild( parentID.get() );
	topGroup->addChild( alpha.get() );
	topGroup->addChild( inheritAlpha.get() );
	topGroup->addChild( collisionDetectionEnabled.get() );
	topGroup->addChild( groundClampState.get() );
	topGroup->addChild( positionGDC.get() );
	topGroup->addChild( orientationGDC.get() );
	topGroup->addChild( positionDB.get() );
	topGroup->addChild( orientationDB.get() );
	topGroup->addChild( childEntityGroup.get() );
	topGroup->addChild( symbolSurfaceGroup.get() );

	nameChanged( entity );
	typeChanged( entity );
	stateChanged( entity );
	parentChanged( entity );
	alphaChanged( entity );
	collisionDetectionEnabledChanged( entity );
	groundClampStateChanged( entity );
	positionGDCChanged( entity );
	positionDBChanged( entity );
	
	entity->nameChanged.connect( BIND_SLOT1( EntityImpS11n::nameChanged, this ) );
	entity->typeChanged.connect( BIND_SLOT1( EntityImpS11n::typeChanged, this ) );
	entity->stateChanged.connect( BIND_SLOT1( EntityImpS11n::stateChanged, this ) );
	entity->parentChanged.connect( BIND_SLOT1( EntityImpS11n::parentChanged, this ) );
	entity->alphaChanged.connect( BIND_SLOT1( EntityImpS11n::alphaChanged, this ) );
	entity->collisionDetectionEnabledChanged.connect( BIND_SLOT1( EntityImpS11n::collisionDetectionEnabledChanged, this ) );
	entity->groundClampStateChanged.connect( BIND_SLOT1( EntityImpS11n::groundClampStateChanged, this ) );
	entity->positionGDCChanged.connect( BIND_SLOT1( EntityImpS11n::positionGDCChanged, this ) );
	entity->positionDBChanged.connect( BIND_SLOT1( EntityImpS11n::positionDBChanged, this ) );

	// check for symbol surfaces already attached to the entity and add them
	SymbolSurfaceContainer::SymbolSurfaceIteratorPair ssIterPair = entity->getSymbolSurfaces();
	SymbolSurfaceContainer::SymbolSurfaceMap::iterator ssIter;
	for( ssIter = ssIterPair.first; ssIter != ssIterPair.second; ssIter++ )
	{
		addedSymbolSurface( entity, ssIter->second.get() );
	}

	// arrange notification of symbol surface creation/destruction
	entity->addedSymbolSurface.connect( BIND_SLOT2( EntityImpS11n::addedSymbolSurface, this ) );
	entity->removedSymbolSurface.connect( BIND_SLOT2( EntityImpS11n::removedSymbolSurface, this ) );
	
	// handle existing child entities
	EntityContainer::EntityIteratorPair eIterPair = entity->getEntities();
	EntityContainer::EntityMap::iterator eIter;
	for( eIter = eIterPair.first; eIter != eIterPair.second; eIter++ )
	{
		addedChildEntity( entity, eIter->second.get() );
	}
	
	entity->addedEntity.connect( BIND_SLOT2( EntityImpS11n::addedChildEntity, this ) );
	entity->removedEntity.connect( BIND_SLOT2( EntityImpS11n::removedChildEntity, this ) );
}


// ================================================
// ~EntityImpS11n
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
EntityImpS11n::~EntityImpS11n()
{
}


// ================================================
// update
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void EntityImpS11n::update( double timeElapsed )
{
}


void EntityImpS11n::nameChanged( Entity * )
{
	name->set( entity->getName() );
}


void EntityImpS11n::typeChanged( Entity * )
{
	type->set( entity->getType() );
}


void EntityImpS11n::stateChanged( Entity * )
{
	switch( entity->getState() )
	{
		case Entity::Standby:
			state->set( "Standby" );
			break;
		case Entity::Active:
			state->set( "Active" );
			break;
		case Entity::Remove:
			state->set( "Remove" );
			break;
		default:
			state->set( "Unknown" );
			break;
	}
}


void EntityImpS11n::parentChanged( Entity * )
{
	if( entity->getIsChild() )
		parentID->set( entity->getParentID() );
	else
		parentID->set( -1 );
}


void EntityImpS11n::alphaChanged( Entity * )
{
	alpha->set( entity->getAlpha() );
	inheritAlpha->set( entity->getInheritAlpha() );
}


void EntityImpS11n::collisionDetectionEnabledChanged( Entity * )
{
	collisionDetectionEnabled->set( entity->getCollisionDetectionEnabled() );
}


void EntityImpS11n::groundClampStateChanged( Entity * )
{
	switch( entity->getGroundClampState() )
	{
		case Entity::NoClamp:
			groundClampState->set( "No clamping" );
			break;
		case Entity::AltClamp:
			groundClampState->set( "Ground clamping, altitude only" );
			break;
		case Entity::AltAttClamp:
			groundClampState->set( "Ground clamping, altitude and attitude" );
			break;
		default:
			groundClampState->set( "Unknown" );
			break;
	}
}


void EntityImpS11n::positionGDCChanged( GeodeticObject * )
{
	const CoordinateSet &coord = entity->getPositionGDC();

	// entity position
	positionGDC->set( 
		mpv::Vect3( coord.LatX, coord.LonY, coord.AltZ ) );
	
	// entity orientation
	orientationGDC->set( 
		mpv::Vect3( coord.Roll, coord.Pitch, coord.Yaw ) );
}


void EntityImpS11n::positionDBChanged( GeodeticObject * )
{
	const CoordinateSet &coord = entity->getPositionDB();

	// entity position
	positionDB->set( 
		mpv::Vect3( coord.LatX, coord.LonY, coord.AltZ ) );
	
	// entity orientation
	orientationDB->set( 
		mpv::Vect3( coord.Roll, coord.Pitch, coord.Yaw ) );
}


void EntityImpS11n::addedChildEntity( EntityContainer *entity, Entity *childEntity )
{
	EntityImpS11n *entityImpS11n = getEntityImpS11nFromEntity( childEntity );
	if( entityImpS11n == NULL )
	{
		std::cerr << "EntityImpS11n::addedChildEntity - warning - The newly-added child entity either has no implementation object or has an implementation that isn't an EntityImpS11n.  Can't add new child to my scene graph.\n";
	}
	else
	{
		childEntityGroup->addChild( entityImpS11n->getTopNode() );
	}
}


void EntityImpS11n::removedChildEntity( EntityContainer *entity, Entity *childEntity )
{
	EntityImpS11n *entityImpS11n = getEntityImpS11nFromEntity( childEntity );
	if( entityImpS11n == NULL )
	{
		std::cerr << "EntityImpS11n::removedChildEntity - warning - The newly-removed child entity either has no implementation object or has an implementation that isn't an EntityImpS11n.  Can't remove child from my scene graph.\n";
	}
	else
	{
		childEntityGroup->removeChild( entityImpS11n->getTopNode() );
	}
}


mpvs11n::Node *EntityImpS11n::getTopNode()
{
	return topGroup.get();
}


// ================================================
// addedSymbolSurface
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void EntityImpS11n::addedSymbolSurface( 
	SymbolSurfaceContainer *, SymbolSurface *surface )
{
	// retrieve nodes from symbol surface, attach them to the entity subgraph
	
	// sanity check? container == entity
	
	if( surface->getAttachState() != SymbolSurface::Entity )
	{
std::cerr << "Debug warning - in EntityImpS11n::addedSymbolSurface, symbol surface is not a entity-attached surface\n";
		return;
	}
	
/* FIXME - SymbolSurfaceImpS11n not implemented yet
	SymbolSurfaceImp *imp = surface->getImplementation();
	if( imp == NULL )
	{
		std::cerr << "Error - in EntityImpS11n::addedSymbolSurface, symbol surface \n"
			<< "has no implementation object.  It is not ready to be attached to the scene!\n";
		return;
	}
	
	// RTTI will hopefully protect us if imp isn't a SymbolSurfaceImpS11n
	SymbolSurfaceImpS11n *impS11n = dynamic_cast<SymbolSurfaceImpS11n*>( imp );
	if( impS11n == NULL )
	{
		return;
	}
	
std::cerr << "EntityImpS11n::addedSymbolSurface - Adding a new entity-attached symbol surface to the scene\n";
	if( !symbolSurfaceGroup->addChild( impS11n->getTopNode() ) )
	{
		std::cerr << "Error - in EntityImpS11n::addedSymbolSurface, symbol surface \n"
			<< "top node couldn't be added to scene!\n";
	}
*/
}


// ================================================
// removedSymbolSurface
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void EntityImpS11n::removedSymbolSurface( 
	SymbolSurfaceContainer *, SymbolSurface *surface )
{
	// retrieve nodes from symbol surface, remove them from the entity subgraph
	
	// sanity check? container == entity
	
/* FIXME - SymbolSurfaceImpS11n not implemented yet
	SymbolSurfaceImp *imp = surface->getImplementation();
	if( imp == NULL )
	{
		std::cerr << "Error - in EntityImpS11n::removedSymbolSurface, symbol surface \n"
			<< "has no implementation object.  It can't be removed from the scene!\n";
		return;
	}
	
	// RTTI will hopefully protect us if imp isn't a SymbolSurfaceImpS11n
	SymbolSurfaceImpS11n *impS11n = dynamic_cast<SymbolSurfaceImpS11n*>( imp );
	if( impS11n == NULL )
	{
		return;
	}
	
std::cerr << "EntityImpS11n::removedSymbolSurface - Removing a entity-attached symbol surface from the scene\n";
	if( !symbolSurfaceGroup->removeChild( impS11n->getTopNode() ) )
	{
		std::cerr << "Error - in EntityImpS11n::removedSymbolSurface, symbol surface \n"
			<< "top node couldn't be removed from scene!\n";
	}
*/
}


// ================================================
// getEntityImpS11nFromEntity
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
EntityImpS11n *EntityImpS11n::getEntityImpS11nFromEntity( Entity *entity )
{
	EntityImpS11n *entityImpS11n = NULL;
	
	if( entity == NULL )
		return NULL;
	
	Entity::EntityImpIteratorPair iterPair = entity->getImplementations();
	Entity::EntityImpList::iterator iter;
	for( iter = iterPair.first; iter != iterPair.second; iter++ )
	{
		// RTTI will (hopefully) tell us if imp is an EntityImpS11n or 
		// something else.
		entityImpS11n = dynamic_cast<EntityImpS11n*>(iter->get());
		if( entityImpS11n != NULL )
		{
			break;
		}
	}

	return entityImpS11n;
}


