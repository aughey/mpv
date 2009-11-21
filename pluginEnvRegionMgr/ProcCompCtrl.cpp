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
 *  FILENAME:   ProcCompCtrl.cpp
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
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#include "ProcCompCtrl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// ================================================
// ProcCompCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ProcEnvRegionCompCtrl::ProcEnvRegionCompCtrl()
{

}

// ================================================
// ~ProcCompCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ProcEnvRegionCompCtrl::~ProcEnvRegionCompCtrl()
{

}





// ================================================
// OnPacketReceived
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ProcEnvRegionCompCtrl::OnPacketReceived(CigiBasePacket *Packet)
{
   CigiCompCtrlV3 *ccp = (CigiCompCtrlV3 *)Packet;

   // check the 'class' for this component ctrl; we're only interested in 
   // 'environmental region' component controls
   if((ccp->GetCompClassV3() == CigiBaseCompCtrl::RegionalSeaSurfaceV3) ||
      (ccp->GetCompClassV3() == CigiBaseCompCtrl::RegionalTerrainSurfaceV3) ||
      (ccp->GetCompClassV3() == CigiBaseCompCtrl::RegionalLayeredWeatherV3))
   {

      int RgnID = (int)ccp->GetInstanceID();
      std::list<EnvRegion>::iterator CrntRgn =
         *(EnvRegionTable + RgnID);

      if(CrntRgn != EnvRegions->end())
         CrntRgn->ProcCompCtrlPckt(ccp);
      else
         OrphanedCompCtrl.push_back(*ccp);

   }

}


// ================================================
// Act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ProcEnvRegionCompCtrl::Act()
{
   // The OrphanedCompCtrl should usually be empty.  There are situations 
   // where it will not be empty.  For example, it is possible (and allowable) 
   // for the host to insert component control packets before the entity 
   // creation packet in the message.
   
   if(OrphanedCompCtrl.empty())
      return;

   int RgnID;
   std::list<EnvRegion>::iterator CrntRgn;

   std::list<CigiCompCtrlV3>::iterator occi;
   for(occi=OrphanedCompCtrl.begin();occi != OrphanedCompCtrl.end();occi++)
   {
      RgnID = occi->GetInstanceID();
      CrntRgn = *(EnvRegionTable + RgnID);

      if(CrntRgn != EnvRegions->end())
         CrntRgn->ProcCompCtrlPckt(&(*occi));

   }

   OrphanedCompCtrl.clear();

}



