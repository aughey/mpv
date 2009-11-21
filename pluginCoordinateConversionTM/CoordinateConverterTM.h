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


#ifndef _COORDINATE_CONVERTER_TM_H_
#define _COORDINATE_CONVERTER_TM_H_


#include "CoordinateConverter.h"


//=========================================================
//! This class performs coordinate conversion for Transverse Mercator (TM) 
//! coordinate systems.
//!
class CoordinateConverterTM : public mpv::CoordinateConverter
{
public:

	//=========================================================
	//! General Constructor
	//!
	CoordinateConverterTM();

	virtual void performConversion( const CoordinateSet &in, CoordinateSet &out );
	
	virtual void performReverseConversion( const CoordinateSet &in, CoordinateSet &out );

	void coordSysParamsChangedCB( mpv::CoordinateConverter* );
	
protected:

	//=========================================================
	//! General Destructor
	//!
	virtual ~CoordinateConverterTM();

	//=========================================================
	//! Earth Reference Model values
	//!
	double erm_a;     //!< Semi-major axis (ellipsoid equatorial radius)
	double erm_f;     //!< flattening f = (a-b)/a or f = 1-sqrt(1-esqu)
	double erm_b;     //!< Semi-minor axis (ellipsoid polar radius)
	double erm_esqu;  //!< eccentricity squared = 2f-f^2
	double erm_e4th;  //!< eccentricity ^ 4th
	double erm_e6th;  //!< eccentricity ^ 6th
	double erm_e;     //!< eccentricity = sqrt(esqu)
	double erm_e1;    //!< (1-sqrt(1-esqu)) / (1+sqrt(1-esqu)), used in TM
	double erm_eprimesqu;    //!< (a^2 - b^2) / b^2

	// Transverse Mercator map parameters
	double TMcentMeridian;     //!< degrees
	double TMfalseEasting;     //!< offset of x=0 from central meridian, meters
	double TMfalseNorthing;    //!< offset of y=0 from equator
	double TM_k0;              //!< Scale at central meridian
	double C1, C2, C3, C4;     //!< Constants used in GDC to TM


	//=========================================================
	//! \param LLA - Input Lat, Lon, Alt, in degrees and meters
	//! \param xyup - Output x, y, up in meters
	//!
	double convertGDCtoTM(
	    const double LLA[3],
	    double xyup[3] );

	//=========================================================
	//! \param xyup - input in meters
	//! \param LLA - output in degrees, altitude in meters
	double convertTMtoGDC(
	    const double xyup[3],
	    double LLA[3] );

#if 0
	//=========================================================
	//! Rate interpolation calculations
	//! \param CigiCoord - The position and attitude of the
	//!   entity in the CIGI coordinate system
	//! \param DBaseCoord - The position and attitude of the
	//!   entity in the Database coordinate system
	//! \param ExtrapData - The rates and accelerations
	//!    needed to extrapolate the current position
	//! \param dTime - The delta time to determine the interpolation
	//! \param TrajEn - A flag specifying that the entity's trajectory
	//!    data is to be used
	//!
	void Extrapolate( CoordinateSet &CigiCoord,
	                  CoordinateSet &DBaseCoord,
	                  ExtrapolationSet &ExtrapData,
	                  float dTime,
	                  bool TrajEn );

	//=========================================================
	//! Rate interpolation calculations for child entities
	//! \param ConvEnt - The position and rate data structure
	//!   of the entity whose coordinates are to be converted.
	//! \param dTime - The delta time to determine the interpolation
	//! \param TrajEn - A flag specifying that the entity's trajectory
	//!    data is to be used
	//!
	void ExtrapChild( CoordinateSet &CigiCoord,
	                  CoordinateSet &DBaseCoord,
	                  ExtrapolationSet &ExtrapData,
	                  float dTime,
	                  bool TrajEn );

	//=========================================================
	//! Adjusts ConvEnt's yaw/pitch/roll according to the attitude rates.
	//! Also ensures that the results are within the valid range of values.
	//! \param ConvEnt - The entity's current position and attitude
	//! \param Extrap  - The entity's current extrapolation data.
	//! \param dTime - The delta time the interpolation uses to
	//!    determine the new position and attitude
	//!
	void ApplyAttitudeRates( CoordinateSet &ConvEnt,
	                         ExtrapolationSet &Extrap,
	                         float dTime );
#endif
};

#endif
