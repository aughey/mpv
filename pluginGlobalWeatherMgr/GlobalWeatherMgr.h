/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2005 The Boeing Company
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
 *  FILENAME:   GlobalWeatherMgr.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This plugin listens for packets from the Host pertaining to the 
 *  global weather state, extracts the data from those packets, and makes 
 *  the results available to the rest of the system via the blackboard.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  11/02/2005 Greg Basler                       MPV_CR_DR_1
 *  Initial Release.
 *
 *  2007-07-14 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#ifndef _PLUGIN_GLOBAL_WEATHER_MANAGER_INCLUDED_
#define _PLUGIN_GLOBAL_WEATHER_MANAGER_INCLUDED_

#include "Plugin.h"
#include "AllCigi.h"
#include "GlobalWeather.h"
#include "ProcAtmosphereCtrl.h"
#include "ProcCompCtrl.h"
#include "ProcShortCompCtrl.h"
#include "ProcWeatherCtrl.h"

#include "DefFileGroup.h"

#include <list>
#include <string>
#include <map>



//=========================================================
//! This plugin listens for packets from the Host pertaining to the 
//! global weather state, extracts the data from those packets, and makes 
//! the results available to the rest of the system via the blackboard.
//!
class GlobalWeatherMgr : public Plugin 
{
public:

   //=========================================================
   //! General Constructor
   //!
	GlobalWeatherMgr();

   //=========================================================
   //! General Destructor
   //!
	virtual ~GlobalWeatherMgr();

	//=========================================================
	//! The per-frame processing that this plugin performs
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );

protected:


   //=========================================================
   //! A pointer to the Cigi outgoing message object
   //!
	CigiOutgoingMsg *OmsgPtr;

   //=========================================================
   //! A pointer to the Cigi incoming message object
   //!
	CigiIncomingMsg *ImsgPtr;

   //=========================================================
   //! The global weather and atmospheric conditions data
   //!
   GlobalWeather GlobalWeatherData;

   //=========================================================
   //! A pointer to the Environmental Region Control
   //!   packet processor object
   //!
	ProcAtmosphereCtrl AtmosphereCtrlP;

   //=========================================================
   //! The Component control packet processing object
   //!
   ProcGlobalWeatherCompCtrl CompCtrlP;

   //=========================================================
   //! The Short Component control packet processing object
   //!
   ProcGlobalWeatherShortCompCtrl ShortCompCtrlP;

   //=========================================================
   //! The Weather control packet processing object
   //!
   ProcGlobalWeatherCtrl WeatherCtrlP;



};

#endif

