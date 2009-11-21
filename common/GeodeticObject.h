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


#ifndef _GEODETICOBJECT_H_
#define _GEODETICOBJECT_H_


#include "Referenced.h"
#include "CoordSet.h"
#include "MPVCommonTypes.h"


namespace mpv
{

//=========================================================
//! Any class which is positioned in geodetic space (lat lon alt) should 
//! inherit from this class.  This class works with the coordinate conversion 
//! observer to ease coordinate conversion and consolidate related code.
//! 
class MPVCMN_SPEC GeodeticObject : public virtual Referenced
{
public:

	boost::signal<void (GeodeticObject*)> positionGDCChanged;
	boost::signal<void (GeodeticObject*)> positionDBChanged;

	//=========================================================
	//! General Constructor
	//! 
	GeodeticObject();
	
	const CoordinateSet &getPositionGDC() const { return positionGDC; }

	const CoordinateSet &getPositionDB() const { return positionDB; }


	void setPositionGDC( const CoordinateSet &newPositionGDC );

	void setPositionDB( const CoordinateSet &newPositionDB );


protected:
	//=========================================================
	//! General Destructor
	//! 
	virtual ~GeodeticObject();
	
	
	//=========================================================
	//! The geodetic coordinate position, and attitude of the object
	//!
	CoordinateSet positionGDC;

	//=========================================================
	//! The database coordinate position, and attitude of the object
	//!
	CoordinateSet positionDB;

};

}
#endif
