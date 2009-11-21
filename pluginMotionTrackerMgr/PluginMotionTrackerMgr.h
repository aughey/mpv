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
 *  FILENAME:   PluginMotionTrackerMgr.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  listen for MotionTrackerControl packets and make the 
 *  information available to other plugins via the blackboard.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  06/28/2005 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 *
 *  2007-07-14 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#ifndef PLUGIN_MOTIONTRACKERMGR_H
#define PLUGIN_MOTIONTRACKERMGR_H

#include <string>
#include <map>

#include "Plugin.h"
#include "DefFileParser.h"
#include "TrackerParams.h"
#include "AllCigi.h"

#include "MotionTrackerCtrlP.h"
#include "PositionRequestP.h"

using namespace MotionTrackerMgr;

//=========================================================
//! This plugin listens for MotionTrackerControl packets and makes the 
//! information available to other plugins via the blackboard.
//! 
class PluginMotionTrackerMgr : public Plugin 
{
public:
	//=========================================================
	//! General Constructor
	//! 
	PluginMotionTrackerMgr();

	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginMotionTrackerMgr() throw();
	
	//=========================================================
	//! The per-frame processing that this plugin performs.
	//! This plugin's act() calls processTracker for each of the 
	//! active trackers.
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );
	
	
private:
	//=========================================================
	//! The incoming message buffer.  Retrieved from the blackboard.
	//! 
	CigiIncomingMsg *ImsgPtr;

	//=========================================================
	//! The outgoing message buffer.  Retrieved from the blackboard.
	//! 
	CigiOutgoingMsg *OmsgPtr;

	//=========================================================
	//! The root of the def-file tree.  Retrieved from the blackboard.
	//! 
	DefFileGroup **DefFileData;
	
	//=========================================================
	//! A std::map of tracker IDs to TrackerParams.  Posted to the blackboard.
	//! 
	std::map< int, TrackerParams * > trackerParamsMap;

	//=========================================================
	//! The packet processor for motion tracker control packets
	//! 
	MotionTrackerCtrlP motionTrackerCtrlP;
	
	//=========================================================
	//! The packet processor for position request packets
	//! 
	PositionRequestP positionRequestP;

	//=========================================================
	//! Retrieves some preferences from the configuration data
	//! 
	void processConfigData( void );
	
	//=========================================================
	//! Processes a given motion tracker.  This includes calculating the 
	//! boresight matrices, and producing a final transformation matrix for 
	//! use by the camera plugin.
	//! \param trackerParams - the tracker to process
	//! 
	void processTracker( TrackerParams *trackerParams );

	//=========================================================
	//! If the Host has requested the position of a tracker, this method will 
	//! send a response to that request.
	//! \param trackerParams - the tracker to use when reporting position and orientation
	//! 
	void replyToPositionRequests( TrackerParams *trackerParams );
};



#endif

