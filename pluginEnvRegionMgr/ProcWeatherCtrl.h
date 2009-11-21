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


#if !defined(_PROC_ENVIRONMENTAL_REGION_WEATHER_CTRL_INCLUDED_)
#define _PROC_ENVIRONMENTAL_REGION_WEATHER_CTRL_INCLUDED_

#include "CigiBaseEventProcessor.h"
#include "EnvRegion.h"
#include "CigiWeatherCtrlV3.h"

//=========================================================
//! This class processes an incoming component control packet.
//!
class ProcEnvRegionWeatherCtrl : public CigiBaseEventProcessor  
{
public:


   //==> Management

   //=========================================================
   //! General Constructor
   //!
	ProcEnvRegionWeatherCtrl();

   //=========================================================
   //! General Destructor
   //!
	virtual ~ProcEnvRegionWeatherCtrl();



   //==> Cigi packet reception and processing


   //=========================================================
   //! A CIGI packet call back method processing the component 
   //! control packet.
   //! \param Packet - A pointer to the incoming packet.
   //!
   virtual void OnPacketReceived(CigiBasePacket *Packet);


   //==> Basic processor functionality


   //=========================================================
   //! Initializes the component control processor
   //! \param EntityTableIn - A pointer to the Entity jump table.
   //!
   void Init(std::list<EnvRegion> *EnvRegionsIn,
             std::list<EnvRegion>::iterator *EnvRegionTableIn)
   {
      EnvRegions = EnvRegionsIn;
      EnvRegionTable = EnvRegionTableIn;
   }

   //=========================================================
   //! The per-frame processing that this object performs
   //!
   void Act(void);

protected:

   //=========================================================
   //! A pointer to the Environmental Region list
   //!
   std::list<EnvRegion> *EnvRegions;

   //=========================================================
   //! A pointer to the Environmental Region jump table
   //!
   std::list<EnvRegion>::iterator *EnvRegionTable;

   //=========================================================
   //! A list of orphaned weather controls
   //!
   std::list<CigiWeatherCtrlV3> OrphanedWeatherCtrl;


};

#endif // !defined(_PROC_ENVIRONMENTAL_REGION_WEATHER_CTRL_INCLUDED_)
