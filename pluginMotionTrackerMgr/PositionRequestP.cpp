/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2004 The Boeing Company
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
 *  FILENAME:   PositionRequestP.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  listen for Position Request packets and act on 
 *  requests for position info from motion trackers.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  10/16/2005 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#include <iostream>
#include <string.h> // for memset

#include "PositionRequestP.h"

using namespace MotionTrackerMgr;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// ================================================
// PositionRequestP()
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PositionRequestP::PositionRequestP()
{
	trackerParamsMap = NULL;
}

// ================================================
// ~PositionRequestP()
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PositionRequestP::~PositionRequestP()
{

}

// ================================================
// OnPacketReceived(CigiBasePacket *Packet)
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PositionRequestP::OnPacketReceived(CigiBasePacket *Packet)
{
	CigiPositionReqV3 *pr = (CigiPositionReqV3 *)(Packet);
	
	if( !trackerParamsMap ) return;
	
	if( pr->GetObjectClass() != CigiBasePositionReq::MotionTracker )
		return;
	
	int trackerID = pr->GetObjectID();

	std::map< int, TrackerParams * >::iterator tpIter = 
		trackerParamsMap->find( trackerID );
	if( tpIter == trackerParamsMap->end() )
	{
		std::cout << "Warning: Host requested position for unknown motion tracker #" 
			<< trackerID << std::endl;
		return;
	}

	TrackerParams *trackerParams = (*tpIter).second;
	
	switch( pr->GetUpdateMode() )
	{
	case CigiBasePositionReq::Continuous:
		trackerParams->positionRequestStatus = TrackerParams::PositionReqContinuous;
		break;
	case CigiBasePositionReq::OneShot:
	default:
		trackerParams->positionRequestStatus = TrackerParams::PositionReqOneShot;
		break;
	
	}
}

