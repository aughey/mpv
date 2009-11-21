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
 *  FILENAME:   EnvRegion.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *   represent an entity.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  11/01/2005 Greg Basler                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#define _EXPORT_MPVCMN_

#include <iostream>

#include "EnvRegion.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


// ================================================
// Initialize the static variable ExtendedDataCount
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int EnvRegion::ExtendedDataCount = 0;

// ================================================
// Entity
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
EnvRegion::EnvRegion() : ExtendedDataSet()
{


	EnvRegionID = 0;
   CigiPos.LatX = 0.0;
   CigiPos.LonY = 0.0;
   CigiPos.AltZ = 0.0;
   CigiPos.Yaw = 0.0;
   CigiPos.Pitch = 0.0;
   CigiPos.Roll = 0.0;
   XSize = 0.0f;
   YSize = 0.0f;
   CornerRadius = 0.0f;
   Transition = 0.0f;
   RgnState = CigiBaseEnvRgnCtrl::Inactive;
   WeatherProp = CigiBaseEnvRgnCtrl::UseLast;
   Aerosol = CigiBaseEnvRgnCtrl::UseLast;
   MaritimeSurface = CigiBaseEnvRgnCtrl::UseLast;
   TerrestrialSurface = CigiBaseEnvRgnCtrl::UseLast;


   CreateExtendedData(ExtendedDataCount);


}


// ================================================
// ~Entity
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
EnvRegion::~EnvRegion()
{

}



// ================================================
// ProcEnvRegionCtrlPckt
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void EnvRegion::ProcEnvRegionCtrlPckt(CigiEnvRgnCtrlV3 *Packet)
{

   if(EnvRegionID != Packet->GetRegionID())
      return;

   RgnState = Packet->GetRgnState();
   WeatherProp = Packet->GetWeatherProp();
   Aerosol = Packet->GetAerosol();
   MaritimeSurface = Packet->GetMaritimeSurface();
   TerrestrialSurface = Packet->GetTerrestrialSurface();
   CigiPos.LatX = Packet->GetLat();
   CigiPos.LonY = Packet->GetLon();
   CigiPos.Yaw = Packet->GetRotation();
   XSize = Packet->GetXSize();
   YSize = Packet->GetYSize();
   CornerRadius = Packet->GetCornerRadius();
   Transition = Packet->GetTransition();

   // Alt, Pitch, and Roll are already set to 0
   if(CoordCigi2DBase != NULL)
      CoordCigi2DBase(CigiPos,DBasePos);

   // Set updated flag
   PcktUpdated = true;

}


