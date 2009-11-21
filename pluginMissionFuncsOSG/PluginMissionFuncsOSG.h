/** <pre>
 * Copyright (c) 2007 General Dynamics Land Systems
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef PLUGIN_MISSION_FUNCS_OSG_H
#define PLUGIN_MISSION_FUNCS_OSG_H

#include <osg/Group>

#include <CigiIncomingMsg.h>

#include "Plugin.h"
#include "DefFileGroup.h"

#include "HOATHandler.h"
#include "LOSHandler.h"
#include "OsgLineDrawer.h"

//=========================================================
//! This plugin is responsible for providing mission functions
//! such as HAT and LOS requests.
//! 
class PluginMissionFuncsOSG : public Plugin 
{
public:

	//=========================================================
	//! General Constructor
	//! 
	PluginMissionFuncsOSG();

	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginMissionFuncsOSG() throw();

	//=========================================================
	//! The per-frame processing that this plugin performs
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );

private:
	//==========================================================================
	// Blackboard Pointers:
	//==========================================================================

	//=========================================================
	//! Configuration data.  Retrieved from the blackboard.
	//! 
	DefFileGroup **DefFileData;
	//=========================================================
	//! The Root Node.  Retrieved from the blackboard.
	//! 
	osg::Group* rootNode;

	//==========================================================================
	// Helper Classes.
	//==========================================================================

	//=========================================================
	//! An OSG Intersection Finder, initialized with root node.
	//! 
	SG::OsgIntersectionFinder *rootIntersectionFinder;
	//=========================================================
	//! An OSG Intersection Finder, initialized with terrain node.
	//! 
	SG::OsgIntersectionFinder *terrainIntersectionFinder;
	//=========================================================
	//! The LOSHandler responsible for generating LOS Responses.
	//! 
	mpv::RefPtr<LOSHandler> losHandler;
	//=========================================================
	//! The HOATHandler responsible for generating HAT/HOT Responses.
	//! 
	mpv::RefPtr<HOATHandler> hoatHandler;
	//=========================================================
	//! The LineDrawer which is capable of drawing a visual
	//! representation of various line-of-sight requests.
	//! 
	SG::LineDrawer *lineDrawer;

	//=========================================================
	//! A group node for storing the lase lines (lines drawn in the scene, 
	//! one for each LOS request; used for debugging)
	//! 
	osg::ref_ptr<osg::Group> laseLinesGroup;
	
	//=========================================================
	//! A flag to prevent setup() from being called more than once
	//! 
	bool haveCalledSetup;

	//=========================================================
	//! Pulls some preferences out of the config file data
	//! 
	void getConfig();

	//=========================================================
	//! Some one-time setup stuff is performed here.
	//! 
	void setup();
};

#endif

