/** <pre>
 *  MPV Mission Functions manager plugin
 *  Copyright (c) 2007 General Dynamics Land Systems
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
 *      Initial release.  Plugin is based in part on the GDLS 
 *      pluginMissionFuncsOSG.
 *  
 *  
 *  </pre>
 */


#ifndef _PLUGIN_MISSION_FUNCS_MGR_H_
#define _PLUGIN_MISSION_FUNCS_MGR_H_

#include "AllCigi.h"
#include "Plugin.h"
#include "DefFileGroup.h"
#include "Entity.h"
#include "CoordinateConverter.h"
#include "MissionFunctionsWorker.h"

#include "HOATDispatcher.h"
#include "LOSDispatcher.h"


//=========================================================
//! Manages mission functions requests and responses.  This includes 
//! processing related CIGI messages, performing coordinate conversion, 
//! tracking continuous/persistent requests, etc.  The actual LOS and HOT 
//! tests are performed by other plugins.
//! 
class PluginMissionFuncsMgr : public Plugin
{
public:
	//=========================================================
	//! General Constructor
	//! 
	PluginMissionFuncsMgr();
	
	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginMissionFuncsMgr() throw();
	
	//=========================================================
	//! The per-frame processing that this plugin performs.
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );
	
private:
	//==========================================================================
	// Blackboard Pointers:
	//==========================================================================

	//=========================================================
	//! Configuration data.  Retrieved from the blackboard.
	//! 
	DefFileGroup **DefFileData;
	
	//=========================================================
	//! The OutgoingMsg pointer.  Retrieved from the blackboard.
	//! 
	CigiOutgoingMsg *OmsgPtr;
	//=========================================================
	//! The IncomingMsg pointer.  Retrieved from the blackboard.
	//! 
	CigiIncomingMsg *ImsgPtr;
	
	//=========================================================
	//! An entity container, containing all the entities.  
	//! Retrieved from the blackboard.
	//! 
	mpv::EntityContainer *allEntities;
	
	//=========================================================
	//! Coordinate converter, for converting request positions from 
	//! GDC->database coordinates.  Retrieved from the blackboard.
	//! 
	mpv::CoordinateConverter *coordinateConverter;
	
	
	//=========================================================
	//! List of mission functions workers.  Other plugins will add workers 
	//! to this list.  Posted to the blackboard.
	//! 
	std::list< mpv::RefPtr< mpv::MissionFunctionsWorker > > workers;
	

	//=========================================================
	//! A flag to prevent setup() from being called more than once
	//! 
	bool haveCalledSetup;
	
	//=========================================================
	//! 
	//! 
	mpv::RefPtr< HOATDispatcher > hoatDispatcher;

	//=========================================================
	//! 
	//! 
	mpv::RefPtr< LOSDispatcher >  losDispatcher;


	//=========================================================
	//! Pulls some preferences out of the config file data
	//! 
	void getConfig();

	//=========================================================
	//! Some one-time setup stuff is performed here.
	//! 
	void setup();

	//=========================================================
	//! This class processes HAT/HOT request packets
	//! 
	class HOATRequestProc : public CigiBaseEventProcessor  
	{
	public:

		//=========================================================
		//! General Constructor
		//! 
		HOATRequestProc( PluginMissionFuncsMgr *_plugin )
			: plugin( _plugin ) {}

		//=========================================================
		//! General Destructor
		//! 
		virtual ~HOATRequestProc() {}

		//=========================================================
		//! Callback; processes a packet
		//! 
		virtual void OnPacketReceived( CigiBasePacket *packet )
		{
			plugin->hoatDispatcher->processRequest( static_cast<CigiHatHotReqV3_2*>(packet) );
		}

	private:

		//=========================================================
		//! Pointer to plugin
		//! 
		PluginMissionFuncsMgr *plugin;

	};

	//=========================================================
	//! Callback object for handling HAT/HOT request packets.
	//! 
	HOATRequestProc hoatRequestProc;

	//=========================================================
	//! This class processes LOS segment test request packets
	//! 
	class LOSSegmentRequestProc : public CigiBaseEventProcessor  
	{
	public:

		//=========================================================
		//! General Constructor
		//! 
		LOSSegmentRequestProc( PluginMissionFuncsMgr *_plugin )
			: plugin( _plugin ) {}

		//=========================================================
		//! General Destructor
		//! 
		virtual ~LOSSegmentRequestProc() {}

		//=========================================================
		//! Callback; processes a packet
		//! 
		virtual void OnPacketReceived( CigiBasePacket *packet )
		{
			plugin->losDispatcher->processRequest( static_cast<CigiLosSegReqV3_2*>(packet) );
		}

	private:

		//=========================================================
		//! Pointer to plugin
		//! 
		PluginMissionFuncsMgr *plugin;

	};

	//=========================================================
	//! Callback object for handling LOS segment test request packets.
	//! 
	LOSSegmentRequestProc losSegmentRequestProc;

	//=========================================================
	//! This class processes LOS vector test request packets
	//! 
	class LOSVectorRequestProc : public CigiBaseEventProcessor  
	{
	public:

		//=========================================================
		//! General Constructor
		//! 
		LOSVectorRequestProc( PluginMissionFuncsMgr *_plugin )
			: plugin( _plugin ) {}

		//=========================================================
		//! General Destructor
		//! 
		virtual ~LOSVectorRequestProc() {}

		//=========================================================
		//! Callback; processes a packet
		//! 
		virtual void OnPacketReceived( CigiBasePacket *packet )
		{
			plugin->losDispatcher->processRequest( static_cast<CigiLosVectReqV3_2*>(packet) );
		}

	private:

		//=========================================================
		//! Pointer to plugin
		//! 
		PluginMissionFuncsMgr *plugin;

	};

	//=========================================================
	//! Callback object for handling LOS vector test request packets.
	//! 
	LOSVectorRequestProc losVectorRequestProc;

};

#endif
