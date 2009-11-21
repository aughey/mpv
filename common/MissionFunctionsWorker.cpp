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


#include "BindSlot.h"

#include "MissionFunctionsWorker.h"

using namespace mpv;

// ================================================
// MissionFunctionsWorker
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
MissionFunctionsWorker::MissionFunctionsWorker() : Referenced()
{
	
}


// ================================================
// ~MissionFunctionsWorker
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
MissionFunctionsWorker::~MissionFunctionsWorker() 
{
	
}


void MissionFunctionsWorker::connectToNewHOTRequestSignal( boost::signal<void (mpv::RefPtr<mpv::HOTRequest>)> *hotSignal )
{
	hotSignal->connect( BIND_SLOT1( MissionFunctionsWorker::_processHOTRequest, this ) );
}


void MissionFunctionsWorker::connectToNewLOSRequestSignal( boost::signal<void (mpv::RefPtr<mpv::LOSRequest>)> *losSignal )
{
	losSignal->connect( BIND_SLOT1( MissionFunctionsWorker::_processLOSRequest, this ) );
}


	
void MissionFunctionsWorker::processHOTRequest( mpv::RefPtr<mpv::HOTRequest> request )
{
	// do-nothing processing; just produces an empty list of responses
	finishedHOTRequest( request->id, HOTResponseList() );
}


void MissionFunctionsWorker::processLOSRequest( mpv::RefPtr<mpv::LOSRequest> request )
{
	// do-nothing processing; just produces an empty list of responses
	finishedLOSRequest( request->id, LOSResponseList() );
}


