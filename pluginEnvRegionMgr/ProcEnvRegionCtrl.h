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
 *  FILENAME:   ProcEnvRegionCtrl.h
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


#ifndef _PROC_ENVREGION_CTRL_INCLUDED_
#define _PROC_ENVREGION_CTRL_INCLUDED_


#include "CigiBaseEventProcessor.h"
#include "EnvRegion.h"
#include <list>



//=========================================================
//! This class processes an incoming entity control packet.
//!
class ProcEnvRegionCtrl : public CigiBaseEventProcessor  
{
public:


   //==> Management

   //=========================================================
   //! General Constructor
   //!
	ProcEnvRegionCtrl();

   //=========================================================
   //! General Destructor
   //!
	virtual ~ProcEnvRegionCtrl();



   //==> Cigi packet reception and processing


   //=========================================================
   //! A CIGI packet call back method processing the entity
   //!   control packet.
   //! \param Packet - A pointer to the incoming packet.
   //!
   virtual void OnPacketReceived(CigiBasePacket *Packet);


   //==> Basic processor functionality


   //=========================================================
   //! Initializes the entity control processor
   //! \param EntLstIn - A pointer to the EnvRegion list.
   //! \param EnvRegionTableIn - A pointer to the entity jump table
   //! \param PosConvIn - A pointer to the position conversion object
   //! \param entityFactoryIn - A pointer to the entity factory object
   //!
   void Init(std::list<EnvRegion> *EnvRegionLstIn,
             std::list<EnvRegion>::iterator *EnvRegionTableIn,
             CoordCigi2DBaseFunctionPtr CoordCigi2DBaseIn,
             CoordDBase2CigiFunctionPtr CoordDBase2CigiIn)
   {
      EnvRegionLst = EnvRegionLstIn;
      EnvRegionTable = EnvRegionTableIn;
      CoordCigi2DBase = CoordCigi2DBaseIn;
      CoordDBase2Cigi = CoordDBase2CigiIn;
   }


protected:


   //=========================================================
   //! A pointer to the entity list
   //!
   std::list<EnvRegion> *EnvRegionLst;

   //=========================================================
   //! A pointer to the environmental region jump table
   //!
   std::list<EnvRegion>::iterator *EnvRegionTable;

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

#endif // _PROC_ENVREGION_CTRL_INCLUDED_
