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


#ifndef _COORDINATE_CONVERTER_GCC_H_
#define _COORDINATE_CONVERTER_GCC_H_


#include "CoordinateConverter.h"


//=========================================================
//! This class performs coordinate conversion for the Geocentric (GCC) 
//! coordinate system.
//!
class CoordinateConverterGCC : public mpv::CoordinateConverter
{
public:

	//=========================================================
	//! General Constructor
	//!
	CoordinateConverterGCC();

	virtual void performConversion( const CoordinateSet &in, CoordinateSet &out );
	
	virtual void performReverseConversion( const CoordinateSet &in, CoordinateSet &out );

	void coordSysParamsChangedCB( mpv::CoordinateConverter* );
	
protected:

	//=========================================================
	//! General Destructor
	//!
	virtual ~CoordinateConverterGCC();
	
	//! \param semiMajorAxis_ - The radius of the earth at the equator
	//! \param semiMinorAxis_ - The radius of the earth at the poles
	void setEarthReferenceModelFromRadii( 
		double semiMajorAxis_, double semiMinorAxis_ );

	//=========================================================
	//! The radius of the earth at the equator.
	//!
	double semiMajorAxis;
	
	//=========================================================
	//! The radius of the earth at the poles.
	//!
	double semiMinorAxis;
	
	//=========================================================
	//! The "first eccentricity" of the earth.  Can be calculated based on 
	//! the radii, or by using the flattening.
	//!
	double eccentricitySqrd;

	
};

#endif
