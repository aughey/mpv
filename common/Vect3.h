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
 *  FILENAME:   Vect3.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *   manage a 3D vector.
 *
 *  General Usage:
 *
 *  1a.  Create a new object using Vect3 and
 *        then call the Set method with the x, y, and z values.
 *  Or
 *  1b.  Create a new object using Vect3 with the x, y, and z values.
 *
 *  Then
 *  2.   Call other methods as needed.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  04/04/2004 Greg Basler                       MPV_CR_DR_1
 *  Initial Release.
 *  
 *  2008-08-30 Andrew Sampson
 *      Renamed class to Vect3.  Moved class to the mpv namespace.  Modified 
 *      class to use an array to store the vector elements, instead of 
 *      individual member variables.
 *  
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#if !defined(_VECTOR_3_INCLUDED_)
#define _VECTOR_3_INCLUDED_

#include "MPVCommonTypes.h"

namespace mpv
{

//=========================================================
//! This class encapsulates a 3D Vector
//!
class MPVCMN_SPEC Vect3  
{
public:


   //==> Management

   //=========================================================
   //! General Constructor
   //!
	Vect3();

   //=========================================================
   //! Constructor with 3D initialization
   //! \param x_in - The length of the vector along the x axis.
   //! \param y_in - The length of the vector along the y axis.
   //! \param z_in - The length of the vector along the z axis.
   //!
   Vect3(double x_in, double y_in, double z_in);

   //=========================================================
   //! General Destructor
   //!
	virtual ~Vect3();



   //==> Basic vector functionality


   //=========================================================
   //! Sets the vector to the specified dimensions
   //! \param x_in - The length of the vector along the x axis.
   //! \param y_in - The length of the vector along the y axis.
   //! \param z_in - The length of the vector along the z axis.
   //!
	void Set(double x_in, double y_in, double z_in);

   //=========================================================
   //! Determines the magnitude of this vector
   //! \return The magnitude of this vector
   //!
	double mag(void) const;

   //=========================================================
   //! Returns the square of the magnitude of this vector.  This avoids 
   //! calling sqrt(), and so is more efficient than mag().  This is useful 
   //! when comparing the magnitude of two vectors, as such a situation 
   //! doesn't require knowing the actual magnitude, only the relative 
   //! difference between the two vectors.
   //! \return The square of the magnitude of this vector
   //!
	double mag2(void) const;

   //=========================================================
   //! Vector multiplication with a scalar
   //! \param k - A scalar value.
   //!
   //! \return The resulting vector
   //!
   Vect3 operator *(const double k) const;

   //=========================================================
   //! Vector division with a scalar
   //! \param k - A scalar value.
   //!
   //! \return The resulting vector
   //!
   Vect3 operator /(const double k) const;

   //=========================================================
   //! Vector addition with another vector
   //! \param v - A 3D vector.
   //!
   //! \return The resulting vector
   //!
	Vect3 operator +(const Vect3 &v) const;

   //=========================================================
   //! Vector subtraction with another vector
   //! \param v - A 3D vector.
   //!
   //! \return The resulting vector
   //!
	Vect3 operator -(const Vect3 &v) const;

   //=========================================================
   //! Vector cross product
   //! \param v - A 3D vector.
   //!
   //! \return The resulting vector
   //!
   Vect3 operator ^(const Vect3 &v) const;  // Cross product

   //=========================================================
   //! Vector Dot product
   //! \param v - A 3D vector.
   //!
   //! \return The dot product result
   //!
	double operator *(const Vect3 &v) const;   // Dot product

   //=========================================================
   //! Determine the angle between this vector and another
   //!   vector in radians.
   //! \param v - A 3D vector.
   //!
   //! \return The Angle between vectors (rad)
   //!
	double angle(Vect3 &v) const;

   //=========================================================
   //! Determine the angle between this vector and another
   //!   vector in degrees.
   //! \param v - A 3D vector.
   //!
   //! \return The Angle between vectors (deg)
   //!
	double angleD(Vect3 &v) const;

   //=========================================================
   //! Determine a unit vector
   //! \return The unit vector
   //!
	Vect3 Unit(void) const;

   //=========================================================
   //! Rotate the vector
   //! \param Normal - The normal around which this vector is rotated.
   //! \param angle - The angle by which the vector is rotated.
   //!
   //! \return The resulting vector
   //!
	Vect3 Rotate(Vect3 &Normal, double angle) const;

   //=========================================================
   //! Convert the vector to its negative value
   //! \return The negative vector
   //!
	Vect3 Neg(void) const;

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
	
	inline double & operator[]( int i ) { return vect[i]; }
	inline double operator[]( int i ) const { return vect[i]; }
	
	
protected:

	//=========================================================
	//! The array of values encapsulated by this class
	//!
	double vect[3];

};

}

#endif
