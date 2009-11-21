
#include "JoystickBindings.h"

using namespace CameraMgr;

JoystickBindings::JoystickBindings()
{
	setProfile( JoystickBindingsProfileSidewinder );
}

JoystickBindings::~JoystickBindings()
{
	
}

void JoystickBindings::setProfile( JoystickBindingsProfile profile )
{
	switch( profile )
	{
	case JoystickBindingsProfileLogitechWingmanExtreme3D:
		axisIndexForwardBackward = 1;
		axisIndexSideways = 0;
		axisIndexYaw = 2;
		axisIndexUpDown = -1;
		axisIndexThrottle = -1;
		buttonIndexToggleTether = 0;
		buttonIndexToggleIgnoreRotation = 5;
		buttonIndexJumpToTarget = 2;
		buttonIndexCycleEntsForward = 3;
		buttonIndexCycleEntsBackward = 4;
		break;
	case JoystickBindingsProfileLogitechFreedom:
		axisIndexForwardBackward = 1;
		axisIndexSideways = 0;
		axisIndexYaw = 2;
		axisIndexUpDown = 5;
		axisIndexThrottle = 3;
		buttonIndexToggleTether = 0;
		buttonIndexToggleIgnoreRotation = 5;
		buttonIndexJumpToTarget = 1;
		buttonIndexCycleEntsForward = 3;
		buttonIndexCycleEntsBackward = 2;
		break;
	case JoystickBindingsProfileSidewinder:
	default:
		axisIndexForwardBackward = 1;
		axisIndexSideways = 0;
		axisIndexYaw = 2;
		axisIndexUpDown = -1;
		axisIndexThrottle = 3;
		buttonIndexToggleTether = 0;
		buttonIndexToggleIgnoreRotation = 5;
		buttonIndexJumpToTarget = 1;
		buttonIndexCycleEntsForward = 3;
		buttonIndexCycleEntsBackward = 2;
		break;
	}
}

