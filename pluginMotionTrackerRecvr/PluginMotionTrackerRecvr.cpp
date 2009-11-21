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
 *  FILENAME:   PluginMotionTrackerRecvr.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  ...
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  08/24/2005 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 *
 *  2007-07-15 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#include <iostream>
#include <string.h> // for memset()

#include <osg/Matrix>

#include <CigiCompCtrlV3.h>

#include "AllCigi.h"
#include "PluginMotionTrackerRecvr.h"



EXPORT_DYNAMIC_CLASS( PluginMotionTrackerRecvr )

// ================================================
// PluginMotionTrackerRecvr
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginMotionTrackerRecvr::PluginMotionTrackerRecvr() : Plugin() 
{
	name_ = "PluginMotionTrackerRecvr";
	licenseInfo_.setLicense( LicenseInfo::LicenseGPL );
	licenseInfo_.setOrigin( "Boeing" );
	
	dependencies_.push_back( "PluginMotionTrackerMgr" );

	ImsgPtr = NULL;
	trackerParamsMap = NULL;
	
	componentCtrlP.motionTrackerRecvrPlugin = this;
}


// ================================================
// ~PluginMotionTrackerRecvr
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginMotionTrackerRecvr::~PluginMotionTrackerRecvr() throw() 
{
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginMotionTrackerRecvr::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{
	
	case SystemState::BlackboardRetrieve:
		// This state is for retrieving things from the blackboard

		bb_->get( "CigiIncomingMsg", ImsgPtr );

		// get the tracker parameters
		bb_->get( "TrackerParamsMap", trackerParamsMap );

		if( ImsgPtr != NULL )
		{
			ImsgPtr->RegisterEventProcessor(
				CIGI_COMP_CTRL_PACKET_ID_V3,
				(CigiBaseEventProcessor *) &componentCtrlP);
		}
		
		break;

	default:
		break;
	}
	
}


// ================================================
// OnPacketReceived
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginMotionTrackerRecvr::ComponentCtrlP::OnPacketReceived(CigiBasePacket *packet)
{
	CigiCompCtrlV3 *ccp = (CigiCompCtrlV3 *)packet;
	
	// check the 'class' for this component ctrl; we're only interested in 
	// 'view' component controls
	if( ccp->GetCompClassV3() == CigiBaseCompCtrl::ViewV3 )
	{
		int viewID = (int)ccp->GetInstanceID();
		int componentID = ccp->GetCompID();
		int trackerID = ccp->GetCompState();
		
		// we're using component 0 for motion tracker info
		if( componentID == 0 )
		{
			motionTrackerRecvrPlugin->updateMotionTracker(
				trackerID,
				viewID,
				osg::Quat(
					ccp->GetFloatCompData( 0 ),
					ccp->GetFloatCompData( 1 ),
					ccp->GetFloatCompData( 2 ),
					ccp->GetFloatCompData( 3 ) )
				);
		}
		
	}
}

// ================================================
// updateMotionTracker
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginMotionTrackerRecvr::updateMotionTracker( int trackerID, int viewID, 
		osg::Quat orientation )
{
	
	if( trackerParamsMap == NULL )
		return;
	
	TrackerParams *trackerParams = (*trackerParamsMap)[trackerID];
	if( trackerParams == NULL )
	{
		// The tracker ID specified doesn't yet have an entry in 
		// trackerParamsMap.  It will be created and added to the map.
		// The default values set by TrackerParams constructor should be good 
		// enough.
		(*trackerParamsMap)[trackerID] = trackerParams = new TrackerParams;
	}
	
	// this tracker plugin generates quaternions
	trackerParams->useQuaternion = true;
	
	trackerParams->trackerRotateQuat[0] = orientation.x();
	trackerParams->trackerRotateQuat[1] = orientation.y();
	trackerParams->trackerRotateQuat[2] = orientation.z();
	trackerParams->trackerRotateQuat[3] = orientation.w();

	trackerParams->trackerAbsoluteYaw = 0.0;

	trackerParams->trackerOffset[0] = 0.0;
	trackerParams->trackerOffset[1] = 0.0;
	trackerParams->trackerOffset[2] = 0.0;
	
}



