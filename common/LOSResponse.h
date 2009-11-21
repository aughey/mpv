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


#ifndef _LOS_RESPONSE_H_
#define _LOS_RESPONSE_H_

#include <list>

#include "Vect3.h"
#include "Vect4.h"
#include "Referenced.h"
#include "LOSRequest.h"

namespace mpv
{

//=========================================================
//! Line-of-sight response.  Used by the mission functions manager to 
//! communicate with mission functions request handlers.
//! 
class MPVCMN_SPEC LOSResponse : public Referenced
{
public:
	//=========================================================
	//! Constructor
	//! \param r The request that this response addresses
	//! 
	LOSResponse();
	
	//! Position of the LOS hit, in database coordinates
	Vect3 hitLocation;
	
	//! Color of polygon/texel at the hit location
	Vect4 color;
	
	//! Material code of polygon/texel at the hit location
	unsigned int materialCode;
	
	//! Normal of the polygon that was hit.  Unit vector, in database 
	//! coordinates relative to hitLocation.
	Vect3 normal;
	
	//! ID of the entity that this LOS hit.  Set to -1 if no entity was hit.
	int entityID;

protected:
	
	//=========================================================
	//! General Destructor
	//! 
	virtual ~LOSResponse();
	
};

typedef std::list< RefPtr<LOSResponse> > LOSResponseList;

}

#endif
