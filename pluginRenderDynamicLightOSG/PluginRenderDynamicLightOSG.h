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
 *  FILENAME:   PluginRenderDynamicLightOSG.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  manage the ambient lighting parameters, as reported by the ephemeris 
 *  plugin.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  05/04/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 *
 *  2007-07-15 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#ifndef PLUGIN_RENDER_DYNAMICLIGHT_OSG_H
#define PLUGIN_RENDER_DYNAMICLIGHT_OSG_H

#include <osg/Group>

#include "Plugin.h"
#include "MPVLight.h"

//=========================================================
//! This plugin is responsible for managing the ambient lighting parameters.
//! 
class PluginRenderDynamicLightOSG : public Plugin 
{
public:

	//=========================================================
	//! General Constructor
	//! 
	PluginRenderDynamicLightOSG();

	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginRenderDynamicLightOSG() throw();
	
	//=========================================================
	//! The per-frame processing that this plugin performs.
	//! This plugin's act() will update the ambient light values based on
	//! data from the ephemeris plugin.  That data is in turn based on the 
	//! time of day, the date, the location, etc.
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );
	
private:

	//=========================================================
	//! Called by act() when the system is in the Operate state.  This 
	//! method pulls the lighting parameters from sunLight and moonLight 
	//! and applies them to sceneStateSet.
	//! 
	void operate( void );

	//=========================================================
	//! The root node of the scene graph.  Retrieved from the blackboard.
	//! 
	osg::Group *rootNode;
	
	//=========================================================
	//! The lighting parameters from the sun.  Retrieved from the blackboard.
	//! 
	MPVLight *sunLight;
	
	//=========================================================
	//! The lighting parameters from the moon.  Retrieved from the blackboard.
	//! 
	MPVLight *moonLight;
	
	//=========================================================
	//! A ref_ptr to the scene's state set.
	//! This is attached to the root node of the scene graph.
	//! 
	osg::ref_ptr<osg::StateSet> sceneStateSet;
};


#endif

