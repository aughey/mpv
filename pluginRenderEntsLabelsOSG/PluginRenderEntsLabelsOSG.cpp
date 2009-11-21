/** <pre>
 *  Plugin to add text labels to entities, for the MPV
 *  Copyright (c) 2006 Andrew Sampson
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
 *  Initial Release: 2006-03-19 Andrew Sampson
 *  
 *  2007-07-10  Andrew Sampson
 *      Ported to the new entity construction system.
 *
 *  2007-07-15 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 * </pre>
 */


#include <iostream>

#include "PluginRenderEntsLabelsOSG.h"
#include "LabelElementFactory.h"

using namespace mpv;
using namespace mpvosg;

EXPORT_DYNAMIC_CLASS( PluginRenderEntsLabelsOSG )

// ================================================
// PluginRenderEntsLabelsOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderEntsLabelsOSG::PluginRenderEntsLabelsOSG() : Plugin() 
{
	name_ = "PluginRenderEntsLabelsOSG";
	licenseInfo_.setLicense( LicenseInfo::LicenseLGPL );
	licenseInfo_.setOrigin( "AndrewSampson" );

	dependencies_.push_back( "PluginRenderEntsOSG" );
}


// ================================================
// ~PluginRenderEntsLabelsOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderEntsLabelsOSG::~PluginRenderEntsLabelsOSG() throw() 
{
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderEntsLabelsOSG::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{
	
	case SystemState::BlackboardRetrieve:
		// This state is for retrieving things from the blackboard

		// get the list of entity element factories from the BB
		{
			std::map< std::string, EntityElementFactory * > *entityElementFactoryMap;

			bb_->get( "EntityElementFactories", entityElementFactoryMap );
			EntityElementFactory *factory = new LabelElementFactory;
			(*entityElementFactoryMap)[factory->getKeyword()] = factory;
		}
		
		break;

	default:
		break;
	}
	
}

