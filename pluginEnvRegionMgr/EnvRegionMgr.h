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
 *  FILENAME:   EnvRegionMgr.h
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


#ifndef _PLUGIN_ENVREGION_MANAGER_INCLUDED_
#define _PLUGIN_ENVREGION_MANAGER_INCLUDED_

#include "Plugin.h"
#include "AllCigi.h"
#include "ProcEnvRegionCtrl.h"
#include "ProcCompCtrl.h"
#include "ProcShortCompCtrl.h"
#include "EnvRegion.h"
#include "ProcWeatherCtrl.h"

#include "DefFileGroup.h"

#include <list>
#include <string>
#include <map>



//=========================================================
//! This class manages environmental regions.
//!
class EnvRegionMgr : public Plugin 
{
public:


   //==> Management

   //=========================================================
   //! General Constructor
   //!
	EnvRegionMgr();

   //=========================================================
   //! General Destructor
   //!
	virtual ~EnvRegionMgr();


   //==> Basic plugin functionality

	//=========================================================
	//! The per-frame processing that this plugin performs
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );


   //==> Environmental Region Extended Data public functions


   //=========================================================
   //! Generates a new entry in each Environmental Regions's 
   //!   extended data vector and increments the size of the 
   //!   environmental region's extended data so that future 
   //!   environmental regions will have the correct
   //!   extended data vector.
   //!
   //! \return - The index of the new element in each 
   //!   environmental region's extended data vector
   //!
	static int PostedAddEnvRegionExtendedData( void );



protected:


   void operate();
   void cleanUp();

   //=========================================================
   //! Generates a new entry in each Environmental Regions's 
   //!   extended data vector and increments the size of the 
   //!   environmental region's extended data so that future 
   //!   environmental regions will have the correct
   //!   extended data vector.
   //!
   //! \return - The index of the new element in each 
   //!   environmental region's extended data vector
   //!
	int AddEnvRegionExtendedData( void );


   //=========================================================
   //! A pointer to the instanced plugin object; used by the Posted* functions
   //!
   static EnvRegionMgr *Singleton;


   //=========================================================
   //! A pointer to the Cigi outgoing message object
   //!
	CigiOutgoingMsg *OmsgPtr;

   //=========================================================
   //! A pointer to the Cigi incoming message object
   //!
	CigiIncomingMsg *ImsgPtr;

   //=========================================================
   //! A pointer to the Environmental Region Control
   //!   packet processor object
   //!
	ProcEnvRegionCtrl EnvRegionCtrlP;

   //=========================================================
   //! The Component control packet processing object
   //!
   ProcEnvRegionCompCtrl CompCtrlP;

   //=========================================================
   //! The Short Component control packet processing object
   //!
   ProcEnvRegionShortCompCtrl ShortCompCtrlP;

   //=========================================================
   //! The Weather control packet processing object
   //!
   ProcEnvRegionWeatherCtrl WeatherCtrlP;


   //=========================================================
   //! A List of the current environmental regions
   //!
   std::list<EnvRegion> EnvRegions;

   //=========================================================
   //! A List of the environmental regions that are marked as
   //!   "destroy" and have not been deleted.
   //!
   std::list<EnvRegion> DestroyEnvRegions;

   //=========================================================
   //! A table of pointers into the environmental regions list
   //!   to specific regions<br>
   //!   (environmental region IDs are used as the index)
   //!
   std::list<EnvRegion>::iterator (EnvRegionTable[65536]);

   //=========================================================
   //! CoordCigi2DBase<br>
   //! A pointer to the CIGI coordinate to Database coordinate
   //!  member function of the position conversion plugin 
   //!
   CoordCigi2DBaseFunctionPtr CoordCigi2DBase;

   //=========================================================
   //! CoordDBase2Cigi<br>
   //! A pointer to the Database coordinate to CIGI coordinate
   //!  member function of the position conversion plugin 
   //!
   CoordDBase2CigiFunctionPtr CoordDBase2Cigi;


};



#endif  // _PLUGIN_ENVREGION_MANAGER_INCLUDED_

