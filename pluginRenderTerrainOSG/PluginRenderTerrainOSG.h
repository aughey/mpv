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
 *  FILENAME:   PluginRenderTerrainOSG.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  create and manage the terrain branch of the scene graph.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  03/29/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 *
 *  2007-07-21 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#ifndef PLUGIN_RENDER_TERRAIN_OSG_H
#define PLUGIN_RENDER_TERRAIN_OSG_H

#include <list>
#include <map>
#include <osg/Group>
#include <osg/PositionAttitudeTransform>

#include "Plugin.h"
#include "Entity.h"
#include "Terrain.h"
#include "TerrainContainer.h"

//=========================================================
//! This plugin is responsible for creating and managing the terrain branch 
//! of the scene graph.
//! 
class PluginRenderTerrainOSG : public Plugin 
{
public:

	//=========================================================
	//! General Constructor
	//! 
	PluginRenderTerrainOSG();

	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginRenderTerrainOSG() throw();
	
	//=========================================================
	//! The per-frame processing that this plugin performs
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
	//! A container, containing all known terrain databases.  
	//! Retrieved from the blackboard.
	//!
	mpv::TerrainContainer *terrainDatabases;

	//=========================================================
	//! The terrain branch of the scene graph.  Attached to the scene root.
	//! 
	osg::ref_ptr<osg::PositionAttitudeTransform> terrainBranchNode;
	
	void addedTerrainCB( mpv::TerrainContainer*, mpv::Terrain *terrain );
};


#endif

