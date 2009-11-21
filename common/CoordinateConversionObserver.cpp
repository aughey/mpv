/** <pre>
 *  The MPV Common Library
 *  Copyright (c) 2008 
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
 *  2008-07-06 Andrew Sampson
 *      Initial release
 *  
 *  
 *  </pre>
 */


#include "BindSlot.h"
#include "CoordinateConversionObserver.h"

using namespace mpv;


// ================================================
// CoordinateConversionObserver
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CoordinateConversionObserver::CoordinateConversionObserver() : Referenced()
{
	
}


// ================================================
// ~CoordinateConversionObserver
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CoordinateConversionObserver::~CoordinateConversionObserver() 
{
	
}


void CoordinateConversionObserver::setCoordinateConverter( CoordinateConverter *newConverter )
{
	if( converter.valid() )
		converter->coordSysParamsChanged.disconnect( BIND_SLOT1( CoordinateConversionObserver::coordSysParamsChanged, this ) );
	
	converter = newConverter;
	converter->coordSysParamsChanged.connect( BIND_SLOT1( CoordinateConversionObserver::coordSysParamsChanged, this ) );
	
	coordSysParamsChanged( converter.get() );
}


void CoordinateConversionObserver::startObserving( GeodeticObject *geodeticObject )
{
	geodeticObject->positionGDCChanged.connect( BIND_SLOT1( CoordinateConversionObserver::performCoordinateConversion, this ) );
	performCoordinateConversion( geodeticObject );
}


void CoordinateConversionObserver::stopObserving( GeodeticObject *geodeticObject )
{
	geodeticObject->positionGDCChanged.disconnect( BIND_SLOT1( CoordinateConversionObserver::performCoordinateConversion, this ) );
}


void CoordinateConversionObserver::performCoordinateConversion( GeodeticObject *geodeticObject )
{
	if( converter.valid() )
	{
		converter->performConversion( geodeticObject );
	}
	else
	{
		// error - no converter
		//fixme - throw exception or something
	}
}


void CoordinateConversionObserver::coordSysParamsChanged( CoordinateConverter *c )
{
	performConversionForAllObservedObjects();
}

