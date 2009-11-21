/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2004 The Boeing Company
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
 *  04/04/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 *  
 *  2008-12-07 Andrew Sampson
 *      Removed EntityDefinition, reworked class.
 *  
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include <string>
#include <map>

#include "DefFileGroup.h"
#include "Entity.h"


//=========================================================
//! Creates entities.  Also keeps a map of entity config data, so that 
//! config data can be provided for newly-created entities.
//!
class EntityFactory {

public:

	EntityFactory();

	~EntityFactory();
	
	//=========================================================
	//! Retrieves configuration info from the def file group.  This info 
	//! is used to map typeIDs to entity-representation groups.
	//! \param root - The root of the config data tree
	//! 
	void init( DefFileGroup *root );
	
	//=========================================================
	//! Creates a new entity
	//! \param id - the entity instance ID to use for the new entity
	//! \param type - the entity type ID to use for the new entity
	//! \return a pointer to the newly-created entity
	//!
	mpv::Entity *createEntity( int id, int type );
	
private:
	
	//=========================================================
	//! A map used to speed up the entity creation process.
	//! Maps typeIDs to config data groups.
	//! 
	std::map< int, DefFileGroup * > typeIDToDefinitionMap;
	
};


#endif
