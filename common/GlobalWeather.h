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
 *  FILENAME:   GlobalWeather.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  FIXME
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

#if !defined(_GLOBAL_WEATHER_INCLUDED_)
#define _GLOBAL_WEATHER_INCLUDED_

#include <CigiAtmosCtrl.h>
#include <list>
#include <string>
#include <vector>
#include "CompCtrlList.h"
#include "WeatherCtrlList.h"
#include "CoordSet.h"

#include "MPVCommonTypes.h"



//=========================================================
//! FIXME
//!
class MPVCMN_SPEC GlobalWeather : public CompCtrlList,
   public WeatherCtrlList
{
public:

   //==> Management

   //=========================================================
   //! General Constructor
   //!
	GlobalWeather();

   //=========================================================
   //! General Destructor
   //!
	virtual ~GlobalWeather();



   //==> Accessing Read-Only Member functions

   //=========================================================
   //! Gets the Atmosphere Enabled flag.
   //! \return the Atmosphere Enabled flag.
   bool GetAtmosEn(void) const
   {
      return(AtmosEn);
   }

   //=========================================================
   //! Gets the Humidity value.
   //! \return the integer Humidity percentage.
   unsigned char GetHumidity(void) const
   {
      return(Humidity);
   }

   //=========================================================
   //! Gets the ambient air temperature.
   //! \return the ambient air temperature in degrees celcius.
   float GetAirTemp(void) const
   {
      return(AirTemp);
   }

   //=========================================================
   //! Gets the Visibility Range.
   //! \return the Visibility Range in meters.
   float GetVisibilityRange(void) const
   {
      return(VisibilityRange);
   }

   //=========================================================
   //! Gets the Horizontal Wind Speed.
   //! \return the Horizontal Wind Speed in m/s.
   float GetHorizWindSp(void) const
   {
      return(HorizWindSp);
   }

   //=========================================================
   //! Gets the Vertical Wind Speed.
   //! \return the Vertical Wind Speed in m/s.
   float GetVertWindSp(void) const
   {
      return(VertWindSp);
   }

   //=========================================================
   //! Gets the Wind Direction.
   //! \return the Wind Direction in degrees.
   //!   This specifies the direction that the wind is blowing from.
   float GetWindDir(void) const
   {
      return(WindDir);
   }

   //=========================================================
   //! Gets the barometric pressure.
   //! \return the barometric pressure in millibars.
   float GetBaroPressure(void) const
   {
      return(BaroPressure);
   }


   //==> Processing Packets Member functions


   //=========================================================
   //! Processes the atmospheric control packet
   //! \param Packet - 
   //!
	void ProcAtmosCtrlPckt(CigiAtmosCtrlV3 *Packet);


   //==> Global Weather Act


   //=========================================================
   //! FIXME
   //!
   void Act(void) { PcktUpdated = false; }



protected:

   //==> Member variables

   //=========================================================
   //! Specifies whether the global atmosphere properties are enabled
   //!
	bool AtmosEn;

   //=========================================================
   //! The integer percentage humidity
   //!
   unsigned char Humidity;

   //=========================================================
   //! Ambient Air Temperature in degrees Celsius
   //!
   float AirTemp;

   //=========================================================
   //! Visibility Range in meters
   //!
   float VisibilityRange;

   //=========================================================
   //! Horizontal wind speed in meters per second.
   //!
   float HorizWindSp;

   //=========================================================
   //! Vertical wind speed in meters per second.
   //!
   float VertWindSp;

   //=========================================================
   //! Wind direction in degrees.
   //! Specifies the direction from which the wind is blowing
   //!
   float WindDir;

   //=========================================================
   //! Barometric Pressure in millibars or hectopascals.
   //!
   float BaroPressure;

   //=========================================================
   //! This Atmosphere parameter set has been updated by a CIGI packet this frame
   //!
   bool PcktUpdated;

};


#endif // !defined(_GLOBAL_WEATHER_INCLUDED_)
