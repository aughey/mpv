/** <pre>
 *  MPV coordinate conversion manager plugin
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
 *  2008-07-06 Andrew Sampson
 *      Initial release
 *  
 *  
 *  </pre>
 */


#include "CoordinateConverterProxy.h"


// ================================================
// CoordinateConverterProxy
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CoordinateConverterProxy::CoordinateConverterProxy() : 
	CoordinateConverter( CoordSysGDC, CoordSysUNKNOWN )
{
	
}


// ================================================
// ~CoordinateConverterProxy
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CoordinateConverterProxy::~CoordinateConverterProxy() 
{
	
}


void CoordinateConverterProxy::setDatabase( 
	CoordinateConverter *newConverter, const Terrain *terrain )
{
	converter = newConverter;
	converter->setParams( terrain->getCoordSys() );
	setParams( terrain->getCoordSys() );
}

	
void CoordinateConverterProxy::performConversion( 
	const CoordinateSet &in, CoordinateSet &out )
{
	if( converter.valid() )
	{
		converter->performConversion( in, out );
	}
	else
	{
		// error - no converter
		//fixme - throw exception or something
	}
}


void CoordinateConverterProxy::performReverseConversion( 
	const CoordinateSet &in, CoordinateSet &out )
{
	if( converter.valid() )
	{
		converter->performReverseConversion( in, out );
	}
	else
	{
		// error - no converter
		//fixme - throw exception or something
	}
}

