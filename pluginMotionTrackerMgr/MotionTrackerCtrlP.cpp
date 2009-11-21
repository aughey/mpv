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
 *  FILENAME:   MotionTrackerCtrlP.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  listen for MotionTrackerControl packets and update the contents of the 
 *  blackboard accordingly.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  06/28/2005 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */

// MotionTrackerCtrlP.cpp: implementation of the MotionTrackerCtrlP class.
//
//////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string.h> // for memset

#include "MotionTrackerCtrlP.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// ================================================
// MotionTrackerCtrlP()
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
MotionTrackerCtrlP::MotionTrackerCtrlP()
{
	trackerParamsMap = NULL;
}

// ================================================
// ~MotionTrackerCtrlP()
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
MotionTrackerCtrlP::~MotionTrackerCtrlP()
{

}

// ================================================
// OnPacketReceived(CigiBasePacket *Packet)
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void MotionTrackerCtrlP::OnPacketReceived(CigiBasePacket *Packet)
{
	CigiMotionTrackCtrlV3 *mtc = (CigiMotionTrackCtrlV3 *)(Packet);
	
	if( !trackerParamsMap ) return;
	
	int trackerID = mtc->GetTrackerID();
	TrackerParams *trackerParams = (*trackerParamsMap)[trackerID];
	if( trackerParams == NULL )
	{
		// The tracker ID specified doesn't yet have an entry in 
		// trackerParamsMap.  It will be created and added to the map.
		(*trackerParamsMap)[trackerID] = trackerParams = new TrackerParams;
	}
	
	trackerParams->trackerID = trackerID;
	trackerParams->enableTracker = mtc->GetTrackerEn();
	trackerParams->boresight = mtc->GetBoresightEn();
	trackerParams->enableX = mtc->GetXEn();
	trackerParams->enableY = mtc->GetYEn();
	trackerParams->enableZ = mtc->GetZEn();
	trackerParams->enableRoll = mtc->GetRollEn();
	trackerParams->enablePitch = mtc->GetPitchEn();
	trackerParams->enableYaw = mtc->GetYawEn();

	trackerParams->viewGroupID = 0;
	trackerParams->viewID = 0;
	if( mtc->GetScope() == CigiBaseMotionTrackCtrl::ViewGrp )
	{
		trackerParams->viewGroupID = mtc->GetViewID();
	}
	else
	{
		trackerParams->viewID = mtc->GetViewID();
	}
}

