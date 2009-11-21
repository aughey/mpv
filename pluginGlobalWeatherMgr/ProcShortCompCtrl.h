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
 *  FILENAME:   ProcShortCompCtrl.h
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
 *  05/19/2004 Greg Basler                       MPV_CR_DR_1
 *  Initial Release.
 *
 *  11/02/2005 Greg Basler
 *  Converted for Enviromental Region processing
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#if !defined(_PROC_GLOBAL_WEATHER_SHORT_COMP_CTRL_INCLUDED_)
#define _PROC_GLOBAL_WEATHER_SHORT_COMP_CTRL_INCLUDED_

#include "CigiBaseEventProcessor.h"
#include "GlobalWeather.h"
#include <list>


//=========================================================
//! This class processes an incoming short component control packet.
//!
class ProcGlobalWeatherShortCompCtrl : public CigiBaseEventProcessor  
{
public:


   //==> Management

   //=========================================================
   //! General Constructor
   //!
   ProcGlobalWeatherShortCompCtrl() { };

   //=========================================================
   //! General Destructor
   //!
   virtual ~ProcGlobalWeatherShortCompCtrl() { };



   //==> Cigi packet reception and processing


   //=========================================================
   //! A CIGI packet call back method processing the short component 
   //! control packet.
   //! \param Packet - A pointer to the incoming packet.
   //!
   virtual void OnPacketReceived(CigiBasePacket *Packet)
   {
      CigiShortCompCtrlV3 *sccp = (CigiShortCompCtrlV3 *)Packet;

      // check the 'class' for this component ctrl; we're only interested in 
      // 'Global Weather' and atmosphere component controls
      if((sccp->GetCompClassV3() == CigiBaseCompCtrl::AtmosphereV3) ||
         (sccp->GetCompClassV3() == CigiBaseCompCtrl::GlobalLayeredWeatherV3))
      {
         pGlobalWeatherData->ProcShortCompCtrlPckt(sccp);
      }
   }


   //==> Basic processor functionality


   //=========================================================
   //! Initializes the short component control processor
   //! \param EntityTableIn - A pointer to the Entity jump table.
   //!
   void Init(GlobalWeather *GlobalWeatherIn)
   {
      pGlobalWeatherData = GlobalWeatherIn;
   }

   //=========================================================
   //! The per-frame processing that this object performs
   //!
   void Act(void) { };

protected:

   //=========================================================
   //! A pointer to the entity list
   //!
   GlobalWeather *pGlobalWeatherData;


};

#endif // !defined(_PROC_GLOBAL_WEATHER_SHORT_COMP_CTRL_INCLUDED_)
