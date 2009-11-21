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
 *  FILENAME:   Vect2.cpp
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

#include "Vect2.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


using namespace mpv;

Vect2::Vect2()
{
	vect[0] = 0.0;
	vect[1] = 0.0;
}


Vect2::Vect2(double x_in, double y_in)
{
	vect[0] = x_in;
	vect[1] = y_in;
}


// ================================================
// Vect2(const Vect3 &v)
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Vect2::Vect2(const Vect3 &v)
{
	vect[0] = v[0];
	vect[1] = v[1];
}


Vect2::~Vect2()
{
}


// ================================================
// operator =(const Vect3 &v)
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Vect2 & Vect2::operator =(const Vect3 &v)
{
	vect[0] = v[0];
	vect[1] = v[1];

	return(*this);
}


void Vect2::Set(double x_in, double y_in)
{
	vect[0] = x_in;
	vect[1] = y_in;
}


// ================================================
// mag
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
double Vect2::mag() const
{
	return sqrt( vect[0]*vect[0] + vect[1]*vect[1] );
}


// ================================================
// mag2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
double Vect2::mag2() const
{
	return( vect[0]*vect[0] + vect[1]*vect[1] );
}



// ================================================
// operator *(const double k)
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Vect2 Vect2::operator *(const double k) const
{

   double rx,ry;

   rx = vect[0] * k;
   ry = vect[1] * k;

   return(Vect2(rx,ry));

}


// ================================================
// operator /(const double k)
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Vect2 Vect2::operator /(const double k) const
{

   double rx,ry;

   rx = vect[0] / k;
   ry = vect[1] / k;

   return(Vect2(rx,ry));

}


// ================================================
// operator +(const Vect2 &v)
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Vect2 Vect2::operator +(const Vect2 &v) const
{

   double rx,ry;

   rx = vect[0] + v.vect[0];
   ry = vect[1] + v.vect[1];

   return(Vect2(rx,ry));

}


// ================================================
// operator +(const Vect3 &v)
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Vect2 Vect2::operator +(const Vect3 &v) const
{

   double rx,ry;

   rx = vect[0] + v[0];
   ry = vect[1] + v[1];

   return(Vect2(rx,ry));

}


// ================================================
// operator -(const Vect2 &v)
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Vect2 Vect2::operator -(const Vect2 &v) const
{

   double rx,ry;

   rx = vect[0] - v.vect[0];
   ry = vect[1] - v.vect[1];

   return(Vect2(rx,ry));

}


// ================================================
// operator -(const Vect3 &v)
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Vect2 Vect2::operator -(const Vect3 &v) const
{

   double rx,ry;

   rx = vect[0] - v[0];
   ry = vect[1] - v[1];

   return(Vect2(rx,ry));

}


// ================================================
// Cross product - operator ^(const Vect2 &v)
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
bool Vect2::operator ^(const Vect2 &v) const
{
   bool stat;

   if((vect[0]*v.vect[1] - vect[1]*v.vect[0]) >= 0.0)
      stat = true;
   else
      stat = false;

   return(stat);

}


// ================================================
// Cross product - operator ^(const Vect3 &v)
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
bool Vect2::operator ^(const Vect3 &v) const
{
   bool stat;

   if((vect[0]*v[1] - vect[1]*v[0]) >= 0.0)
      stat = true;
   else
      stat = false;

   return(stat);

}


// ================================================
// Dot product - operator *(const Vect2 &v)
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
double Vect2::operator *(const Vect2 &v) const
{
	return( vect[0]*v.vect[0] + vect[1]*v.vect[1] );
}


// ================================================
// Dot product - operator *(const Vect3 &v)
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
double Vect2::operator *(const Vect3 &v) const
{
	return( vect[0]*v[0] + vect[1]*v[1] );
}


// ================================================
// angle(Vect2 &v)
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
double Vect2::angle(Vect2 &v) const
{
   return((double)acos((*this*v)/(mag()*v.mag())));
}


// ================================================
// angle(Vect3 &v)
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
double Vect2::angle(Vect3 &v) const
{
   Vect2 p = v;
   return((double)acos((*this*p)/(mag()*p.mag())));
}


// ================================================
// Unit vector
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Vect2 Vect2::Unit() const
{

   double rx,ry,mg;

   mg = mag();
   if( mg != 0 )
   {
      rx = vect[0] / mg;
      ry = vect[1] / mg;
   }
   else
      rx = ry = 0.0;

   return(Vect2(rx,ry));

}


// ================================================
// Rotate
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Vect2 Vect2::Rotate(double angle) const
{

   double C = cos(angle);  // cosine angle
   double S = sin(angle);  // sin angle

   double rx, ry;

   rx = (vect[0] * C) - (vect[1] * S);
   ry = (vect[1] * C) + (vect[0] * S);


   return(Vect2(rx,ry));

}


// ================================================
// Neg
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Vect2 Vect2::Neg() const
{
	return Vect2( -vect[0], -vect[1] );
}
