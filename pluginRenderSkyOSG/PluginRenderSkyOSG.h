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
 *  FILENAME:   PluginRenderSkyOSG.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  create and manage the sky dome.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  04/26/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 *
 *  2007-07-15 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#ifndef PLUGIN_RENDER_SKY_OSG_H
#define PLUGIN_RENDER_SKY_OSG_H

#include <list>

#include <osg/Group>

#include "Plugin.h"
#include "DefFileGroup.h"

//=========================================================
//! This plugin is responsible for creating and managing the sky dome.
//! 
class PluginRenderSkyOSG : public Plugin 
{
public:

	//=========================================================
	//! General Constructor
	//! 
	PluginRenderSkyOSG();

	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginRenderSkyOSG() throw();
	
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
	//! The root of the def-file tree.  Retrieved from the blackboard.
	//! 
	DefFileGroup **DefFileData;
	
	//=========================================================
	//! The sky branch of the scene graph.  Attached to the scene root.
	//! 
	osg::ref_ptr<osg::Group> skyBranchNode;
	
	//=========================================================
	//! A list of skydome nodes; populated when the configuration data is 
	//! retrieved, emptied in act()
	//! 
	std::list<osg::ref_ptr<osg::Node> > skydomeList;
	
	//=========================================================
	//! Retrieves configuration data from DefFileData
	//! 
	void getConfig( void );
};


#endif

