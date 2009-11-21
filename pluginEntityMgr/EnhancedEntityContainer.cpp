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


#include "EnhancedEntityContainer.h"

using namespace mpv;

// ================================================
// EnhancedEntityContainer
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
EnhancedEntityContainer::EnhancedEntityContainer() : EntityContainer()
{
	
}


// ================================================
// ~EnhancedEntityContainer
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
EnhancedEntityContainer::~EnhancedEntityContainer() 
{
	
}


// ================================================
// addEntity
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void EnhancedEntityContainer::addEntity( Entity *entity )
{
	if( entity == NULL )
		return;
	entityArray[entity->getID()] = entity;
	EntityContainer::addEntity( entity );
}


// ================================================
// removeEntity
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void EnhancedEntityContainer::removeEntity( Entity *entity )
{
	if( entity == NULL )
		return;
	EntityContainer::removeEntity( entity );
	entityArray[entity->getID()] = NULL;
}


// ================================================
// findEntity
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Entity *EnhancedEntityContainer::findEntity( int entityID )
{
	if( entityID < 0 || entityID >= 0xffff )
		return NULL;
	
	return entityArray[entityID].get();
}


