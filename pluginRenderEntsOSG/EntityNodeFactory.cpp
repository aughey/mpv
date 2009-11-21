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
 *  FILENAME:   EntityNodeFactory.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  create the scene graph nodes which will represent an entity.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  05/27/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 *
 *  2008-07-26 Andrew Sampson
 *      Ported plugin to the new entity interface.  Now contains code from 
 *      the symbology rendering plugin.
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#include <iostream>
#include <osg/Group>

#include "EntityImpOSG.h"

#include "EntityNodeFactory.h"
#include "GroupElement.h"
#include "SwitchElementFactory.h"
#include "TransformElementFactory.h"

using namespace mpv;
using namespace mpvosg;

// ================================================
// EntityNodeFactory
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
EntityNodeFactory::EntityNodeFactory() : Referenced()
{
	entityBranchNode = new osg::Group;
	entityBranchNode->setName( "Entity Branch Node" );

	groupFactory = new GroupElementFactory;

	EntityElementFactory *factory;
	factory = groupFactory;
	entityElementFactoryMap[factory->getKeyword()] = factory;
	factory = new SwitchElementFactory;
	entityElementFactoryMap[factory->getKeyword()] = factory;
	factory = new TransformElementFactory;
	entityElementFactoryMap[factory->getKeyword()] = factory;

}


// ================================================
// ~EntityNodeFactory
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
EntityNodeFactory::~EntityNodeFactory()
{
	// delete all the factories
	ElementFactoryMap::iterator iter = entityElementFactoryMap.begin();
	for( ; iter != entityElementFactoryMap.end(); iter++ )
	{
		delete iter->second;
	}
	entityElementFactoryMap.clear();
}


// ================================================
// addedEntity
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void EntityNodeFactory::addedEntity( EntityContainer *, Entity *entity )
{
	if( entity == NULL )
	{
		// error - why is this entity NULL?
		return;
	}

	// sanity check
	if( entity->getState() == Entity::Remove )
		return;

	EntityImpOSG *implementation = EntityImpOSG::getEntityImpOSGFromEntity( entity );
	if( implementation == NULL )
	{
std::cerr << "EntityNodeFactory::addedEntity - Adding a new entity imp to entity " << entity->getID() << std::endl;
		EntityElement *element = createNewNodes( entity );
		if( element == NULL )
		{
			std::cout << "Warning - a scene graph could not be created for entity "
				<< entity->getID() << " (type " << entity->getType() 
				<< ") ... this should never happen" << std::endl;
		}
		else
		{
			implementation = new EntityImpOSG( entity );

			implementation->setEntityElement( element );

			entity->addImplementation( implementation );
		}
	}

}

// ================================================
// 
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void EntityNodeFactory::addedTopLevelEntity( 
	EntityContainer *container, Entity *entity )
{
	if( entity == NULL )
	{
		// error - why is this entity NULL?
		return;
	}

	// sanity check
	if( entity->getState() == Entity::Remove )
		return;

	EntityImpOSG *implementation = EntityImpOSG::getEntityImpOSGFromEntity( entity );
	if( implementation == NULL )
	{
		// perhaps the entity was added to the top-level entities list before 
		// being added to the all-entities list?
		addedEntity( container, entity );
		implementation = EntityImpOSG::getEntityImpOSGFromEntity( entity );
	}
	
	entityBranchNode->addChild( implementation->getTopNode() );
}


// ================================================
// 
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void EntityNodeFactory::removedTopLevelEntity( 
	EntityContainer *, Entity *entity )
{
	if( entity == NULL )
	{
		// error - why is this entity NULL?
		return;
	}

	// sanity check
	EntityImpOSG *implementation = EntityImpOSG::getEntityImpOSGFromEntity( entity );
	if( implementation == NULL )
	{
		// hmm... no EntityImpOSG attached to entity...
		return;
	}
	
	entityBranchNode->removeChild( implementation->getTopNode() );
}


// ================================================
// createNewNodes
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
EntityElement *EntityNodeFactory::createNewNodes( Entity *entity )
{
	// Create the root model node.  It is just a Group.
	// Note that it is entirely acceptable if none of the assembly line 
	// workers make any changes to the model node tree, in which case this 
	// group will remain empty.  There is at least one typeID (typeID 0) 
	// which will typically not have a visual representation associated with it.
	EntityElement *result = new GroupElement;
	// Ordinarily, a EntityElementFactory is responsible for calling construct(); 
	// I'm cheating a bit here, as I know that GroupElement::construct doesn't 
	// use its first argument.
	result->construct( NULL, entity );
	
	DefFileGroup *configGroup = entity->getConfig();

	if( configGroup )
	{
		// check the configGroup entry for any attributes that should 
		// be handled at this point
		DefFileAttrib *attr;
		attr = configGroup->getAttribute( "ignore_alpha" );
		if( attr )
		{
			std::cout << "Warning - ignore_alpha is not implemented yet" << std::endl;
		}
		
		handleChildElements( configGroup, result, entity );
	}
	
	
	return result;
}


// ================================================
// 
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void EntityNodeFactory::handleChildElements( 
	DefFileGroup *config, EntityElement *parentElement, Entity *entity )
{
	// for each "element" group...
	std::list< DefFileGroup * >::iterator iter = config->children.begin();
	for( ; iter != config->children.end(); iter++ )
	{
		if( (*iter)->getName() == "element" )
		{
			DefFileGroup *elementGroup = (*iter);
			DefFileAttrib *attr = elementGroup->getAttribute( "element_type" );
			if( attr )
			{
				std::string elementType = attr->asString();
				EntityElementFactory *factory = NULL;
				
				// look up factory in entityElementFactoryMap
				ElementFactoryMap::iterator factoryIter = 
					entityElementFactoryMap.find( elementType );
				if( factoryIter != entityElementFactoryMap.end() )
				{
					factory = factoryIter->second;
				}
				else
				{
					std::cout << "Warning - Could not find an element factory "
						<< "that knows how to create \"" << elementType
						<< "\" elements... ignoring this element" << std::endl;
					// if a matching factory is not found, fall back 
					// on groupFactory
					factory = groupFactory;
				}
				
				EntityElement *element = factory->createElement( elementGroup, entity );
				if( element == NULL )
				{
					std::cout << "Warning - There was a problem creating an "
						<< "element of type \"" << elementType
						<< "\" for entity #" << entity->getID() 
						<< " (type " << entity->getType() << ")" 
						<< std::endl;
					
					// try again with the groupFactory
					factory = groupFactory;
					element = factory->createElement( elementGroup, entity );
				}
				
				if( element == NULL )
				{
					std::cout << "Error - There was a serious error while "
						<< "creating an element of type \"" << elementType
						<< "\" for entity #" << entity->getID() 
						<< " (type " << entity->getType() << ")" 
						<< std::endl;
					// don't recurse... just let the loop move on to the next element
				}
				else
				{
					parentElement->addChildElement( element );

					// recursion
					handleChildElements( elementGroup, element, entity );
				}
				
			}
		}
	}
}


