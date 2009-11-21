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
 *  FILENAME:   PluginRenderParticleSysOSG.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  create and maintain particle system entity subgraphs.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  03/23/2005 Andrew Sampson
 *      Initial Release.
 *  
 *  2007-07-10 Andrew Sampson
 *      Plugin reworked to use new entity-construction system.
 *  
 *  2007-07-15 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#include <iostream>

#include "range.h"

#include "GenerateID.h"

#include "PluginRenderParticleSysOSG.h"

using namespace mpv;
using namespace mpvosg;

//=========================================================
//! A pointer to our synchronized RNG.  Retrieved from the blackboard.
//! 
static GenerateID *randNumGen = NULL;


int randNumGenWrapper( void )
{
	std::string dummy;
	if( randNumGen )
		return randNumGen->GetNextID( dummy );
	return 0;
}


EXPORT_DYNAMIC_CLASS( PluginRenderEntsParticleSysOSG )

// ================================================
// PluginRenderEntsParticleSysOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderEntsParticleSysOSG::PluginRenderEntsParticleSysOSG() : Plugin() 
{
	name_ = "PluginRenderEntsParticleSysOSG";

	// The HPS library contains a modified version of osgParticle, and 
	// osgParticle is released under the OSG license.
	licenseInfo_.setLicense( LicenseInfo::LicenseOSG );
	licenseInfo_.setOrigin( "OSG+Boeing" );

	dependencies_.push_back( "PluginRenderOSG" );
	dependencies_.push_back( "PluginRenderEntsOSG" );
	dependencies_.push_back( "PluginDefFileReader" );
	// The synced rand num gen plugin was optional, and I later made it a 
	// dependency... may want to revert to it being optional... not sure.
//	dependencies_.push_back( "PluginSyncedRandomNumbers" );

	rootNode = NULL;
	
	particleSysElementFactory = new ParticleSysElementFactory;
}


// ================================================
// ~PluginRenderEntsParticleSysOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderEntsParticleSysOSG::~PluginRenderEntsParticleSysOSG() throw() 
{
	// pluginRenderEnts will take care of deleting particleSysElementFactory.
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderEntsParticleSysOSG::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{
	
	case SystemState::BlackboardRetrieve:
		// This state is for retrieving things from the blackboard
		{

			// get the root node from the BB
			bb_->get( "RootNodeOSG", rootNode );

			worldSpaceBranchNode = new osg::Group;
			worldSpaceBranchNode->setName( "World-Space Branch Node" );
			rootNode->addChild( worldSpaceBranchNode.get() );
			particleSysElementFactory->setWorldSpaceBranch( worldSpaceBranchNode.get() );

			bb_->get("DefinitionData", DefFileData);

			int *randNumGenMax = NULL;
			if( bb_->get( "RandomNumberGenerator", randNumGen, false ) && 
				bb_->get( "RandomNumberGeneratorMax", randNumGenMax, false ) )
				// set the osg::range objects to use the synchronized RNG 
				osgParticleHPS::rangeSetRandFunc( randNumGenWrapper, *randNumGenMax );
		
			// get the list of entity element factories from the BB
			std::map< std::string, EntityElementFactory * > *entityElementFactoryMap;
			bb_->get( "EntityElementFactories", entityElementFactoryMap );
			(*entityElementFactoryMap)[particleSysElementFactory->getKeyword()] = particleSysElementFactory;
		}
		
		break;

	case SystemState::ConfigurationProcess:
		particleSysElementFactory->init( *DefFileData );
		break;

	case SystemState::Reset:
	case SystemState::Shutdown:
		// fixme - need to do something on reset and shutdown... need to clear 
		// the contents of worldSpaceBranchNode
		break;

	default:
		break;
	}
	
}


