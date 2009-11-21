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
 *  FILENAME:   PluginMotionTrackerRecvr.h
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


#ifndef PLUGIN_MOTION_TRACKER_RECVR_H
#define PLUGIN_MOTION_TRACKER_RECVR_H

#include <map>

#include <osg/Quat>

#include "Plugin.h"
#include "TrackerParams.h"
#include "AllCigi.h"
#include "DefFileGroup.h"

//=========================================================
//! This plugin is 
//! 
class PluginMotionTrackerRecvr : public Plugin 
{
public:

	//=========================================================
	//! General Constructor
	//! 
	PluginMotionTrackerRecvr();

	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginMotionTrackerRecvr() throw();
	
	//=========================================================
	//! The per-frame processing that this plugin performs
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );
	
	
private:

	//=========================================================
	//! A pointer to the Cigi incoming message object.  Retrieved 
	//! from the blackboard.
	//!
	CigiIncomingMsg *ImsgPtr;

	//=========================================================
	//! A std::map of tracker IDs to TrackerParams.  Retrieved from the 
	//! blackboard.
	//! 
	std::map< int, TrackerParams * > *trackerParamsMap;

	//=========================================================
	//! The class handling the CIGI component control packets.
	//!
	class ComponentCtrlP : public CigiBaseEventProcessor  
	{
	public:
	
		//=========================================================
		//! The callback handler for the CIGI component control packet
		//! \param packet - The pointer to the component control packet object.
		//!
		virtual void OnPacketReceived(CigiBasePacket *packet);

		//=========================================================
		//! A pointer to the plugin
		//!
		PluginMotionTrackerRecvr *motionTrackerRecvrPlugin;
	};
	
	//=========================================================
	//! The component control packet handler.
	//!
	ComponentCtrlP componentCtrlP;
	

	//=========================================================
	//! Updates a motion tracker.  Called by componentCtrlP
	//! \param ...
	//! 
	void updateMotionTracker( int trackerID, int viewID, 
		osg::Quat orientation );


};



#endif

