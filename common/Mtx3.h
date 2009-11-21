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
 *  FILENAME:   Mtx3.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *   manage a 3x3 matrix.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  04/04/2004 Greg Basler                       MPV_CR_DR_1
 *  Initial Release.
 *  
 *  2008-08-30 Andrew Sampson
 *      Renamed class to Mtx3.  Moved class to the mpv namespace.
 *  
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#if !defined(_MATRIX_3_INCLUDED_)
#define _MATRIX_3_INCLUDED_

#include "MPVCommonTypes.h"

namespace mpv
{

class Vect3;

//=========================================================
//! The class encapsulates a 3x3 matrix.
//!
class MPVCMN_SPEC Mtx3  
{
public:


   //==> Management

   //=========================================================
   //! General Constructor
   //!
	Mtx3();

   //=========================================================
   //! General Destructor
   //!
	virtual ~Mtx3();



   //==> Basic matrix functionality


   //=========================================================
   //! Set the matrix using an array arranged in column major form
   //! \param matrix - The matrix (double float) array arranged
   //!    in column major form.
   //!
   //! \return A success/fail flag
   //!
	bool pcset(double *matrix);

   //=========================================================
   //! Set the matrix using an array arranged in row major form
   //! \param matrix - The matrix (double float) array arranged
   //!    in row major form.
   //!
   //! \return A success/fail flag
   //!
	bool prset(double *matrix);

   //=========================================================
   //! Set the matrix using an array arranged in column major form
   //! \param matrix - The matrix (double float) array arranged
   //!    in column major form.
   //!
   //! \return A success/fail flag
   //!
	bool pcset(float *matrix);

   //=========================================================
   //! Set the matrix using an array arranged in row major form
   //! \param matrix - The matrix (double float) array arranged
   //!    in row major form.
   //!
   //! \return A success/fail flag
   //!
	bool prset(float *matrix);

   //=========================================================
   //! A matrix multiply with another 3x3 matrix
   //! \param mtx - The other 3x3 matrix.
   //!
   //! \return The resulting 3x3 matrix
   //!
	Mtx3 operator *(Mtx3 &mtx);

   //=========================================================
   //! A matrix multiply with a 3D vector
   //! \param hv - The 3D vector.
   //!
   //! \return The resulting 3D vector
   //!
	Vect3 operator *(Vect3 &hv);
 
   //=========================================================
   //! Get the matrix
   //! \return A pointer to the matrix as an array of double
   //!   in row major form.
   //!
  double * GetM(void) { return((double *)m); }
 
   //=========================================================
   //! Get transpose of this matrix
   //! \return The transpose matrix
   //!
	Mtx3 T(void);

   //=========================================================
   //! Create a rotation matrix
   //! \param yaw - The yaw rotation angle
   //! \param pitch - The pitch rotation angle
   //! \param roll - The roll rotation angle
   //!
   //! \return A success/fail flag
   //!
	bool GenRot(double yaw, double pitch, double roll);

   //=========================================================
   //! Get the rotation angles from the matrix
   //! \param yaw - The yaw rotation angle
   //! \param pitch - The pitch rotation angle
   //! \param roll - The roll rotation angle
   //!
   //! \return A success/fail flag
   //!
	bool GetRot(double &yaw, double &pitch, double &roll);


protected:

   //=========================================================
   //! The 3x3 matrix itself.
   //! The matrix is stored row major - i.e. m[row][column]
   //!
   double m[3][3];

};

}

#include "Vect3.h"

#endif
