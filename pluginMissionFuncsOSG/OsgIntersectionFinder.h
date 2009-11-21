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

#ifndef OSG_INTERSECTION_FINDER_H
#define OSG_INTERSECTION_FINDER_H

#include <vector>

#include <osg/Node>
#include <osgUtil/IntersectVisitor>

#include "IntersectionFinder.h"

namespace SG
{

using namespace std;

//========================================================================
// OsgIntersection.
//========================================================================

struct OsgIntersection : public ISECT::Intersection
{
   osgUtil::Hit hit;
};

//========================================================================
// OsgIntersectionFinder.
//========================================================================

class OsgIntersectionFinder : public ISECT::IntersectionFinder
{
public:
   OsgIntersectionFinder(osg::Node *top_node);
   virtual ~OsgIntersectionFinder() { }

   virtual void find_intersections_along_segment(
         vector<ISECT::Intersection> &intersections,
         ISECT::LineSeg const &ls) const;
   virtual void find_intersections_along_segment(
         vector<OsgIntersection> &intersections,
         ISECT::LineSeg const &ls) const;

   virtual void find_intersections_along_direction(
         vector<ISECT::Intersection> &intersections,
         ISECT::Point const &a,
         ISECT::Vector const &dir,
         double max_range=0) const;
   virtual void find_intersections_along_direction(
         vector<OsgIntersection> &intersections,
         ISECT::Point const &a,
         ISECT::Vector const &dir,
         double max_range=0) const;

   ///
   /// Add a node name to the exclusion list.  On every intersection
   /// the nodepath is examined which is the list of nodes from
   /// the root of the subgraph all the way down to the node that
   /// was intersected.  If any of these nodenames are identical to
   /// a name in the exclusion list this Intersection will be
   /// removed from the returned results.
   ///
   void add_nodename_exclusion(std::string const &name)
   {
      exclusion_list.insert(name);
   }

   ///
   /// Exclude backfacing polygons along the path of the test
   /// line segment.
   ///
   void exclude_backfacing_polygons(bool yes_or_no)
   {
      exclude_backfacing_polys = yes_or_no;
   }

#if 0
   ///
   /// Set a minimum alpha which must exist for an
   /// intersection to be included in the returns.
   ///@param alpha A normalized value from [0.0 - 1.0] describing alpha.
   ///
   void set_minimum_alpha(double alpha)
   {
      minimum_alpha = alpha;
   }
#endif


private:
   osg::ref_ptr<osg::Node> top_node;
   std::set<std::string> exclusion_list;
   bool exclude_backfacing_polys;
   //double minimum_alpha;

   void populate_hit_list(osgUtil::IntersectVisitor::HitList &hits, osg::LineSegment *ls) const;
   void remove_hits_by_nodename(osgUtil::IntersectVisitor::HitList &hits, std::set<std::string> const &node_names) const;
   void remove_backfacing_polygons(osgUtil::IntersectVisitor::HitList &hits, osg::Vec3 const &line) const;
   //void remove_hits_by_alpha(osgUtil::IntersectVisitor::HitList &hits) const;
};

}; // end namespace SG

#endif
