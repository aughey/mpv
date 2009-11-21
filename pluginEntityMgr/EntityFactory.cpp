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


#include <iostream>

#include "EntityFactory.h"
#include "MPVExceptions.h"

using namespace mpv;

// ================================================
// EntityFactory
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
EntityFactory::EntityFactory()
{

}

// ================================================
// ~EntityFactory
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
EntityFactory::~EntityFactory()
{
	
}


// ================================================
// init
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void EntityFactory::init( DefFileGroup *root )
{
	typeIDToDefinitionMap.clear();
	
	DefFileGroup *entitiesGroup = root->getGroupByURI( "/entities/" );
	if( entitiesGroup == NULL )
		return;

	// Pointers to the entity representation groups will be stored in the 
	// typeIDToDefinitionMap.  This speeds up the lookup process when an 
	// entity is created.
	std::list< DefFileGroup * >::iterator repGroupIter;
	for( repGroupIter = entitiesGroup->children.begin(); 
		 repGroupIter != entitiesGroup->children.end(); repGroupIter++ )
	{
		DefFileGroup *group = (*repGroupIter);
		if( group->getName() != "entity" )
		{
			continue;
		}

		DefFileAttrib *attr = group->getAttribute( "type_id" );
		if( attr )
		{
			// check for the special string "default", and map it to the 
			// "catch-all" typeID of -1 (which is not a valid CIGI type id)
			if( attr->asString() == "default" )
			{
				typeIDToDefinitionMap[-1] = group;
			}
			else
			{
				int typeID = attr->asInt();
				if( typeID >= 0 && typeID < 65536 )
					typeIDToDefinitionMap[typeID] = group;
				else
					std::cout << "Warning - In the entity config file: entity type ID " 
						<< typeID << " is outside the valid range of type IDs."
						<< std::endl;

				if( typeID == 0 )
					std::cout << "Warning - In the entity config file: entity type ID 0 " 
						<< "is not supposed to have \n\tany geometry associated with it.  "
						<< "The MPV will allow the user to \n\tassociate geometry with "
						<< "type 0, but this behavior does not conform to \n\t"
						<< "the CIGI standard." << std::endl;
			}
		}
	}
	
}


// ================================================
// createEntity
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Entity * EntityFactory::createEntity( int id, int typeID )
{
	Entity *result = new Entity;
	result->setID( id );
	result->setType( typeID );
	
	DefFileGroup *configGroup = typeIDToDefinitionMap[typeID];

	if( configGroup == NULL && typeID != 0 )
	{
		/* 
		If:
		 - this entity typeID has no explicit configuration 
		 - and it's not a stealth (typeID of 0) 
		 - and there exists a catchall entity configuration (typeID of -1) 
		then this entity will use the catchall entity configuration.
		*/
		
		DefFileGroup *catchAll = typeIDToDefinitionMap[-1];
		if( catchAll )
		{
			std::cout << "Note - New entity " << id 
				<< " uses type ID " << typeID 
				<< ", which is not specified in the config files.\n"
				<< "\tUsing the default entity configuration instead." << std::endl;
			configGroup = catchAll;
		}
	}
	
	if( configGroup )
	{
		result->setConfig( configGroup );
		
		// retrieve the type name
		DefFileAttrib *attr = configGroup->getAttribute( "name" );
		if( attr )
		{
			result->setName( attr->asString() );
		}
	}

	return result;
}


