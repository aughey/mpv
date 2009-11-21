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
 *  2008-09-01 Andrew Sampson
 *      Initial release
 *  
 *  
 *  </pre>
 */


#ifndef _MISSIONFUNCTIONSWORKER_H_
#define _MISSIONFUNCTIONSWORKER_H_

#include <list>

#include "Referenced.h"
#include "HOTRequest.h"
#include "LOSRequest.h"
#include "HOTResponse.h"
#include "LOSResponse.h"

namespace mpv
{

//=========================================================
//! In Command pattern terminology, this class is the Handler and 
//! HOTRequest/LOSRequest is the Command.
//! 
class MPVCMN_SPEC MissionFunctionsWorker : public Referenced
{
public:
	
	boost::signal<void (int, HOTResponseList)> finishedHOTRequest;
	boost::signal<void (int, LOSResponseList)> finishedLOSRequest;
	
	//=========================================================
	//! General Constructor
	//! 
	MissionFunctionsWorker();
	
	void connectToNewHOTRequestSignal( boost::signal<void (mpv::RefPtr<mpv::HOTRequest>)> *hotSignal );
	void connectToNewLOSRequestSignal( boost::signal<void (mpv::RefPtr<mpv::LOSRequest>)> *losSignal );
	
protected:

	//=========================================================
	//! General Destructor
	//! 
	virtual ~MissionFunctionsWorker();
	
	//! child classes are expected to override this method
	virtual void processHOTRequest( mpv::RefPtr<mpv::HOTRequest> request );
	
	//! child classes are expected to override this method
	virtual void processLOSRequest( mpv::RefPtr<mpv::LOSRequest> request );
	
private:
	
	void _processHOTRequest( mpv::RefPtr<mpv::HOTRequest> request )
	{
		processHOTRequest( request );
	}

	void _processLOSRequest( mpv::RefPtr<mpv::LOSRequest> request )
	{
		processLOSRequest( request );
	}
	
};

}

#endif
