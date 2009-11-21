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
 *  FILENAME:   PluginRenderDynamicLightOSG.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  manage the ambient lighting parameters, as reported by the ephemeris 
 *  plugin.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  05/04/2004 Andrew Sampson                       MPV_CR_DR_1
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

#include <osg/Switch>
#include <osg/PositionAttitudeTransform>
#include <osg/StateSet>
#include <osg/Geode>
#include <osg/Geometry>
#include <osgDB/ReadFile>
#include <osg/Light>

#include "PluginRenderDynamicLightOSG.h"

EXPORT_DYNAMIC_CLASS( PluginRenderDynamicLightOSG )

// ================================================
// PluginRenderDynamicLightOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderDynamicLightOSG::PluginRenderDynamicLightOSG() : Plugin() 
{
	name_ = "PluginRenderDynamicLightOSG";
	licenseInfo_.setLicense( LicenseInfo::LicenseGPL );
	licenseInfo_.setOrigin( "Boeing" );
	dependencies_.push_back( "PluginRenderOSG" );
	dependencies_.push_back( "PluginEphemerisModel" );
	
	rootNode = NULL;
	sunLight = NULL;
	moonLight = NULL;
}


// ================================================
// ~PluginRenderDynamicLightOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderDynamicLightOSG::~PluginRenderDynamicLightOSG() throw() 
{
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderDynamicLightOSG::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{
	case SystemState::BlackboardRetrieve:
		// This state is for retrieving things from the blackboard

		// get the root node from the BB
		bb_->get( "RootNodeOSG", rootNode );
		sceneStateSet = rootNode->getOrCreateStateSet();

		// get the lighting data from the BB
		bb_->get( "SunLight", sunLight );
		bb_->get( "MoonLight", moonLight );
		
		break;

	case SystemState::Operate:
	case SystemState::Debug:
		operate();
		break;

	default:
		break;
	}
	
}


// ================================================
// operate
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderDynamicLightOSG::operate( void ) 
{
	osg::Light *lightStateAttr = 
	(osg::Light *)sceneStateSet->getAttribute( osg::StateAttribute::LIGHT );
	if( lightStateAttr == NULL ) {
		lightStateAttr = new osg::Light;
	}
	
	lightStateAttr->setLightNum( 0 );
	
	// Figure out which light is brighter, the sun or the moon.
	// Vect4's mag2() is used instead of mag() for efficiency reasons.
	MPVLight *lightSource = sunLight;
	if( moonLight->diffuse.mag2() > sunLight->diffuse.mag2() )
		lightSource = moonLight;
	
	if( lightSource->directional )
	{
		// Using a 0 for the 'w' component of the light position coordinate 
		// will cause this light to be treated as a directional light
		// (ie like the sun, not like a street lamp)
		// The -1's are applied because the light source is given as a 
		// *direction*, but setPosition needs a *position*.
		lightStateAttr->setPosition( osg::Vec4( 
			-1.0 * lightSource->direction.getx(), 
			-1.0 * lightSource->direction.gety(), 
			-1.0 * lightSource->direction.getz(), 0.0 ) );
		lightStateAttr->setAmbient( osg::Vec4( 
			lightSource->ambient.getx(), 
			lightSource->ambient.gety(), 
			lightSource->ambient.getz(), 
			lightSource->ambient.getw() ) );
		lightStateAttr->setDiffuse( osg::Vec4( 
			lightSource->diffuse.getx(), 
			lightSource->diffuse.gety(), 
			lightSource->diffuse.getz(), 
			lightSource->diffuse.getw() ) );
		lightStateAttr->setSpecular( osg::Vec4( 
			lightSource->specular.getx(), 
			lightSource->specular.gety(), 
			lightSource->specular.getz(), 
			lightSource->specular.getw() ) );
	}
	else
	{
		// The sun/moon haven't been set as directional light sources?  
		// What gives?
		lightStateAttr->setPosition( osg::Vec4( 0.0, 0.0, 1.0, 0.0 ) );
		lightStateAttr->setAmbient( osg::Vec4( 0.8, 0.8, 0.8, 1.0 ) );
	}
	
	sceneStateSet->setAttribute( lightStateAttr, osg::StateAttribute::ON );
}


