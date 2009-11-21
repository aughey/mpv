/** <pre>
 *  MPV plugin to automatically manipulate entity articulations
 *  Copyright (c) 2008 Andrew Sampson
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
 *  2008-11-02  Andrew Sampson
 *      Initial release
 *
 * </pre>
 */


#include <iostream>

#include "PluginRenderEntsAutoArticulationOSG.h"
#include "AutoArticulationElementFactory.h"

using namespace mpv;
using namespace mpvosg;

EXPORT_DYNAMIC_CLASS( PluginRenderEntsAutoArticulationOSG )

// ================================================
// PluginRenderEntsAutoArticulationOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderEntsAutoArticulationOSG::PluginRenderEntsAutoArticulationOSG() : Plugin() 
{
	name_ = "PluginRenderEntsAutoArticulationOSG";
	licenseInfo_.setLicense( LicenseInfo::LicenseLGPL );
	licenseInfo_.setOrigin( "AndrewSampson" );

	dependencies_.push_back( "PluginRenderEntsOSG" );
}


// ================================================
// ~PluginRenderEntsAutoArticulationOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderEntsAutoArticulationOSG::~PluginRenderEntsAutoArticulationOSG() throw() 
{
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderEntsAutoArticulationOSG::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{
	
	case SystemState::BlackboardRetrieve:
		// This state is for retrieving things from the blackboard

		// get the list of entity element factories from the BB
		{
			std::map< std::string, EntityElementFactory * > *entityElementFactoryMap;

			bb_->get( "EntityElementFactories", entityElementFactoryMap );
			EntityElementFactory *factory = new AutoArticulationElementFactory;
			(*entityElementFactoryMap)[factory->getKeyword()] = factory;
		}
		
		break;

	default:
		break;
	}
	
}

