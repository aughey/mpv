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
 *  FILENAME:   EnvRegionMgr.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class manages environmental regions.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  04/04/2004 Greg Basler                       MPV_CR_DR_1
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

#include "EnvRegionMgr.h"

EnvRegionMgr *EnvRegionMgr::Singleton = NULL;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// ================================================
// EnvRegionMgr
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
EnvRegionMgr::EnvRegionMgr() : Plugin() 
{
	name_ = "EnvRegionMgr";
	licenseInfo_.setLicense( LicenseInfo::LicenseGPL );
	licenseInfo_.setOrigin( "Boeing" );

#ifdef USE_PUBLIC_POSITION_CONV
	dependencies_.push_back( "libpluginOSPositionConv" );
#else
	dependencies_.push_back( "libpluginPositionConv" );
#endif

	OmsgPtr = NULL;
	ImsgPtr = NULL;

   for(int ndx=0;ndx<65536;ndx++)
   {
      EnvRegionTable[ndx] = EnvRegions.end();
   }

   Singleton = this;

}


// ================================================
// ~EnvRegionMgr
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
EnvRegionMgr::~EnvRegionMgr() 
{
   cleanUp();
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void EnvRegionMgr::act( SystemState::ID state, StateContext &stateContext )
{
   switch( state )
   {

   case SystemState::BlackboardPost:
      // This state is for posting things to the blackboard
      bb_->put("EnvRegionsList",&EnvRegions);
      bb_->put("DestroyEnvRegionsList",&DestroyEnvRegions);
      bb_->put("EnvRegionAccessTable",&EnvRegionTable[0]);
      bb_->put("EnvRegionExtendedData",PostedAddEnvRegionExtendedData);

      break;


   case SystemState::BlackboardRetrieve:
      // This state is for retrieving things from the blackboard
      bb_->get( "CigiOutgoingMsg", OmsgPtr );
      bb_->get( "CigiIncomingMsg", ImsgPtr );

      // Position conversion
      bb_->get("CoordCigi2DBase", CoordCigi2DBase);
      bb_->get("CoordDBase2Cigi", CoordDBase2Cigi);

      // Initialize the packet processors
      EnvRegionCtrlP.Init(&EnvRegions,
                          &EnvRegionTable[0],
                          CoordCigi2DBase,
                          CoordDBase2Cigi);
      CompCtrlP.Init(&EnvRegions,&EnvRegionTable[0]);
      ShortCompCtrlP.Init(&EnvRegions,&EnvRegionTable[0]);
      WeatherCtrlP.Init(&EnvRegions,&EnvRegionTable[0]);

      if( ImsgPtr != NULL )
      {
         ImsgPtr->RegisterEventProcessor(CIGI_ENV_RGN_CTRL_PACKET_ID_V3,
            (CigiBaseEventProcessor *) &EnvRegionCtrlP);

         ImsgPtr->RegisterEventProcessor(CIGI_COMP_CTRL_PACKET_ID_V3,
            (CigiBaseEventProcessor *) &CompCtrlP);

         ImsgPtr->RegisterEventProcessor(CIGI_SHORT_COMP_CTRL_PACKET_ID_V3,
            (CigiBaseEventProcessor *) &ShortCompCtrlP);

         ImsgPtr->RegisterEventProcessor(CIGI_WEATHER_CTRL_PACKET_ID_V3,
            (CigiBaseEventProcessor *) &WeatherCtrlP);
      }

      break;

   case SystemState::Reset:
      cleanUp();
      break;

   case SystemState::Operate:
   case SystemState::Debug:
      operate();
	  break;

   case SystemState::Shutdown:
      cleanUp();
      break;

   default:
      break;
	}
	
}


// ================================================
// operate
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void EnvRegionMgr::operate() 
{

   std::list<EnvRegion>::iterator iRgn;
   std::list<EnvRegion>::iterator iRgnHold;


   // Clear the destroyed Environmental Regions list from the last frame
   // FIXME  -  simplified deletion
   DestroyEnvRegions.clear();

   // Remove all environmental regions who's state is destroy
   iRgn = EnvRegions.begin();
   while(iRgn != EnvRegions.end())
   {
      if(iRgn->GetRgnState() == CigiBaseEnvRgnCtrl::Destroyed)
      {
         // Remove from table
         *(EnvRegionTable + iRgn->GetEnvRegionID()) = EnvRegions.end();

         // move the entity to the removed list
         iRgnHold = iRgn;
         iRgn++;
         DestroyEnvRegions.splice(DestroyEnvRegions.begin(),
                                  EnvRegions,iRgnHold);
      }
      else
         iRgn++;

   }


   // Maintain the destroyed Environmental Regions list
   // FIXME  -  simplified deletion
//   DestroyEnvRegions.clear();

//   for(chl=DestroyEnvRegions.begin();chl!=DestroyEnvRegions.end();chl++)
//   {
//      chl = DestroyEnvRegions.erase(chl);
//   }


   // Process orphan artpart,comp ctrl, rate, and trajectory

   CompCtrlP.Act();
   ShortCompCtrlP.Act();

   // Process each Environmental Region's Act
   // %%%> Get time
   float dTime = 1.0f/60.0f;  // for now just set to 60 Hz
   for(iRgn=EnvRegions.begin();iRgn!=EnvRegions.end();iRgn++)
   {
      iRgn->Act(dTime);
   }

}


// ================================================
// cleanUp
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void EnvRegionMgr::cleanUp()
{
   EnvRegions.clear();

   DestroyEnvRegions.clear();

   for(int ndx=0;ndx<65536;ndx++)
   {
      EnvRegionTable[ndx] = EnvRegions.end();
   }
}


// ================================================
// PostedAddEntityExtendedData
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int EnvRegionMgr::PostedAddEnvRegionExtendedData( void )
{
   int Count = 0;

   if(Singleton != NULL)
      Count = Singleton->AddEnvRegionExtendedData();

   return(Count);
}


// ================================================
// PostedAddEntityExtendedData
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int EnvRegionMgr::AddEnvRegionExtendedData( void )
{
   EnvRegion::IncrementExtendedDataCount();

   std::list<EnvRegion>::iterator iRgn;

   // Add data element to all current entities
   for(iRgn=EnvRegions.begin();iRgn!=EnvRegions.end();iRgn++)
   {
      iRgn->AddElementToExtendedData();
   }

   return(EnvRegion::GetExtendedDataCount() - 1);
}



