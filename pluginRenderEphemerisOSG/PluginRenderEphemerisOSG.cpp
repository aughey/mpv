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
 *  FILENAME:   PluginRenderEphemerisOSG.cpp
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

#include <stdio.h>
#include <ios>
#include <iostream>
#include <string>

#include <osg/Vec3>
#include <osg/Depth>

#include "PluginRenderEphemerisOSG.h"

EXPORT_DYNAMIC_CLASS( PluginRenderEphemerisOSG )

// ================================================
// PluginRenderEphemerisOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderEphemerisOSG::PluginRenderEphemerisOSG() : Plugin() 
{
	name_ = "PluginRenderEphemerisOSG";

	// This plugin contains code from the CSP project.
	licenseInfo_.setLicense( LicenseInfo::LicenseGPL );
	licenseInfo_.setOrigin( "CSP+Boeing+AndrewSampson" );

	dependencies_.push_back( "PluginRenderOSG" );
	dependencies_.push_back( "PluginEphemerisModel" );
	rootNode = NULL;
	DefFileData = NULL;

	m_Latitude = NULL;
	m_LMST = NULL;
	sunAltitude = NULL;
	sunAzimuth = NULL;
	skyIntensity = NULL;
	moonAltitude = NULL;
	moonAzimuth = NULL;
	moonPhase = NULL;
	moonEarthShine = NULL;
	
	skyDomeSliceAngles = NULL;
	skyDomeColors = NULL;
	skyDomeNumWedges = NULL;
	skyDomeNumSlices = NULL;
	
	skyTransform = new FollowEyePointTransform;
	setupSkyTransform( skyTransform.get() );

	skyTransformWithCompression = new FollowEyePointTransformWithCompression;
	setupSkyTransform( skyTransformWithCompression.get() );

	moon = new MoonNode;
	stars = new StarDomeNode;
	sky = new SkyDomeNode;

	// isTransparent, disableLighting, compressWithAltitude, renderBin
	addSkyComponent( stars.get(), false, false, false, -5 );
	addSkyComponent( moon.get(),  true,  true,  false, -4 );
	addSkyComponent( sky.get(),   true,  true,  true,  -3 );
	
}


// ================================================
// ~PluginRenderEphemerisOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderEphemerisOSG::~PluginRenderEphemerisOSG() throw() 
{
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderEphemerisOSG::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{

	case SystemState::BlackboardRetrieve:
		// This state is for retrieving things from the blackboard

		bb_->get( "DefinitionData", DefFileData );

		// get the root node from the BB
		bb_->get( "RootNodeOSG", rootNode );

		skyBranchNode = new osg::Group;
		skyBranchNode->setName( "Sky Branch Node" );
		rootNode->addChild( skyBranchNode.get() );

		skyBranchNode->addChild( skyTransform.get() );
		skyBranchNode->addChild( skyTransformWithCompression.get() );

		bb_->get( "EphemerisEyepointLatitude", m_Latitude );

		bb_->get( "EphemerisLocalMeanSiderealTime", m_LMST );

		bb_->get( "EphemerisSunAltitude", sunAltitude );

		bb_->get( "EphemerisSunAzimuth", sunAzimuth );

		bb_->get( "EphemerisSkyIntensity", skyIntensity );

		bb_->get( "EphemerisMoonAltitude", moonAltitude );

		bb_->get( "EphemerisMoonAzimuth", moonAzimuth );

		bb_->get( "EphemerisMoonPhase", moonPhase );

		bb_->get( "EphemerisMoonEarthShine", moonEarthShine );

		bb_->get( "SkyDomeSliceAngles", skyDomeSliceAngles );

		bb_->get( "SkyDomeColors", skyDomeColors );

		bb_->get( "SkyDomeNumWedges", skyDomeNumWedges );

		bb_->get( "SkyDomeNumSlices", skyDomeNumSlices );
		
		if( skyDomeSliceAngles && skyDomeNumWedges && skyDomeNumSlices )
		{
			sky->init( *skyDomeNumWedges, *skyDomeNumSlices, skyDomeSliceAngles );
		}
		
		stars->init();

		break;

	case SystemState::ConfigurationProcess:
		getConfig();
		break;

	case SystemState::Operate:
	case SystemState::Debug:
		moon->updateMoon( *m_Latitude, *moonAltitude, *moonAzimuth, 
			*moonPhase, *moonEarthShine, 1738000.0, 383896832.0 );
			// FIXME - hard-coded dimensions for the moon

		stars->updateStars( *m_Latitude, *m_LMST, *skyIntensity );

		sky->updateSky( skyDomeColors );
		break;

	default:
		break;
	}
	
}


// ================================================
// getConfig
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderEphemerisOSG::getConfig()
{
	DefFileGroup *root = *DefFileData;
	if( root == NULL )
		return;
	
	DefFileGroup *ephemerisGroup = root->getGroupByURI( "/ephemeris/" );
	if( ephemerisGroup == NULL )
		return;
	
	DefFileAttrib *attr;

	attr = ephemerisGroup->getAttribute( "moon_image" );
	if( attr )
	{
		moon->loadTexture( attr->asString() );
	}

	
}


// ================================================
// setupSkyTransform
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderEphemerisOSG::setupSkyTransform( osg::Node *transform )
{
	// Turn off culling for the skydome.  This is important; with culling enabled, 
	// the sky might disappear when the eyepoint moves away from the origin.
	transform->setCullingActive(false);
	
	osg::StateSet *dstate = new osg::StateSet;

	dstate->setMode( GL_CULL_FACE, osg::StateAttribute::OFF );

	// clear the depth to the far plane.
	osg::Depth* depth = new osg::Depth;
	depth->setFunction(osg::Depth::ALWAYS);
	depth->setRange(1.0,1.0);   
	dstate->setAttributeAndModes(depth,osg::StateAttribute::ON );

	dstate->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
	dstate->setMode(GL_FOG, osg::StateAttribute::OFF);

/* These lines were in one of the CSPSky classes (Moon, specifically).  
I don't know if these are necessary; they don't seem to be. 
	dstate->setMode(GL_CLIP_PLANE0, osg::StateAttribute::OFF);
	dstate->setMode(GL_CLIP_PLANE1, osg::StateAttribute::OFF);
	dstate->setMode(GL_CLIP_PLANE2, osg::StateAttribute::OFF);
	dstate->setMode(GL_CLIP_PLANE3, osg::StateAttribute::OFF);
	dstate->setMode(GL_CLIP_PLANE4, osg::StateAttribute::OFF);
	dstate->setMode(GL_CLIP_PLANE5, osg::StateAttribute::OFF);*/

	// NOTE - render bin manipulation is done elsewhere now (addSkyComponent)

	transform->setStateSet( dstate );
}


// ================================================
// addSkyComponent
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderEphemerisOSG::addSkyComponent( osg::Node *skydome, 
	bool isTransparent, bool disableLighting, bool compressWithAltitude,
	int renderBin )
{
	
	osg::StateSet *dstate = skydome->getOrCreateStateSet();

	if( isTransparent )
	{
		dstate->setMode( GL_BLEND, osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE );
		dstate->setMode( GL_ALPHA_TEST, osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE );
	}
	else
	{
		dstate->setMode( GL_BLEND, osg::StateAttribute::OFF|osg::StateAttribute::OVERRIDE );
		dstate->setMode( GL_ALPHA_TEST, osg::StateAttribute::OFF|osg::StateAttribute::OVERRIDE );
	}
	
	if( disableLighting )
	{
		dstate->setMode( GL_LIGHTING, osg::StateAttribute::OFF|osg::StateAttribute::OVERRIDE );
	}
	
	if( compressWithAltitude )
		skyTransformWithCompression->addChild( skydome );
	else
		skyTransform->addChild( skydome );

	dstate->setRenderBinDetails( renderBin, "RenderBin" );
}


