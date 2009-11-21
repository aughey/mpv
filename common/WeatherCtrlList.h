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
 *  FILENAME:   WeatherCtrlList.h
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
 *  11/03/2005 Greg Basler                       MPV_CR_DR_1
 *  Initial Release.
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#if !defined(_WEATHER_CTRL_LIST_INCLUDED_)
#define _WEATHER_CTRL_LIST_INCLUDED_

#include <list>
#include "WeatherCtrl.h"
#include "CigiIGCtrlV3.h"

#include "MPVCommonTypes.h"

#if defined(_MSC_VER)
   #pragma warning(push)
   #pragma warning(disable : 4251)
#endif

//=========================================================
//! FIXME
//!
class MPVCMN_SPEC WeatherCtrlList
{
public:

   //==> Management

   //=========================================================
   //! General Constructor
   //!
   WeatherCtrlList() {};

   //=========================================================
   //! General Destructor
   //!
   virtual ~WeatherCtrlList()
   {
      WeatherCtrls.clear();
   }


   //==> Accessing Read-Only Member functions


   //=========================================================
   //! Gets An iterator of the Weather Control List
   //! \return The WeatherCtrl iterator.
   //!
   std::list<WeatherCtrl>::iterator GetWeatherCtrls(void) { return(WeatherCtrls.begin()); }

   //=========================================================
   //! Gets an iterator pointing to the end of the Weather Control List
   //! \return The WeatherCtrl iterator.
   //!
   std::list<WeatherCtrl>::iterator GetWeatherCtrlsEnd(void) { return(WeatherCtrls.end()); }


   //==> Processing Packets Member functions


   //=========================================================
   //! Processes the Weather Control packet
   //! \param Packet - A pointer to the received CigiWeatherCtrlV3 
   //!   packet for this entity
   //!
	void ProcWeatherCtrlPckt(CigiWeatherCtrlV3 *Packet)
   {

      // Check each comp ctrl until found or end
      std::list<WeatherCtrl>::iterator pCmp = WeatherCtrls.begin();
      while(pCmp != WeatherCtrls.end())
      {
         if(pCmp->GetLayerID() == Packet->GetLayerID())
         {
            pCmp->ProcWeatherCtrlPckt(Packet);
            break;
         }
         else
            pCmp++;
      }

   }


   //=========================================================
   //! FIXME
   //!
   void disableAll()
   {
      // disable each weather ctrl
      std::list<WeatherCtrl>::iterator pCmp = WeatherCtrls.begin();
      while(pCmp != WeatherCtrls.end())
      {
         pCmp->SetWeatherEn(false);
         pCmp++;
      }
   }

   //=========================================================
   //! FIXME
   //!
   void enableAll()
   {
      // disable each weather ctrl
      std::list<WeatherCtrl>::iterator pCmp = WeatherCtrls.begin();
      while(pCmp != WeatherCtrls.end())
      {
         pCmp->SetWeatherEn(true);
         pCmp++;
      }
   }


   //==> Weather Control Management Member functions


   //=========================================================
   //! Adds a weather control to the WeatherCtrl list
   //! \return - A reference to the component control object 
   //!   added to the WeatherCtrl list.
   //!
  	WeatherCtrl &AddWeatherCtrl(void)
   {
      WeatherCtrl tWeatherCtrl;
      WeatherCtrls.push_back(tWeatherCtrl);
      return(WeatherCtrls.back());
   }

   //=========================================================
   //! Adds a weather control to the WeatherCtrl list
   //! \param WeatherCtrlIn - A reference to the weather control object 
   //!   to be added to the WeatherCtrl list.
   //!
   void AddWeatherCtrl(WeatherCtrl &WeatherCtrlIn)
   {
      WeatherCtrls.push_back(WeatherCtrlIn);
   }



protected:

   //==> Member variables


   //=========================================================
   //! WeatherCtrls<br>
   //! A list of component controls for this entity.
   //!
   std::list<WeatherCtrl> WeatherCtrls;

};

#if defined(_MSC_VER)
   #pragma warning(pop)
#endif

#endif // !defined(_WEATHER_CTRL_LIST_INCLUDED_)
