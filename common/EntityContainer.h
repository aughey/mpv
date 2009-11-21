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

#ifndef ENTITY_CONTAINER_H
#define ENTITY_CONTAINER_H

#include <utility>
#include <map>

#include "Referenced.h"
#include "MPVCommonTypes.h"


namespace mpv
{

// forward declaration; defined via an include at the bottom of the file
class Entity;

//=========================================================
//! Implements a container for entities.  The lifetime of the entities in the 
//! container will be managed, and some of the entity-related signals will 
//! be handled here.
//! 
class MPVCMN_SPEC EntityContainer : public virtual Referenced
{
public:

	boost::signal<void (EntityContainer*, Entity*)> addedEntity;
	boost::signal<void (EntityContainer*, Entity*)> removedEntity;
	
	typedef std::map< int, RefPtr<Entity> > EntityMap;
	typedef std::pair< EntityMap::iterator, EntityMap::iterator > EntityIteratorPair;

	//=========================================================
	//! Constructor
	//! 
	EntityContainer();

	//=========================================================
	//! Calls update() on the entities in this container
	//! 
	void updateEntities( double timeElapsed );

	//=========================================================
	//! Flags each entity in this container as 
	//! destroyed.
	//! 
	void flagAllEntitiesAsDestroyed();
	
	//=========================================================
	//! Adds the specified entity to this container.
	//! \param entity - the entity to add
	//! 
	virtual void addEntity( Entity *entity );
	
	//=========================================================
	//! Removes the specified entity from this container.  
	//! \param entity - the entity to remove
	//! 
	virtual void removeEntity( Entity *entity );
	
	//=========================================================
	//! Returns the begin and end iterators for the entity container.
	//! \return the begin and end iterators for the entity container.
	//! 
	EntityIteratorPair getEntities()
	{
		return EntityIteratorPair( entities.begin(), entities.end() );
	}
	
	//=========================================================
	//! Searches the container for the given entity.
	//! \return a pointer to the requested Entity, or NULL if not found
	//! 
	virtual Entity *findEntity( int entityID )
	{
		EntityMap::iterator mapIter = 
			entities.find( entityID );
		if( mapIter != entities.end() )
		{
			return mapIter->second.get();
		}
		return NULL;
	}
	
#if 0
	//=========================================================
	//! Returns the underlying map data structure.  This should only be 
	//! called in very rare situations... I didn't plan on allowing 
	//! direct access to this variable.
	//! \return a pointer to the underlying map data structure
	//! 
	EntityMap *getEntityMap()
	{
		return &entities;
	}
#endif
	
protected:
	
	//=========================================================
	//! Destructor
	//! 
	virtual ~EntityContainer();
	
	//=========================================================
	//! The entities attached to this object.
	//! 
	EntityMap entities;

	//=========================================================
	//! Callback; notification of state change for a entity
	//! 
	void entityChangedState( Entity *entity );
};

}

#include "Entity.h"

#endif
