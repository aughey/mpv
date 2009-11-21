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
 *  FILENAME:   Mtx4.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *   manage a 4x4 matrix.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  04/21/2005 Greg Basler                       MPV_CR_DR_1
 *  Initial Release.
 *  
 *  2008-08-30 Andrew Sampson
 *      Renamed class to Mtx4.  Moved class to the mpv namespace.
 *  
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#if !defined(_MATRIX_4_INCLUDED_)
#define _MATRIX_4_INCLUDED_

#include "MPVCommonTypes.h"

namespace mpv
{

class Vect3;
class Vect4;

//=========================================================
//! The class encapsulates a 4x4 matrix.
//!
class MPVCMN_SPEC Mtx4  
{
public:
   //==> Management

   //=========================================================
   //! General Constructor
   //!
	Mtx4();

   //=========================================================
   //! General Destructor
   //!
	virtual ~Mtx4();


   //==> Basic matrix functionality

	void setIdentity();
	
	void setAsTranslate( const Vect3 &offset );

	//! arguments are in radians
	void setAsRotate( double yawRad, double pitchRad, double rollRad );

	void setAsRotateAboutXAxis( double pitch );
	void setAsRotateAboutYAxis( double roll );
	void setAsRotateAboutZAxis( double yaw );

   //=========================================================
   //! Set the matrix using an array arranged in column major form
   //! \param matrix - The matrix (double float) array arranged
   //!    in column major form.
   //!
   //! \return A success/fail flag
   //!
	bool pcset(const double *matrix);

   //=========================================================
   //! Set the matrix using an array arranged in row major form
   //! \param matrix - The matrix (double float) array arranged
   //!    in row major form.
   //!
   //! \return A success/fail flag
   //!
	bool prset(const double *matrix);

   //=========================================================
   //! A matrix multiply with another 4x4 matrix
   //! \param mtx - The other 4x4 matrix.
   //!
   //! \return The resulting 4x4 matrix
   //!
	Mtx4 operator *(const Mtx4 &mtx) const;

	//=========================================================
	//! A matrix multiply with a 3D vector
	//! \param v - The 3D vector.
	//!
	//! \return The resulting 3D vector
	//!
	Vect3 operator *(const Vect3 &v) const;

   //=========================================================
   //! A matrix multiply with a 4D vector
   //! \param hv - The 4D vector.
   //!
   //! \return The resulting 4D vector
   //!
	Vect4 operator *(const Vect4 &hv) const;
	
	//! Create a matrix that is the transpose of this matrix
	//! \return a transposed copy of this matrix
	Mtx4 transpose() const;
 
   //=========================================================
   //! Get the underlying matrix array
   //! \return A pointer to the matrix as an array of double
   //!   in row major form.
   //!
   double * GetM() { return((double *)m); }

   //=========================================================
   //! Get the underlying matrix array (const version)
   //! \return A pointer to the matrix as an array of double
   //!   in row major form.
   //!
   const double * GetM() const { return((double *)m); }

protected:
   //=========================================================
   //! The 4x4 matrix itself.
   //! The matrix is stored row major - i.e. m[row][column]
   //!
   double m[4][4];

};

}

#include "Vect3.h"
#include "Vect4.h"

#endif
