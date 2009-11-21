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


#ifndef _HOT_RESPONSE_H_
#define _HOT_RESPONSE_H_

#include <list>

#include "Vect3.h"
#include "Vect4.h"
#include "Referenced.h"

namespace mpv
{

//=========================================================
//! Height-of-terrain response.  Used by the mission functions manager to 
//! communicate with mission functions request handlers.
//! 
class MPVCMN_SPEC HOTResponse : public Referenced
{
public:
	//=========================================================
	//! General Constructor
	//! 
	HOTResponse();
	
	//! Position of the HOT hit, in database coordinates.
	//! (Why not just a double?  Because of geocentric databases.)
	Vect3 hitLocation;
	
	//! Material code of polygon/texel at the hit location
	int materialCode;
	
	//! Normal of the polygon that was hit.  Unit vector, in database 
	//! coordinates relative to hitLocation.
	Vect3 normal;
	
protected:
	
	//=========================================================
	//! General Destructor
	//! 
	virtual ~HOTResponse();
	
};

typedef std::list< RefPtr<HOTResponse> > HOTResponseList;

}

#endif
