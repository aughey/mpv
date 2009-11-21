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
 *  FILENAME:   PluginRenderEntitiesOSG.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  create and maintain the entity branch of the scene graph.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  03/29/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 *
 *  2007-03-28 Andrew Sampson
 *      Completely reworked the mechanism by which entity subgraphs are 
 *      constructed.
 *
 *  2007-07-15 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 *  2007-11-03  Andrew Sampson
 *      Moved ModelElement code out of pluginRenderEntsOSG and into its own 
 *      plugin
 *
 *  2008-07-26 Andrew Sampson
 *      Ported plugin to the new entity interface.  Now contains code from 
 *      the symbology rendering plugin.
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */

// Note - the naming convention for subgraphs, etc., is as follows: 
// "entity subgraph" - refers to ALL of the OSG nodes associated with 
// an entity.  This includes transform nodes, switch nodes, the model subgraph 
// (see next item) AND in some cases the entity subgraphs for the entity's 
// children.
// "model subgraph" - refers to the osg nodes associated with the visual 
// representation of an entity.  
// "entity" - refers to something in the world/scene/gaming-area that moves.
// "model" - refers to the visual representation of an entity.
// "top level entity" - an entity that has no parent.


#ifndef PLUGIN_RENDER_ENTITIES_OSG_H
#define PLUGIN_RENDER_ENTITIES_OSG_H

#include <map>
#include <string>

#include <osg/Group>

#include "Referenced.h"
#include "Plugin.h"
#include "Entity.h"
#include "EntityContainer.h"
#include "DefFileGroup.h"

#include "EntityElementFactory.h"

#include "EntityNodeFactory.h"


//=========================================================
//! This plugin is responsible for creating and maintaining the 
//! entity branch of the scene graph.  All entities in the system will 
//! have a representative node under the entity branch.
class PluginRenderEntsOSG : public Plugin 
{
public:

	//=========================================================
	//! General Constructor
	//! 
	PluginRenderEntsOSG();

	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginRenderEntsOSG() throw();
	
	//=========================================================
	//! The per-frame processing that this plugin performs.
	//! This plugin's act() will update the existing entity subgraphs, 
	//! and create or remove entity subgraphs as needed.
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );
	
	
private:
	//=========================================================
	//! The root node of the scene graph.  Retrieved from the blackboard.
	//! 
	osg::Group *rootNode;
	
	//=========================================================
	//! An entity container, containing all the entities.  
	//! Retrieved from the blackboard.
	//! 
	mpv::EntityContainer *allEntities;
	
	//=========================================================
	//! An entity container, containing only the top-level entities.  
	//! Retrieved from the blackboard.
	//! 
	mpv::EntityContainer *topLevelEntities;
	
	//=========================================================
	//! Configuration data.  Retrieved from the blackboard.
	//! 
	DefFileGroup **DefFileData;

	//=========================================================
	//! EntityNodeFactory object.  Is used to create the model subgraphs.
	//! 
	mpv::RefPtr<EntityNodeFactory> nodeFactory;
	
	//=========================================================
	//! Called by act() when the system is in the Operate state.  This 
	//! method will update the existing entity subgraphs, and create 
	//! or remove entity subgraphs as needed.
	//! 
	void operate( void );
	
};



#endif

