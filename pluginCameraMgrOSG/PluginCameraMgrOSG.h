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
 *  FILENAME:   PluginCameraMgrOSG.h
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


#ifndef PLUGIN_CAMERA_MGR_OSG_H
#define PLUGIN_CAMERA_MGR_OSG_H

#include <map>
#include <list>
#include <osg/Vec3d>
#include <osg/Matrix>
#include <osgGA/GUIEventHandler>

#include "Plugin.h"
#include "Entity.h"
#include "View.h"
#include "TrackerParams.h"
#include "DefFileGroup.h"

#include "JoystickBindings.h"

using namespace CameraMgr;

//=========================================================
//! This plugin is responsible for updating the camera matrix.
//! It uses the ownship CIGI Entity to determine camera location 
//! and orientation.  Optionally, the camera position can be driven 
//! by user input (mouse, joystick) instead.
//! 
class PluginCameraMgrOSG : public Plugin 
{
public:

	//=========================================================
	//! General Constructor
	//! 
	PluginCameraMgrOSG();

	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginCameraMgrOSG() throw();
	
	//=========================================================
	//! The per-frame processing that this plugin performs
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );

	//=========================================================
	//! Copies the location and attitude of the first viewport's target entity 
	//! into the vectors used when in local-camera-control mode.  
	//! 
	void copyTargetEntOffsetToCumulativeTransform();

	void toggleLocalCameraControl();
	void toggleIgnoreEntityRotation();
	void cycleThroughTargetEnts( bool backward = false );

	//=========================================================
	//! dumps info about entities to the console
	//! 
	void listEntities();


private:
	//=========================================================
	//! The root of the def-file tree.  Retrieved from the blackboard.
	//! 
	DefFileGroup **configData;

	//=========================================================
	//! An entity container, containing all the entities.  
	//! Retrieved from the blackboard.
	//! 
	mpv::EntityContainer *allEntities;
	
	//=========================================================
	//! Map containing the view group parameters (provides additional 
	//! view transformation, etc).  Retrieved from the blackboard.
	//! 
	std::map< int, mpv::ViewGroupParams * > *viewGroupParamsMap;

	//=========================================================
	//! Map containing the view parameters (used for frustum setup,
	//! entity-following, etc).  Retrieved from the blackboard.
	//! 
	std::map< int, mpv::RefPtr<mpv::View> > *viewMap;

	//=========================================================
	//! Map of TrackerParams.  TrackerParams contain info from the Host 
	//! regarding motion trackers, such as boresight-enable, etc.  
	//! Retrieved from the blackboard.
	//! 
	std::map< int, TrackerParams * > *trackerParamsMap;
	
	//=========================================================
	//! A list of pointers to osgGA event handlers.  Retrieved from the 
	//! blackboard.
	//! 
	std::list< osg::ref_ptr<osgGA::GUIEventHandler> > *eventHandlerList;
	
	//=========================================================
	//! The camera matrices; incorporates rotation and translation.  Posted 
	//! to the blackboard.
	//! 
	std::map< int, osg::Matrix > camMtxMap;
	
	//=========================================================
	//! Convenience variable
	//! 
	osg::Vec3d xAxis;
	//=========================================================
	//! Convenience variable
	//! 
	osg::Vec3d yAxis;
	//=========================================================
	//! Convenience variable
	//! 
	osg::Vec3d zAxis;
	
	//=========================================================
	//! variable used for mouse camera manipulation
	//! 
	osg::Vec3d viewDeltaXYZ;
	//=========================================================
	//! variable used for mouse camera manipulation
	//! 
	osg::Vec3d viewDeltaYPR;
	//=========================================================
	//! variable used for mouse camera manipulation
	//! 
	osg::Vec3d viewCumulativeXYZ;
	//=========================================================
	//! variable used for mouse camera manipulation
	//! 
	osg::Vec3d viewCumulativeYPR;
	
	//=========================================================
	//! false means CIGI is in charge, true means mouse is in charge
	//! 
	int localCameraControl;
	
	//=========================================================
	//! if true, ignore entity pitch and roll when concatenating the 
	//! target entity's orientation
	//! 
	bool ignoreEntityRotation;
	
	//=========================================================
	//! A map of osg::Matrices, boresights a head tracker data by
	//! having a matrix to multiply before and after the current
	//! tracker data.
	std::map< int, osg::Matrix > trackerPreMap, trackerPostMap;
	
	//=========================================================
	//! retrieves configuration info from the config tree (width, 
	//! height, etc)
	//! 
	void getConfig();

	//=========================================================
	//! performs sanity-checking on a ypr triplet
	//! \param ypr - the yaw-pitch-roll triplet to check
	//! 
	void boundYPR( osg::Vec3d &ypr ) ;

	//=========================================================
	//! travels up the Entity hierarchy, concatenating the transform matrices 
	//! of the entity's parents and, lastly, that of the entity itself
	//! \param mtx - the matrix to manipulate
	//! \param ent - the entity to use
	//! 
	void transformMatrixToEntHierarchy( osg::Matrix &mtx, mpv::Entity *ent );

	//=========================================================
	//! Updates the view transformation matrices for each of the viewports.
	//! 
	void updateMatrices();

	//=========================================================
	//! Calculates the additional transformation (from, say, a head tracker) 
	//! to be applied to a given view.
	//! \param view - the view parameters for the view being manipulated
	//! \param camMtx - the current camera matrix to be manipulated
	//! 
	void calculateAdditionalViewTransformation( mpv::View *view, osg::Matrix &camMtx );

	//=========================================================
	//! an object for storing joystick button bindings
	//!
	JoystickBindings joystickBindings;

	
};



#endif

