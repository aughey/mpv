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
 *  FILENAME:   PluginRenderEntsNearModel.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *
 *  PROGRAM DESCRIPTION:
 *  This class contains the data and methods necessary to modify the near and
 *  far clipping planes for the view glFrustum.  This is required when objects
 *  are closer than the near clipping plane and yet need to be visible.
 *  Objects such as the cockpit are an example.
 *
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *
 *  03-29-2005 David Fries                       MPV_CR_DR_1
 *  Initial Release.
 *
 *  2007-05-22 Andrew Sampson
 *      Ported to new entity-construction system.
 *
 *  2007-07-15 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#include <iostream>
#include <list>

#include "PluginRenderEntsNearModel.h"

#include "NearFarManipulatorElementFactory.h"

using namespace mpv;
using namespace mpvosg;

EXPORT_DYNAMIC_CLASS( PluginRenderEntsNearModel )

// ================================================
// PluginRenderEntsNearModel
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderEntsNearModel::PluginRenderEntsNearModel() : Plugin()
{
	name_ = "PluginRenderEntsNearModel";
	licenseInfo_.setLicense( LicenseInfo::LicenseGPL );
	licenseInfo_.setOrigin( "Boeing" );

	dependencies_.push_back( "PluginRenderEntsOSG" );
}


// ================================================
// ~PluginRenderEntsNearModel
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderEntsNearModel::~PluginRenderEntsNearModel() throw()
{
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderEntsNearModel::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{

	case SystemState::BlackboardRetrieve:
		// This state is for retrieving things from the blackboard

		// get the list of entity element factories from the BB
		{
			std::map< std::string, EntityElementFactory * > *entityElementFactoryMap;

			bb_->get( "EntityElementFactories", entityElementFactoryMap );
			NearFarManipulatorElementFactory *factory = new NearFarManipulatorElementFactory;
			(*entityElementFactoryMap)[factory->getKeyword()] = factory;
		}
		
		break;

	default:
		break;
	}

}

