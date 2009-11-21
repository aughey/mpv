/** <pre>
 *  MPV TM coordinate conversion plugin
 *  Copyright (c) 2008 The Boeing Company
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
 *  04/04/2004 Greg Basler
 *      Initial Release.
 *  
 *  2007-07-03 Andrew Sampson
 *      Changed interface to use new state machine API
 *  
 *  2008-07-20 Andrew Sampson
 *      Ported old pluginOSPositionConv code to the new coordinate conversion 
 *      API.  The plugin class was based on PluginCoordinateConversionMgr.
 *      The conversion code was based on "PositionConversion".
 *  
 * </pre>
 */


#include <iostream>

#include "PluginCoordinateConversionTM.h"


EXPORT_DYNAMIC_CLASS( PluginCoordinateConversionTM )

// ================================================
// PluginCoordinateConversionTM
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginCoordinateConversionTM::PluginCoordinateConversionTM() : Plugin()
{
	name_ = "PluginCoordinateConversionTM";

	licenseInfo_.setLicense( LicenseInfo::LicenseGPL );
	licenseInfo_.setOrigin( "Boeing+AndrewSampson" );

	dependencies_.push_back( "PluginCoordinateConversionMgr" );

	coordinateConverters = NULL;
	
	coordConverterTM = new CoordinateConverterTM();
	coordConverterEasyTM = new CoordinateConverterEasyTM();
}


// ================================================
// ~PluginCoordinateConversionTM
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginCoordinateConversionTM::~PluginCoordinateConversionTM() throw() 
{
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginCoordinateConversionTM::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{
	case SystemState::BlackboardRetrieve:
		// This state is for retrieving things from the blackboard

		bb_->get( "CoordinateConverterMap", coordinateConverters );
		
		if( coordinateConverters != NULL )
		{
			(*coordinateConverters)[coordConverterTM->getCoordinateSystems()] = coordConverterTM.get();
			(*coordinateConverters)[coordConverterEasyTM->getCoordinateSystems()] = coordConverterEasyTM.get();
		}

		break;

	default:
		break;
	}
	
}

