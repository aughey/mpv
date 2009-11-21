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
 *  FILENAME:   CoordSet.h
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
 *  03/29/2004 Greg Basler                       CIGI_CR_DR_1
 *  Initial Release.
 *  
 *  2008-07-06 Andrew Sampson
 *      Converted struct to a class, added a constructor
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#ifndef _COORDINATE_SET_H_
#define _COORDINATE_SET_H_


#include "MPVCommonTypes.h"

//=========================================================
//! A structure that contains the position and attitude data 
//!  needed to place an entity in either the CIGI coordinate 
//!  system or the Database coordinate system.
//!
class MPVCMN_SPEC CoordinateSet
{
public:
	
	CoordinateSet();
	
	~CoordinateSet();
	
   //=========================================================
   //! For Top Level entities:<br>
   //!    For Cigi:<br>
   //!       - Specifies the Latitude of the entity.<br>
   //!    For Database:<br>
   //!       - Specifies the position of the entity along
   //!         the Database's coordinate system X axis<br>
   //! For Children:<br>
   //!    - Specifies the position of the entity along
   //!      the parent's coordinate system X axis<br>
   //!
   double LatX;

   //=========================================================
   //! For Top Level entities:<br>
   //!    For Cigi:<br>
   //!       - Specifies the Longitude of the entity.<br>
   //!    For Database:<br>
   //!       - Specifies the position of the entity along
   //!         the Database's coordinate system Y axis<br>
   //! For Children:<br>
   //!    - Specifies the position of the entity along
   //!      the parent's coordinate system Y axis<br>
   //!
   double LonY;

   //=========================================================
   //! For Top Level entities:<br>
   //!    For Cigi:<br>
   //!       - Specifies the Altitude of the entity.<br>
   //!    For Database:<br>
   //!       - Specifies the position of the entity along
   //!         the Database's coordinate system Z axis<br>
   //! For Children:<br>
   //!    - Specifies the position of the entity along
   //!      the parent's coordinate system Z axis<br>
   //!
   double AltZ;

   //=========================================================
   //! For Top Level entities:<br>
   //!    For Cigi:<br>
   //!       - Absolute Yaw attitude in the NED coordinate system<br>
   //!    For Database:<br>
   //!       - Absolute Yaw attitude in the Database's coordinate
   //!         system<br>
   //! For Children:<br>
   //!    - Relative Yaw attitude in the parent's coordinate system<br>
   //! Units are degrees.
   //!
   double Yaw;

   //=========================================================
   //! For Top Level entities:<br>
   //!    For Cigi:<br>
   //!       - Absolute Pitch attitude in the NED coordinate system<br>
   //!    For Database:<br>
   //!       - Absolute Pitch attitude in the Database's coordinate
   //!         system<br>
   //! For Children:<br>
   //!    - Relative Pitch attitude in the parent's coordinate system<br>
   //! Units are degrees.
   //!
   double Pitch;

   //=========================================================
   //! For Top Level entities:<br>
   //!    For Cigi:<br>
   //!       - Absolute Roll attitude in the NED coordinate system<br>
   //!    For Database:<br>
   //!       - Absolute Roll attitude in the Database's coordinate
   //!         system<br>
   //!    - Absolute Roll attitude in the NED coordinate system<br>
   //! For Children:<br>
   //!    - Relative Roll attitude in the parent's coordinate system<br>
   //! Units are degrees.
   //!
   double Roll;

};

typedef CoordinateSet CoordSet;

#endif
