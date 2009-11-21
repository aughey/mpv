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
 *  FILENAME:   ProcEntityCtrl.cpp
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


#include "ProcEnvRegionCtrl.h"
#include "AllCigi.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// ================================================
// ProcEnvRegionCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ProcEnvRegionCtrl::ProcEnvRegionCtrl()
{

}

// ================================================
// ~ProcEnvRegionCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ProcEnvRegionCtrl::~ProcEnvRegionCtrl()
{

}



// ================================================
// OnPacketReceived
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ProcEnvRegionCtrl::OnPacketReceived(CigiBasePacket *Packet)
{
   CigiEnvRgnCtrlV3 *ercp = (CigiEnvRgnCtrlV3 *)Packet;

   int RgnID = (int)ercp->GetRegionID();
   std::list<EnvRegion>::iterator CrntRgn = 
      *(EnvRegionTable + RgnID);

   if(CrntRgn != EnvRegionLst->end())
   {
      CrntRgn->ProcEnvRegionCtrlPckt(ercp);
   }
   else
   {
      EnvRegion *TempRgn = new EnvRegion;

      EnvRegionLst->push_back(*TempRgn);
      
      delete TempRgn;

      // Get an iterator to the new environmental region
      CrntRgn = EnvRegionLst->end();
      CrntRgn--;

      // place a pointer to the environmental region in the lookup table
      *(EnvRegionTable + RgnID) = CrntRgn;
      
      // set up the new region according to the contents of the packet
      CrntRgn->SetEnvRegionID(RgnID);
      
      CrntRgn->SetPosConv(CoordCigi2DBase,
                       CoordDBase2Cigi);
	  
      CrntRgn->ProcEnvRegionCtrlPckt(ercp);

      // Create the 10 CIGI defined weather layers
      int ndx;
      WeatherCtrl InitWeatherCtrl;
      for(ndx=0;ndx<10;ndx++)
      {
         InitWeatherCtrl.SetLayerID(ndx);
         CrntRgn->AddWeatherCtrl(InitWeatherCtrl);
      }

   }

}

