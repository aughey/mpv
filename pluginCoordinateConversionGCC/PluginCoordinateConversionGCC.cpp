/** <pre>
 *  Multi-Purpose Viewer - GCC coordinate conversion plugin
 *  Copyright (c) 2009 Andrew Sampson
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
 *  2009-01-17 Andrew Sampson
 *      Initial release
 *  
 *  
 *  </pre>
 */


#include <iostream>

#include "PluginCoordinateConversionGCC.h"


EXPORT_DYNAMIC_CLASS( PluginCoordinateConversionGCC )

// ================================================
// PluginCoordinateConversionGCC
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginCoordinateConversionGCC::PluginCoordinateConversionGCC() : Plugin()
{
	name_ = "PluginCoordinateConversionGCC";

	licenseInfo_.setLicense( LicenseInfo::LicenseLGPL );
	licenseInfo_.setOrigin( "AndrewSampson" );

	dependencies_.push_back( "PluginCoordinateConversionMgr" );

	coordinateConverters = NULL;
	
	coordConverterGCC = new CoordinateConverterGCC();
}


// ================================================
// ~PluginCoordinateConversionGCC
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginCoordinateConversionGCC::~PluginCoordinateConversionGCC() throw()
{
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginCoordinateConversionGCC::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{
	case SystemState::BlackboardRetrieve:
		// This state is for retrieving things from the blackboard

		bb_->get( "CoordinateConverterMap", coordinateConverters );
		
		if( coordinateConverters != NULL )
		{
			(*coordinateConverters)[coordConverterGCC->getCoordinateSystems()] = coordConverterGCC.get();
		}

		break;

	default:
		break;
	}
	
}

