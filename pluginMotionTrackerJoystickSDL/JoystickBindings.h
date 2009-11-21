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


#ifndef JOYSTICK_BINDINGS_H
#define JOYSTICK_BINDINGS_H

#include "DefFileGroup.h"

namespace JoystickMotionTracker
{

//=========================================================
//! Stores a set of mappings from hardware axes and buttons to logical 
//! axes and buttons
//! 
class JoystickBindings
{
public:
	JoystickBindings();
	~JoystickBindings();
	void setConfigData( DefFileGroup *configRoot );
	void setProfileByString( const char *joystickName );
	
	int axisIndexXOffset;
	int axisIndexYOffset;
	int axisIndexZOffset;
	int axisIndexYaw;
	int axisIndexPitch;
	int axisIndexRoll;
	
	int buttonIndexToggleTether;
	int buttonIndexToggleIgnoreRotation;
	int buttonIndexJumpToTarget;
	int buttonIndexCycleEntsForward;
	int buttonIndexCycleEntsBackward;
	int buttonIndexCycleActiveView;
	
private:
	enum JoystickBindingsProfile
	{
		JoystickBindingsProfileSidewinder,
		JoystickBindingsProfileLogitechWingmanExtreme3D,
		JoystickBindingsProfileLogitechFreedom,
		JoystickBindingsProfileThrustmasterFirestorm
	};
	
	void useBuiltInProfile( JoystickBindingsProfile profile );

	DefFileGroup *bindingsConfigGroup;
};

}

#endif
