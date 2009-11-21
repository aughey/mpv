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
 *  FILENAME:   GlobalWeather.cpp
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

#include "GlobalWeather.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


// ================================================
// GlobalWeather
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
GlobalWeather::GlobalWeather()
{

	AtmosEn = false;
   Humidity = 20;
   AirTemp = 0.0f;
   VisibilityRange = 60000.0f;
   HorizWindSp = 0.0f;
   VertWindSp = 0.0f;
   WindDir = 0.0f;
   BaroPressure = 1013.2f;
   PcktUpdated = false;

}


// ================================================
// ~GlobalWeather
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
GlobalWeather::~GlobalWeather()
{

}



// ================================================
// ProcAtmosCtrlPckt
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void GlobalWeather::ProcAtmosCtrlPckt(CigiAtmosCtrlV3 *Packet)
{

	AtmosEn = Packet->GetAtmosEn();
   Humidity = Packet->GetHumidity();
   AirTemp = Packet->GetAirTemp();
   VisibilityRange = Packet->GetVisibility();
   HorizWindSp = Packet->GetHorizWindSp();
   VertWindSp = Packet->GetVertWindSp();
   WindDir = Packet->GetWindDir();
   BaroPressure = Packet->GetBaroPress();

   // Set updated flag
   PcktUpdated = true;

}


