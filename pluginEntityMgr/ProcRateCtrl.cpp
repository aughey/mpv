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
 *  FILENAME:   ProcRateCtrl.cpp
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


#include "ProcRateCtrl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// ================================================
// ProcRateCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ProcRateCtrl::ProcRateCtrl()
{
   EntityTable = NULL;
}

// ================================================
// ~ProcRateCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ProcRateCtrl::~ProcRateCtrl()
{

}




// ================================================
// OnPacketReceived
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ProcRateCtrl::OnPacketReceived(CigiBasePacket *Packet)
{
   CigiRateCtrlV3 *rcp = (CigiRateCtrlV3 *)Packet;

   int EntID = (int)rcp->GetEntityID();
   Entity *CrntEnt = EntityTable[EntID];

   if(CrntEnt != NULL)
      CrntEnt->ProcRatePckt(rcp);
   else
      OrphRate.push_back(*rcp);

}


// ================================================
// Act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ProcRateCtrl::Act()
{
   // The OrphRate should always be empty.
   // However, it is possible (and allowable) for the host to insert
   //   art part packets before the entity creation packet in the message.
   if(OrphRate.empty())
      return;

   int EntID;
   Entity *CrntEnt;

   std::list<CigiRateCtrlV3>::iterator orci;
   for(orci=OrphRate.begin();orci != OrphRate.end();orci++)
   {
      EntID = orci->GetEntityID();
      CrntEnt = EntityTable[EntID];

      if(CrntEnt != NULL)
         CrntEnt->ProcRatePckt(&(*orci));
   }

   OrphRate.clear();

}



