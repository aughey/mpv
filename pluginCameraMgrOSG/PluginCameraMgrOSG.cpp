/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2006 The Boeing Company
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
 *  FILENAME:   PluginCameraMgrOSG.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  update the camera position.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  2006-11-05 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 *  2007-07-04 Andrew Sampson
 *      Ported to new plugin API.
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#ifdef WIN32
#define _USE_MATH_DEFINES
#include <float.h>
#define isnan _isnan
#endif

#include <math.h>
#include <iostream>

#include "PluginCameraMgrOSG.h"

using namespace mpv;

namespace CameraMgr
{

// KEYBOARD HANDLER ///////////////////////////////////////////////////

class KeyboardHandler: public osgGA::GUIEventHandler 
{
public:
	KeyboardHandler( PluginCameraMgrOSG *plug ) 
	{
		plugin = plug;
	}

	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &)
	{
		if( plugin == NULL )
			return false;
		
		if( ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN )
		{
			int key = ea.getKey();
			if( key == 'c' )
			{
				plugin->toggleLocalCameraControl();
				return true;
			}
			if( key == 'i' )
			{
				plugin->toggleIgnoreEntityRotation();
				return true;
			}
			if( key == 'v' )
			{
				plugin->copyTargetEntOffsetToCumulativeTransform();
				return true;
			}
			/*
			if( key == 'g' )
			{
				plugin->toggleCaptureEntLoc();
				return true;
			}
			*/
			if( key == 'e' )
			{
				plugin->listEntities();
				return true;
			}
			if( key == '[' )
			{
				// change target ent to prev ent
				plugin->cycleThroughTargetEnts( true );
				return true;
			}
			if( key == ']' )
			{
				// change target ent to next ent
				plugin->cycleThroughTargetEnts();
				return true;
			}
		}

		return false;
	}
	
	virtual void getUsage( osg::ApplicationUsage &usage ) const
	{
		usage.addKeyboardMouseBinding( "c", "Toggle camera tether mode (mouse versus host)" );
		usage.addKeyboardMouseBinding( "i", "Toggle camera pitch and roll values" );
		usage.addKeyboardMouseBinding( "v", "In untethered (mouse) camera mode, snaps camera to host position" );
		usage.addKeyboardMouseBinding( "e", "Print all active entities to console" );
		usage.addKeyboardMouseBinding( "[ and ]", "In tethered (host) camera mode, switches to next/previous entity" );
	}

private:
	PluginCameraMgrOSG *plugin;
};


// MOUSE HANDLER ///////////////////////////////////////////////////

#define PCM_METERS_PER_FRAME 10.0
#define PCM_DEGREES_PER_FRAME 1.0

class MouseHandler: public osgGA::GUIEventHandler 
{
public:
	MouseHandler( osg::Vec3d &xyz, osg::Vec3d &ypr ): viewDeltaXYZ(xyz), viewDeltaYPR(ypr) {}

	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &)
	{
		if( ea.getEventType() == osgGA::GUIEventAdapter::PUSH ) {
			// clear viewDeltas
			viewDeltaXYZ.set( 0., 0., 0. );
			viewDeltaYPR.set( 0., 0., 0. );
			// remember axis values
			prevAxisValues.set( ea.getXnormalized(), ea.getYnormalized(), 0., 0. );
			return true;
		}
		else if( ea.getEventType() == osgGA::GUIEventAdapter::DRAG ) {
			osg::Vec4d currAxisValues( ea.getXnormalized(), ea.getYnormalized(), 0., 0. );
			osg::Vec4d axisDelta = currAxisValues - prevAxisValues;
			calculateViewDeltas( axisDelta, ea.getButtonMask() );
			return true;
		}
		else if( ea.getEventType() == osgGA::GUIEventAdapter::RELEASE ) {
			// clear viewDeltas
			viewDeltaXYZ.set( 0., 0., 0. );
			viewDeltaYPR.set( 0., 0., 0. );
			return true;
		}

		return false;
	}

	virtual void getUsage( osg::ApplicationUsage &usage ) const
	{
		usage.addKeyboardMouseBinding( "Left mouse", "In untethered mode, pan and tilt camera" );
		usage.addKeyboardMouseBinding( "Middle mouse", "In untethered mode, move camera forward/backward" );
		usage.addKeyboardMouseBinding( "Right mouse", "In untethered mode, move camera left/right, up/down" );
	}

private:
	osg::Vec3d &viewDeltaXYZ;
	osg::Vec3d &viewDeltaYPR;
	osg::Vec4d prevAxisValues;
	
	// use axisDelta and buttonmask to determine viewDeltas
	void calculateViewDeltas( osg::Vec4d &axisDelta, unsigned int btnMask ) {
		
		// middle button (or both l&r)
		if( ( (btnMask & osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON) &&
			(btnMask & osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON) ) || 
			(btnMask & osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON) )
		{
			// mouse left/right: rotate yaw
			viewDeltaYPR[0] = PCM_DEGREES_PER_FRAME * axisDelta[0];
			// mouse up/down: move forward/backward
			viewDeltaXYZ[1] = PCM_METERS_PER_FRAME * axisDelta[1];
		}
		// left button
		else if( btnMask & osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON )
		{
			// mouse left/right: rotate yaw
			viewDeltaYPR[0] = PCM_DEGREES_PER_FRAME * axisDelta[0];
			// mouse up/down: rotate pitch
			viewDeltaYPR[1] = PCM_DEGREES_PER_FRAME * axisDelta[1];
		}
		// right button
		else if( btnMask & osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON )
		{
			// mouse left/right: strafe left/right
			viewDeltaXYZ[0] = PCM_METERS_PER_FRAME * axisDelta[0];
			// mouse up/down: move up/down
			viewDeltaXYZ[2] = PCM_METERS_PER_FRAME * axisDelta[1];
		}
	}
};



// JOYSTICK HANDLER ///////////////////////////////////////////////////

class JoystickHandler: public osgGA::GUIEventHandler 
{
public:
	JoystickHandler( PluginCameraMgrOSG *plug, osg::Vec3d &xyz, osg::Vec3d &ypr, JoystickBindings *jb ): 
		plugin(plug), viewDeltaXYZ(xyz), viewDeltaYPR(ypr), joystickBindings(jb) {}

	bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &)
	{
#if 0
// this code is disabled until there is some way to send/recv joystick events via osgGA -ADS
		const osgSDL::SDLEventAdapter *sdl_ea = 
			dynamic_cast<const osgSDL::SDLEventAdapter *>( &ea );
		bool handled = false;
		// osgGA has no concept of joystick events, only keyboard and mouse 
		// events.  In order to retrieve any joystick event info, we need to 
		// get at the SDL events directly, and that requires casting ea to a 
		// SDLEventAdapter.
		if( sdl_ea )
		{
			const SDL_Event &sdlEvent = sdl_ea->getSDLEvent();
			switch( sdlEvent.type )
			{
			case SDL_JOYAXISMOTION:
				{
				const SDL_JoyAxisEvent &jaEvent = sdlEvent.jaxis;
				float axisValue = (float)jaEvent.value / 32770.;
				if( jaEvent.axis == joystickBindings->axisIndexSideways )
					viewDeltaXYZ[0] = PCM_METERS_PER_FRAME * axisValue;
				else if( jaEvent.axis == joystickBindings->axisIndexForwardBackward )
					viewDeltaXYZ[1] = PCM_METERS_PER_FRAME * axisValue * -1.0;
				else if( jaEvent.axis == joystickBindings->axisIndexYaw )
					viewDeltaYPR[0] = PCM_DEGREES_PER_FRAME * axisValue;
				else if( jaEvent.axis == joystickBindings->axisIndexUpDown )
					viewDeltaXYZ[2] = PCM_METERS_PER_FRAME * axisValue * -1.0;
				
				handled = true;
				}
				break;
			case SDL_JOYHATMOTION:
				{
				const SDL_JoyHatEvent &jhEvent = sdlEvent.jhat;
				if( true /* jhEvent.hat == 0 */)
				{
					if( jhEvent.value == SDL_HAT_CENTERED )
					{
						//viewDeltaXYZ.set( 0., 0., 0. );
						viewDeltaXYZ[0] = 0.;
						viewDeltaXYZ[2] = 0.;
					}
					else
					{
						if( jhEvent.value & SDL_HAT_UP )
							viewDeltaXYZ[2] = PCM_METERS_PER_FRAME * 1.;
						if( jhEvent.value & SDL_HAT_RIGHT )
							viewDeltaXYZ[0] = PCM_METERS_PER_FRAME * 1.;
						if( jhEvent.value & SDL_HAT_DOWN )
							viewDeltaXYZ[2] = PCM_METERS_PER_FRAME * -1.;
						if( jhEvent.value & SDL_HAT_LEFT )
							viewDeltaXYZ[0] = PCM_METERS_PER_FRAME * -1.;
					}
				}
				
				handled = true;
				}
				break;
			case SDL_JOYBUTTONDOWN:
				{
				const SDL_JoyButtonEvent &jbEvent = sdlEvent.jbutton;
				if( jbEvent.button == joystickBindings->buttonIndexToggleTether )
					plugin->toggleLocalCameraControl();
				else if( jbEvent.button == joystickBindings->buttonIndexToggleIgnoreRotation )
					plugin->toggleIgnoreEntityRotation();
				else if( jbEvent.button == joystickBindings->buttonIndexJumpToTarget )
					plugin->copyTargetEntOffsetToCumulativeTransform();
				else if( jbEvent.button == joystickBindings->buttonIndexCycleEntsBackward )
					// change target ent to prev ent
					plugin->cycleThroughTargetEnts( true );
				else if( jbEvent.button == joystickBindings->buttonIndexCycleEntsForward )
					// change target ent to next ent
					plugin->cycleThroughTargetEnts();
				
				handled = true;
				}
				break;
			case SDL_JOYBUTTONUP:
//				printf( "joy button up\n" );
				handled = true;
				break;
			default:
				break;
			}
		}
		
		return handled;
#else
		return false;
#endif
	}

private:
	PluginCameraMgrOSG *plugin;
	osg::Vec3d &viewDeltaXYZ;
	osg::Vec3d &viewDeltaYPR;
	JoystickBindings *joystickBindings;
};

} // namespace







// PLUGINRENDERCAMERAOSG //////////////////////////////////////////////

EXPORT_DYNAMIC_CLASS( PluginCameraMgrOSG )

// ================================================
// PluginCameraMgrOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginCameraMgrOSG::PluginCameraMgrOSG() : Plugin() 
{
	name_ = "PluginCameraMgrOSG";
	licenseInfo_.setLicense( LicenseInfo::LicenseGPL );
	licenseInfo_.setOrigin( "Boeing" );
	dependencies_.push_back( "PluginEntityMgr" );
	dependencies_.push_back( "PluginViewMgr" );
	dependencies_.push_back( "PluginMotionTrackerMgr" );
	dependencies_.push_back( "PluginUserInputMgrOSGGA" );

	configData = NULL;
	allEntities = NULL;
	viewGroupParamsMap = NULL;
	viewMap = NULL;
	trackerParamsMap = NULL;
	eventHandlerList = NULL;

	xAxis.set( 1.0, 0.0, 0.0 );
	yAxis.set( 0.0, 1.0, 0.0 );
	zAxis.set( 0.0, 0.0, 1.0 );
	viewCumulativeXYZ.set( 0.0, 0.0, 0.0 );
	viewCumulativeYPR.set( 0.0, 0.0, 0.0 );
	
	localCameraControl = 0;
	ignoreEntityRotation = false;
}


// ================================================
// ~PluginCameraMgrOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginCameraMgrOSG::~PluginCameraMgrOSG() throw() 
{

}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginCameraMgrOSG::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{
	
	case SystemState::BlackboardPost:

		// post the camera matrices to the blackboard
		bb_->put( "CameraMatrixMap", &camMtxMap );

		break;

	case SystemState::BlackboardRetrieve:

		bb_->get( "DefinitionData", configData );

		// get the entity list from the BB
		bb_->get( "AllEntities", allEntities );

		// get the view parameters
		bb_->get( "ViewMap", viewMap );
		bb_->get( "ViewGroupParamsMap", viewGroupParamsMap );
		
		// get the motion tracker parameters
		bb_->get( "TrackerParamsMap", trackerParamsMap );
		
		// get the list of user input callbacks
		bb_->get( "GUIEventHandlerList", eventHandlerList );
		
		if( eventHandlerList )
		{
			eventHandlerList->push_back( new CameraMgr::KeyboardHandler( this ) );
			eventHandlerList->push_back( new CameraMgr::MouseHandler( viewDeltaXYZ, viewDeltaYPR ) );
			eventHandlerList->push_back( new CameraMgr::JoystickHandler( this, viewDeltaXYZ, viewDeltaYPR, &joystickBindings ) );
		}
		
		break;

	case SystemState::ConfigurationProcess:
		getConfig();
		break;

	case SystemState::Operate:
	case SystemState::Debug:
		// some testing has shown that is is less confusing for people using the 
		// stealth viewer functionality if the "free mode" position is constantly 
		// updated when in "tether mode"
		if( !localCameraControl )
			copyTargetEntOffsetToCumulativeTransform();

		updateMatrices();
		break;

	default:
		break;
	}
	
}


// ================================================
// getConfig
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginCameraMgrOSG::getConfig()
{
	if( configData == NULL || *configData == NULL )
		return;
	
	// fixme - load config for joystick bindings?

/*
// Fixme - the substrings for the SideWinder and Freedom are correct, but 
// the substring for the Wingman may not be correct.  I can't get the 
// Wingman to work under linux, so I have no real way to test it.
				const char * joyname = SDL_JoystickName(0);
				if( strstr( joyname, "SideWinder" ) != NULL )
					joystickBindings.setProfile( JoystickBindings::JoystickBindingsProfileSidewinder );
				else if( strstr( joyname, "Wingman" ) != NULL )
					joystickBindings.setProfile( JoystickBindings::JoystickBindingsProfileLogitechWingmanExtreme3D );
				else if( strstr( joyname, "Freedom" ) != NULL )
					joystickBindings.setProfile( JoystickBindings::JoystickBindingsProfileLogitechFreedom );
*/
}


// ================================================
// boundYPR
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginCameraMgrOSG::boundYPR( osg::Vec3d &ypr ) 
{
	// we will bound the angles to valid ranges: 
	// yaw 0-360, pitch +-89.0, roll 0
	
	while( ypr[0] < 0.0 ) 
		ypr[0] += 360.0;
	
	while( ypr[0] > 360.0 ) 
		ypr[0] -= 360.0;
	
	if( ypr[1] < -89.0 ) 
		ypr[1] = -89.0;
	
	if( ypr[1] > 89.0 ) 
		ypr[1] = 89.0;
	
	ypr[2] = 0.0;
}


// ================================================
// transformMatrixToEntHierarchy
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginCameraMgrOSG::transformMatrixToEntHierarchy( 
	osg::Matrix &mtx, Entity *ent )
{
	if( ent == NULL )
		return;

	// walk up the hierarchy
	transformMatrixToEntHierarchy( mtx, ent->getParent() );

	const CoordinateSet &dbCoordinate = ent->getPositionDB();

	mtx *= osg::Matrix::translate( 
		-1.0 * dbCoordinate.LatX,
		-1.0 * dbCoordinate.LonY,
		-1.0 * dbCoordinate.AltZ );

	if( ignoreEntityRotation )
		mtx *= osg::Matrix::rotate( 
			osg::DegreesToRadians(dbCoordinate.Yaw), zAxis ); 
	else
		mtx *= osg::Matrix::rotate( 
			osg::DegreesToRadians(dbCoordinate.Yaw), zAxis,
			osg::DegreesToRadians(dbCoordinate.Pitch * -1.0 ), xAxis,
			osg::DegreesToRadians(dbCoordinate.Roll * -1.0 ), yAxis ); 
}


// ================================================
// updateMatrices
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginCameraMgrOSG::updateMatrices()
{
	int targetEntID = 0;
	ViewGroupParams *viewGroup = NULL;
	bool useViewGroupOffsets = false;

	std::map< int, RefPtr<View> >::iterator vpIter = viewMap->begin();
	for( ; vpIter != viewMap->end(); vpIter++ )
	{
		View *view = (*vpIter).second.get();
		if( view == NULL )
			continue;
		
		osg::Matrix &camMtx = camMtxMap[view->getID()];
		camMtx.makeIdentity();

		// set the target entity
		targetEntID = view->getEntityID();

		viewGroup = NULL;
		useViewGroupOffsets = false;

		if( viewGroupParamsMap )
		{
			viewGroup = (*viewGroupParamsMap)[view->getGroupID()];

			// enable the view group's offset and rotate, provided the 
			// view is part of a view group
			if( view->getGroupID() != 0 && viewGroup )
			{
				useViewGroupOffsets = true;
				targetEntID = viewGroup->entityID;
			}
		}

		// use the mouse/joystick to update the view matrix
		if( localCameraControl ) 
		{

			viewCumulativeYPR += viewDeltaYPR;
			// for viewCumulativeYPR, we will bound the angles to valid ranges
			boundYPR( viewCumulativeYPR );
			
			// We want the camera to move in the direction it is pointed, so we 
			// need to rotate the delta vector so that it points in the same 
			// direction as the camera.  We'll do that by multiplying the delta 
			// vector by a rotation matrix.  It has been decided that, when the 
			// user wishes to go "forward", the altitude should not change.  
			// Hence, the only rotation component included in the rotation matrix 
			// will be yaw.  You can uncomment the pitch component if you want to 
			// change this behavior.
			osg::Matrix rotateMtx;
			rotateMtx.makeIdentity();
			rotateMtx *= osg::Matrix::rotate( 
				osg::DegreesToRadians(viewCumulativeYPR[0] * -1.0 ), zAxis,
				osg::DegreesToRadians(/* we want pitch to be ignored viewCumulativeYPR[1] * -1.0 */ 0.0 ), xAxis,
				osg::DegreesToRadians(/* we definitely want roll to be ignored viewCumulativeYPR[2] */ 0.0 ), yAxis );

			osg::Vec3d addToVCXYZ = osg::Matrix::transform3x3( viewDeltaXYZ, rotateMtx );
			
			// add the rotated delta to the cumulative camera offset
			viewCumulativeXYZ += addToVCXYZ;
			
			// move the camera according to viewCumulativeXYZ
			camMtx *= osg::Matrix::translate( 
				-1.0 * viewCumulativeXYZ[0],
				-1.0 * viewCumulativeXYZ[1],
				-1.0 * viewCumulativeXYZ[2] );
			
			// rotate the camera according to viewCumulativeYPR
			camMtx *= osg::Matrix::rotate( 
				osg::DegreesToRadians(-90.0), xAxis );
			camMtx *= osg::Matrix::rotate( 
				osg::DegreesToRadians(viewCumulativeYPR[0]), yAxis,
				osg::DegreesToRadians(viewCumulativeYPR[1] * -1.0 ), xAxis,
				osg::DegreesToRadians(viewCumulativeYPR[2]), zAxis );


		}
		// use the targetEnt's location to update the view matrix
		else
		{

			// Note - targetEnt may be NULL
			Entity *targetEnt = allEntities->findEntity( targetEntID );

			// TARGET TRANSFORMATION AND ROTATION ///////////////////////////////
			if( targetEnt != NULL )
			{
				/*std::cout << "targetEnt is at " << 
					targetEnt->GetdbXPos() << " " << 
					targetEnt->GetdbYPos() << " " << 
					targetEnt->GetdbZPos() << std::endl;*/

				transformMatrixToEntHierarchy( camMtx, targetEnt );
			}

			// VIEW GROUP TRANSFORMATION AND ROTATION ///////////////////////////
			if( useViewGroupOffsets )
			{
			camMtx *= osg::Matrix::translate( 
				(double)(-1.0 * viewGroup->groupOffset[0]),
				(double)(-1.0 * viewGroup->groupOffset[1]),
				(double)(-1.0 * viewGroup->groupOffset[2]) );

			camMtx *= osg::Matrix::rotate( 
				osg::DegreesToRadians((double)(viewGroup->groupRotate[0])), zAxis,
				osg::DegreesToRadians((double)(viewGroup->groupRotate[1] * -1.0 )), xAxis,
				osg::DegreesToRadians((double)(viewGroup->groupRotate[2] * -1.0 )), yAxis );
			}
			

			// VIEW TRANSFORMATION AND ROTATION /////////////////////////////////
			Vect3 offset = view->getViewOffset();
			camMtx *= osg::Matrix::translate( 
				(double)(-1.0 * offset.getx()),
				(double)(-1.0 * offset.gety()),
				(double)(-1.0 * offset.getz()) );

			Vect3 rotation = view->getViewRotate();
			camMtx *= osg::Matrix::rotate( 
				osg::DegreesToRadians((double)(rotation.getx())), zAxis,
				osg::DegreesToRadians((double)(rotation.gety() * -1.0 )), xAxis,
				osg::DegreesToRadians((double)(rotation.getz() * -1.0 )), yAxis );


			// ADDITIONAL TRANSFORMATION AND ROTATION ///////////////////////////
			calculateAdditionalViewTransformation( view, camMtx );

			// The OSG camera, by default, points along the -Z axis.  We need 
			// to rotate it.
			camMtx *= osg::Matrix::rotate( 
				osg::DegreesToRadians(-90.0), xAxis );


		}

	}
}


// ================================================
// copyTargetEntOffsetToCumulativeTransform
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginCameraMgrOSG::copyTargetEntOffsetToCumulativeTransform()
{
	std::map< int, RefPtr<View> >::iterator vpIter = viewMap->begin();
	if( vpIter == viewMap->end() )
		return;

	View *view = (*vpIter).second.get();
	int targetEntID = view->getEntityID();

	Entity *targetEnt = allEntities->findEntity( targetEntID );
	if( targetEnt == NULL )
		return;
	targetEnt = targetEnt->getTopLevelParent();
	
	const CoordinateSet &dbCoordinate = targetEnt->getPositionDB();
	viewCumulativeXYZ[0] = dbCoordinate.LatX;
	viewCumulativeXYZ[1] = dbCoordinate.LonY;
	viewCumulativeXYZ[2] = dbCoordinate.AltZ;

	viewCumulativeYPR[0] = dbCoordinate.Yaw;
	viewCumulativeYPR[1] = dbCoordinate.Pitch;
	viewCumulativeYPR[2] = dbCoordinate.Roll;
	
}


// ================================================
// toggleLocalCameraControl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginCameraMgrOSG::toggleLocalCameraControl()
{
	localCameraControl = !localCameraControl;
}


// ================================================
// toggleIgnoreEntityRotation
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginCameraMgrOSG::toggleIgnoreEntityRotation()
{
	ignoreEntityRotation = !ignoreEntityRotation;
}


// ================================================
// cycleThroughTargetEnts
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginCameraMgrOSG::cycleThroughTargetEnts( bool backward )
{
	std::map< int, RefPtr<View> >::iterator vpIter = viewMap->begin();
	if( vpIter == viewMap->end() )
	{
		std::cout << "Error in PluginCameraMgrOSG::cycleThroughTargetEnts: there are no views.\n";
		return;
	}

	// modify only the first view (even if multiple views are present)
	View *view = (*vpIter).second.get();
	
	EntityContainer::EntityIteratorPair iterPair = allEntities->getEntities();
	EntityContainer::EntityMap::iterator eIter;
	for( eIter = iterPair.first; eIter != iterPair.second; eIter++ )
	{
		Entity *ent = eIter->second.get();
		if( ent->getID() == view->getEntityID() ) 
		{
			if( backward )
			{
				if( eIter == iterPair.first )
				{
					// wrap around
					eIter = iterPair.second;
				}
				eIter--;
			}
			else
			{
				eIter++;
				if( eIter == iterPair.second )
				{
					// wrap around
					eIter = iterPair.first;
				}
			}
			view->setEntityID( eIter->second->getID() );
			break;
		}
	}
}


// ================================================
// listEntities
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginCameraMgrOSG::listEntities() 
{
	// fixme - this should be moved to the entity-rendering plugin once a standalone user-input plugin is written
	
	// heading at top of table
	std::cout << "---------- ENTITY LIST ----------\n"
	<< "ID\t"
	<< "Type\t"
	<< "Parent's ID\t"
	<< "X position\t"
	<< "Y position\t"
	<< "Z position\t"
	<< "Yaw\t"
	<< "Pitch\t"
	<< "Roll\t"
	<< "\n";

	EntityContainer::EntityIteratorPair iterPair = allEntities->getEntities();
	EntityContainer::EntityMap::iterator eIter;
	for( eIter = iterPair.first; eIter != iterPair.second; eIter++ )
	{
		Entity *ent = eIter->second.get();

		std::cout << ent->getID() << "\t";
		std::cout << ent->getType() << "\t" ;
		std::cout << ent->getParentID() << "\t" ;

		const CoordinateSet &dbCoordinate = ent->getPositionDB();
		std::cout << dbCoordinate.LatX << "\t" 
		          << dbCoordinate.LonY << "\t" 
		          << dbCoordinate.AltZ << "\t" ;

		std::cout << dbCoordinate.Yaw << "\t" 
		          << dbCoordinate.Pitch << "\t" 
		          << dbCoordinate.Roll << "\t" ;

		std::cout << "\n";
	}
	std::cout << "------ END ENTITY LIST ----------\n\n";
	
}


// ================================================
// calculateAdditionalViewTransformation
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginCameraMgrOSG::calculateAdditionalViewTransformation( View *view,
	osg::Matrix &camMtx)
{

	TrackerParams *trackerParams = NULL;
	
	// check the list of motion tracker params to see if any of them affects 
	// this view
	if( trackerParamsMap )
	{
		std::map< int, TrackerParams * >::iterator iter;
		for( iter = trackerParamsMap->begin(); 
			iter != trackerParamsMap->end(); iter++ )
		{
			TrackerParams *tp = (*iter).second;
			
			// skip over trackers that do not exist
			if( tp == NULL )
				continue;

			// skip over trackers that are not enabled
			if( !tp->enableTracker )
				continue;
			
			if( tp->viewGroupID == 0 )
			{
				if( tp->viewID == view->getID() )
					trackerParams = tp;
			}
			else
			{
				// fixme - trackers which affect view groups are ignored!
			}
			
		}
	}
	
	// bail if there's no tracker associated with this view
	if( trackerParams == NULL )
		return;
	
	// if the tracker's transform matrix is OK, apply it to the camera matrix
	if( trackerParams->transformMtxValid )
	{
		osg::Matrix transformMtx( trackerParams->transformMtx );
		camMtx *= transformMtx;
	}
}


