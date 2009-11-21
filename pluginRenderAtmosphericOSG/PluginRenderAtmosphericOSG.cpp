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
 *  FILENAME:   PluginRenderAtmosphericOSG.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  create and manage global atmospheric effects like fog, etc.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  08/17/2004 Andrew Sampson                       MPV_CR_DR_1
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

#include <osg/Fog>
#include <osg/StateSet>

#include "PluginRenderAtmosphericOSG.h"

EXPORT_DYNAMIC_CLASS( PluginRenderAtmosphericOSG )

// ================================================
// PluginRenderAtmosphericOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderAtmosphericOSG::PluginRenderAtmosphericOSG() : Plugin() 
{
	name_ = "PluginRenderAtmosphericOSG";
	licenseInfo_.setLicense( LicenseInfo::LicenseGPL );
	licenseInfo_.setOrigin( "Boeing" );
	dependencies_.push_back( "PluginRenderOSG" );
	dependencies_.push_back( "PluginDefFileReader" );
	dependencies_.push_back( "PluginGlobalWeatherMgr" );
// libpluginEphemerisModel is optional
	
	rootNode = NULL;
	DefFileData = NULL;
	globalWeatherData = NULL;
	sunLight = NULL;
	moonLight = NULL;
	
	fogHasChanged = false;
	
	fogColor.set( 1., 1., 1., 1. );
	fogStart = 10000.0;
	fogEnd   = 40000.0;
}


// ================================================
// ~PluginRenderAtmosphericOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderAtmosphericOSG::~PluginRenderAtmosphericOSG() throw() 
{
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderAtmosphericOSG::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{

	case SystemState::BlackboardRetrieve:
		// This state is for retrieving things from the blackboard

		bb_->get( "DefinitionData", DefFileData );

		// get the root node from the BB
		bb_->get( "RootNodeOSG", rootNode );
		sceneStateSet = rootNode->getOrCreateStateSet();
		
		bb_->get( "GlobalWeather", globalWeatherData );
		
		// these two blackboard entries are from the ephemeris plugin, and are 
		// optional
		bb_->get( "SunLight", sunLight, false );
		bb_->get( "MoonLight", moonLight, false );

		break;

	case SystemState::ConfigurationProcess:
		getConfig();
		break;

	case SystemState::Operate:
	case SystemState::Debug:
		getWeatherState();
		getEphemerisState();
		updateScene();
		break;

	default:
		break;
	}
	
}


// ================================================
// getConfig
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderAtmosphericOSG::getConfig( void ) 
{
	DefFileGroup *root = *DefFileData;
	if(root == NULL)
		return;
	
	DefFileGroup *atmosphericGroup = root->getGroupByURI( "/atmospheric/" );
	if(atmosphericGroup == NULL)
		return;

	fogHasChanged = true;
	
	DefFileAttrib *attr;

	attr = atmosphericGroup->getAttribute( "fog_start" );
	if( attr )
	{
		fogStart = attr->asFloat();
	}

	attr = atmosphericGroup->getAttribute( "fog_end" );
	if( attr )
	{
		fogEnd = attr->asFloat();
	}

	attr = atmosphericGroup->getAttribute( "fog_color" );
	if( attr )
	{
		std::vector<float> c = attr->asFloats();
		for( unsigned int i = 0; i < 4 && i < c.size(); i++ )
			fogColor[i] = c[i];
	}

}


// ================================================
// getWeatherState
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderAtmosphericOSG::getWeatherState()
{
	if( globalWeatherData != NULL )
	{
		// Currently, the only information retrieved is the visibility 
		// range.  More attributes, such as humidity, could also be used.
		float currentVisibility = globalWeatherData->GetVisibilityRange();
		
		// fixme:
		// Wikipedia's entry on visibility (in the aviation sense) vaguely 
		// defines it as the distance at which an object of suitable 
		// dimensions can be recognized when observed against a contrasting 
		// background.  It is important to point out that this definition 
		// does not specify visibility as the distance of total opacity.
		// I don't know CIGI's definition of visibility.
		
		
		// floating point equality tests can be iffy, but I think we're OK here
		if( fogEnd != currentVisibility )
		{
			fogEnd = currentVisibility;
			fogHasChanged = true;
		}
	}
}


// ================================================
// getEphemerisState
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderAtmosphericOSG::getEphemerisState()
{
	if( moonLight != NULL && sunLight != NULL )
	{
		// Figure out which light is brighter, the sun or the moon.
		// Vect4's mag2() is used instead of mag() for efficiency reasons.
		MPVLight *lightSource = sunLight;
		if( moonLight->diffuse.mag2() > sunLight->diffuse.mag2() )
			lightSource = moonLight;
		
// fixme - should I actually implement desaturation?  just went 
// outside and watched the sun set... haze color was the same as 
// the "diffuse" light for the rest of the "scene".
		// Desaturate the color a bit.  At sunset, the scene light will turn 
		// orange.  If the scene light were applied directly to the fog color, 
		// we'd have orange fog.  Orange fog would look silly.
		// Might also want to play games with the fog color based on the 
		// current atmospheric conditions.  
		
		fogColor[0] = lightSource->diffuse.getx();
		fogColor[1] = lightSource->diffuse.gety();
		fogColor[2] = lightSource->diffuse.getz();
		fogColor[3] = lightSource->diffuse.getw();
		fogHasChanged = true;
	}
}


// ================================================
// updateScene
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderAtmosphericOSG::updateScene()
{
	if( fogHasChanged )
	{
		fogHasChanged = false;

		osg::Fog *fogStateAttr = 
			(osg::Fog *)sceneStateSet->getAttribute( osg::StateAttribute::FOG );
		if( fogStateAttr == NULL )
		{
			fogStateAttr = new osg::Fog;
		}

		fogStateAttr->setMode( osg::Fog::LINEAR );
		fogStateAttr->setStart( fogStart );
		fogStateAttr->setEnd( fogEnd );
		fogStateAttr->setColor( fogColor );

		sceneStateSet->setMode( GL_FOG, osg::StateAttribute::ON );
		sceneStateSet->setAttribute( fogStateAttr, osg::StateAttribute::ON );
	}
}
