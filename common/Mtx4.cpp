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
 *  FILENAME:   Mtx4.h
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

#include <math.h>
#include <string.h> // for memset

#include "Mtx4.h"


using namespace mpv;

// ================================================
// Mtx4
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Mtx4::Mtx4()
{
	setIdentity();
}

// ================================================
// ~Mtx4
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Mtx4::~Mtx4()
{

}

void Mtx4::setIdentity()
{
	memset( m, 0, sizeof(m) );
	for( int i = 0; i < 4; i++ )
		m[i][i] = 1.0;
}


void Mtx4::setAsTranslate( const Vect3 &offset )
{
	setIdentity();
	m[0][3] = offset[0];
	m[1][3] = offset[1];
	m[2][3] = offset[2];
}


void Mtx4::setAsRotate( double yaw, double pitch, double roll )
{
	double cosYaw = cos( yaw );
	double sinYaw = sin( yaw );
	
	double cosPitch = cos( pitch );
	double sinPitch = sin( pitch );
	
	double cosRoll = cos( roll );
	double sinRoll = sin( roll );
	
	setIdentity();

	m[0][0] = cosYaw * cosRoll - sinYaw * sinPitch * sinRoll;
	m[0][1] = -1. * sinYaw * cosPitch;
	m[0][2] = cosYaw * sinRoll + sinYaw * sinPitch * cosRoll;
	m[1][0] = sinYaw * cosRoll + cosYaw * sinPitch * sinRoll;
	m[1][1] = cosYaw * cosPitch;
	m[1][2] = sinYaw * sinRoll - cosYaw * sinPitch * cosRoll;
	m[2][0] = -1. * cosPitch * sinRoll;
	m[2][1] = sinPitch;
	m[2][2] = cosPitch * cosRoll;
}

/*
void Mtx4::setAsRotate( double yaw, double pitch, double roll )
{
	Mtx4 yawMtx, pitchMtx, rollMtx;
	
	yawMtx.setAsRotateAboutZAxis( yaw );
	pitchMtx.setAsRotateAboutXAxis( pitch );
	rollMtx.setAsRotateAboutYAxis( roll );
	
	Mtx4 tempMtx = yawMtx * pitchMtx * rollMtx;
	prset( (double*)tempMtx.m );
}
*/

void Mtx4::setAsRotateAboutXAxis( double pitch )
{
	double cosPitch = cos( pitch );
	double sinPitch = sin( pitch );
	
	setIdentity();

	m[1][1] = cosPitch;
	m[1][2] = -1. * sinPitch;
	m[2][1] = sinPitch;
	m[2][2] = cosPitch;
}


void Mtx4::setAsRotateAboutYAxis( double roll )
{
	double cosRoll = cos( roll );
	double sinRoll = sin( roll );
	
	setIdentity();

	m[0][0] = cosRoll;
	m[0][2] = sinRoll;
	m[2][0] = -1. * sinRoll;
	m[2][2] = cosRoll;
}


void Mtx4::setAsRotateAboutZAxis( double yaw )
{
	double cosYaw = cos( yaw );
	double sinYaw = sin( yaw );
	
	setIdentity();

	m[0][0] = cosYaw;
	m[0][1] = -1. * sinYaw;
	m[1][0] = sinYaw;
	m[1][1] = cosYaw;
}


// ================================================
// pcset - double
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
bool Mtx4::pcset(const double *matrix)
{
   // Set the matrix using a pointer to column major data

   int c;
   int r;

   for(c=0;c<4;c++)
   {
      for(r=0;r<4;r++)
      {
         m[r][c] = *matrix++;
      }
   }

   return(true);

}

// ================================================
// prset - double
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
bool Mtx4::prset(const double *matrix)
{
   // Set the matrix using a pointer to row major data

   int c;
   int r;

   for(r=0;r<4;r++)
   {
      for(c=0;c<4;c++)
      {
         m[r][c] = *matrix++;
      }
   }

   return(true);

}


// ================================================
// operator *(Mtx4 &mtx)
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Mtx4 Mtx4::operator *(const Mtx4 &mtx) const
{

   int c;
   int r;
   Mtx4 rslt;


   for(r=0;r<4;r++)
   {
      for(c=0;c<4;c++)
      {
         rslt.m[r][c] = (m[r][0] * mtx.m[0][c]) +
                        (m[r][1] * mtx.m[1][c]) +
                        (m[r][2] * mtx.m[2][c]) +
                        (m[r][3] * mtx.m[3][c]);
      }
   }

   return(rslt);

}



// ================================================
// operator *(Vect4 &hv)
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Vect4 Mtx4::operator *(const Vect4 &v) const
{
   Vect4 rslt;
   int r;

   for(r=0;r<4;r++)
   {
      rslt[r] = (m[r][0] * v[0]) +
                (m[r][1] * v[1]) +
                (m[r][2] * v[2]) +
                (m[r][3] * v[3]);
   }

   return rslt;
}


Vect3 Mtx4::operator *(const Vect3 &v) const
{
	Vect4 temp;
	int r;

	for(r=0;r<4;r++)
	{
		temp[r] = 
			(m[r][0] * v[0]) +
			(m[r][1] * v[1]) +
			(m[r][2] * v[2]) +
			(m[r][3] * 1.0);
	}

	return Vect3( 
		temp[0] / temp[3], 
		temp[1] / temp[3], 
		temp[2] / temp[3] );
}


// ================================================
// transpose
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Mtx4 Mtx4::transpose() const
{
	Mtx4 result;
	int r, c;
	
	// could just call result.pcset( m ) which would end up doing the same thing
	for(r=0;r<4;r++)
	{
		for(c=0;c<4;c++)
		{
			result.m[r][c] = m[c][r];
		}
	}
	
	return result;
}

