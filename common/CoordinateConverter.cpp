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


#include "CoordinateConverter.h"

using namespace mpv;


// ================================================
// CoordinateConverter
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CoordinateConverter::CoordinateConverter( 
	CoordinateSystem from, 
	CoordinateSystem to ) : 
		Referenced(),
		coordSysFrom( from ),
		coordSysTo( to )
{
	
}


// ================================================
// ~CoordinateConverter
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CoordinateConverter::~CoordinateConverter() 
{
	
}


// ================================================
// setParams
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void CoordinateConverter::setParams( const CoordSysParams &newCoordSysParams )
{
	coordSysParams = newCoordSysParams;
	coordSysParamsChanged( this );
}


// ================================================
// getCoordinateSystems
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CoordinateSystemPair CoordinateConverter::getCoordinateSystems() const
{
	return CoordinateSystemPair( coordSysFrom, coordSysTo );
}


// ================================================
// getCoordinateSystems
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void CoordinateConverter::getCoordinateSystems( 
	CoordinateSystem &from, 
	CoordinateSystem &to ) const
{
	from = coordSysFrom;
	to = coordSysTo;
}


// ================================================
// performCoordinateConversion
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void CoordinateConverter::performConversion( 
	GeodeticObject *geodeticObject )
{
	if( coordSysFrom == CoordSysGDC )
	{
		CoordinateSet result;
		performConversion( geodeticObject->getPositionGDC(), result );
		geodeticObject->setPositionDB( result );
	}
	else
	{
		// error - expected source coordinate system is not GDC
		//fixme - throw an exception or something
	}
}

