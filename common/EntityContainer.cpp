/** <pre>
 *  The MPV Common Library
 *  Copyright (c) 2008
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
 *  2008-07-05 Andrew Sampson
 *      Initial release
 *
 * </pre>
 */

#include "BindSlot.h"
#include "EntityContainer.h"

using namespace mpv;


EntityContainer::EntityContainer() : Referenced()
{
	
}


EntityContainer::~EntityContainer()
{
	// The CIGI standard states that when a Entity is deleted, all Entities 
	// attached to it are deleted.
	// 
	// This EntityContainer class is used in Entity and elsewhere.  In all 
	// these situations, this is the desired behavior.
	
	flagAllEntitiesAsDestroyed();
}


void EntityContainer::updateEntities( double timeElapsed )
{
	EntityMap::iterator iter = entities.begin();
	for( ; iter != entities.end(); iter++ )
	{
		iter->second->update( timeElapsed );
	}
}


void EntityContainer::flagAllEntitiesAsDestroyed()
{
	while( !entities.empty() )
	{
		// hold on to a reference until signal emission is complete
		RefPtr<Entity> entity( entities.begin()->second );

		entity->setState( Entity::Remove );
		// note - callback entityChangedState will handle removal of entity 
		// from the map
	}
}


void EntityContainer::addEntity( Entity *entity )
{
	if( entity != NULL )
	{
		entity->stateChanged.connect( BIND_SLOT1( EntityContainer::entityChangedState, this ) );
		entities[entity->getID()] = entity;

		addedEntity( this, entity );
	}
}


void EntityContainer::removeEntity( Entity *entity )
{
	EntityMap::iterator iter = entities.find( entity->getID() );
	if( iter != entities.end() )
	{
		// hold on to a reference until signal emission is complete
		RefPtr<Entity> entityReference( entity );
		
		entity->stateChanged.disconnect( BIND_SLOT1( EntityContainer::entityChangedState, this ) );
		entities.erase( iter );
		
		removedEntity( this, entity );
	}
}


void EntityContainer::entityChangedState( Entity *entity )
{
	if( entity->getState() == Entity::Remove )
		removeEntity( entity );
}

