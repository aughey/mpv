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
 *  FILENAME:   WeatherCtlr.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *   represent a weather control
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  11/09/2005 Greg Basler                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#include <iostream>
#include <memory.h>

#include "WeatherCtrl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


// ================================================
// WeatherCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
WeatherCtrl::WeatherCtrl()
{
	Clear();
}


// ================================================
// ~WeatherCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
WeatherCtrl::~WeatherCtrl()
{

}



// ================================================
// ProcWeatherCtrlPckt
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void WeatherCtrl::ProcWeatherCtrlPckt(CigiWeatherCtrlV3 *Packet)
{

   if((Packet->GetScope() == CigiBaseWeatherCtrl::Entity) ||
      (LayerID == Packet->GetLayerID()))
   {
      Humidity = Packet->GetHumidity();
      WeatherEn = Packet->GetWeatherEn();
      ScudEn = Packet->GetScudEn();
      RandomWindsEn = Packet->GetRandomWindsEn();
      RandomLightningEn = Packet->GetRandomLightningEn();
      CloudType = Packet->GetCloudType();
      Scope = Packet->GetScope();
      Severity = Packet->GetSeverity();
      AirTemp = Packet->GetAirTemp();
      VisibilityRng = Packet->GetVisibilityRng();
      ScudFreq = Packet->GetScudFreq();
      Coverage = Packet->GetCoverage();
      BaseElev = Packet->GetBaseElev();
      Thickness = Packet->GetThickness();
      Transition = Packet->GetTransition();
      HorizWindSp = Packet->GetHorizWindSp();
      VertWindSp = Packet->GetVertWindSp();
      WindDir = Packet->GetWindDir();
      BaroPress = Packet->GetBaroPress();
      Aerosol = Packet->GetAerosol();
   }
}



// ================================================
// Clear
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void WeatherCtrl::Clear() 
{
   LayerID = 0;
   Humidity = 0;
   WeatherEn = false;
   ScudEn = false;
   RandomWindsEn = false;
   RandomLightningEn = false;
   CloudType = CigiBaseWeatherCtrl::None;
   Scope = CigiBaseWeatherCtrl::Global;
   Severity = 0;
   AirTemp = 0.0f;
   VisibilityRng = 0.0f;
   ScudFreq = 0.0f;
   Coverage = 0.0f;
   BaseElev = 0.0f;
   Thickness = 0.0f;
   Transition = 0.0f;
   HorizWindSp = 0.0f;
   VertWindSp = 0.0f;
   WindDir = 0.0f;
   BaroPress = 0.0f;
   Aerosol = 0.0f;

}

