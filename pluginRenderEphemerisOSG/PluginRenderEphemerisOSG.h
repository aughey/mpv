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
 *  FILENAME:   PluginRenderEphemerisOSG.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  create and manage the sky dome.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  2006-02-18 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 *
 *  2007-07-15 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#ifndef PLUGIN_RENDER_EPHEMERIS_OSG_H
#define PLUGIN_RENDER_EPHEMERIS_OSG_H

#include <list>

#include <osg/Group>

#include "Plugin.h"
#include "DefFileGroup.h"

#include "FollowEyePointTransform.h"
#include "MoonNode.h"
#include "StarDomeNode.h"
#include "SkyDomeNode.h"

//=========================================================
//! This plugin is responsible for creating and managing the sky dome.
//! 
class PluginRenderEphemerisOSG : public Plugin 
{
public:

	//=========================================================
	//! General Constructor
	//! 
	PluginRenderEphemerisOSG();

	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginRenderEphemerisOSG() throw();
	
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
	//! Local Mean Sidereal Time.  Units: unknown.  
	//! Retrieved from the blackboard.
	//!
	double *m_LMST;
	
	//=========================================================
	//! Altitude of the sun in the sky.  Units: radians.  
	//! Retrieved from the blackboard.
	//!
	double *m_Latitude;
	
	//=========================================================
	//! Altitude of the sun in the sky.  Units: radians.  
	//! Currently unused.
	//! Retrieved from the blackboard.
	//!
	double *sunAltitude;
	
	//=========================================================
	//! Azimuth of the sun in the sky.  Units: radians.  
	//! Currently unused.
	//! Retrieved from the blackboard.
	//!
	double *sunAzimuth;
	
	//=========================================================
	//! Average light intensity of the sky dome.  Units: unknown.  
	//! Retrieved from the blackboard.
	//!
	float *skyIntensity;
	
	//=========================================================
	//! Altitude of the moon in the sky.  Units: radians.  
	//! Retrieved from the blackboard.
	//!
	double *moonAltitude;
	
	//=========================================================
	//! Azimuth of the moon in the sky.  Units: radians.  
	//! Retrieved from the blackboard.
	//!
	double *moonAzimuth;
	
	//=========================================================
	//! The phase of the moon.  Units: unknown.  
	//! Retrieved from the blackboard.
	//!
	double *moonPhase;
	
	//=========================================================
	//! The amount of light reflected off of the earth toward the moon, 
	//! then reflected back toward the earth.  Only noticable on the 
	//! dark parts of the moon, not illuminated directly by the sun.
	//! Units: unknown.  Retrieved from the blackboard.
	//!
	float *moonEarthShine;
	
	//=========================================================
	//! An array of angles, which indicates the vertical spacing of the 
	//! sampling points for the sky dome colors.  This array is needed because 
	//! the vertical spacing of the sample points, unlike the horizontal 
	//! spacing, is not uniform.  The length of the array is indicated by 
	//! skyDomeNumSlices.  Retrieved from the blackboard.
	//!
	float *skyDomeSliceAngles;
	
	//=========================================================
	//! The skydome color array.  The array consists of a number of color 
	//! quadruplets (rgba).  The number of quadruplets is indicated by 
	//! skyDomeNumWedges and skyDomeNumSlices.  Retrieved from the blackboard.
	//!
	float *skyDomeColors;
	
	//=========================================================
	//! The number of color samples around the perimeter of the sky dome. 
	//! This information is needed in order to make use of the sky dome 
	//! color array.  Retrieved from the blackboard.
	//!
	int *skyDomeNumWedges;
	
	//=========================================================
	//! The number of color samples along the vertical axis of the sky dome. 
	//! This information is needed in order to make use of the sky dome 
	//! color array.  Retrieved from the blackboard.
	//!
	int *skyDomeNumSlices;
	
	//=========================================================
	//! The sky branch of the scene graph.  Attached to the scene root.
	//! 
	osg::ref_ptr<osg::Group> skyBranchNode;
	
	//=========================================================
	//! Transform node which keeps the sky domes centered around the 
	//! eyepoint.  This prevents the eyepoint from escaping the skydome, etc.
	//! 
	osg::ref_ptr< FollowEyePointTransform > skyTransform;

	//=========================================================
	//! Transform node which keeps the sky domes centered around the 
	//! eyepoint.  This prevents the eyepoint from escaping the skydome, etc.
	//! This node will also "compress" the skydome as the eyepoint's 
	//! altitude increases.
	//! 
	osg::ref_ptr< FollowEyePointTransformWithCompression > skyTransformWithCompression;

	//=========================================================
	//! M O O N that spells Tom.
	//! 
	osg::ref_ptr< MoonNode > moon;
	
	//=========================================================
	//! Star dome
	//! 
	osg::ref_ptr< StarDomeNode > stars;
	
	//=========================================================
	//! Sky dome
	//! 
	osg::ref_ptr< SkyDomeNode > sky;

	//=========================================================
	//! Configures a node for use as a sky dome; depth testing is disabled, 
	//! culling is disabled, and the node is placed in an appropriate 
	//! render bin.
	//! 
	void setupSkyTransform( osg::Node *transform );

	//=========================================================
	//! Attaches a sky dome node to the scene
	//! 
	void addSkyComponent( osg::Node *skydome, 
		bool isTransparent, bool disableLighting, 
		bool compressWithAltitude, int renderBin );

	//=========================================================
	//! Retrieves configuration data from DefFileData
	//! 
	void getConfig( void );
};


#endif

