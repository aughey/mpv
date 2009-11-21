/** <pre>
 *  CIGI Multi-Purpose Viewer common library
 *  Copyright (c) 2008 The Boeing Company
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
 *  2008-08-30 Andrew Sampson
 *      Initial release
 *  
 *  
 *  </pre>
 */


#ifndef _HOT_REQUEST_H_
#define _HOT_REQUEST_H_

#include <CigiBaseHatHotReq.h>

#include "Vect3.h"
#include "Vect4.h"
#include "Referenced.h"

namespace mpv
{

//=========================================================
//! Note: HAT requests are decomposed into HOT requests.
//! 
class MPVCMN_SPEC HOTRequest : public Referenced
{
public:
	//=========================================================
	//! General Constructor
	//! 
	HOTRequest();
	
	//! HOT request ID
	int id;
	
	//! Request location, in database coordinates
	Vect3 location;
	
	//! Request location, with altitude=0, in database coordinates.
	//! Valid if request type is HOT or Extended.  (HAT requests don't 
	//! need this information.)
	Vect3 locationMSL;
	
	//! "Up" vector at the request location, in database coordinates.
	//! This value is obvious for flat-earth databases, but is significant 
	//! for geocentric databases.
	Vect3 up;
	
	//! the request type, ie hat, hot, or extended
	CigiBaseHatHotReq::ReqTypeGrp type;
	
	//! The frame number from the IGCtrl that accompanied the HOT Request.
	//! This is used when generating the response packet.
	unsigned int hostFrameNumber;
	
protected:
	
	//=========================================================
	//! General Destructor
	//! 
	virtual ~HOTRequest();
	
};

}

#endif
