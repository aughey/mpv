/** <pre>
 *  Plugin to add model files to entities, for the MPV
 *  Copyright (c) 2007 Andrew Sampson
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
 *  2007-03-28 Andrew Sampson
 *      Completely reworked the mechanism by which entity subgraphs are 
 *      constructed.
 *
 *  2007-07-15 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 *  2007-11-03  Andrew Sampson
 *      Moved code out of pluginRenderEntsOSG and into its own plugin
 *
 * </pre>
 */


#include <iostream>

#include "PluginRenderEntsModelFileOSG.h"
#include "ModelElementFactory.h"

using namespace mpv;
using namespace mpvosg;

EXPORT_DYNAMIC_CLASS( PluginRenderEntsModelFileOSG )

// ================================================
// PluginRenderEntsModelFileOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderEntsModelFileOSG::PluginRenderEntsModelFileOSG() : Plugin() 
{
	name_ = "PluginRenderEntsModelFileOSG";
	licenseInfo_.setLicense( LicenseInfo::LicenseLGPL );
	licenseInfo_.setOrigin( "AndrewSampson" );

	dependencies_.push_back( "PluginRenderEntsOSG" );
}


// ================================================
// ~PluginRenderEntsModelFileOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderEntsModelFileOSG::~PluginRenderEntsModelFileOSG() throw() 
{
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderEntsModelFileOSG::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{
	
	case SystemState::BlackboardRetrieve:
		// This state is for retrieving things from the blackboard

		// get the list of entity element factories from the BB
		{
			std::map< std::string, EntityElementFactory * > *entityElementFactoryMap;

			bb_->get( "EntityElementFactories", entityElementFactoryMap );
			EntityElementFactory *factory = new ModelElementFactory;
			(*entityElementFactoryMap)[factory->getKeyword()] = factory;
		}
		
		break;

	default:
		break;
	}
	
}

