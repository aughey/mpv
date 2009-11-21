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
 *  03/29/2004 Andrew Sampson
 *      Initial Release.
 *  
 *  2008-10-05 Andrew Sampson
 *      Renamed from DatabaseParams to CoordSysParams, removed all members 
 *      not related to coordinate system.
 *  
 * </pre>
 */


#ifndef _COORDSYSPARAMS_INCLUDED_
#define _COORDSYSPARAMS_INCLUDED_

#include <string>
#include <string.h> // for memset
#include <utility> // for std::pair

#include "MPVCommonTypes.h"


//=========================================================
//! The enumeration for the coordinate system types
//!
enum CoordinateSystem
{
	CoordSysGDC,      //!< Geodetic (lat lon alt)
	CoordSysGCC,      //!< Geocentric
	CoordSysTM,       //!< Transverse Mercator
	CoordSysEasyTM,   //!< Easy Transverse Mercator
	CoordSysUTM,      //!< Universal Transverse Mercator
	CoordSysLCC,      //!< Lambert Conformal Conic

	CoordSysUNKNOWN
};

typedef std::pair<CoordinateSystem, CoordinateSystem> CoordinateSystemPair;


//=========================================================
//! Structure containing parameters for databases using the 
//! GCC coordinate system
//!
struct MPVCMN_SPEC GCCParams {
	int dummy;
};

//=========================================================
//! Structure containing parameters for databases using the 
//! TM coordinate system
//!
struct MPVCMN_SPEC TMParams {
	double CentralMeridian;
	double FalseEasting;
	double FalseNorthing;
	double ScaleFactor;
};

//=========================================================
//! Structure containing parameters for databases using the 
//! EasyTM coordinate system
//!
struct MPVCMN_SPEC EasyTMParams {
	double CentralMeridian;
	double OriginLatitude;
	double OriginLongitude;
	double ScaleFactor;
};

//=========================================================
//! Structure containing parameters for databases using the 
//! UTM coordinate system
//!
struct MPVCMN_SPEC UTMParams {
	int Zone;
};

//=========================================================
//! Structure containing parameters for databases using the 
//! LCC coordinate system
//!
struct MPVCMN_SPEC LCCParams {
	double LatOrig;
	double LonOrig;
	double Lat1;
	double Lat2;
	unsigned int Line;
	unsigned int Orientation;
	unsigned int Option;
};


//=========================================================
//! Union of the parameter structures for the various coordinate systems.
//! Making it a union helps save a few bytes (not really important) 
//! and helps to remind other programmers that only one set of parameters 
//! is valid at any given time (more important).
//!
typedef MPVCMN_SPEC union {
	struct GCCParams	gccParams;
	struct TMParams		tmParams;
	struct EasyTMParams easyTMParams;
	struct UTMParams    utmParams;
	struct LCCParams	lccParams;
} CombinedParamType;

//=========================================================
//! Contains data necessary for defining a geo-reference coordinate system.
//!
struct MPVCMN_SPEC CoordSysParams
{
	CoordSysParams()
	{
		// initialize the database parameters
		coordSys = CoordSysTM;
		memset( &coordSysParams, 0, sizeof( coordSysParams ) );
	}
	
	//! The coordinate system of the database.  The coordinate system 
	//! is the algorithm by which real-world positions (latitude, longitude, 
	//! altitude) are turned into database positions.  This conversion 
	//! process is a necessary evil, resulting from mapping positions on a 
	//! curved surface (the earth) to positions on a flat surface (the 
	//! terrain database).
	CoordinateSystem coordSys;
	
	//! A union containing one of several different sets of parameters 
	//! pertaining to the coordinate system in use.
	CombinedParamType coordSysParams;
	
};


#endif
