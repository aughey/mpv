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
 *  FILENAME:   PluginRenderAmbientLightOSG.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  manage the ambient lighting parameters.
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
#include <osg/LightModel>

#include "PluginRenderAmbientLightOSG.h"

EXPORT_DYNAMIC_CLASS( PluginRenderAmbientLightOSG )

// ================================================
// PluginRenderAmbientLightOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderAmbientLightOSG::PluginRenderAmbientLightOSG() : Plugin(), 
	sunPos(0.0, 1.0, 0.0, 0.0),
	sunAmb(0.8f, 0.8f, 0.8f, 1.0f),
	sunDif(0.8f, 0.8f, 0.8f, 1.0f),
	sunSpc(0.05f, 0.05f, 0.05f, 1.0f),
	glbAmb(0.00f, 0.00f, 0.00f, 1.0f)
{
	name_ = "PluginRenderAmbientLightOSG";
	licenseInfo_.setLicense( LicenseInfo::LicenseGPL );
	licenseInfo_.setOrigin( "Boeing+GeneralDynamics" ); // GD provided getConfig
	dependencies_.push_back( "PluginRenderOSG" );
	rootNode = NULL;

}


// ================================================
// ~PluginRenderAmbientLightOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderAmbientLightOSG::~PluginRenderAmbientLightOSG() throw() 
{
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderAmbientLightOSG::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{

	case SystemState::BlackboardRetrieve:
		bb_->get( "DefinitionData", DefFileData );

		// get the root node from the BB
		bb_->get( "RootNodeOSG", rootNode );
		sceneStateSet = rootNode->getOrCreateStateSet();
		break;

	case SystemState::ConfigurationProcess:
		{
			getConfig();

			osg::Light *lightStateAttr =
				( osg::Light * ) sceneStateSet->getAttribute( osg::StateAttribute::LIGHT );
			if( lightStateAttr == NULL )
			{
				lightStateAttr = new osg::Light;
			}

			lightStateAttr->setLightNum( 0 );

			// using a 0 for the 'w' component of the light position coordinate
			// will cause this light to be treated as a directional light
			// (ie like the sun, not like a street lamp)
			// Note that +y seems to be "up"...
			//lightStateAttr->setPosition( osg::Vec4( .5, -1., 1., 0.0 ) );
			//lightStateAttr->setPosition( osg::Vec4( 0.0, 1.0, 0.0, 0.0 ) ); // orig

			// The ambient and diffuse light values are values that should
			// eventually be driven by the emphemeris model.  For now, we'll
			// just hard-code some ambient light in.
			//lightStateAttr->setAmbient( osg::Vec4( 0.8, 0.8, 0.8, 1.0 ) );
			//

			//
			// Create the Sun
			//
			lightStateAttr->setPosition( sunPos );
			lightStateAttr->setAmbient( sunAmb );
			lightStateAttr->setDiffuse( sunDif );
			lightStateAttr->setSpecular( sunSpc );
			sceneStateSet->setAttributeAndModes( lightStateAttr, osg::StateAttribute::ON );

			//
			// Create ambient LightModel
			//
			osg::LightModel* lightmodel = new osg::LightModel;
			lightmodel->setAmbientIntensity( glbAmb );
			sceneStateSet->setAttributeAndModes( lightmodel, osg::StateAttribute::ON );
			sceneStateSet->setMode( GL_LIGHTING, osg::StateAttribute::ON );

		}
		break;

	default:
		break;
	}

}


// ================================================
// getConfig
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderAmbientLightOSG::getConfig( void ) 
{
	DefFileGroup *root = *DefFileData;
	if(root == NULL)
		return;
	
	DefFileGroup *lightingGroup = root->getGroupByURI( "/lighting/" );
	if(lightingGroup == NULL)
		return;

	DefFileAttrib *attr;

	attr = lightingGroup->getAttribute( "sun_pos" );
	if( attr )
	{
		std::vector<float> c = attr->asFloats();
		for( unsigned int i = 0; i < 4 && i < c.size(); i++ )
			sunPos[i] = c[i];
	}

	attr = lightingGroup->getAttribute( "sun_ambient" );
	if( attr )
	{
		std::vector<float> c = attr->asFloats();
		for( unsigned int i = 0; i < 4 && i < c.size(); i++ )
			sunAmb[i] = c[i];
	}

	attr = lightingGroup->getAttribute( "sun_diffuse" );
	if( attr )
	{
		std::vector<float> c = attr->asFloats();
		for( unsigned int i = 0; i < 4 && i < c.size(); i++ )
			sunDif[i] = c[i];
	}

	attr = lightingGroup->getAttribute( "sun_specular" );
	if( attr )
	{
		std::vector<float> c = attr->asFloats();
		for( unsigned int i = 0; i < 4 && i < c.size(); i++ )
			sunSpc[i] = c[i];
	}

	attr = lightingGroup->getAttribute( "global_ambient" );
	if( attr )
	{
		std::vector<float> c = attr->asFloats();
		for( unsigned int i = 0; i < 4 && i < c.size(); i++ )
			glbAmb[i] = c[i];
	}


}

