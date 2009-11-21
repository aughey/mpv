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


#ifndef _COORDINATECONVERTERPROXY_H_
#define _COORDINATECONVERTERPROXY_H_

#include "Terrain.h"
#include "CoordinateConverter.h"

using namespace mpv;

//=========================================================
//! 
//! 
class CoordinateConverterProxy : public CoordinateConverter
{
public:
	//=========================================================
	//! General Constructor
	//! 
	CoordinateConverterProxy();
	
	void setDatabase( CoordinateConverter *newConverter, const Terrain *terrain );
	
	virtual void performConversion( const CoordinateSet &in, CoordinateSet &out );
	
	virtual void performReverseConversion( const CoordinateSet &in, CoordinateSet &out );

private:
	//=========================================================
	//! General Destructor
	//! 
	virtual ~CoordinateConverterProxy();
	
	RefPtr<CoordinateConverter> converter;
	
};

#endif
