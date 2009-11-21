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
 *  FILENAME:   PluginRenderSkyOSG.cpp
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
 *  04/26/2004 Andrew Sampson                       MPV_CR_DR_1
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

#include "PluginRenderSkyOSG.h"
#include "SkyDomeNode.h"
#include "SkyDomeFileNode.h"

EXPORT_DYNAMIC_CLASS( PluginRenderSkyOSG )

// ================================================
// PluginRenderSkyOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderSkyOSG::PluginRenderSkyOSG() : Plugin() 
{
	name_ = "PluginRenderSkyOSG";
	licenseInfo_.setLicense( LicenseInfo::LicenseGPL );
	licenseInfo_.setOrigin( "Boeing" );
	dependencies_.push_back( "PluginRenderOSG" );
	rootNode = NULL;
	DefFileData = NULL;
}


// ================================================
// ~PluginRenderSkyOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderSkyOSG::~PluginRenderSkyOSG() throw() 
{
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderSkyOSG::act( SystemState::ID state, StateContext &stateContext )
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

		skyBranchNode->addChild( new SkyDomeNode );

		break;

	case SystemState::ConfigurationProcess:
		getConfig();
		break;

	case SystemState::Operate:
	case SystemState::Debug:
		if( !skydomeList.empty() )
		{
			// I don't remember why this plugin adds the sky domes to the 
			// scene graph in the Operate/Debug state.  It seems to me that 
			// this could be done in the ConfigurationProcess state, avoiding 
			// the complexity of keeping a list of sky dome nodes... -ADS
			
			skyBranchNode->removeChild( 0, skyBranchNode->getNumChildren() );
			std::list<osg::ref_ptr<osg::Node> >::iterator iter;
			for( iter = skydomeList.begin(); iter != skydomeList.end(); iter++ )
			{
				skyBranchNode->addChild( (*iter).get() );
			}
			skydomeList.clear();
		}
		break;

	default:
		break;
	}
	
}


// ================================================
// getConfig
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderSkyOSG::getConfig( void ) 
{
	skydomeList.clear();

	DefFileGroup *root = *DefFileData;
	if(root == NULL)
		return;
	
	DefFileGroup *skyGroup = root->getGroupByURI( "/sky/" );
	if(skyGroup == NULL)
		return;

	for( std::list< DefFileGroup * >::iterator iter = skyGroup->children.begin();
		iter != skyGroup->children.end(); iter++ )
	{
		DefFileGroup *group = (*iter);
		if( group->getName() == "skydome" ) 
		{
			DefFileAttrib *attr;
			std::string filename("");
			osg::Vec3 yprV3;
			bool isTransparent = false;
			
			attr = group->getAttribute( "filename" );
			if( attr )
			{
				filename = attr->asString();
			}
			
			attr = group->getAttribute( "rotate" );
			if( attr )
			{
				std::vector<float> ypr = attr->asFloats();
				for( unsigned int i = 0; i < 3 && i < ypr.size(); i++ )
					yprV3[i] = osg::DegreesToRadians( ypr[i] );
			}
			
			attr = group->getAttribute( "transparent" );
			if( attr )
			{
				isTransparent = attr->asInt();
			}
			
			skydomeList.push_back( 
				new SkyDomeFileNode( filename.c_str(), yprV3, isTransparent ) );
		}
	}
	
}

