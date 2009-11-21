/** <pre>
 *  PROJECTNAME
 *  Copyright (c) 2008 AUTHORNAME
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
 *  2008-08-03 AUTHORNAME
 *      Initial release
 *  
 *  
 *  </pre>
 */

#include "BindSlot.h"

#include "CoordinateConverterEasyTM.h"


// ================================================
// CoordinateConverterEasyTM
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CoordinateConverterEasyTM::CoordinateConverterEasyTM() : 
	CoordinateConverter( CoordSysGDC, CoordSysEasyTM )
{
	converter = new CoordinateConverterTM();
	coordSysParamsChanged.connect( BIND_SLOT1( CoordinateConverterEasyTM::coordSysParamsChangedCB, this ) );
}


// ================================================
// ~CoordinateConverterEasyTM
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CoordinateConverterEasyTM::~CoordinateConverterEasyTM() 
{
	
}


// ================================================
// performConversion
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void CoordinateConverterEasyTM::performConversion( 
	const CoordinateSet &in, CoordinateSet &out )
{
	converter->performConversion( in, out );
}


// ================================================
// performReverseConversion
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void CoordinateConverterEasyTM::performReverseConversion( 
	const CoordinateSet &in, CoordinateSet &out )
{
	converter->performReverseConversion( in, out );
}


// ================================================
// coordSysParamsChangedCB
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void CoordinateConverterEasyTM::coordSysParamsChangedCB( CoordinateConverter* )
{
	// construct a coordSysParams with coord sys set to TM and 
	// false easting/northing set to 0's
	CoordSysParams allZeroesTMParams;
	allZeroesTMParams.coordSys = CoordSysTM;
	allZeroesTMParams.coordSysParams.tmParams.CentralMeridian = 
		coordSysParams.coordSysParams.easyTMParams.CentralMeridian;
	allZeroesTMParams.coordSysParams.tmParams.FalseEasting = 0.0;
	allZeroesTMParams.coordSysParams.tmParams.FalseNorthing = 0.0;
	allZeroesTMParams.coordSysParams.tmParams.ScaleFactor = 
		coordSysParams.coordSysParams.easyTMParams.ScaleFactor;
	converter->setParams( allZeroesTMParams );

	// We can produce a fairly accurate estimate of the false easting/northing 
	// by performing a GDC->TM conversion on the database origin lat/lon.  
	// The resulting database coordinates are used as false easting/northing.
	// The false easting will probably be 0, since we're probably using the 
	// origin longitude as the central meridian.
	CoordinateSet in, out;
	in.LatX = coordSysParams.coordSysParams.easyTMParams.OriginLatitude;
	in.LonY = coordSysParams.coordSysParams.easyTMParams.OriginLongitude;
	converter->performConversion( in, out );
	
	// construct a coordSysParams with the estimated false easting/northing
	CoordSysParams estimatedTMParams;
	estimatedTMParams.coordSys = CoordSysTM;
	estimatedTMParams.coordSysParams.tmParams.CentralMeridian = 
		coordSysParams.coordSysParams.easyTMParams.CentralMeridian;
	estimatedTMParams.coordSysParams.tmParams.FalseEasting = -1.0 * out.LatX;
	estimatedTMParams.coordSysParams.tmParams.FalseNorthing = -1.0 * out.LonY;
	estimatedTMParams.coordSysParams.tmParams.ScaleFactor = 
		coordSysParams.coordSysParams.easyTMParams.ScaleFactor;
	converter->setParams( estimatedTMParams );
	
}


