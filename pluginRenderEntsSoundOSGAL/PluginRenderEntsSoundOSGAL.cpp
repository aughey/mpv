/** <pre>
 *  Plugin to add positional audio to entities, for the MPV
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
 *  Initial Release: 2007-04-07 Andrew Sampson
 *
 *  2007-07-15 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 * </pre>
 */


#include <iostream>
#include <list>
#include <osg/MatrixTransform>

#include "PluginRenderEntsSoundOSGAL.h"
#include "SoundElementFactory.h"


EXPORT_DYNAMIC_CLASS( PluginRenderEntsSoundOSGAL )

// ================================================
// PluginRenderEntsSoundOSGAL
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderEntsSoundOSGAL::PluginRenderEntsSoundOSGAL() : Plugin()
{
	name_ = "PluginRenderEntsSoundOSGAL";
	licenseInfo_.setLicense( LicenseInfo::LicenseLGPL );
	licenseInfo_.setOrigin( "AndrewSampson" );
	dependencies_.push_back( "PluginRenderSoundRootOSGAL" );
	dependencies_.push_back( "PluginRenderEntsOSG" );
	dependencies_.push_back( "PluginDefFileReader" );
}


// ================================================
// ~PluginRenderEntsSoundOSGAL
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderEntsSoundOSGAL::~PluginRenderEntsSoundOSGAL() throw()
{
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderEntsSoundOSGAL::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{

	case SystemState::BlackboardRetrieve:
		// This state is for retrieving things from the blackboard

		// get the list of entity element factories from the BB
		{
			std::map< std::string, EntityElementFactory * > *entityElementFactoryMap;

			bb_->get( "EntityElementFactories", entityElementFactoryMap );
			SoundElementFactory *factory = new SoundElementFactory;
			(*entityElementFactoryMap)[factory->getKeyword()] = factory;
		}
		
		break;
		
	default:
		break;
	}

}

