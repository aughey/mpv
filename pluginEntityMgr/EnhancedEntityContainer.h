/** <pre>
 *  MPV Entity Manager plugin
 *  Copyright (c) 2008 Andrew Sampson
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
 *  2008-08-10 Andrew Sampson
 *      Initial release
 *  
 *  
 *  </pre>
 */


#ifndef _ENHANCEDENTITYCONTAINER_H_
#define _ENHANCEDENTITYCONTAINER_H_

#include "EntityContainer.h"

//=========================================================
//! A subclass of EntityContainer, which has been augmented to provide 
//! fast entity lookup.
//! 
class EnhancedEntityContainer : public mpv::EntityContainer
{
public:
	//=========================================================
	//! General Constructor
	//! 
	EnhancedEntityContainer();
	
	//=========================================================
	//! Adds the specified entity to this container.
	//! \param entity - the entity to add
	//! 
	virtual void addEntity( mpv::Entity *entity );
	
	//=========================================================
	//! Removes the specified entity from this container.  
	//! \param entity - the entity to remove
	//! 
	virtual void removeEntity( mpv::Entity *entity );
	
	//=========================================================
	//! Searches the container for the given entity.
	//! \return a pointer to the requested Entity, or NULL if not found
	//! 
	virtual mpv::Entity *findEntity( int entityID );
	
	
protected:
	//=========================================================
	//! General Destructor
	//! 
	virtual ~EnhancedEntityContainer();
	
	//! An array of entities.  Provides fast lookup of entities.
	mpv::RefPtr<mpv::Entity> entityArray[0xffff];
};

#endif
