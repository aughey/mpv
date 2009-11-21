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


#ifndef _COORDINATECONVERTER_H_
#define _COORDINATECONVERTER_H_

#include "Referenced.h"
#include "CoordSet.h"
#include "GeodeticObject.h"
#include "CoordSysParams.h"
#include "MPVCommonTypes.h"


namespace mpv
{

//=========================================================
//! 
//! 
class MPVCMN_SPEC CoordinateConverter : public Referenced
{
public:
	boost::signal<void (CoordinateConverter*)> coordSysParamsChanged;

	//=========================================================
	//! General Constructor
	//! 
	CoordinateConverter( CoordinateSystem from, CoordinateSystem to );
	
	void setParams( const CoordSysParams &newCoordSysParams );
	
	CoordinateSystemPair getCoordinateSystems() const;
	void getCoordinateSystems( CoordinateSystem &from, CoordinateSystem &to ) const;
	CoordinateSystem getCoordinateSystemFrom() const { return coordSysFrom; }
	CoordinateSystem getCoordinateSystemTo() const { return coordSysTo; }
	
	//! Convenience function which retrieves geodeticObject's GDC position 
	//! and converts it to database coordinates.  Method assumes that 
	//! this converter's coordSysFrom is GDC.
	void performConversion( GeodeticObject *geodeticObject );
	
	virtual void performConversion( const CoordinateSet &in, CoordinateSet &out ) = 0;
	
	virtual void performReverseConversion( const CoordinateSet &in, CoordinateSet &out ) = 0;
	
protected:
	//=========================================================
	//! General Destructor
	//! 
	virtual ~CoordinateConverter();
	
	CoordinateSystem coordSysFrom;
	CoordinateSystem coordSysTo;

	CoordSysParams coordSysParams;
};

}
#endif
