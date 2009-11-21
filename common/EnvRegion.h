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
 *  FILENAME:   EnvRegion.h
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

#if !defined(_ENVIRONMENTAL_REGION_INCLUDED_)
#define _ENVIRONMENTAL_REGION_INCLUDED_

#include <CigiEnvRgnCtrlV3.h>
#include <list>
#include <string>
#include <vector>
#include "CompCtrlList.h"
#include "WeatherCtrlList.h"
#include "CoordSet.h"
#include "BasePositionConversion.h"

#include "ExtendedData.h"

#include "MPVCommonTypes.h"



//=========================================================
//! This class encapsulates the Environmental Region
//!   functionality and data
//!
class MPVCMN_SPEC EnvRegion : public ExtendedDataSet, public CompCtrlList,
   public WeatherCtrlList
{
public:

   //==> Management

   //=========================================================
   //! General Constructor
   //!
	EnvRegion();

   //=========================================================
   //! General Destructor
   //!
	virtual ~EnvRegion();



   //==> Accessing Modifiable Member functions


   //+> EnvRegion ID

   //=========================================================
   //! Sets the Environmental Region ID
   //! \param EntityIDIn - The new Environmental Region ID.
   //!
   void SetEnvRegionID(unsigned short EnvRegionIDIn) { EnvRegionID = EnvRegionIDIn; }

   //=========================================================
   //! Gets the Environmental Region ID
   //! \return The Environmental Region ID.
   //!
   unsigned short GetEnvRegionID(void) { return(EnvRegionID); }



   //==> Accessing Write-Only Member functions


   //=========================================================
   //! Sets the pointer to the position conversion object
   //! \param PosConvIn - The pointer to the position conversion object.
   //!
   void SetPosConv(CoordCigi2DBaseFunctionPtr CoordCigi2DBaseIn,
                   CoordDBase2CigiFunctionPtr CoordDBase2CigiIn)
   {
      CoordCigi2DBase = CoordCigi2DBaseIn;
      CoordDBase2Cigi = CoordDBase2CigiIn;
   }
      


   //==> Accessing Read-Only Member functions

   //=========================================================
   //! Gets the RgnState value.
   //! \return the current RgnState.
   CigiBaseEnvRgnCtrl::RgnStateGrp GetRgnState(void) const
   {
      return(RgnState);
   }

   //=========================================================
   //! Gets the WeatherProp value.
   //! \return the current WeatherProp.
   CigiBaseEnvRgnCtrl::MergeCtrlGrp GetWeatherProp(void) const
   {
      return(WeatherProp);
   }

   //=========================================================
   //! Gets the Aerosol value.
   //! \return the current Aerosol.
   CigiBaseEnvRgnCtrl::MergeCtrlGrp GetAerosol(void) const
   {
      return(Aerosol);
   }

   //=========================================================
   //! Gets the MaritimeSurface value.
   //! \return the current MaritimeSurface.
   CigiBaseEnvRgnCtrl::MergeCtrlGrp GetMaritimeSurface(void) const
   {
      return(MaritimeSurface);
   }

   //=========================================================
   //! Gets the TerrestrialSurface value.
   //! \return the current TerrestrialSurface.
   CigiBaseEnvRgnCtrl::MergeCtrlGrp GetTerrestrialSurface(void) const
   {
      return(TerrestrialSurface);
   }

   //=========================================================
   //! Gets the Lat value.
   //! \return the current Lat.
   double GetLat(void) const { return(CigiPos.LatX); }

   //=========================================================
   //! Gets the Lon value.
   //! \return the current Lon.
   double GetLon(void) const { return(CigiPos.LonY); }

   //=========================================================
   //! Rotation of the environmental region
   //! \return The rotation in degrees of the environmental region<br>
   double GetRotation(void) const { return(CigiPos.Yaw); }

   //=========================================================
   //! Gets the XSize value.
   //! \return the current XSize.
   float GetXSize(void) const { return(XSize); }

   //=========================================================
   //! Gets the YSize value.
   //! \return the current YSize.
   float GetYSize(void) const { return(YSize); }

   //=========================================================
   //! Gets the CornerRadius value.
   //! \return the current CornerRadius.
   float GetCornerRadius(void) const { return(CornerRadius); }

   //=========================================================
   //! Gets the Transition value.
   //! \return the current Transition.
   float GetTransition(void) const { return(Transition); }

   //=========================================================
   //! Gets the X position in the database coordinate system.
   //! \return the current X position.
   double GetDbaseX(void) const { return(DBasePos.LatX); }

   //=========================================================
   //! Gets the Y position in the database coordinate system.
   //! \return the current Y position.
   double GetDbaseY(void) const { return(DBasePos.LonY); }

   //=========================================================
   //! Gets the Rotation in the database coordinate system.
   //! \return the current Database rotation.
   double GetDbaseRotation(void) const { return(DBasePos.Yaw); }



   //==> Processing Packets Member functions


   //=========================================================
   //! Processes the environmental region control packet
   //! \param Packet - A pointer to the received CigiEnvRgnCtrlV3 
   //!   packet for this environmental region
   //!
	void ProcEnvRegionCtrlPckt(CigiEnvRgnCtrlV3 *Packet);


   //==> Env Region Processing Member functions


   //=========================================================
   //! Processes the environmental region for this frame.<br>
   //!   This clears the updated flag to prepair for the next frame.
   //! \param dTime - The elapsed or delta time in seconds between
   //!   the last rendered frame and this rendered frame.
   //!
   void Act(float dTime) { PcktUpdated = false; }


   //==> Environmental Region Extended Data public functions


   //=========================================================
   //! Gets the number of extended data items
   //! \return - The number of extended data items.
   //!
   static int GetExtendedDataCount(void) { return(ExtendedDataCount); }

   //=========================================================
   //! Gets the number of extended data items
   //! \return - The number of extended data items.
   //!
   static void IncrementExtendedDataCount(void) { ExtendedDataCount++; }



protected:

   //==> Member variables

   //=========================================================
   //! Entity ID<br>
   //! Uniquely identifies the specific environmental region instance<br>
   //!
	unsigned short EnvRegionID;

   //=========================================================
   //! XSize<br>
   //! X size of the region
   //!
   float XSize;

   //=========================================================
   //! YSize<br>
   //! Y size of the region
   //!
   float YSize;

   //=========================================================
   //! CornerRadius<br>
   //! Radius of the corners
   //!
   float CornerRadius;

   //=========================================================
   //! Transition<br>
   //! Width of the transition band.
   //!
   float Transition;


   //==> Member flags

   //=========================================================
   //! RgnState<br>
   //! Region state control<br>
   //!  0-Inactive<br>
   //!  1-Active<br>
   //!  2-Destroyed
   //!
   CigiBaseEnvRgnCtrl::RgnStateGrp RgnState;

   //=========================================================
   //! WeatherProp<br>
   //! Weather property merge control
   //!
   CigiBaseEnvRgnCtrl::MergeCtrlGrp WeatherProp;

   //=========================================================
   //! Aerosol<br>
   //! Aerosol Concentration merge control
   //!
   CigiBaseEnvRgnCtrl::MergeCtrlGrp Aerosol;

   //=========================================================
   //! MaritimeSurface<br>
   //! Maritime surface conditions merge control
   //!
   CigiBaseEnvRgnCtrl::MergeCtrlGrp MaritimeSurface;

   //=========================================================
   //! TerrestrialSurface<br>
   //! Terrestrial surface conditions merge control
   //!
   CigiBaseEnvRgnCtrl::MergeCtrlGrp TerrestrialSurface;


   //==> Position/Attitude/Rate Data

   //=========================================================
   //! CigiPos<br>
   //! The Cigi Coordinate Position, and Attitude of the Entity
   //!
   CoordSet CigiPos;

   //=========================================================
   //! DBasePos<br>
   //! The Database Coordinate Position, and Attitude of the Entity
   //!
   CoordSet DBasePos;


   //==> Overall Flags and pointers


   //=========================================================
   //! ExtendCount<br>
   //! The number of extended data items per entity
   //!
   static int ExtendedDataCount;

   //=========================================================
   //! PcktUpdated<br>
   //! This environmental region has been updated by a CIGI packet this frame
   //!
   bool PcktUpdated;

   //=========================================================
   //! CoordCigi2DBase<br>
   //! A pointer to the CIGI coordinate to Database coordinate
   //!  member function of the position conversion plugin 
   //!
   CoordCigi2DBaseFunctionPtr CoordCigi2DBase;

   //=========================================================
   //! CoordDBase2Cigi<br>
   //! A pointer to the Database coordinate to CIGI coordinate
   //!  member function of the position conversion plugin 
   //!
   CoordDBase2CigiFunctionPtr CoordDBase2Cigi;


};


#endif // !defined(_ENVIRONMENTAL_REGION_INCLUDED_)
