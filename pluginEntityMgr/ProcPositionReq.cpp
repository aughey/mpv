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
 *  FILENAME:   ProcPositionReq.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *   process the articulated parts packets.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  11/16/2005 Greg Basler                       MPV_CR_DR_1
 *  Initial Release.
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#include "ProcPositionReq.h"
#include "ArtPartList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// ================================================
// ProcEntityPositionReq
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ProcEntityPositionReq::ProcEntityPositionReq()
{
   EntityTable = NULL;
   PositionReqs = NULL;
}

// ================================================
// ~ProcEntityPositionReq
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ProcEntityPositionReq::~ProcEntityPositionReq()
{

}





// ================================================
// OnPacketReceived
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ProcEntityPositionReq::OnPacketReceived(CigiBasePacket *Packet)
{
   CigiPositionReqV3 *Pckt = (CigiPositionReqV3 *)Packet;

   // only process Entity and articulated part position requests
   if((Pckt->GetObjectClass() == CigiBasePositionReq::Entity) ||
      (Pckt->GetObjectClass() == CigiBasePositionReq::ArtPart))
   {
      // At this time, the MPV cannot process articulated part
      //   position requests that request a coordinate system
      //   other than submodel.
      if((Pckt->GetObjectClass() != CigiBasePositionReq::ArtPart) ||
         (Pckt->GetCoordSys() == CigiBasePositionReq::Submodel))
      {
         // Check for valid Entity
         int EntID = (int)Pckt->GetObjectID();

         if(EntityTable[EntID] != NULL)
            InsertPositionReq(Pckt);
         else
            OrphPosReq.push_back(*Pckt);

      }
   }
}


// ================================================
// Act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ProcEntityPositionReq::Act()
{
   // The OrphPosReq should always be empty.
   // However, it is possible (and allowable) for the host to insert
   //   Position request packets before the entity creation packet in the message.
   if(OrphPosReq.empty())
      return;

   int EntID;

   std::list<CigiPositionReqV3>::iterator opri;
   for(opri=OrphPosReq.begin();opri != OrphPosReq.end();opri++)
   {
      // Check for valid Entity
      EntID = (int)opri->GetObjectID();

      if(EntityTable[EntID] != NULL)
         InsertPositionReq(&(*opri));
   }

   OrphPosReq.clear();

}


// ================================================
// InsertPositionReq
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ProcEntityPositionReq::InsertPositionReq(CigiPositionReqV3 *Pckt)
{
   // Check for an existing position request in the list
   std::list<CigiPositionReqV3>::iterator ipri = PositionReqs->begin();
   bool found = false;
   while(!found && (ipri != PositionReqs->end()))
   {
      if(Pckt->GetObjectID() == ipri->GetObjectID())
      {
         if((Pckt->GetObjectClass() == CigiBasePositionReq::Entity)&&
            (ipri->GetObjectClass() == CigiBasePositionReq::Entity))
         {
            // found entity
            *ipri = *Pckt;
            found = true;
         }
         else if((Pckt->GetObjectClass() == CigiBasePositionReq::Submodel)&&
                 (ipri->GetObjectClass() == CigiBasePositionReq::Submodel))
         {
            // this is an articulated part
            // check the artpart ids
            if(Pckt->GetArtPartID() == ipri->GetArtPartID())
            {
               *ipri = *Pckt;
               found = true;
            }
         }
      }
      ipri++;
   }

   if(!found)
   {
      // if an entity -- add to the end
      if(Pckt->GetObjectClass() == CigiBasePositionReq::Entity)
         PositionReqs->push_back(*Pckt);
      else
      {
         // This is an articulated part
         // Verify that this is a valid articulated part
         // Get the entity
         Entity *CrntEnt = EntityTable[Pckt->GetObjectID()];
         // Go through the art part list
         std::list<ArtPart*>::iterator iap = CrntEnt->GetArtParts();
         std::list<ArtPart*>::iterator biap = CrntEnt->GetArtPartsEnd();
         while(!found && (iap != biap))
         {
            if(Pckt->GetArtPartID() == (*iap)->GetArtPartID())
               found = true;

            iap++;
         }

         if(found)
            PositionReqs->push_back(*Pckt);
      }
   }

}



