#ifndef JOYSTICK_BINDINGS_H
#define JOYSTICK_BINDINGS_H

namespace CameraMgr
{

//=========================================================
//! 
//! 
class JoystickBindings
{
public:
	enum JoystickBindingsProfile
	{
		JoystickBindingsProfileSidewinder,
		JoystickBindingsProfileLogitechWingmanExtreme3D,
		JoystickBindingsProfileLogitechFreedom
	};
	
	JoystickBindings();
	~JoystickBindings();
	void setProfile( JoystickBindingsProfile profile );
	
	int axisIndexForwardBackward;
	int axisIndexSideways;
	int axisIndexYaw;
	int axisIndexUpDown;
	int axisIndexThrottle;
	
	int buttonIndexToggleTether;
	int buttonIndexToggleIgnoreRotation;
	int buttonIndexJumpToTarget;
	int buttonIndexCycleEntsForward;
	int buttonIndexCycleEntsBackward;
	
};

}

#endif
