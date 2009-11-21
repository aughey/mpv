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
 *  FILENAME:   GlobalWeatherMgr.cpp
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


#include <iostream>
#include <memory>

#include "GlobalWeatherMgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// ================================================
// GlobalWeatherMgr
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
GlobalWeatherMgr::GlobalWeatherMgr() : Plugin() 
{
	name_ = "GlobalWeatherMgr";
	licenseInfo_.setLicense( LicenseInfo::LicenseGPL );
	licenseInfo_.setOrigin( "Boeing" );

	OmsgPtr = NULL;
	ImsgPtr = NULL;

   int ndx;
   WeatherCtrl InitWeatherCtrl;
   for(ndx=0;ndx<10;ndx++)
   {
      InitWeatherCtrl.SetLayerID((unsigned char)ndx);
      GlobalWeatherData.AddWeatherCtrl(InitWeatherCtrl);
   }
}


// ================================================
// ~GlobalWeatherMgr
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
GlobalWeatherMgr::~GlobalWeatherMgr() 
{
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void GlobalWeatherMgr::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{

	case SystemState::BlackboardPost:
		// This state is for posting things to the blackboard
		bb_->put("GlobalWeather",&GlobalWeatherData);
		break;


	case SystemState::BlackboardRetrieve:
      // This state is for retrieving things from the blackboard
      bb_->get( "CigiOutgoingMsg", OmsgPtr );
      bb_->get( "CigiIncomingMsg", ImsgPtr );

      // Initialize the packet processors
      AtmosphereCtrlP.Init(&GlobalWeatherData);
      CompCtrlP.Init(&GlobalWeatherData);
      ShortCompCtrlP.Init(&GlobalWeatherData);
      WeatherCtrlP.Init(&GlobalWeatherData);

      if( ImsgPtr != NULL )
      {
         ImsgPtr->RegisterEventProcessor(CIGI_ATMOS_CTRL_PACKET_ID_V3,
            (CigiBaseEventProcessor *) &AtmosphereCtrlP);

         ImsgPtr->RegisterEventProcessor(CIGI_COMP_CTRL_PACKET_ID_V3,
            (CigiBaseEventProcessor *) &CompCtrlP);

         ImsgPtr->RegisterEventProcessor(CIGI_SHORT_COMP_CTRL_PACKET_ID_V3,
            (CigiBaseEventProcessor *) &ShortCompCtrlP);

         ImsgPtr->RegisterEventProcessor(CIGI_WEATHER_CTRL_PACKET_ID_V3,
            (CigiBaseEventProcessor *) &WeatherCtrlP);
      }

      break;

	case SystemState::Reset:
		/*
		Greg originally had a packet processor set up for IG Ctrl in this 
		plugin.  It would call SetWeatherEn(false) on each of the WeatherCtrls
		when the IGMode was set to Reset.  It is not clear what purpose this 
		served.  Additionally, Greg provided no code to set the value back to 
		true when the IG resumed normal operation.  Given this, I assume that 
		the code was never tested.  The disableAll() call below would do the 
		equivalent, but it is commented out, and will remain so until some 
		explanation is given for the original design. -ADS
		*/
		// GlobalWeatherData.disableAll();
		break;

	case SystemState::Operate:
	case SystemState::Debug:
		GlobalWeatherData.Act();
		break;

	default:
		break;
	}
	
}


