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
 *  FILENAME:   ExtrapolationSet.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  A structure that contains the all the data needed
 *    to place an entity and to extrapolate it when needed.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  10/14/2005 Greg Basler                       CIGI_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#if !defined(_EXTRAPOLATION_SET_INCLUDED_)
#define _EXTRAPOLATION_SET_INCLUDED_


#include "MPVCommonTypes.h"


//=========================================================
//! CoordinateSet<br>
//! A structure that contains the all the data needed<br>
//!  to place an entity and to extrapolate it when needed
//!
typedef struct ExtrapolationFactorSet
{

   //==> Commanded Rates

   //=========================================================
   //! XRate<br>
   //! The meters per second velocity along the X axis.<br>
   //!  For an entity - The entity coordinate system X axis rate.<br>
   //!  For an art part - The art part's X axis rate.
   //!
   float CmdXRate;

   //=========================================================
   //! YRate<br>
   //! The meters per second velocity along the Y axis.<br>
   //!  For an entity - The entity coordinate system Y axis rate.<br>
   //!  For an art part - The art part's Y axis rate.
   //!
   float CmdYRate;

   //=========================================================
   //! ZRate<br>
   //! The meters per second velocity along the Z axis.<br>
   //!  For an entity - The entity coordinate system Z axis rate.<br>
   //!  For an art part - The art part's Z axis rate.
   //!
   float CmdZRate;


   //==> Current Rates

   //=========================================================
   //! XRate<br>
   //! The meters per second velocity along the X axis.<br>
   //!  For an entity - The entity coordinate system X axis rate.<br>
   //!  For an art part - The art part's X axis rate.
   //!
   float XRate;

   //=========================================================
   //! YRate<br>
   //! The meters per second velocity along the Y axis.<br>
   //!  For an entity - The entity coordinate system Y axis rate.<br>
   //!  For an art part - The art part's Y axis rate.
   //!
   float YRate;

   //=========================================================
   //! ZRate<br>
   //! The meters per second velocity along the Z axis.<br>
   //!  For an entity - The entity coordinate system Z axis rate.<br>
   //!  For an art part - The art part's Z axis rate.
   //!
   float ZRate;

   //=========================================================
   //! RollRate<br>
   //! The degrees per second velocity about the X axis.<br>
   //!  For an entity - The Geodetic surface NED pitch and
   //!    yaw rotated X axis.<br>
   //!  For an art part - The host entity's body X axis.
   //!
   float RollRate;

   //=========================================================
   //! PitchRate<br>
   //! The degrees per second velocity about the Y axis.<br>
   //!  For an entity - The Geodetic surface NED yaw rotated Y axis.<br>
   //!  For an art part - The host entity's body Y axis.
   //!
   float PitchRate;

   //=========================================================
   //! YawRate<br>
   //! The degrees per second velocity about the Z axis.<br>
   //!  For an entity - The Geodetic surface NED Z axis.<br>
   //!  For an art part - The host entity's body Z axis.
   //!
   float YawRate;


   //==> Trajectory

   //=========================================================
   //! AccelX<br>
   //! The acceleration along the X (North) axis
   //!
   float AccelX;

   //=========================================================
   //! AccelY<br>
   //! The acceleration along the Y (East) axis
   //!
   float AccelY;

   //=========================================================
   //! AccelZ<br>
   //! The acceleration along the Z (Down) axis
   //!
   float AccelZ;

   //=========================================================
   //! RetardationRate<br>
   //! Deceleration (usually due to drag)
   //!
   float RetardationRate;

   //=========================================================
   //! TermVel<br>
   //! Terminal Velocity
   //!
   float TermVel;

   //=========================================================
   //! DragF<br>
   //! Drag Factor: A factor when multiplied by current velocity
   //!   and elapsed time for this frame generates a velocity
   //!   that should be subtracted from the current velocity
   //!   to simulate drag.
   //!
   double DragF;


} ExtrapolationSet;



#endif   // _EXTRAPOLATION_SET_INCLUDED_
