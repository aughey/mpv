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


#ifndef _LOS_REQUEST_H_
#define _LOS_REQUEST_H_

#include <CigiBaseLosSegReq.h>

#include "Vect3.h"
#include "Vect4.h"
#include "Referenced.h"

namespace mpv
{

//=========================================================
//! Note: "Vector" requests, where a starting point and a direction are 
//! specified, are decomposed into point-to-point ("Segment") requests.
//! 
class MPVCMN_SPEC LOSRequest : public Referenced
{
public:
	
	//! The type of response that the Host requested.
	//! Note - CCL's LOS segment and vector packet classes don't share enums, 
	//! so this class has its own.
	enum RequestType
	{
		Basic,
		Extended
	};
	
	//! Coordinate system for LOS responses
	//! Note - CCL's LOS segment and vector packet classes don't share enums, 
	//! so this class has its own.
	enum ResponseCoordinateSystem
	{
		Geodetic,
		Entity
	};
	
	//=========================================================
	//! General Constructor
	//! 
	LOSRequest();
	
	//! LOS request ID
	int id;
	
	//! starting point for the ray, in database coordinates
	Vect3 start;
	
	//! end point for the ray, in database coordinates
	Vect3 end;
	
	//! 
	unsigned char alphaThreshold;
	
	//! According to the standard, each bit in this int corresponds to a 
	//! range of material IDs.  If the bit is 1, then the polygons with 
	//! materials in that range will be considered for LOS testing.  
	unsigned int materialMask;
	
	//! the request type, ie basic or extended
	RequestType type;
	
	//! Original reference point for the ray, in database coordinates.
	//! For los segment requests, this will be equal to "start".
	//! For los vector requests where minRange > 0, this value 
	//! will not equal "start".
	//! This is used when generating the response packet, and isn't needed 
	//! by mission functions workers.
	Vect3 origin;
	
	//! The coordinate system that the host requested for hits on entities.
	//! This is used when generating the response packet, and isn't needed 
	//! by mission functions workers.
	ResponseCoordinateSystem requestedResponseCoordinateSystem;
	
	//! The frame number from the IGCtrl that accompanied the LOS Request.
	//! This is used when generating the response packet, and isn't needed 
	//! by mission functions workers.
	unsigned int hostFrameNumber;
	
protected:
	
	//=========================================================
	//! General Destructor
	//! 
	virtual ~LOSRequest();
	
};

}

#endif
