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


#ifdef WIN32
#define _USE_MATH_DEFINES
#include <float.h>
#define isnan _isnan
#endif

#include <math.h>
#include <iostream>


#include "PluginMotionTrackerJoystickSDL.h"

using namespace JoystickMotionTracker;



EXPORT_DYNAMIC_CLASS( PluginMotionTrackerJoystickSDL )

// ================================================
// PluginMotionTrackerJoystickSDL
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginMotionTrackerJoystickSDL::PluginMotionTrackerJoystickSDL() : Plugin() 
{
	name_ = "PluginMotionTrackerJoystickSDL";
	licenseInfo_.setLicense( LicenseInfo::LicenseLGPL );
	licenseInfo_.setOrigin( "AndrewSampson" );

	dependencies_.push_back( "PluginMotionTrackerMgr" );
	dependencies_.push_back( "PluginDefFileReader" );

	trackerParamsMap = NULL;
	DefFileData = NULL;

	firstTimeThrough = true;
	calledSDLInit = false;
	joy = NULL;
	joystickTrackerParams = NULL;
}


// ================================================
// ~PluginMotionTrackerJoystickSDL
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginMotionTrackerJoystickSDL::~PluginMotionTrackerJoystickSDL() throw() 
{

	if( calledSDLInit )
	{
		SDL_QuitSubSystem( SDL_INIT_JOYSTICK );
		calledSDLInit = false;
	}

}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginMotionTrackerJoystickSDL::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{
		
	case SystemState::BlackboardRetrieve:

		SDL_InitSubSystem( SDL_INIT_JOYSTICK );
		calledSDLInit = true;
		
		// Check for joystick
		if( SDL_NumJoysticks() > 0 )
		{
			// Open joystick
			joy = SDL_JoystickOpen(0);
			
			if( joy )
			{
				printf("Opened Joystick 0\n");
				printf("Name: %s\n", SDL_JoystickName(0));
				printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joy));
				printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joy));
				printf("Number of Hats: %d\n", SDL_JoystickNumHats(joy));
				printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joy));

			}
			else
				printf("Couldn't open Joystick 0\n");
			
			// disable the event-based (callback-based) joystick handling
			SDL_JoystickEventState( SDL_IGNORE );
		}

		// get the motion tracker parameters
		bb_->get( "TrackerParamsMap", trackerParamsMap );
		
		bb_->get( "DefinitionData", DefFileData );

		break;


	case SystemState::ConfigurationProcess:
		joystickBindings.setConfigData(*DefFileData);
		break;

	case SystemState::Operate:
	case SystemState::Debug:
		operate();
		break;

	case SystemState::Shutdown:
/*
FIXME - there is a race condition with other SDL-using plugins (well, 
pluginCameraCigiSDLOSG), in that it is unknown which plugin will call SDL_Quit 
first.  When SDL_Quit is called, the joystick stuff will be closed and 
de-allocated.  SDL provides no way to determine if SDL_Quit has been called, 
and SDL_JoystickOpened will still return true after SDL_Quit has been called.  
If SDL_JoystickClose is called after SDL_Quit, a segfault will result.  
Shame on the SDL maintainers.

For this reason, joystick cleanup is left to the SDL_Quit procedure.
		if( joy )
		{
			// Close if opened
			if( SDL_JoystickOpened(0) )
				SDL_JoystickClose(joy);
			joy = NULL;
		}
*/
		if( calledSDLInit )
		{
			SDL_QuitSubSystem( SDL_INIT_JOYSTICK );
			calledSDLInit = false;
		}
		break;

	default:
		break;
	}
	
}


// ================================================
// operate
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginMotionTrackerJoystickSDL::operate()
{
	// only do stuff if a joystick was detected
	if( joy )
	{

		// if this is the first time through, the joystick needs to be 
		// registered as a motion tracker with the motion tracker manager
		// and the joystick bindings need to be set up
		if( firstTimeThrough )
		{
			firstTimeThrough = false;
			
			const char * joyname = SDL_JoystickName(0);
			joystickBindings.setProfileByString( joyname );

			joystickTrackerParams = new TrackerParams;

			// fixme - I'm arbitrarily picking tracker #99 
			joystickTrackerParams->trackerID = 99;

			// Bind tracker to view 99.  This way, it has no visible effects, 
			// but is still enabled.
			joystickTrackerParams->viewID = 99;

			// this tracker deals in euler angles, not quaternions
			joystickTrackerParams->useQuaternion = false;
			
			if( (*trackerParamsMap)[joystickTrackerParams->trackerID] == NULL )
			{
				(*trackerParamsMap)[joystickTrackerParams->trackerID] = joystickTrackerParams;
			}
		}
		
		// grab data from the joystick and 
		// put data into the motion tracker params
		SDL_JoystickUpdate();
		for( int axis = 0, numAxes = SDL_JoystickNumAxes( joy ); 
			axis < numAxes; axis++ )
		{
			float axisValue = (float)SDL_JoystickGetAxis( joy, axis ) / 
				JOYSTICK_MAX_AXIS_VALUE;
			if( axis == joystickBindings.axisIndexXOffset )
				joystickTrackerParams->trackerOffset[0] = axisValue;
			else if( axis == joystickBindings.axisIndexYOffset )
				joystickTrackerParams->trackerOffset[1] = axisValue * -1.0;
			else if( axis == joystickBindings.axisIndexZOffset )
				joystickTrackerParams->trackerOffset[2] = axisValue * -1.0;
			else if( axis == joystickBindings.axisIndexYaw )
			{
				float yaw = axisValue * 180.0;
				if( yaw < 0.0 )
					yaw += 360.0;
				joystickTrackerParams->trackerRotateRPY[2] = yaw;
			}
			else if( axis == joystickBindings.axisIndexPitch )
				joystickTrackerParams->trackerRotateRPY[1] = axisValue * 90.0;
			else if( axis == joystickBindings.axisIndexRoll )
				joystickTrackerParams->trackerRotateRPY[0] = axisValue * 90.0;
		}
		
		
	}
	
}

