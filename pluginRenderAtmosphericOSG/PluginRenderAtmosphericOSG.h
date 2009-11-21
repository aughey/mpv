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
 *  FILENAME:   PluginRenderAtmosphericOSG.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  create and manage global atmospheric effects like fog, etc.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  08/17/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 *
 *  2007-07-15 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#ifndef PLUGIN_RENDER_ATMOSPHERIC_OSG_H
#define PLUGIN_RENDER_ATMOSPHERIC_OSG_H

#include <osg/Group>
#include <osg/Vec4>

#include "Plugin.h"
#include "DefFileGroup.h"
#include "GlobalWeather.h"
#include "MPVLight.h"

//=========================================================
//! This plugin is responsible for creating and managing global atmospheric 
//! effects like fog, etc.
//! 
class PluginRenderAtmosphericOSG : public Plugin 
{
public:

	//=========================================================
	//! General Constructor
	//! 
	PluginRenderAtmosphericOSG();

	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginRenderAtmosphericOSG() throw();
	
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
	//! The global weather and atmospheric conditions data.  Retrieved 
	//! from the blackboard.
	//!
	GlobalWeather *globalWeatherData;
	
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
	
	//=========================================================
	//! A boolean flag to indicate that the fog attributes have changed and 
	//! that the changes need to be propagated to the scene.
	//! 
	bool fogHasChanged;
	
	//=========================================================
	//! RGBA for the fog
	//! 
	osg::Vec4 fogColor;
	
	//=========================================================
	//! Scene features closer to the eyepoint than this distance 
	//! will not be affected by fog
	//! 
	float fogStart;
	
	//=========================================================
	//! The fog will be in full effect at this distance
	//! 
	float fogEnd;

	//=========================================================
	//! Retrieves configuration data from DefFileData
	//! 
	void getConfig( void );

	//=========================================================
	//! Retrieves fog parameters from the global weather data (set by the 
	//! Host).  
	//! 
	void getWeatherState();

	//=========================================================
	//! Retrieves fog parameters from the data published by the 
	//! ephemeris plugin.  This allows the fog to be set to the correct 
	//! color; bright white fog in a night scene looks silly.
	//! 
	void getEphemerisState();

	//=========================================================
	//! applies the fog parameters to the scene stateset
	//! 
	void updateScene();
};


#endif

