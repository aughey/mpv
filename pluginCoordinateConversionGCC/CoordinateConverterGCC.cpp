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
#include <string>
#include <stdlib.h>
#include <math.h>

#include "Vect3.h"
#include "Mtx3.h"
#include "BindSlot.h"

#include "CoordinateConverterGCC.h"

// From the ERDAS field guide, 5th ed., pg 447
#define WGS84_SemiMajorAxis 6378137.0
#define WGS84_SemiMinorAxis 6356752.31424517929

// a*D/c magic value (where a=SemiMajorAxis, c=SemiMinorAxis) proposed by 
// [Toms 1995] for altitudes -1.*10^5 to 2.*10^6 ("Region 1")
#define BowringInitialMagicNumber_aDc_Region1 1.0026000

#define D2R 0.017453292519943295
#define R2D 57.295779513082323

using namespace mpv;

// ================================================
// CoordinateConverterGCC
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CoordinateConverterGCC::CoordinateConverterGCC() : 
	CoordinateConverter( CoordSysGDC, CoordSysGCC ),
	semiMajorAxis( 0. ),
	semiMinorAxis( 0. ),
	eccentricitySqrd( 0. )
{
	setEarthReferenceModelFromRadii( WGS84_SemiMajorAxis, WGS84_SemiMinorAxis );
	
	coordSysParamsChanged.connect( BIND_SLOT1( CoordinateConverterGCC::coordSysParamsChangedCB, this ) );
}


// ================================================
// ~CoordinateConverterGCC
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CoordinateConverterGCC::~CoordinateConverterGCC()
{

}


// ================================================
// performConversion
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void CoordinateConverterGCC::performConversion( 
	const CoordinateSet &in, CoordinateSet &out )
{
	// equation is from http://en.wikipedia.org/wiki/Geodetic_system#Conversion

	double lat = in.LatX * D2R;
	double lon = in.LonY * D2R;
	double alt = in.AltZ;

	double sinLat = sin( lat );
	double cosLat = cos( lat );
	double sinLon = sin( lon );
	double cosLon = cos( lon );
	
	double chi = sqrt( 1. - (eccentricitySqrd * sinLat * sinLat) );
	// N = "radius of curvature in the prime vertical"
	double N = semiMajorAxis/chi; // chi can't be 0, so no risk of divide-by-zero
	// p = "distance from the polar axis to the point"
	double p = (N + alt) * cosLat;
	
	out.LatX = p * cosLon;
	out.LonY = p * sinLon;
	out.AltZ = ( N*(1.-eccentricitySqrd) + alt ) * sinLat;

	// FIXME - orientation
	out.Yaw = 0.;
	out.Pitch = 0.;
	out.Roll = 0.;
}


// ================================================
// performReverseConversion
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void CoordinateConverterGCC::performReverseConversion( 
	const CoordinateSet &in, CoordinateSet &out )
{
	// The reverse conversion, GCC->GDC, is much tougher than GDC->GCC.
	// The equations used here are from "An Efficient Algorithm for Geocentric 
	// to Geodetic Coordinate Conversion", by Ralph M. Toms, Sept. 1995.
	
	double lat, lon, alt;

	// longitude is easy
	if( in.LatX == 0. )
	{
		// prevent divide-by-zero
		
		if( in.LonY > 0. )
			lon = 90. * D2R;
		else
			lon = -90. * D2R;
	}
	else
	{
		lon = atan2( in.LonY, in.LatX );
	}
	
	// latitude is tough.  the following implements the "improved Bowring 
	// method" as described by [Toms 1995].
	
	// projectedRadiusXY is "W" in [Toms 1995 (8)]
	double projectedRadiusXY = sqrt( in.LatX * in.LatX + in.LonY * in.LonY );

	// avoid divide-by-zero
	if( in.AltZ == 0. )
	{
		// if Z = 0, then latitude = 0 (equator)
		lat = 0.;
		alt = projectedRadiusXY - semiMajorAxis;
	}
	else
	{
		// the "Step N" comments here correspond to those listed in [Toms 1995].
		
		// Step 3
		double T0 = in.AltZ * BowringInitialMagicNumber_aDc_Region1;

		// Step 4
		double S0 = sqrt( T0 * T0 + projectedRadiusXY * projectedRadiusXY );

		// Step 5
		// divide-by-zero is very unlikely here, as Z=0 was checked above
		double sinB0 = T0 / S0;
		double cosB0 = projectedRadiusXY / S0;

		// Step 6
		// eccentricitySqrdPrime is "E'^2" in [Toms 1995 (11)]
		double eccentricitySqrdPrime = (semiMajorAxis * semiMajorAxis - semiMinorAxis * semiMinorAxis) / (semiMinorAxis * semiMinorAxis);
		double T1 = in.AltZ + (semiMinorAxis * eccentricitySqrdPrime * sinB0 * sinB0 * sinB0);

		// Step 7
		double tempS1 = projectedRadiusXY - (semiMajorAxis * eccentricitySqrd * cosB0 * cosB0 * cosB0);
		double S1 = sqrt( T1 * T1 + tempS1 * tempS1 );

		// Step 8
		// divide-by-zero is very unlikely here; S1 would only be 0 if the earth's radius were 0
		double sinLat = T1 / S1;
		double cosLat = tempS1 / S1;

		// Step 12
		// re. divide-by-zero, I'm pretty sure that it isn't possible here
		lat = atan2( sinLat, cosLat );

		// once latitude is known, height can be calculated
		double chi = sqrt( 1. - (eccentricitySqrd * sinLat * sinLat) );
		// N = "radius of curvature in the prime vertical"
		double N = semiMajorAxis/chi; // chi can't be 0, so no risk of divide-by-zero
		if( fabs( lat ) > 67.5 * D2R )
		{
			// the polar regions use a different equation for height; [Toms 1995 (13)]
			// divide-by-zero is unlikely; sinLat shouldn't be zero, because 
			// Z=0 was checked above
			alt = (in.AltZ / sinLat) + (N * (eccentricitySqrd - 1.));
		}
		else
		{
			// non-polar regions; [Toms 1995 (12)]
			// divide-by-zero is unlikely, as cosLat would only be zero at poles
			alt = (projectedRadiusXY / cosLat ) - N;
		}
	}
	
	// convert from radians to degrees
	out.LatX = lat * R2D;
	out.LonY = lon * R2D;
	out.AltZ = alt;
	
	// FIXME - orientation
	out.Yaw = 0.;
	out.Pitch = 0.;
	out.Roll = 0.;
}


// ================================================
// coordSysParamsChangedCB
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void CoordinateConverterGCC::coordSysParamsChangedCB( CoordinateConverter* )
{
	if( coordSysParams.coordSys == CoordSysGCC )
	{
		// GCC doesn't have any parameters...
	}
	else
	{
		// error - coordinate system specified isn't handled by this converter
	}
}


// ================================================
// setEarthReferenceModelFromRadii
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void CoordinateConverterGCC::setEarthReferenceModelFromRadii( 
	double semiMajorAxis_, double semiMinorAxis_ )
{
	this->semiMajorAxis = semiMajorAxis_;
	this->semiMinorAxis = semiMinorAxis_;
	
	// flattening is a ratio of the major and minor radii
	// equation is from the ERDAS field guide, 5th ed., pg 444
	double flattening = (semiMajorAxis_ - semiMinorAxis_) / semiMajorAxis_;
	// equation is from the ERDAS field guide, 5th ed., pg 444
	this->eccentricitySqrd = (2. * flattening) - (flattening * flattening);
	// some sources define eccentricitySqrd as (major^2 - minor^2)/major^2
}


