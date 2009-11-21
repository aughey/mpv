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
 *  FILENAME:   PluginRenderTerrainOSG.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  create and manage the terrain branch of the scene graph.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  03/29/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 *
 *  2007-07-21 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#include <iostream>

#include <osg/PositionAttitudeTransform>
#include <osg/StateSet>
#include <osg/AlphaFunc>

#include "BindSlot.h"

#include "PluginRenderTerrainOSG.h"
#include "TerrainImpOSG.h"


using namespace mpv;

EXPORT_DYNAMIC_CLASS( PluginRenderTerrainOSG )

// ================================================
// PluginRenderTerrainOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderTerrainOSG::PluginRenderTerrainOSG() : Plugin() 
{
	name_ = "PluginRenderTerrainOSG";
	licenseInfo_.setLicense( LicenseInfo::LicenseGPL );
	licenseInfo_.setOrigin( "Boeing" );
	dependencies_.push_back( "PluginRenderOSG" );
	dependencies_.push_back( "PluginTerrainMgr" );
	rootNode = NULL;
	terrainDatabases = NULL;

	terrainBranchNode = new osg::PositionAttitudeTransform;
	terrainBranchNode->setName( "Terrain Branch Node" );

	osg::StateSet *stateset = terrainBranchNode->getOrCreateStateSet();

	// enable blending for the terrain
	stateset->setMode( GL_BLEND, osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON );
	osg::AlphaFunc *alphaFunc = new osg::AlphaFunc;
	alphaFunc->setFunction( osg::AlphaFunc::GREATER );
	alphaFunc->setReferenceValue( 0.5 );
	stateset->setAttribute( alphaFunc );

	// disable lighting for the terrain
//		stateset->setMode( GL_LIGHTING, osg::StateAttribute::OVERRIDE|osg::StateAttribute::OFF );
}


// ================================================
// ~PluginRenderTerrainOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderTerrainOSG::~PluginRenderTerrainOSG() throw() 
{
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderTerrainOSG::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{

	case SystemState::BlackboardRetrieve:
		// This state is for retrieving things from the blackboard

		// get the root node from the BB
		bb_->get( "RootNodeOSG", rootNode );

		rootNode->addChild( terrainBranchNode.get() );

		bb_->get( "TerrainDatabases", terrainDatabases );
		terrainDatabases->addedTerrain.connect( 
			BIND_SLOT2( PluginRenderTerrainOSG::addedTerrainCB, this ) );

		break;


	default:
		break;
	}
	
}


// ================================================
// addedTerrainCB
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderTerrainOSG::addedTerrainCB( TerrainContainer*, Terrain *terrain )
{
	terrain->addImplementation( 
		new TerrainImpOSG( terrain, terrainBranchNode.get() ) );
}

