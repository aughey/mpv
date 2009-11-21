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

#include "OsgIntersectionFinder.h"

#include <osg/Geometry>
#include <osg/io_utils>

#include <cmath>
#include <iostream>
#include <vector>

namespace SG
{

using namespace std;

//========================================================================
// OsgIntersectionFinder.
//========================================================================
//------------------------------------------------------------------------
// OsgIntersectionFinder.
//------------------------------------------------------------------------

OsgIntersectionFinder::OsgIntersectionFinder(osg::Node* top_node) : top_node(top_node)
{
   exclude_backfacing_polys = false;
   //minimum_alpha = 0.0;
}

//------------------------------------------------------------------------
// find_intersections_along_segment.
//------------------------------------------------------------------------

void OsgIntersectionFinder::find_intersections_along_segment(
      vector<ISECT::Intersection> &intersections,
      ISECT::LineSeg const &ls) const
{
   vector<OsgIntersection> ois;
   find_intersections_along_segment(ois, ls);
   for(unsigned int i=0; i<ois.size(); ++i)
   {
      intersections.push_back(ois[i]);
   }
}

//------------------------------------------------------------------------

void OsgIntersectionFinder::find_intersections_along_segment(
      vector<OsgIntersection> &intersections,
      ISECT::LineSeg const &ls) const
{
   osg::Vec3 const start(ls.start.x, ls.start.y, ls.start.z);
   osg::ref_ptr<osg::LineSegment> ols = new osg::LineSegment(
         start,
         osg::Vec3(ls.end.x, ls.end.y, ls.end.z));

   //std::cout << "test_segment = " << ols->start() << " ... " << ols->end() << std::endl;

   osgUtil::IntersectVisitor::HitList hits;
   populate_hit_list(hits, ols.get());

   if ( !hits.empty() )
   {
      // First remove nodes by name
      remove_hits_by_nodename(hits, exclusion_list);

      // If enabled, remove backfacing polygons
      if(exclude_backfacing_polys)
      {
         remove_backfacing_polygons(hits, ols->end() - ols->start());
      }

      // Remove any hits that are below the minimum alpha.
      //remove_hits_by_alpha(hits);

      // Add any remaining intersections
      for (osgUtil::IntersectVisitor::HitList::iterator hitr=hits.begin(); hitr != hits.end(); ++hitr)
      {
         OsgIntersection osg_intersection;
         osg_intersection.hit = *hitr;

         osg::Vec3 ipt = (*hitr).getWorldIntersectPoint();
         osg_intersection.pt = ISECT::Point(ipt.x(),
                                            ipt.y(),
                                            ipt.z());

         intersections.push_back(osg_intersection);
      }
   }
   return;
}

//------------------------------------------------------------------------
// find_intersections_along_direction.
//------------------------------------------------------------------------

void OsgIntersectionFinder::find_intersections_along_direction(
      vector<ISECT::Intersection> &intersections,
      ISECT::Point const &a,
      ISECT::Vector const &dir,
      double max_range) const
{
   vector<OsgIntersection> ois;
   find_intersections_along_direction(ois, a, dir, max_range);
   for(unsigned int i=0; i<ois.size(); ++i)
   {
      intersections.push_back(ois[i]);
   }
}

//------------------------------------------------------------------------

void OsgIntersectionFinder::find_intersections_along_direction(
      vector<OsgIntersection> &intersections,
      ISECT::Point const &a,
      ISECT::Vector const &dir,
      double max_range) const
{
   if(max_range == 0)
   {
      max_range = 9999999999999999.0;
   }

   ISECT::Point b(
         a.x + (dir.dx * max_range),
         a.y + (dir.dy * max_range),
         a.z + (dir.dz * max_range));
   
   find_intersections_along_segment(intersections, ISECT::LineSeg(a, b));
}

//------------------------------------------------------------------------
// populate_hit_list.
//------------------------------------------------------------------------

void OsgIntersectionFinder::populate_hit_list(
      osgUtil::IntersectVisitor::HitList &hits,
      osg::LineSegment *ls) const
{
   osgUtil::IntersectVisitor visitor;
   visitor.addLineSegment(ls);
   top_node->accept(visitor);

   hits = visitor.getHitList(ls);
}

//------------------------------------------------------------------------
// remove_backfacing_polygons.
//------------------------------------------------------------------------

void OsgIntersectionFinder::remove_backfacing_polygons(
      osgUtil::IntersectVisitor::HitList &hits,
      osg::Vec3 const &line) const
{
   osgUtil::IntersectVisitor::HitList::iterator itr;
   for(itr = hits.begin(); itr != hits.end(); ++itr)
   {
      osg::Vec3 normal = itr->getWorldIntersectNormal();
      if(normal * line > 0)
      {
         hits.erase(itr);
         --itr;
      }
   }
}

//------------------------------------------------------------------------
// remove_hits_by_alpha.
//------------------------------------------------------------------------
#if 0

void OsgIntersectionFinder::remove_hits_by_alpha( osgUtil::IntersectVisitor::HitList &hits) const
{
   if(minimum_alpha <= 0.0) return;

   osgUtil::IntersectVisitor::HitList::iterator itr;
   for(itr = hits.begin(); itr != hits.end(); ++itr)
   {
      osg::Drawable *drawable = (*itr).getDrawable();
      osg::Geometry *geometry = drawable->asGeometry();
      if(geometry)
      {
         osg::Vec4Array *colors = dynamic_cast<osg::Vec4Array*> (geometry->getColorArray());
         if(colors)
         {
            std::cout << "colors.size() = " << colors->size() << std::endl;
            if(colors->size() > 0)
            {
               osg::Vec4f const &color = colors->operator [](0);
               std::cout << "RGB = " << color.r() << " " << color.g() << " " << color.b() << std::endl;
               std::cout << "Alpha = " << color.a() << std::endl;
            }
         }
      }
      else
      {
         std::cout << "not a geometry" << std::endl;
      }
   }
}
#endif

//------------------------------------------------------------------------
// remove_hits_by_nodename.
//------------------------------------------------------------------------
      
void OsgIntersectionFinder::remove_hits_by_nodename(
      osgUtil::IntersectVisitor::HitList &hits, std::set<std::string> const &node_names) const
{
   // For every hit...
   bool match = false;
   osgUtil::IntersectVisitor::HitList::iterator itr = hits.begin();
   while(itr != hits.end())
   {
   	match = false;
   	
      osg::NodePath &np = itr->getNodePath();

      // Examine every node in the node path for this hit...
      osg::NodePath::iterator np_itr;
      for(np_itr = np.begin(); np_itr != np.end(); ++np_itr)
      {
         // Check to see if it matches one of our nodenames
         // we want to ignore.
         if(node_names.find( (*np_itr)->getName() ) != node_names.end())
         {
         	match = true;
            break;
         }
      }
      
    	// If it matches, remove this hit from the hitlist
    	//   otherwise just go to the next hit
      if(match)
         itr = hits.erase(itr);
      else
      	itr++;
   }
}

//------------------------------------------------------------------------

}; // end namespace SG

