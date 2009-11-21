/** <pre>
 *  Joystick-based motion tracker plugin, for the MPV
 *  Copyright (c) 2005 Andrew Sampson
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
 *  Initial Release: 2005-10-08 Andrew Sampson
 *
 *  2007-07-14 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 * </pre>
 */


#ifndef PLUGIN_MOTIONTRACKERJOYSTICKSDL_H
#define PLUGIN_MOTIONTRACKERJOYSTICKSDL_H

#include <map>
#include <list>

#include <SDL.h>

#include "Plugin.h"
#include "TrackerParams.h"
#include "DefFileGroup.h"

#include "JoystickBindings.h"

#define JOYSTICK_MAX_AXIS_VALUE 32770.0


namespace JoystickMotionTracker
{

//=========================================================
//! This plugin is a motion tracker plugin.  It registers itself with the 
//! motion tracker system, and uses position input from a SDL joystick 
//! to "fake" a motion tracker.  This plugin is mostly used for testing.
//! 
class PluginMotionTrackerJoystickSDL : public Plugin 
{
public:

	//=========================================================
	//! General Constructor
	//! 
	PluginMotionTrackerJoystickSDL();

	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginMotionTrackerJoystickSDL() throw();
	
	//=========================================================
	//! The per-frame processing that this plugin performs
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );

private:

	//=========================================================
	//! Called by act() when in Operate mode.  Reads in joystick data and 
	//! populates the tracker parameters with the resulting data.
	//! 
	void operate();

	//=========================================================
	//! Map of TrackerParams.  TrackerParams contain info from the Host 
	//! regarding motion trackers, such as boresight-enable, etc.  
	//! Retrieved from the blackboard.
	//! 
	std::map< int, TrackerParams * > *trackerParamsMap;
	
	//=========================================================
	//! Configuration data.  Retrieved from the blackboard.
	//! 
	DefFileGroup **DefFileData;

	//=========================================================
	//! Boolean that indicates if this is the first frame in the simulation.
	//! 
	bool firstTimeThrough;
	
	//=========================================================
	//! used to prevent SDLQuit from being called twice.
	//! 
	bool calledSDLInit;
	
	//=========================================================
	//! Joystick object retrieved from SDL
	//! 
	SDL_Joystick *joy;
	
	//=========================================================
	//! Stores a set of mappings from hardware axes and buttons to logical 
	//! axes and buttons
	//! 
	JoystickBindings joystickBindings;
	
	//=========================================================
	//! This object is the means by which motion tracker data is fed into the 
	//! motion tracker manager.  It will be added to trackerParamsMap if a 
	//! joystick is detected.
	//! 
	TrackerParams *joystickTrackerParams;

};

}

#endif

