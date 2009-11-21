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
 *  FILENAME:   ProcWeatherCtrl.cpp
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


#include "ProcWeatherCtrl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// ================================================
// ProcEntityWeatherCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ProcEntityWeatherCtrl::ProcEntityWeatherCtrl()
{
   EntityTable = NULL;
}

// ================================================
// ~ProcEntityWeatherCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ProcEntityWeatherCtrl::~ProcEntityWeatherCtrl()
{

}





// ================================================
// OnPacketReceived
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ProcEntityWeatherCtrl::OnPacketReceived(CigiBasePacket *Packet)
{
   CigiWeatherCtrlV3 *ccp = (CigiWeatherCtrlV3 *)Packet;

   // check the 'Scopy' for this Weather ctrl.
   // We're only interested in 'entity' Weather controls
   if(ccp->GetScope() == CigiBaseWeatherCtrl::Entity)
   {

      int EntID = (int)ccp->GetEntityID();
      Entity *CrntEnt = EntityTable[EntID];

      if(CrntEnt != NULL)
         CrntEnt->EntityWeather.ProcWeatherCtrlPckt(ccp);
      else
         OrphanedWeatherCtrl.push_back(*ccp);

   }

}


// ================================================
// Act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ProcEntityWeatherCtrl::Act()
{
   // The OrphanedCompCtrl should usually be empty.  There are situations 
   // where it will not be empty.  For example, it is possible (and allowable) 
   // for the host to insert weather control packets before the entity 
   // creation packet in the message.
   
   if(OrphanedWeatherCtrl.empty())
      return;

   int EntID;
   Entity *CrntEnt;

   std::list<CigiWeatherCtrlV3>::iterator occi;
   for(occi=OrphanedWeatherCtrl.begin();occi != OrphanedWeatherCtrl.end();occi++)
   {
      EntID = occi->GetEntityID();
      CrntEnt = EntityTable[EntID];

      if(CrntEnt != NULL)
         CrntEnt->EntityWeather.ProcWeatherCtrlPckt(&(*occi));

   }

   OrphanedWeatherCtrl.clear();

}


