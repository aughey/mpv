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


#ifndef _COORDINATECONVERSIONOBSERVER_H_
#define _COORDINATECONVERSIONOBSERVER_H_

#include "Referenced.h"
#include "GeodeticObject.h"
#include "CoordinateConverter.h"
#include "MPVCommonTypes.h"


namespace mpv
{

//=========================================================
//! 
//! 
class MPVCMN_SPEC CoordinateConversionObserver : public Referenced
{
public:
	//=========================================================
	//! General Constructor
	//! 
	CoordinateConversionObserver();
	
	void setCoordinateConverter( CoordinateConverter *newConverter );
	
	void startObserving( GeodeticObject *geodeticObject );

	void stopObserving( GeodeticObject *geodeticObject );
	
protected:
	//=========================================================
	//! General Destructor
	//! 
	virtual ~CoordinateConversionObserver();
	
	void performCoordinateConversion( GeodeticObject *geodeticObject );

	void coordSysParamsChanged( CoordinateConverter *c );

	//! This method will be called when the database parameters change.
	//! The child class will need to iterate over the objects being observed 
	//! and repeat conversion for each.
	virtual void performConversionForAllObservedObjects() = 0;
	
	RefPtr<CoordinateConverter> converter;
};

}
#endif
