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
 *  FILENAME:   ProcRateCtrl.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *   process the rate control packets.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  04/04/2004 Greg Basler                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#if !defined(_PROC_RATE_CTRL_INCLUDED_)
#define _PROC_RATE_CTRL_INCLUDED_

#include "CigiBaseEventProcessor.h"
#include "Entity.h"
#include <list>


//=========================================================
//! This class processes an incoming rate control packet.
//!
class ProcRateCtrl : public CigiBaseEventProcessor  
{
public:


   //==> Management

   //=========================================================
   //! General Constructor
   //!
	ProcRateCtrl();

   //=========================================================
   //! General Destructor
   //!
	virtual ~ProcRateCtrl();



   //==> Cigi packet reception and processing


   //=========================================================
   //! A CIGI packet call back method processing the rate
   //!   control packet.
   //! \param Packet - A pointer to the incoming packet.
   //!
   virtual void OnPacketReceived(CigiBasePacket *Packet);


   //==> Basic processor functionality


   //=========================================================
   //! Initializes the rate control processor
   //! \param EntityTableIn - A pointer to the Entity jump table.
   //!
   void Init(Entity **EntityTableIn)
   {
      EntityTable = EntityTableIn;
   }

   //=========================================================
   //! The per-frame processing that this object performs
   //!
   void Act(void);

protected:

   //=========================================================
   //! A pointer to the entity jump table
   //!
   Entity **EntityTable;

   //=========================================================
   //! A list of orphaned rate controls
   //!
   std::list<CigiRateCtrlV3> OrphRate;


};

#endif // !defined(_PROC_RATE_CTRL_INCLUDED_)
