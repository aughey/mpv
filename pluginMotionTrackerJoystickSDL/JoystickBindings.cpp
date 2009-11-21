/** <pre>
 *  Joystick-based motion tracker plugin, for the MPV
 *  Copyright (c) 2005 Andrew Sampson
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
 *  Initial Release: 2005-10-08 Andrew Sampson
 * </pre>
 */

#include <stdio.h>
#include <string.h>

#include "JoystickBindings.h"

using namespace JoystickMotionTracker;

JoystickBindings::JoystickBindings()
{
	bindingsConfigGroup = NULL;
	useBuiltInProfile( JoystickBindingsProfileSidewinder );
}

JoystickBindings::~JoystickBindings()
{
	
}

void JoystickBindings::setConfigData( DefFileGroup *configRoot )
{
	// it gets set to NULL if group not found
	bindingsConfigGroup = configRoot->getGroupByURI( "/trackers/joystick_tracker/" );
}

void JoystickBindings::setProfileByString( const char *joystickName )
{
	bool found = false;
	
	if( bindingsConfigGroup != NULL )
	{
		std::list< DefFileGroup * >::iterator iter = 
			bindingsConfigGroup->children.begin();
		for( ; iter != bindingsConfigGroup->children.end() && !found; iter++ )
		{
			if( (*iter)->getName() == "bindings" )
			{
				DefFileGroup *bindGroup = *iter;
				DefFileAttrib *deviceID = bindGroup->getAttribute( "device_id_string" );
				if( deviceID != NULL && 
					strstr( joystickName, deviceID->asString().c_str() ) != NULL )
				{
					found = true;
					
					DefFileAttrib *attr = NULL;
					
					attr = bindGroup->getAttribute( "x_offset_axis" );
					if( attr )
						axisIndexXOffset = attr->asInt();
					else
						axisIndexXOffset = -1;
					
					attr = bindGroup->getAttribute( "y_offset_axis" );
					if( attr )
						axisIndexYOffset = attr->asInt();
					else
						axisIndexYOffset = -1;
					
					attr = bindGroup->getAttribute( "z_offset_axis" );
					if( attr )
						axisIndexZOffset = attr->asInt();
					else
						axisIndexZOffset = -1;
					
					attr = bindGroup->getAttribute( "yaw_axis" );
					if( attr )
						axisIndexYaw = attr->asInt();
					else
						axisIndexYaw = -1;
					
					attr = bindGroup->getAttribute( "pitch_axis" );
					if( attr )
						axisIndexPitch = attr->asInt();
					else
						axisIndexPitch = -1;
					
					attr = bindGroup->getAttribute( "roll_axis" );
					if( attr )
						axisIndexRoll = attr->asInt();
					else
						axisIndexRoll = -1;
					
				}
			}
		}
	}
	
	if( !found )
	{
// FIXME - The substring for the Wingman may not be correct.  I can't get the 
// Wingman to work under linux, so I have no real way to test it.
		if( strstr( joystickName, "SideWinder" ) != NULL )
			useBuiltInProfile( JoystickBindingsProfileSidewinder );
		else if( strstr( joystickName, "Wingman" ) != NULL )
			useBuiltInProfile( JoystickBindingsProfileLogitechWingmanExtreme3D );
		else if( strstr( joystickName, "Freedom" ) != NULL )
			useBuiltInProfile( JoystickBindingsProfileLogitechFreedom );
		else if( strstr( joystickName, "FireStorm" ) != NULL )
			useBuiltInProfile( JoystickBindingsProfileThrustmasterFirestorm );
		else
		{
			printf( "Warning: Unknown joystick type \"%s\".  Defaulting to Sidewinder bindings.\n",
				joystickName );
			useBuiltInProfile( JoystickBindingsProfileSidewinder );
		}
	}
}


void JoystickBindings::useBuiltInProfile( JoystickBindingsProfile profile )
{
	switch( profile )
	{
	case JoystickBindingsProfileLogitechWingmanExtreme3D:
		axisIndexXOffset = 0;
		axisIndexYOffset = 1;
		axisIndexZOffset = -1;
		axisIndexYaw = 2;
		axisIndexPitch = -1;
		axisIndexRoll = -1;
		buttonIndexToggleTether = 0;
		buttonIndexToggleIgnoreRotation = 5;
		buttonIndexJumpToTarget = 2;
		buttonIndexCycleEntsForward = 3;
		buttonIndexCycleEntsBackward = 4;
		buttonIndexCycleActiveView = -1;
		break;
	case JoystickBindingsProfileLogitechFreedom:
		axisIndexXOffset = 0;
		axisIndexYOffset = 1;
		axisIndexZOffset = 5;
		axisIndexYaw = 2;
		axisIndexPitch = 3;
		axisIndexRoll = -1;
		buttonIndexToggleTether = 0;
		buttonIndexToggleIgnoreRotation = 5;
		buttonIndexJumpToTarget = 1;
		buttonIndexCycleEntsForward = 3;
		buttonIndexCycleEntsBackward = 2;
		buttonIndexCycleActiveView = 6;
		break;
	case JoystickBindingsProfileThrustmasterFirestorm:
		axisIndexXOffset = 2;
		axisIndexYOffset = 3;
		axisIndexZOffset = 5;
		axisIndexYaw = 0;
		axisIndexPitch = 1;
		axisIndexRoll = 4;
		buttonIndexToggleTether = 0;
		buttonIndexToggleIgnoreRotation = 5;
		buttonIndexJumpToTarget = 1;
		buttonIndexCycleEntsForward = 3;
		buttonIndexCycleEntsBackward = 2;
		buttonIndexCycleActiveView = 6;
		break;
	case JoystickBindingsProfileSidewinder:
	default:
		axisIndexXOffset = 0;
		axisIndexYOffset = 1;
		axisIndexZOffset = -1;
		axisIndexYaw = 2;
		axisIndexPitch = 3;
		axisIndexRoll = -1;
		buttonIndexToggleTether = 0;
		buttonIndexToggleIgnoreRotation = 5;
		buttonIndexJumpToTarget = 1;
		buttonIndexCycleEntsForward = 3;
		buttonIndexCycleEntsBackward = 2;
		buttonIndexCycleActiveView = 6;
		break;
	}
}

