/** <pre>
 *  MPV Serialization Framework - Root plugin
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
 *  2008-12-29 Andrew Sampson
 *      Initial release
 *  
 *  
 *  </pre>
 */


#include <iostream>
#include <string.h> // for memcpy

#include "PluginS11nRoot.h"

using namespace mpv;
using namespace mpvs11n;

EXPORT_DYNAMIC_CLASS( PluginS11nRoot )

// ================================================
// PluginS11nRoot
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginS11nRoot::PluginS11nRoot() : Plugin()
{
	name_ = "PluginS11nRoot";

	licenseInfo_.setLicense( LicenseInfo::LicenseLGPL );
	licenseInfo_.setOrigin( "AndrewSampson" );

	rootNode = new GroupNode( "mpv_state" );
}


// ================================================
// ~PluginS11nRoot
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginS11nRoot::~PluginS11nRoot() throw()
{
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginS11nRoot::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{
	case SystemState::BlackboardPost:
		// This state is for posting things to the blackboard and other 
		// initialization tasks
		
		bb_->put( "SerializationRootNode", rootNode.get() );
		
		break;

	case SystemState::Shutdown:
		// it is a good idea to clear out the node graph as much as possible 
		// before the plugin destructors are called
		rootNode->removeAllChildren();
		
		break;
		
	default:
		break;
	}
	
}


