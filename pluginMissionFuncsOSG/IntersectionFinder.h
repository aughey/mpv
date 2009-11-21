//////////////////////////////////////////////////////////////////////////
//
//  $Workfile$
//
//////////////////////////////////////////////////////////////////////////

/** <pre>
 * Copyright (c) 2007 General Dynamics Land Systems
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef INTERSECTION_FINDER_H
#define INTERSECTION_FINDER_H

#include <vector>

namespace ISECT
{

using namespace std;

//========================================================================
//  Point.
/// A point abstraction for use with an IntersectionFinder.
//========================================================================

struct Point
{
   Point() { x=0.0; y=0.0; z=0.0; }
   Point(double x, double y, double z) : x(x), y(y), z(z) { }
   void set(double x, double y, double z)
   {
      this->x=x; this->y=y; this->z=z;
   }

   double x, y, z;
};

//========================================================================
//  LineSeg.
/// A line segment abstraction for use with an IntersectionFinder.
//========================================================================

struct LineSeg
{
   LineSeg() { }
   LineSeg(Point const &start, Point const &end) : start(start), end(end) {}

   Point start;
   Point end;
};

//========================================================================
//  Vector.
/// A vector abstraction for use with an IntersectionFinder.
//========================================================================

struct Vector
{
   Vector() { dx=0.0; dy=0.0; dz=0.0; }
   Vector(double dx, double dy, double dz) : dx(dx), dy(dy), dz(dz) { }
   void set(double dx, double dy, double dz)
   {
      this->dx=dx; this->dy=dy; this->dz=dz;
   }

   double dx, dy, dz;
};

//========================================================================
//  Intersection.
/// A container class describing intersections.
//========================================================================

struct Intersection
{
   Point pt;
};

//========================================================================
//  IntersectionFinder.
/// An abstract interface for finding intersections in a scenegraph.
//========================================================================

class IntersectionFinder
{
   public:
      virtual ~IntersectionFinder() { }

      virtual void find_intersections_along_segment(
            vector<Intersection> &i,
            LineSeg const &LS) const = 0;

      virtual void find_intersections_along_direction(
            vector<Intersection> &i,
            Point const &A,
            Vector const &dir,
            double max_range = 0) const = 0;
};

}; // end namespace ISECT

#endif
