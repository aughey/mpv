/** <pre>
 *  Plugin to incorporate Video4Linux input (tv capture cards and the like)
 *  into the MPV scene.
 *  Copyright (c) 2006 David Fries
 *  
 *  This component is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; specifically 
 *  version 2.1 of the License.
 *  
 *  This software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *  
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this software; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *  
 *  
 *  Initial Release: 2006-XX-XX David Fries
 *
 *  2007-07-21 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 * </pre>
 */


#include <iostream>

#include <osg/MatrixTransform>
#include <osg/Projection>

#include "PluginRenderTV.h"
#include "osgTV.h"
#include "BuildRectangle.h"


EXPORT_DYNAMIC_CLASS( PluginRenderTV )

// ================================================
// PluginRenderTV
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderTV::PluginRenderTV() : Plugin() 
{
	name_ = "PluginRenderTV";
	licenseInfo_.setLicense( LicenseInfo::LicenseLGPL );
	licenseInfo_.setOrigin( "DavidFries" );
	dependencies_.push_back( "PluginRenderOSG" );
	dependencies_.push_back( "PluginDefFileReader" );

	Width=Height=TextureHeight=256;
	StartLine=StartColumn=0;
	StopLine=StopColumn=256;
	DeviceFile="/dev/video0";
	
	firstTimeThrough = true;
}


// ================================================
// ~PluginRenderTV
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderTV::~PluginRenderTV() throw() 
{
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderTV::act( SystemState::ID state, StateContext &stateContext )
{
	DefFileGroup *config;
	DefFileAttrib *attrib;
	switch( state )
	{

	case SystemState::BlackboardRetrieve:
		// This stage is for retrieving things from the blackboard

		// get the root node from the BB
		bb_->get( "RootNodeOSG", RootNode );

		// get the root of the def tree from the blackboard
		bb_->get( "DefinitionData", DefFileData );

		
		break;

	case SystemState::ConfigurationProcess:
		if(!DefFileData)
			break;
		config=DefFileData[0]->getGroupByURI("/render_tv/");
		if(!config)
			break;
		if((attrib=config->getAttribute("width")))
			Width=attrib->asInt();
		if((attrib=config->getAttribute("height")))
			Height=attrib->asInt();
		// Default to full screen unless overridden.
		StartLine=StartColumn=0;
		StopColumn=Width;
		StopLine=Height;
		if((attrib=config->getAttribute("textureheight")))
			TextureHeight=attrib->asInt();
		if((attrib=config->getAttribute("startline")))
			StartLine=attrib->asFloat();
		if((attrib=config->getAttribute("stopline")))
			StopLine=attrib->asFloat();
		if((attrib=config->getAttribute("startcolumn")))
			StartColumn=attrib->asFloat();
		if((attrib=config->getAttribute("stopcolumn")))
			StopColumn=attrib->asFloat();

		if((attrib=config->getAttribute("devicefile")))
			DeviceFile=attrib->asString();
		break;

	case SystemState::Operate:
	case SystemState::Debug:
		if(firstTimeThrough && RootNode)
		{
			firstTimeThrough = false;
			osg::Node *tvnode=CreateTVNode();
			if(tvnode)
				RootNode->addChild(tvnode);
		}
		break;

	default:
		break;
	}
	
}


// ================================================
// Create a TV node complete with an orthographic projection.
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
osg::Node* PluginRenderTV::CreateTVNode()
{
	BuildRectangle br(4, 3);
	br.ScaleTexture(Width, Height, StartLine, StopLine,
		StartColumn, StopColumn);
	osg::Node *tvNode=br.GetRectangle();
	tvNode->setStateSet(GenerateUpdatingStateSet(
		new osgTV(DeviceFile, Width, Height, TextureHeight)));

	// Disable lighting, it would look funny otherwise.
	osg::StateSet *stateset=tvNode->getOrCreateStateSet();
	stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
	// Set it to render first.
	stateset->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
	stateset->setRenderBinDetails(-3,"RenderBin");

	osg::MatrixTransform *modelview=new osg::MatrixTransform;
        modelview->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
        modelview->addChild(tvNode);

	osg::Projection *projection=new osg::Projection;
	projection->setMatrix(osg::Matrix::ortho2D(-2, 2, -1.5, 1.5));
	projection->addChild(modelview);

	return projection;
}
