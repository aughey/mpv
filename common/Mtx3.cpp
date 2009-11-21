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
 *  FILENAME:   Mtx3.cpp
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

#define _USE_MATH_DEFINES

#include <math.h>

#define d2r (M_PI/180.0)
#define r2d (180.0/M_PI)

#include "Mtx3.h"


using namespace mpv;

// ================================================
// Mtx3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Mtx3::Mtx3()
{

}


// ================================================
// ~Mtx3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Mtx3::~Mtx3()
{

}


// ================================================
// pcset - double
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
bool Mtx3::pcset(double *matrix)
{

   int c;
   int r;
   double *mbr;

   mbr = matrix;

   for(c=0;c<3;c++)
   {
      for(r=0;r<3;r++)
      {
         m[r][c] = *mbr++;
      }
   }

   return(true);

}


// ================================================
// prset - double
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
bool Mtx3::prset(double *matrix)
{

   int c;
   int r;
   double *mbr;

   mbr = matrix;

   for(r=0;r<3;r++)
   {
      for(c=0;c<3;c++)
      {
         m[r][c] = *mbr++;
      }
   }

   return(true);

}



// ================================================
// pcset - float
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
bool Mtx3::pcset(float *matrix)
{

   int c;
   int r;
   float *mbr;

   mbr = matrix;

   for(c=0;c<3;c++)
   {
      for(r=0;r<3;r++)
      {
         m[r][c] = (double)*mbr++;
      }
   }

   return(true);

}


// ================================================
// prset - float
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
bool Mtx3::prset(float *matrix)
{

   int c;
   int r;
   float *mbr;

   mbr = matrix;

   for(r=0;r<3;r++)
   {
      for(c=0;c<3;c++)
      {
         m[r][c] = (double)*mbr++;
      }
   }

   return(true);

}


// ================================================
// operator *(Mtx3 &mtx)
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Mtx3 Mtx3::operator *(Mtx3 &mtx)
{

   int c;
   int r;
   Mtx3 rslt;


   for(r=0;r<3;r++)
   {
      for(c=0;c<3;c++)
      {
         rslt.m[r][c] = (m[r][0] * mtx.m[0][c]) +
                        (m[r][1] * mtx.m[1][c]) +
                        (m[r][2] * mtx.m[2][c]);
      }
   }

   return(rslt);

}


// ================================================
// operator *(Vect3 &hv)
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Vect3 Mtx3::operator *(Vect3 &hv)
{

   double v[3];
   double rslt[3];
   int r;

   v[0] = hv.getx();
   v[1] = hv.gety();
   v[2] = hv.getz();

   for(r=0;r<3;r++)
   {
      rslt[r] = (m[r][0] * v[0]) +
                (m[r][1] * v[1]) +
                (m[r][2] * v[2]);
   }

   return(Vect3(rslt[0],rslt[1],rslt[2]));

}


// ================================================
// T   -  transpose
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Mtx3 Mtx3::T()
{

   Mtx3 Rslt;

   for(int r=0;r<3;r++)
   {
      for(int c=0;c<3;c++)
      {
         Rslt.m[c][r] = m[r][c];
      }
   }

   return(Rslt);

}


// ================================================
// GenRot
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
bool Mtx3::GenRot(double yaw, double pitch, double roll)
{

   yaw = d2r * yaw;
   pitch = d2r * pitch;
   roll = d2r * roll;

   double cy = cos(yaw);
   double sy = sin(yaw);
   double cp = cos(pitch);
   double sp = sin(pitch);
   double cr = cos(roll);
   double sr = sin(roll);

   m[0][0] = cy * cp;
   m[0][1] = (cy*sp*sr)-(sy*cr);
   m[0][2] = (sy*sr)+(cy*sp*cr);
   m[1][0] = sy*cp;
   m[1][1] = (cy*cr)+(sy*sp*sr);
   m[1][2] = (sy*sp*cr)-(cy*sr);
   m[2][0] = -sp;
   m[2][1] = cp*sr;
   m[2][2] = cp*cr;

   return(true);
}


// ================================================
// GetRot
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
bool Mtx3::GetRot(double &yaw, double &pitch, double &roll)
{

   yaw = atan2(m[1][0],m[0][0]);
   roll = atan2(m[2][1],m[2][2]);
   pitch = atan2(-m[2][0],(m[0][0]/cos(yaw)));

   yaw = r2d * yaw;
   pitch = r2d * pitch;
   roll = r2d * roll;

   while(pitch > 180.0)
   {
      pitch -= 360.0;
   }

   while(pitch < -180.0)
   {
      pitch += 360.0;
   }

   if(pitch > 90.0)
   {
      pitch = 180.0 - pitch;
      yaw += 180.0;
      roll += 180.0;
   }
   else if(pitch < -90.0)
   {
      pitch = -180.0 - pitch;
      yaw += 180.0;
      roll += 180.0;
   }

   while(yaw > 360.0)
   {
      yaw -= 360.0;
   }

   while(yaw < 0.0)
   {
      yaw += 360.0;
   }

   while(roll > 180.0)
   {
      roll -= 360.0;
   }

   while(roll < -180.0)
   {
      roll += 360.0;
   }

   return(true);
}

