/** <pre>
 * Copyright (c) 2007 General Dynamics Land Systems
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <iostream>
#include <list>

#include "PluginMissionFuncsOSG.h"

#include "MPVExceptions.h"

EXPORT_DYNAMIC_CLASS( PluginMissionFuncsOSG )

// ================================================
// PluginMissionFuncsOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginMissionFuncsOSG::PluginMissionFuncsOSG() : Plugin() 
{
	name_ = "PluginMissionFuncsOSG";
	licenseInfo_.setLicense( LicenseInfo::LicenseLGPL );
	licenseInfo_.setOrigin( "GDLS" );

	dependencies_.push_back( "PluginDefFileReader" );
	dependencies_.push_back( "PluginMissionFuncsMgr" );
	dependencies_.push_back( "PluginRenderOSG" );
	dependencies_.push_back( "PluginRenderTerrainOSG" );

	// bb lookups
	DefFileData = NULL;
	rootNode = NULL;

	// helper classes
	rootIntersectionFinder = NULL;
	terrainIntersectionFinder = NULL;
	lineDrawer = NULL;

	laseLinesGroup = new osg::Group;
	
	haveCalledSetup = false;
}

// ================================================
// ~PluginMissionFuncsOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginMissionFuncsOSG::~PluginMissionFuncsOSG() throw() 
{

	delete lineDrawer;
	delete terrainIntersectionFinder;
	delete rootIntersectionFinder;
}

// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginMissionFuncsOSG::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{
		case SystemState::BlackboardRetrieve:
			bb_->get("DefinitionData", DefFileData);

			bb_->get( "RootNodeOSG", rootNode );
			
			if( !haveCalledSetup )
			{
				// setup must be called after we've retrieved the scene graph root
				setup();
				haveCalledSetup = true;
			}

			{
				std::list< mpv::RefPtr< mpv::MissionFunctionsWorker > > *workers;
				bb_->get( "MissionFunctionsWorkers", workers );
				workers->push_back( hoatHandler.get() );
				workers->push_back( losHandler.get() );
			}

			break;

		case SystemState::ConfigurationProcess:
			getConfig();
			break;

		case SystemState::Operate:
		case SystemState::Debug:
//			losHandler->heartbeat();
//			hoatHandler->heartbeat();
			break;

		case SystemState::Shutdown:
			// fixme - Need to sever references to rootNode, terrain node... 
			// Could do this by freeing rootIntersectionFinder, terrainIntersectionFinder members
			// (or all members, since we *are* in Shutdown...)
			// Alternatively, could replace ref_ptr in OsgIntersectionFinder with a normal pointer.
			break;

		default:
			break;
	}
}

// ================================================
// getConfig
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginMissionFuncsOSG::getConfig( void ) 
{
	DefFileGroup *root = *DefFileData;
	if(!root) return;

	DefFileGroup *mission_functions_group = root->getGroupByURI( "/mission_functions/" );
	if(!mission_functions_group) return;

	DefFileAttrib *attr = 0;

	attr = mission_functions_group->getAttribute("enable_vis_lase");
	if (attr)
	{
		int enable_vis_lase = attr->asInt();
		if (enable_vis_lase >= 1)
		{
			losHandler->setLineDrawer( lineDrawer );
		}
		else
		{
			losHandler->setLineDrawer( 0 );
		}
	}

	attr = mission_functions_group->getAttribute("ownship_entity_id");
	if (attr)
	{
		losHandler->setOwnshipEntityID(attr->asInt());
	}

	attr = mission_functions_group->getAttribute( "lase_timeout_frames" );
	if(attr)
	{
		int t = attr->asInt();
		if(t >= 0)
		{
			losHandler->setLaseTimeoutFrames( static_cast<unsigned int> (t) );
		}
	}

	attr = mission_functions_group->getAttribute( "max_lase_lines" );
	if(attr)
	{
		int num = attr->asInt();
		if(num >= 0)
		{
			losHandler->setMaxLaseLines( static_cast<unsigned int> (num) );
		}
	}

	attr = mission_functions_group->getAttribute("ignore_back_face");
	if(attr)
	{
		rootIntersectionFinder->exclude_backfacing_polygons( bool(attr->asInt()) );
	}
}

// ================================================
// setup
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginMissionFuncsOSG::setup()
{
	// some one-time setup stuff occurs here

	// The Terrain Root Node.  Determined from the root node.
	osg::Node* terrainRoot = NULL;

	// Assign root node for LOS lookups
	rootIntersectionFinder = new SG::OsgIntersectionFinder(rootNode);
	rootIntersectionFinder->add_nodename_exclusion("Sky Branch Node");
	rootNode->addChild( laseLinesGroup.get() );
	lineDrawer = new SG::LineDrawer( laseLinesGroup.get() );

	// For HAT/HOT lookups we only care about the terrain branch.
	unsigned int numChildren = rootNode->getNumChildren();
	for(unsigned int i=0; i<numChildren; ++i)
	{
		osg::Node* child = rootNode->getChild(i);
		if( child->getName() == "Terrain Branch Node")
		{
			terrainRoot = child;
			break;
		}
	}
	if(!terrainRoot)
	{
		throw MPVPluginInitException("Unable to find Terrain Branch Node");
	}
	terrainIntersectionFinder = new SG::OsgIntersectionFinder(terrainRoot);

	losHandler = new LOSHandler( *rootIntersectionFinder );
	losHandler->setLineDrawer(lineDrawer);
	hoatHandler = new HOATHandler( *terrainIntersectionFinder );

}

