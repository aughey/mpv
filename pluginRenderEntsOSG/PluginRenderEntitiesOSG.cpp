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
 *  FILENAME:   PluginRenderEntitiesOSG.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  create and maintain the entity branch of the scene graph.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  03/29/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 *
 *  2007-03-28 Andrew Sampson
 *      Completely reworked the mechanism by which entity subgraphs are 
 *      constructed.
 *
 *  2007-07-15 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 *  2007-11-03  Andrew Sampson
 *      Moved ModelElement code out of pluginRenderEntsOSG and into its own 
 *      plugin
 *
 *  2008-07-26 Andrew Sampson
 *      Ported plugin to the new entity interface.  Now contains code from 
 *      the symbology rendering plugin.
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#include <iostream>

#include "BindSlot.h"

#include "PluginRenderEntitiesOSG.h"


using namespace mpv;
using namespace mpvosg;


EXPORT_DYNAMIC_CLASS( PluginRenderEntsOSG )

// ================================================
// PluginRenderEntsOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderEntsOSG::PluginRenderEntsOSG() : Plugin() 
{
	name_ = "PluginRenderEntsOSG";
	licenseInfo_.setLicense( LicenseInfo::LicenseGPL );
	licenseInfo_.setOrigin( "Boeing" );
	dependencies_.push_back( "PluginRenderOSG" );
	dependencies_.push_back( "PluginEntityMgr" );
	dependencies_.push_back( "PluginDefFileReader" );

	rootNode = NULL;
	allEntities = NULL;
	topLevelEntities = NULL;
	
	nodeFactory = new EntityNodeFactory;
}


// ================================================
// ~PluginRenderEntsOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderEntsOSG::~PluginRenderEntsOSG() throw() 
{
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderEntsOSG::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{
	
	case SystemState::BlackboardPost:
		// This state is for posting things to the blackboard and other 
		// initialization tasks

		bb_->put( "EntityElementFactories", nodeFactory->getElementFactoryList() );

		break;


	case SystemState::BlackboardRetrieve:
		// This state is for retrieving things from the blackboard

		// get the root node from the BB
		bb_->get( "RootNodeOSG", rootNode );

		rootNode->addChild( nodeFactory->getEntityBranchNode() );

		bb_->get( "AllEntities", allEntities );
		bb_->get( "TopLevelEntities", topLevelEntities );
		
		// set up notification for:
		// - entity creation 
		allEntities->addedEntity.connect( BIND_SLOT2( EntityNodeFactory::addedEntity, nodeFactory.get() ) );
		// - entity addition/removal from the top-level container
		topLevelEntities->addedEntity.connect( BIND_SLOT2( EntityNodeFactory::addedTopLevelEntity, nodeFactory.get() ) );
		topLevelEntities->removedEntity.connect( BIND_SLOT2( EntityNodeFactory::removedTopLevelEntity, nodeFactory.get() ) );

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
void PluginRenderEntsOSG::operate( void ) 
{

}

