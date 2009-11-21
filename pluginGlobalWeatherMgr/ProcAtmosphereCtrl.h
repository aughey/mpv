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
 *  FILENAME:   ProcAtmosphereCtrl.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *   process the entity control packets.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  11/02/2005 Greg Basler                       MPV_CR_DR_1
 *  Initial Release.
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#ifndef _PROC_ATMOSPHERE_CTRL_INCLUDED_
#define _PROC_ATMOSPHERE_CTRL_INCLUDED_


#include "CigiBaseEventProcessor.h"
#include "GlobalWeather.h"
#include <list>



//=========================================================
//! This class processes an incoming entity control packet.
//!
class ProcAtmosphereCtrl : public CigiBaseEventProcessor  
{
public:


   //==> Management

   //=========================================================
   //! General Constructor
   //!
	ProcAtmosphereCtrl()
   {
      pGlobalWeatherData = NULL;
   }

   //=========================================================
   //! General Destructor
   //!
   virtual ~ProcAtmosphereCtrl() { };



   //==> Cigi packet reception and processing


   //=========================================================
   //! A CIGI packet call back method processing the entity
   //!   control packet.
   //! \param Packet - A pointer to the incoming packet.
   //!
   virtual void OnPacketReceived(CigiBasePacket *Packet)
   {
      if(pGlobalWeatherData != NULL)
      {
         pGlobalWeatherData->ProcAtmosCtrlPckt((CigiAtmosCtrlV3 *)Packet);
      }
   }


   //==> Basic processor functionality


   //=========================================================
   //! Initializes the Global Weather processor
   //! \param GlobalWeatherIn - A pointer to the Global Weather
   //!    Data processor.
   //!
   void Init(GlobalWeather *GlobalWeatherIn)
   {
      pGlobalWeatherData = GlobalWeatherIn;
   }


protected:


   //=========================================================
   //! A pointer to the Global Weather Data processor
   //!
   GlobalWeather *pGlobalWeatherData;


};

#endif // _PROC_ATMOSPHERE_CTRL_INCLUDED_
