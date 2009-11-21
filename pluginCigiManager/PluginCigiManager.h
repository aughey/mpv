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
 *  FILENAME:   PluginCigiManager.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  implement a simple manager.  A manager is equivalent to 
 *  the "Client" in Mantis nomenclature, ie it is a piece of software 
 *  that speaks directly with a Host, and passes messages back and forth 
 *  between the IG's and the Host.  In this manner, it acts as a proxy.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  07/27/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#ifndef PLUGIN_CIGI_MANAGER_H
#define PLUGIN_CIGI_MANAGER_H

#include <list>
#include "AllCigi.h"

#include "Plugin.h"
#include "DefFileGroup.h"

#include "HostToIGSignalProc.h"
//#include "IGToHostSignalProc.h"
#include "IGConnection.h"

// This plugin forwards CIGI traffic from the upstream system (the Host) to 
// other CIGI IG systems (the "renderers").  If this plugin is running in an 
// MPV instance that has a functioning motion tracker, this plugin can forward 
// that motion tracker's data to the renderers.  This feature uses CIGI 
// Component Control packets, and as such is not part of the CIGI protocol 
// proper.  (A Component Control packet was used instead of a ViewCtrl packet 
// because the ViewCtrl packet uses Euler angles, but the motion tracker data 
// is in quaternions.)  This feature is a bit of a hack.
// For these reasons, this feature is disabled by default.
//#define FORWARD_MOTION_TRACKER_INFO

#ifdef FORWARD_MOTION_TRACKER_INFO
#include "TrackerParams.h"
#endif

//=========================================================
//! This plugin is responsible for 
//! 
class PluginCigiManager : public Plugin 
{
public:

	//=========================================================
	//! General Constructor
	//! 
	PluginCigiManager();

	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginCigiManager() throw();
	
	//=========================================================
	//! Used by the kernel to coordinate system initialization, and to 
	//! notify plugins when a reset event has occurred.
	//! \param stage - The current stage of initialization
	//! 
	virtual void init( InitializationStates stage );
	
	//=========================================================
	//! Called by the kernel once per-plugin, per-frame.
	//! This plugin's act() will ...
	//! 
	virtual void act( void );
	
	
private:
	//=========================================================
	//! The Cigi incoming message object.  Retrieved from the blackboard.
	//!
	CigiIncomingMsg *HostIMsg;
	
	//=========================================================
	//! The Cigi outgoing message object.  Retrieved from the blackboard.
	//!
	CigiOutgoingMsg *HostOMsg;

	//=========================================================
	//! A pointer to a pointer to the Definition File data.  
	//! Retrieved from the blackboard.
	//!
	DefFileGroup **DefinitionData;
	
	//=========================================================
	//! The function pointer for sending SOF.  Retrieved from the blackboard.
	//! 
	void (*sendNetMessagesCB)(void);

#ifdef FORWARD_MOTION_TRACKER_INFO
	//=========================================================
	//! Map of TrackerParams.  TrackerParams contain info from the Host 
	//! regarding motion trackers, such as boresight-enable, etc.  
	//! Retrieved from the blackboard.
	//! 
	std::map< int, TrackerParams * > *trackerParamsMap;
#endif

	HostToIGSignalProc h2iSignalProc;
//	IGToHostSignalProc i2hSignalProc;
	
	std::list< IGConnection* > igConnections;


	//=========================================================
	//! Closes all of the IG connections, frees them, and removes them from 
	//! the list.
	//!
	void deleteIGConnections( void );

	//=========================================================
	//! Checks the contents of the def file tree, and initializes the 
	//! IG connections accordingly.
	//!
	void getDefFileData( void );

};


#endif

