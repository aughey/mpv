/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2008 The Boeing Company
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
 *  FILENAME:   Vect2.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  manage a 2D vector.
 *
 *  General Usage:
 *
 *  1a.  Create a new object using Vect2 and
 *        then call the Set method with the x and y values.
 *  Or
 *  1b.  Create a new object using Vect2 with the x and y values.
 *
 *  Then
 *  2.   Call other methods as needed.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  2008-01-09 Greg Basler
 *      Initial Release.
 *  
 *  2008-08-30 Andrew Sampson
 *      Renamed class to Vect2.  Moved class to the mpv namespace.  Modified 
 *      class to use an array to store the vector elements, instead of 
 *      individual member variables.
 *  
 * </pre>
 */



#if !defined(_VECTOR_2D_INCLUDED_)
#define _VECTOR_2D_INCLUDED_

#include "MPVCommonTypes.h"
#include "Vect3.h"

namespace mpv
{

//=========================================================
//! The Vect2 class contains the processing and handling
//!  of two dimensional vectors.  It handles matrix addition, 
//!  multiplication, cross-products, dot products, and several 
//!  other vector related functions.  It also handles 
//!  multiplication and division with a scalar value.
//!
class MPVCMN_SPEC Vect2
{

public:

   //==> Management

   //=========================================================
   //! General Constructor
   //!
   Vect2();

   //=========================================================
   //! Constructor with the two element values
   //! \param x_in - The length along the x axis
   //! \param y_in - The length along the y axis
   //!
   Vect2(double x_in, double y_in);

   //=========================================================
   //! Constructor with a 3d vector
   //! \param v - A reference to the initializing 3d vector
   //!
   Vect2(const Vect3 &v);

   //=========================================================
   //! General Destructor
   //!
   virtual ~Vect2();


   //=========================================================
   //! Sets this 2d vector from the data of the 3d vector
   //! \param v - A reference to a 3D vector
   //!
   //! \return A reference to this vector 
   //!
   Vect2 & operator =(const Vect3 &v);

   //=========================================================
   //! Initializes the element values with the supplied values
   //! \param x_in - The length along the x axis
   //! \param y_in - The length along the y axis
   //!
	void Set(double x_in, double y_in);


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
   //! Multiplies this vector by a supplied scalar value
   //! \param k - A scalar value
   //!
   //! \return The resulting 2D vector
   //!
   Vect2 operator *(const double k) const;

   //=========================================================
   //! Divides this vector by a supplied scalar value
   //! \param k - A scalar value
   //!
   //! \return The resulting 2D vector
   //!
   Vect2 operator /(const double k) const;

   //=========================================================
   //! Adds this vector and a supplied 2D vector
   //! \param v - The supplied 2D vector
   //!
   //! \return The resulting 2D vector
   //!
	Vect2 operator +(const Vect2 &v) const;

   //=========================================================
   //! Subtracts a supplied 2D vector from this vector 
   //! \param v - The supplied 2D vector
   //!
   //! \return The resulting 2D vector
   //!
	Vect2 operator -(const Vect2 &v) const;

   //=========================================================
   //! Adds this vector and a supplied 3D vector
   //!  This performs as if the 3D vector is a 2D vector
   //! \param v - The supplied 3D vector
   //!
   //! \return The resulting 3D vector
   //!
  	Vect2 operator +(const Vect3 &v) const;

   //=========================================================
   //! Subtracts a supplied 3D vector from this vector 
   //!  This performs as if the 3D vector is a 2D vector
   //! \param v - The supplied 3D vector
   //!
   //! \return The resulting 2D vector
   //!
	Vect2 operator -(const Vect3 &v) const;

   //=========================================================
   //! Determines a psuedo-cross product of this vector and 
   //!  a supplied 2D vector.
   //!  the cross product is only used to determine
   //!  if a point is inside a vector
   //!  i.e. if the vector from the first vertex to the test point
   //!  is counter-clockwise from the vector from the virst vertex
   //!  to the second vertex
   //! \param v - The supplied 2D vector
   //!
   //! \return The sweep between this vector and the supplied vector 
   //!  is counter-clockwise (true) or clockwise (false) 
   //!
   bool operator ^(const Vect2 &v) const;

   //=========================================================
   //! Determines a psuedo-cross product of this vector and 
   //!  a supplied 3D vector.
   //!  The 3D vector is treated as a 2D vector
   //!  the cross product is only used to determine
   //!  if a point is inside a vector
   //!  i.e. if the vector from the first vertex to the test point
   //!  is counter-clockwise from the vector from the virst vertex
   //!  to the second vertex
   //! \param v - The supplied 3D vector
   //!
   //! \return The sweep between this vector and the supplied vector 
   //!  is counter-clockwise (true) or clockwise (false) 
   //!
   bool operator ^(const Vect3 &v) const;  // Cross product

   //=========================================================
   //! Determines the Dot product of this vector and a supplied 
   //!  2D vector
   //! \param v - The supplied 2D vector
   //!
   //! \return The Dot product of the two vectors
   //!
   double operator *(const Vect2 &v) const;   // Dot product

   //=========================================================
   //! Determines the Dot product of this vector and a supplied 
   //!  3D vector.
   //!  The 3D vector is treated as a 2D vector.
   //! \param v - The supplied 3D vector
   //!
   //! \return The Dot product of the two vectors
   //!
   double operator *(const Vect3 &v) const;   // Dot product

   //=========================================================
   //! Determines the angle between this vector and the 
   //!  supplied vector in radians
   //! \param v - The supplied 2D vector
   //!
   //! \return This returns the angle between vectors in radians 
   //!
	double angle(Vect2 &v) const;

   //=========================================================
   //! Determines the angle between this vector and the 
   //!  supplied vector in radians
   //!  The 3D vector is treated as a 2D vector.
   //! \param v - The supplied 3D vector
   //!
   //! \return This returns the angle between vectors in radians 
   //!
	double angle(Vect3 &v) const;

   //=========================================================
   //! Determines the angle between this vector and the 
   //!  supplied vector in degrees
   //! \param v - The supplied 2D vector
   //!
   //! \return This returns the angle between vectors in degrees 
   //!
   double angleD(Vect2 &v) const { return(angle(v) * (180.0/3.1415926)); }

   //=========================================================
   //! Determines the angle between this vector and the 
   //!  supplied vector in degrees
   //!  The 3D vector is treated as a 2D vector.
   //! \param v - The supplied 3D vector
   //!
   //! \return This returns the angle between vectors in degrees 
   //!
	double angleD(Vect3 &v) const { return(angle(v) * (180.0/3.1415926)); }

   //=========================================================
   //! Determines the unit vector of this vector 
   //! \return The unit vector of this vector 
   //!
	Vect2 Unit(void) const;

   //=========================================================
   //! Rotates this vector counter-clockwise by the supplied 
   //!  angle in radians
   //! \param angle - The supplied angle in radians
   //!
   //! \return The resulting 2D vector
   //!
	Vect2 Rotate(double angle) const;

   //=========================================================
   //! Determines negative of this vector
   //! \return The negative of this vector
   //!
	Vect2 Neg(void) const;

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

	inline double & operator[]( int i ) { return vect[i]; }
	inline double operator[]( int i ) const { return vect[i]; }


protected:

	//=========================================================
	//! The array of values encapsulated by this class
	//!
	double vect[2];

};

}

#endif
