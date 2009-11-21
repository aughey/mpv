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
 *  2008-07-06 Andrew Sampson
 *      Initial release
 *  
 *  
 *  </pre>
 */


#include "BindSlot.h"
#include "EntityCoordinateConversionObserver.h"

using namespace mpv;

// ================================================
// EntityCoordinateConversionObserver
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
EntityCoordinateConversionObserver::EntityCoordinateConversionObserver() : 
	mpv::CoordinateConversionObserver()
{
	
}


// ================================================
// ~EntityCoordinateConversionObserver
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
EntityCoordinateConversionObserver::~EntityCoordinateConversionObserver() 
{
	stopObservingContainer();
}


// ================================================
// startObservingContainer
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void EntityCoordinateConversionObserver::startObservingContainer( EntityContainer *entContainer )
{
	stopObservingContainer();
	
	container = entContainer;

	// register existing entities
	EntityContainer::EntityIteratorPair iterPair = container->getEntities();
	EntityContainer::EntityMap::iterator iter = iterPair.first;
	for( ; iter != iterPair.second; iter++ )
	{
		startObserving( iter->second.get() );
	}
	
	// listen for new entities, so that they can be registered as well
	container->addedEntity.connect( BIND_SLOT2( 
		EntityCoordinateConversionObserver::startObservingEntity, this ) );

	// listen for entity removal, so that they can be un-registered
	container->removedEntity.connect( BIND_SLOT2( 
		EntityCoordinateConversionObserver::stopObservingEntity, this ) );
}


// ================================================
// stopObservingContainer
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void EntityCoordinateConversionObserver::stopObservingContainer()
{
	if( !container.valid() )
		return;

	// un-register existing entities
	EntityContainer::EntityIteratorPair iterPair = container->getEntities();
	EntityContainer::EntityMap::iterator iter = iterPair.first;
	for( ; iter != iterPair.second; iter++ )
	{
		stopObserving( iter->second.get() );
	}

	// stop listening for new entities
	container->addedEntity.disconnect( BIND_SLOT2( 
		EntityCoordinateConversionObserver::startObservingEntity, this ) );

	// stop listening for entity removal
	container->removedEntity.disconnect( BIND_SLOT2( 
		EntityCoordinateConversionObserver::stopObservingEntity, this ) );

	container = NULL;
}


// ================================================
// performConversionForAllObservedObjects
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void EntityCoordinateConversionObserver::performConversionForAllObservedObjects()
{
	EntityContainer::EntityIteratorPair iterPair = container->getEntities();
	EntityContainer::EntityMap::iterator iter = iterPair.first;
	for( ; iter != iterPair.second; iter++ )
	{
		performCoordinateConversion( iter->second.get() );
	}
}

// ================================================
// startObservingEntity
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void EntityCoordinateConversionObserver::startObservingEntity( EntityContainer*, Entity *entity )
{
	startObserving( entity );
}


// ================================================
// stopObservingEntity
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void EntityCoordinateConversionObserver::stopObservingEntity( EntityContainer*, Entity *entity )
{
	stopObserving( entity );
}


