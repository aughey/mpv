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
 *  FILENAME:   WeatherCtrl.h
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

#if !defined(_WEATHER_CTRL_INCLUDED_)
#define _WEATHER_CTRL_INCLUDED_

#include <CigiWeatherCtrlV3.h>
#include <string>

#include "MPVCommonTypes.h"


//=========================================================
//! A class encapsulating the CIGI Weather Control
//!
class MPVCMN_SPEC WeatherCtrl  
{
public:


   //==> Management

   //=========================================================
   //! General Constructor
   //!
	WeatherCtrl();

   //=========================================================
   //! General Destructor
   //!
	virtual ~WeatherCtrl();


   //==> Normal periodic action


   //=========================================================
   //! A per frame update
   //!
   void Act(void) { };


   //==> Setting Members from Cigi Packets


   //=========================================================
   //! Sets the weather data from the incoming weather control packet
   //! \param Packet - A pointer to the incoming packet
   //!
   void ProcWeatherCtrlPckt(CigiWeatherCtrlV3 *Packet);



   //==> Accessing Weather Ctrl packet data


   //=========================================================
   //! Sets the LayerID value.
   //! \param LayerIDIn -  the current LayerID.
   void SetLayerID(unsigned char LayerIDIn) { LayerID = LayerIDIn; }

   //=========================================================
   //! Gets the LayerID value.
   //! \return the current LayerID.
   unsigned char GetLayerID(void) const { return(LayerID); }

   //=========================================================
   //! Gets the Humidity value.
   //! \return the current Humidity.
   unsigned char GetHumidity(void) const { return(Humidity); }

   //=========================================================
   //! Gets the CloudType value.
   //! \return the current CloudType.
   CigiBaseWeatherCtrl::CloudTypeGrp GetCloudType(void) const { return(CloudType); }

   //=========================================================
   //! Gets the RandomLightningEn value.
   //! \return the current RandomLightningEn.
   bool GetRandomLightningEn(void) const { return(RandomLightningEn); }

   //=========================================================
   //! Gets the RandomWindsEn value.
   //! \return the current RandomWindsEn.
   bool GetRandomWindsEn(void) const { return(RandomWindsEn); }

   //=========================================================
   //! Gets the ScudEn value.
   //! \return the current ScudEn.
   bool GetScudEn(void) const { return(ScudEn); }

   //=========================================================
   //! Gets the WeatherEn value.
   //! \return the current WeatherEn.
   bool GetWeatherEn(void) const { return(WeatherEn); }

   //=========================================================
   //! Gets the WeatherEn value.
   //! \return the current WeatherEn.
   void SetWeatherEn(bool WeatherEnIn) { WeatherEn = WeatherEnIn; }

   //=========================================================
   //! Gets the Severity value.
   //! \return the current Severity.
   unsigned char GetSeverity(void) const { return(Severity); }

   //=========================================================
   //! Gets the Scope value.
   //! \return the current Scope.
   CigiBaseWeatherCtrl::ScopeGrp GetScope(void) const { return(Scope); }

   //=========================================================
   //! Gets the AirTemp value.
   //! \return the current AirTemp.
   float GetAirTemp(void) const { return(AirTemp); }

   //=========================================================
   //! Gets the VisibilityRng value.
   //! \return the current VisibilityRng.
   float GetVisibilityRng(void) const { return(VisibilityRng); }

   //=========================================================
   //! Gets the ScudFreq value.
   //! \return the current ScudFreq.
   float GetScudFreq(void) const { return(ScudFreq); }

   //=========================================================
   //! Gets the Coverage value.
   //! \return the current Coverage.
   float GetCoverage(void) const { return(Coverage); }

   //=========================================================
   //! Gets the BaseElev value.
   //! \return the current BaseElev.
   float GetBaseElev(void) const { return(BaseElev); }

   //=========================================================
   //! Gets the Thickness value.
   //! \return the current Thickness.
   float GetThickness(void) const { return(Thickness); }

   //=========================================================
   //! Gets the Transition value.
   //! \return the current Transition.
   float GetTransition(void) const { return(Transition); }

   //=========================================================
   //! Gets the HorizWindSp value.
   //! \return the current HorizWindSp.
   float GetHorizWindSp(void) const { return(HorizWindSp); }

   //=========================================================
   //! Gets the VertWindSp value.
   //! \return the current VertWindSp.
   float GetVertWindSp(void) const { return(VertWindSp); }

   //=========================================================
   //! Gets the WindDirAz value.
   //! \return the current WindDirAz.
   float GetWindDir(void) const { return(WindDir); }

   //=========================================================
   //! Gets the BaroPress value.
   //! \return the current BaroPress.
   float GetBaroPress(void) const { return(BaroPress); }

   //=========================================================
   //! Gets the Aerosol value.
   //! \return the current Aerosol.
   float GetAerosol(void) const { return(Aerosol); }


protected:

   //==> Member variables


   //=========================================================
   //! The Layer ID that this packet controls
   //!
   unsigned char LayerID;

   //=========================================================
   //! The humidity in this weather zone
   //!
   unsigned char Humidity;

   //=========================================================
   //! Enables weather
   //!
   bool WeatherEn;

   //=========================================================
   //! Enables Scud
   //!
   bool ScudEn;

   //=========================================================
   //! Enables random winds
   //!
   bool RandomWindsEn;

   //=========================================================
   //! Enables random lightning
   //!
   bool RandomLightningEn;

   //=========================================================
   //! Type of clouds being controlled.<br>
   //!  0-None<br>
   //!  1-Altocumulus<br>
   //!  2-Altostratus<br>
   //!  3-Cirrocumulus<br>
   //!  4-Cirrostratus<br>
   //!  5-Cirrus<br>
   //!  6-Cumulonimbus<br>
   //!  7-Cumulus<br>
   //!  8-Nimbostratus<br>
   //!  9-Stratocumulus<br>
   //!  10-Stratus<br>
   //!  11-DefA<br>
   //!  12-DefB<br>
   //!  13-DefC<br>
   //!  14-DefD<br>
   //!  15-DefE
   //!
   //!
   CigiBaseWeatherCtrl::CloudTypeGrp CloudType;

   //=========================================================
   //! Specifies whether the weather being controlled is to be<br>
   //!   global, attached to a region, or attached to an entity.<br>
   //!  0-Global<br>
   //!  1-Regional<br>
   //!  2-Entity
   //!
   //!
   CigiBaseWeatherCtrl::ScopeGrp Scope;

   //=========================================================
   //! Weather severity<br>
   //!  0-Least severe
   //!  5-Most severe
   //!
   unsigned char Severity;

   //=========================================================
   //! Temperature of the air.
   //!
   float AirTemp;

   //=========================================================
   //! Visibility range
   //!
   float VisibilityRng;

   //=========================================================
   //! Scud frequency (i.e. percentage coverage.)
   //!
   float ScudFreq;

   //=========================================================
   //! Percentage of (cloud?) coverage.
   //!
   float Coverage;

   //=========================================================
   //! Altitude of the base of the cloud
   //!
   float BaseElev;

   //=========================================================
   //! Thickness of the cloud from bottom to top.
   //!
   float Thickness;

   //=========================================================
   //! Thickness of the transition band
   //!
   float Transition;

   //=========================================================
   //! Horizontal Wind Speed
   //!
   float HorizWindSp;

   //=========================================================
   //! Vertical Wind Speed
   //!
   float VertWindSp;

   //=========================================================
   //! Wind direction
   //!
   float WindDir;

   //=========================================================
   //! Barometric pressure
   //!
   float BaroPress;

   //=========================================================
   //! Amount of aerosol
   //!
   float Aerosol;


   //=========================================================
   //! Clears the contents of this weather control; called by the constructors.
   void Clear();
};

#endif // !defined(_WEATHER_CTRL_INCLUDED_)
