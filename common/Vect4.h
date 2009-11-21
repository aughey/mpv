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
 *  FILENAME:   Vect4.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  A class to manage and manipulate 3D homogeneous vectors.
 *  This is not a true 4 element vector.
 *  The 4th element is a scaling factor not a true element.
 *
 *  General Usage:
 *
 *  1a.  Create a new object using Vect4 and
 *         then call the Set method with the x, y, z, and w values.
 *  Or
 *  1b.  Create a new object using Vect4 with the x, y, z, and w values.
 *
 *  Then
 *  2.   Call other methods as needed.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  04/21/2005 Greg Basler                       MPV_CR_DR_1
 *  Initial Release.
 *  
 *  2008-08-30 Andrew Sampson
 *      Renamed class to Vect4.  Moved class to the mpv namespace.  Modified 
 *      class to use an array to store the vector elements, instead of 
 *      individual member variables.
 *  
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#if !defined(_VECT_4_INCLUDED_)
#define _VECT_4_INCLUDED_

#include "MPVCommonTypes.h"

namespace mpv
{

//=========================================================
//! This class encapsulates a 4D Vector
//!
class MPVCMN_SPEC Vect4  
{
public:

   //==> Management

   //=========================================================
   //! General Constructor
   //!
	Vect4();

   //=========================================================
   //! Constructor with initialization
   //! \param x_in - The length of the vector along the x axis.
   //! \param y_in - The length of the vector along the y axis.
   //! \param z_in - The length of the vector along the z axis.
   //! \param w_in - The value of the vector's scaling factor.
   //! 
   Vect4(double x_in, double y_in, double z_in, double w_in);

   //=========================================================
   //! General Destructor
   //!
	virtual ~Vect4();

   //==> Basic vector functionality


   //=========================================================
   //! Sets the vector to the specified dimensions
   //! \param x_in - The length of the vector along the x axis.
   //! \param y_in - The length of the vector along the y axis.
   //! \param z_in - The length of the vector along the z axis.
   //! \param w_in - The value of the vector's scaling factor.
   //!
	void Set(double x_in, double y_in, double z_in, double w_in);

   //=========================================================
   //! Determines the magnitude of this vector.  Only the first 3 elements 
   //! (x, y, z) are used.
   //! \return The magnitude of this vector
   //!
	double mag(void) const;

   //=========================================================
   //! Returns the square of the magnitude of this vector.  This avoids 
   //! calling sqrt(), and so is more efficient than mag().  This is useful 
   //! when comparing the magnitude of two vectors, as such a situation 
   //! doesn't require knowing the actual magnitude, only the relative 
   //! difference between the two vectors.  
   //! Only the first 3 elements (x, y, z) are used.
   //! \return The square of the magnitude of this vector
   //!
	double mag2(void) const;

   //=========================================================
   //! Vector multiplication with a scalar
   //! \param k - A scalar value.
   //!
   //! \return The resulting vector
   //!
   Vect4 operator *(const double k) const;

   //=========================================================
   //! Vector division with a scalar
   //! \param k - A scalar value.
   //!
   //! \return The resulting vector
   //!
   Vect4 operator /(const double k) const;

   //=========================================================
   //! Vector addition with another vector
   //! \param v - A 3D vector.
   //!
   //! \return The resulting vector
   //!
	Vect4 operator +(const Vect4 &v) const;

   //=========================================================
   //! Vector subtraction with another vector
   //! \param v - A 3D vector.
   //!
   //! \return The resulting vector
   //!
	Vect4 operator -(const Vect4 &v) const;

   //=========================================================
   //! Vector cross product
   //! \param v - A 3D vector.
   //!
   //! \return The resulting vector
   //!
   Vect4 operator ^(const Vect4 &v) const;  // Cross product

   //=========================================================
   //! Vector Dot product
   //! \param v - A 3D vector.
   //!
   //! \return The dot product result
   //!
	double operator *(const Vect4 &v) const;   // Dot product

   //=========================================================
   //! Determine the angle between this vector and another
   //!   vector in radians.
   //! \param v - A 3D vector.
   //!
   //! \return The Angle between vectors (rad)
   //!
	double angle(Vect4 &v) const;   // Angle between vectors (rad)

   //=========================================================
   //! Determine the angle between this vector and another
   //!   vector in degrees.
   //! \param v - A 3D vector.
   //!
   //! \return The Angle between vectors (deg)
   //!
	double angleD(Vect4 &v) const;  // Angle between vectors (deg)

   //=========================================================
   //! Determine a unit vector
   //! \return The unit vector
   //!
	Vect4 Unit(void) const;

   //=========================================================
   //! Rotate the vector
   //! \param Normal - The normal around which this vector is rotated.
   //! \param angle - The angle by which the vector is rotated.
   //!
   //! \return The resulting vector
   //!
	Vect4 Rotate(Vect4 &Normal, double angle) const;

   //=========================================================
   //! Convert the vector to its negative value
   //! \return The negative vector
   //!
	Vect4 Neg(void) const;

   //=========================================================
   //! Gets the length of the vector along the x axis
   //! \return The x value
   //!
   double getx(void) const { return vect[0]; }

   //=========================================================
   //! Gets the length of the vector along the y axis
   //! \return The y value
   //!
   double gety(void) const { return vect[1]; }

   //=========================================================
   //! Gets the length of the vector along the z axis
   //! \return The z value
   //!
   double getz(void) const { return vect[2]; }

   //=========================================================
   //! Gets the value of the vector's scaling factor.
   //! \return The w value
   //!
   double getw(void) const { return vect[3]; }
	
	inline double & operator[]( int i ) { return vect[i]; }
	inline double operator[]( int i ) const { return vect[i]; }
	
	
protected:

	//=========================================================
	//! The array of values encapsulated by this class
	//!
	double vect[4];

};

}

#endif

