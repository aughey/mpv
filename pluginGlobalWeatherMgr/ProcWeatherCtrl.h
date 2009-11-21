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
 *  FILENAME:   ProcWeatherCtrl.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *   process the component control packets.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  11/10/2005 Greg Basler                       MPV_CR_DR_1
 *  Initial Release.
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#if !defined(_PROC_GLOBAL_WEATHER_CTRL_INCLUDED_)
#define _PROC_GLOBAL_WEATHER_CTRL_INCLUDED_

#include "CigiBaseEventProcessor.h"
#include "GlobalWeather.h"
#include "CigiWeatherCtrlV3.h"

//=========================================================
//! This class processes an incoming component control packet.
//!
class ProcGlobalWeatherCtrl : public CigiBaseEventProcessor  
{
public:


   //==> Management

   //=========================================================
   //! General Constructor
   //!
   ProcGlobalWeatherCtrl() { };

   //=========================================================
   //! General Destructor
   //!
   virtual ~ProcGlobalWeatherCtrl() { };



   //==> Cigi packet reception and processing


   //=========================================================
   //! A CIGI packet call back method processing the component 
   //! control packet.
   //! \param Packet - A pointer to the incoming packet.
   //!
   virtual void OnPacketReceived(CigiBasePacket *Packet)
   {
      CigiWeatherCtrlV3 *ccp = (CigiWeatherCtrlV3 *)Packet;

      // check the 'Scopy' for this Weather ctrl.
      // We're only interested in 'global' Weather controls
      if(ccp->GetScope() == CigiBaseWeatherCtrl::Global)
      {
         pGlobalWeatherData->ProcWeatherCtrlPckt(ccp);
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

#endif // !defined(_PROC_GLOBAL_WEATHER_CTRL_INCLUDED_)
