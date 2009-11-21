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
 *  FILENAME:   EntityNodeFactory.h
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


#ifndef ENTITY_NODE_FACTORY_H
#define ENTITY_NODE_FACTORY_H

#include <list>
#include <map>
#include <string>

#include "Referenced.h"
#include "Entity.h"
#include "DefFileGroup.h"
#include "EntityElement.h"
#include "EntityElementFactory.h"

#include "GroupElementFactory.h"



//=========================================================
//! This class is used to create an entity's model subgraph, ie the 
//! portion of the scene graph which depicts the entity.
//! The entity's representation is not constructed by just one software 
//! component; the construction process ... FIXME
//! 
class EntityNodeFactory : public mpv::Referenced
{
public:
	
	typedef std::map< std::string, mpvosg::EntityElementFactory * > ElementFactoryMap;

	//=========================================================
	//! General Constructor
	//! 
	EntityNodeFactory();

	//=========================================================
	//! Adds a EntityImpOSG to newly-created entities.
	//! \param entity - the newly-added entity
	//! 
	void addedEntity( mpv::EntityContainer *, mpv::Entity *entity );
	
	//=========================================================
	//! 
	//! \param entity - the entity recently promoted to top-level
	//! 
	void addedTopLevelEntity( mpv::EntityContainer *container, mpv::Entity *entity );
	
	//=========================================================
	//! 
	//! \param entity - the entity recently un-top-leveled (ahem)
	//! 
	void removedTopLevelEntity( mpv::EntityContainer *, mpv::Entity *entity );

	//=========================================================
	//! Returns the entity element factory list
	//! 
	ElementFactoryMap *getElementFactoryList()
	{
		return &entityElementFactoryMap;
	}
	
	//=========================================================
	//! Returns the scene graph node where top-level entities are attached.
	//! 
	osg::Group *getEntityBranchNode()
	{
		return entityBranchNode.get();
	}
	
private:
	
	//=========================================================
	//! General Destructor
	//! 
	~EntityNodeFactory();
	
	//=========================================================
	//! List of entity element factories.  Posted to the blackboard.
	//! 
	ElementFactoryMap entityElementFactoryMap;

	//=========================================================
	//! Stores all of the entity subgraphs.  Is attached to the root osg node.
	//! 
	osg::ref_ptr<osg::Group> entityBranchNode;
	
	//=========================================================
	//! The fall-back element factory.  If the config file specifies a 
	//! factory that can't be found, then this factory is used instead.
	//! 
	GroupElementFactory *groupFactory;

	//=========================================================
	//! This method creates entity elements are groups them into a 
	//! hierarchy.
	//! \param entity - the entity to create model nodes for
	//! \return a pointer to a newly allocated EntityElement, 
	//! or NULL if this method could not create nodes for this typeID.
	//! 
	mpvosg::EntityElement *createNewNodes( mpv::Entity *entity );
	

	void handleChildElements( 
		DefFileGroup *config, mpvosg::EntityElement *parentElement, mpv::Entity *entity );
};



#endif
