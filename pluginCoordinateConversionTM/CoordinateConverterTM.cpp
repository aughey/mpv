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
 *  04/04/2004 Greg Basler
 *      Initial Release.
 *  
 *  2007-07-03 Andrew Sampson
 *      Changed interface to use new state machine API
 *  
 *  2008-07-20 Andrew Sampson
 *      Ported old pluginOSPositionConv code to the new coordinate conversion 
 *      API.  This class was based on "PositionConversion".
 *  
 * </pre>
 */


#include <iostream>
#include <string>
#include <stdlib.h>
#include <math.h>
#include "Vect3.h"
#include "Mtx3.h"

#include "BindSlot.h"

#include "CoordinateConverterTM.h"

static const double PI = 3.1415926535897932384626433832795;

// Degree/Radian conversion
#define DTOR(d) ((d)*(PI) / 180.0)
#define RTOD(r) ((r)*180.0 / (PI))

static const double WGS84_a  = 6378137.0;
static const double WGS84_f = 1.0 / 298.257223563;

using namespace mpv;


// ================================================
// CoordinateConverterTM
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CoordinateConverterTM::CoordinateConverterTM() : 
	CoordinateConverter( CoordSysGDC, CoordSysTM )
{
	erm_a = WGS84_a;
	erm_f = WGS84_f;
	erm_b = erm_a * ( 1.0 - erm_f );
	erm_esqu = 2 * erm_f - erm_f * erm_f;
	erm_e4th = erm_esqu * erm_esqu;
	erm_e6th = erm_e4th * erm_esqu;
	erm_e    = sqrt( erm_esqu );
	erm_e1   = ( 1 - sqrt( 1 - erm_esqu ) ) / ( 1 + sqrt( 1 - erm_esqu ) );
	erm_eprimesqu = (( erm_a * erm_a - erm_b * erm_b ) / ( erm_b * erm_b ) );

	C1 = 1. - erm_esqu / 4. - erm_e4th * 3. / 64. - erm_e6th * 5. / 256.;
	C2 = erm_esqu * 3. / 8. + erm_e4th * 3. / 32. + erm_e6th * 45. / 1024.;
	C3 = erm_e4th * 15. / 256. + erm_e6th * 45. / 1024.;
	C4 = erm_e6th * 35. / 3072.;

	coordSysParamsChanged.connect( BIND_SLOT1( CoordinateConverterTM::coordSysParamsChangedCB, this ) );
}


// ================================================
// ~CoordinateConverterTM
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CoordinateConverterTM::~CoordinateConverterTM()
{

}


// ================================================
// performConversion
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void CoordinateConverterTM::performConversion( 
	const CoordinateSet &in, CoordinateSet &out )
{
	double lla[3];
	double xyz[3];
	double gridDeclination;

	lla[0] = in.LatX;
	lla[1] = in.LonY;
	lla[2] = in.AltZ;

	gridDeclination = convertGDCtoTM( lla, xyz );

	out.Yaw = in.Yaw - gridDeclination;
	out.Pitch = in.Pitch;
	out.Roll = in.Roll;

	out.LatX = xyz[0];
	out.LonY = xyz[1];
	out.AltZ = xyz[2];
}


// ================================================
// performReverseConversion
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void CoordinateConverterTM::performReverseConversion( 
	const CoordinateSet &in, CoordinateSet &out )
{
	double lla[3];
	double xyz[3];
	double gridDeclination;

	xyz[0] = in.LatX;
	xyz[1] = in.LonY;
	xyz[2] = in.AltZ;

	gridDeclination = convertTMtoGDC( xyz, lla );

	out.Yaw = in.Yaw + gridDeclination;
	out.Pitch = in.Pitch;
	out.Roll = in.Roll;

	out.LatX = lla[0];
	out.LonY = lla[1];
	out.AltZ = lla[2];
}


// ================================================
// coordSysParamsChangedCB
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void CoordinateConverterTM::coordSysParamsChangedCB( CoordinateConverter* )
{
	if( coordSysParams.coordSys == CoordSysTM )
	{
		TMcentMeridian  = coordSysParams.coordSysParams.tmParams.CentralMeridian;
		TMfalseEasting  = coordSysParams.coordSysParams.tmParams.FalseEasting;
		TMfalseNorthing = coordSysParams.coordSysParams.tmParams.FalseNorthing;
		TM_k0           = coordSysParams.coordSysParams.tmParams.ScaleFactor;
	}
	else
	{
		// error - coordinate system specified isn't handled by this converter
	}
}


// ================================================
// convertGDCtoTM
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
double CoordinateConverterTM::convertGDCtoTM(
    const double LLA[3],  // Input Lat, Lon, Alt, in degrees and meters
    double xyup[3] )      // Output x, y, up in meters
{
	double lat = DTOR( LLA[0] );
	double lon = DTOR( LLA[1] );
	double N, T, C, A, M;
	double sinLat = sin( lat );
	double cosLat = cos( lat );
	double tanLat = sinLat / cosLat;

	N = erm_a / sqrt( 1 - erm_esqu * sinLat * sinLat );
	T = tanLat * tanLat;
	C = erm_eprimesqu * cosLat * cosLat;
	A = cosLat * ( lon - DTOR( TMcentMeridian ) );
	M = erm_a * ( C1 * lat - C2 * sin( 2 * lat ) + C3 * sin( 4 * lat ) - C4 * sin( 6 * lat ) );

	// Easting (x)
	xyup[0] =  TMfalseEasting +
	           TM_k0 * N * ( A + ( 1 - T + C ) * A * A * A / 6. +
	                         ( 5. - 18.*T + T * T + 72.*C - 58 * erm_eprimesqu ) * A * A * A * A * A / 120. );

	// Northing (y)
	xyup[1] = TMfalseNorthing +
	          TM_k0 * ( M + N * tanLat * ( A * A / 2. + ( 5. - T + 9 * C + 4 * C * C ) *
	                                       A * A * A * A / 24. + ( 61. - 58.*T + T * T + 600.*C - 330.*erm_eprimesqu ) *
	                                       A * A * A * A * A * A / 720. ) );

	xyup[2] = LLA[2];    // Altitude

	// Return Grid Declination (North correction angle), in degrees
	return sinLat * ( LLA[1] - TMcentMeridian );
}


// ================================================
// convertGDCtoTM
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
double CoordinateConverterTM::convertTMtoGDC(
    const double xyup[3],      // input in meters
    double LLA[3] )            // output in degrees, altitude in meters
{
	double e1sq = erm_e1 * erm_e1;
	double e1_3rd = e1sq * erm_e1;
	double e1_4th = e1sq * e1sq;

	double N1, T1, T1sq, C1, C1sq, R1, D, Dsq, D4th, D6th, M;
	double mu, phi1, sinPhi1, cosPhi1, tanPhi1;
	double x, y;

	x = xyup[0] - TMfalseEasting;
	y = xyup[1] - TMfalseNorthing;

	M = y / TM_k0;
	mu = M / ( erm_a * ( 1 - erm_esqu / 4 - 3 * erm_e4th / 64 - 5 * erm_e6th / 256 ) );

	phi1 = mu + ( 3.*erm_e1 /   2. - 27.*e1_3rd / 32. ) * sin( 2 * mu )
	       + ( 21.*e1sq   /  16. - 55.*e1_4th / 32. ) * sin( 4 * mu )
	       + ( 151.*e1_3rd /  96. ) * sin( 6 * mu )
	       + ( 1097.*e1_4th / 512. ) * sin( 8 * mu );
	sinPhi1 = sin( phi1 );
	cosPhi1 = cos( phi1 );
	tanPhi1 = sinPhi1 / cosPhi1;

	N1 = erm_a / sqrt( 1 - erm_esqu * sinPhi1 * sinPhi1 );
	T1 = tanPhi1 * tanPhi1;
	T1sq = T1 * T1;
	C1 = erm_eprimesqu * cosPhi1 * cosPhi1;
	C1sq = C1 * C1;
	R1 = erm_a * ( 1 - erm_esqu ) /
	     pow( 1 - erm_esqu * sinPhi1 * sinPhi1, 1.5 );
	D = x / ( N1 * TM_k0 );
	Dsq = D * D;
	D4th = Dsq * Dsq;
	D6th = D4th * Dsq;

	LLA[0] = RTOD( phi1 - ( N1 * tanPhi1 / R1 ) *
	               ( Dsq / 2. - ( 5. + 3.*T1 + 10.*C1 - 4.*C1sq - 9.*erm_eprimesqu ) * D4th / 24. +
	                 ( 61. + 90.*T1 + 298.*C1 + 45.*T1sq - 252.*erm_eprimesqu - 3.*C1sq ) *
	                 D6th / 720. ) );

	LLA[1] = TMcentMeridian +
	         RTOD(( D - ( 1. + 2.*T1 + C1 ) * Dsq * D / 6 +
	                ( 5. - 2.*C1 + 28.*T1 - 3.*C1sq + 8.*erm_eprimesqu + 24.*T1sq ) *
	                D4th * D / 120. ) / cosPhi1 );

	LLA[2] = xyup[2];

	// To avoid an extra sine function call, Grid Declination is
	// calculated using the "footprint" latitude (phi1) instead of the
	// final latitude.  The worst-case error at 3 degrees off the
	// central meridian is only 0.0016 degrees (occurs between 30 and 40
	// degrees latitude).
	return sinPhi1 * ( LLA[1] - TMcentMeridian );
}


#if 0
// ================================================
// Extrapolate
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void CoordinateConverterTM::Extrapolate( CoordinateSet &CigiCoord,
        CoordinateSet &DBaseCoord,
        ExtrapolationSet &ExtrapData,
        float dTime,
        bool TrajEn )
{
	int stat;
	double lla[3];
	double xyz[3];

	Vect3 RateV;  // Rate vector
	Vect3 RateVb;  // Rate vector (in body)
	Vect3 RateVa; // Rate vector - from accel parameters
	Vect3 EnuRate; // Rate vector - East-North-Up referance

	double tRate;
	double mRate;
	double cRate;

	float ypr[3];
	float nedv[3];
	float neda[3];

	float gypr[3];
	float gccv[3];
	float gcca[3];

	double gridDeclination = sin( CigiCoord.LatX ) *
	                         ( CigiCoord.LatX - TMcentMeridian );

	// All rates are in entity body coordinates
	// All accels are based on the North-East-Down
	//   coordinate system.

	// Get the current rate
	RateVb.Set( ExtrapData.XRate, ExtrapData.YRate, ExtrapData.ZRate );

	// Generate current rotation matrix
	Mtx3 RotMtx;
	RotMtx.GenRot( CigiCoord.Yaw, CigiCoord.Pitch, CigiCoord.Roll );

	// Convert Rates to ned
	RateV = RotMtx * RateVb;

	if( TrajEn )
	{
		mRate = RateV.mag();

		RateVa.Set( ExtrapData.AccelX, ExtrapData.AccelY, ExtrapData.AccelZ );

		// Determine drag coef.
		if( ExtrapData.TermVel > 0.000001 )
			ExtrapData.DragF = ( double )RateVa.mag() / ExtrapData.TermVel;
		else if( mRate > 0.000001 )
			ExtrapData.DragF = ( double )ExtrapData.RetardationRate / mRate;
		else
			ExtrapData.DragF = 0.0;

		RateVa = RateVa * dTime;
		RateV = RateV + RateVa;
		cRate = RateV.mag();
		tRate = cRate - ( cRate * ExtrapData.DragF * dTime );

		RateV = RateV.Unit() * tRate;

		// Convert back to body coordinate system
		RateVb = RateV * RotMtx;

		// Store current rates
		ExtrapData.XRate = RateVb.getx();
		ExtrapData.YRate = RateVb.gety();
		ExtrapData.ZRate = RateVb.getz();

	}

	// Convert Linear Rates to act in the Database Coordinate system
	//  and apply them to Database Corrdinate system entity position
	if( ConvMeth == 1 )  // WCS
	{
		// Not Supported at this time

	}
	else
	{
		if( ConvMeth == 2 )  // TM
		{
			// Convert from NED to ENU
			EnuRate.Set( RateV.gety(), RateV.getx(), -RateV.getz() );
			// this only affects the x and y rates
			Vect3 XYNorm( 0.0, 0.0, 1.0 );
			EnuRate.Rotate( XYNorm, gridDeclination );
		}
		else  // LCC  (ConvMeth == 3)
		{
			// Not Supported at this time

		}

		ApplyAttitudeRates( DBaseCoord, ExtrapData, dTime );

	}

	// Note: This is in east-north-up
	DBaseCoord.LatX += EnuRate.getx() * dTime;
	DBaseCoord.LonY += EnuRate.gety() * dTime;
	DBaseCoord.AltZ += EnuRate.getz() * dTime;


	// Generate lla position and attitude
	CoordDBase2Cigi( DBaseCoord, CigiCoord );
}


// ================================================
// ExtrapChild
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void CoordinateConverterTM::ExtrapChild( CoordinateSet &CigiCoord,
        CoordinateSet &DBaseCoord,
        ExtrapolationSet &ExtrapData,
        float dTime,
        bool TrajEn )
{

	Vect3 RateV;  // Rate vector (in parent entity)
	Vect3 RateVb;  // Rate vector (in body)
	Vect3 RateVa; // Rate vector - from accel parameters

	double tRate;
	double mRate;
	double cRate;

	// All rates are in entity body coordinates

	// Get the current rate
	RateVb.Set( ExtrapData.XRate, ExtrapData.YRate, ExtrapData.ZRate );

	// Generate current rotation matrix
	Mtx3 RotMtx;
	RotMtx.GenRot( CigiCoord.Yaw, CigiCoord.Pitch, CigiCoord.Roll );

	// Convert Rates to Parent entity coordinates
	RateV = RotMtx * RateVb;

	// Trajectory affects on child entities are not
	//  processed at this time
	/*
	if( TrajEn )
	{
		mRate = RateV.mag();

		RateVa.Set( ExtrapData.AccelX, ExtrapData.AccelY, ExtrapData.AccelZ );

		// Determine drag coef.
		if( ExtrapData.TermVel > 0.000001 )
			ExtrapData.DragF = ( double )RateVa.mag() / ExtrapData.TermVel;
		else if( mRate > 0.000001 )
			ExtrapData.DragF = ( double )ExtrapData.RetardationRate / mRate;
		else
			ExtrapData.DragF = 0.0;

		RateVa = RateVa * dTime;
		RateV = RateV + RateVa;
		cRate = RateV.mag();
		tRate = cRate - ( cRate * ExtrapData.DragF * dTime );

		RateV = RateV.Unit() * tRate;

		// Store current rates
		ExtrapData.XRate = RateV.getx();
		ExtrapData.YRate = RateV.gety();
		ExtrapData.ZRate = RateV.getz();

	}
	*/

	// update position
	// (DBase coord in enu not ned
	//  even in parent coord system)
	DBaseCoord.LatX += ( RateV.gety() * dTime );
	DBaseCoord.LonY += ( RateV.getx() * dTime );
	DBaseCoord.AltZ -= ( RateV.getz() * dTime );

	// update attitude
	ApplyAttitudeRates( DBaseCoord, ExtrapData, dTime );

	CigiCoord.LatX = DBaseCoord.LonY;
	CigiCoord.LonY = DBaseCoord.LatX;
	CigiCoord.AltZ = -DBaseCoord.AltZ;
	CigiCoord.Yaw = DBaseCoord.Yaw;
	CigiCoord.Pitch = DBaseCoord.Pitch;
	CigiCoord.Roll = DBaseCoord.Roll;

}


// ================================================
// ApplyAttitudeRates
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void CoordinateConverterTM::ApplyAttitudeRates( CoordinateSet &ConvEnt,
        ExtrapolationSet &Extrap,
        float dTime )
{

	Mtx3 Mrot;
	Mtx3 Mrate;

	// Create the Rotation matrix
	Mrot.GenRot( ConvEnt.Yaw, ConvEnt.Pitch, ConvEnt.Roll );

	// Create the rotation rate matrix
	Mrate.GenRot(( Extrap.YawRate * dTime ),
	             ( Extrap.PitchRate * dTime ),
	             ( Extrap.RollRate * dTime ) );

	// Make the rotation adjustment
	Mtx3 Att = Mrot * Mrate;

	// Get the angles
	Att.GetRot( ConvEnt.Yaw, ConvEnt.Pitch, ConvEnt.Roll );



	/* This is strictly NED relative attitude rates
	   not body relative attitude rates.
	   -- not used at this time
	===============================================
	// Determine the rotation change (delta - d)
	float dYaw = Extrap.YawRate * dTime;
	float dPitch = Extrap.PitchRate * dTime;
	float dRoll = Extrap.RollRate * dTime;

	// limit deltas
	while( dYaw > 360.0 )
	{
		dYaw -= 360.0;
	}

	while( dYaw < 0.0 )
	{
		dYaw += 360.0;
	}

	while( dPitch > 180.0 )
	{
		dPitch -= 360.0;
	}

	while( dPitch < -180.0 )
	{
		dPitch += 360.0;
	}

	while( dRoll > 180.0 )
	{
		dRoll -= 360.0;
	}

	while( dRoll < -180.0 )
	{
		dRoll += 360.0;
	}

	// Update YPR
	ConvEnt.Yaw = ConvEnt.Yaw + dYaw;
	ConvEnt.Pitch = ConvEnt.Pitch + dPitch;
	ConvEnt.Roll = ConvEnt.Roll + dRoll;

	// Correct and limit angles to proper values
	while( ConvEnt.Pitch > 180.0 )
	{
		ConvEnt.Pitch -= 360.0;
	}

	while( ConvEnt.Pitch < -180.0 )
	{
		ConvEnt.Pitch += 360.0;
	}

	if( ConvEnt.Pitch > 90.0 )
	{
		ConvEnt.Pitch = 180.0 - ConvEnt.Pitch;
		ConvEnt.Yaw += 180.0;
		ConvEnt.Roll += 180.0;
		Extrap.PitchRate = -Extrap.PitchRate;
	}
	else if( ConvEnt.Pitch < -90.0 )
	{
		ConvEnt.Pitch = -180.0 - ConvEnt.Pitch;
		ConvEnt.Yaw += 180.0;
		ConvEnt.Roll += 180.0;
		Extrap.PitchRate = -Extrap.PitchRate;
	}

	while( ConvEnt.Roll > 180.0 )
	{
		ConvEnt.Roll -= 360.0;
	}

	while( ConvEnt.Roll < -180.0 )
	{
		ConvEnt.Roll += 360.0;
	}

	// Yaw limit adjustment
	while( ConvEnt.Yaw > 360.0 )
	{
		ConvEnt.Yaw -= 360.0;
	}

	while( ConvEnt.Yaw < 0.0 )
	{
		ConvEnt.Yaw += 360.0;
	}
	*/

}
#endif


