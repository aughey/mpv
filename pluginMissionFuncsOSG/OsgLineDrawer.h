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

#ifndef OSG_LINE_DRAWER_H
#define OSG_LINE_DRAWER_H

#include <osg/Group>
#include <osg/Geometry>
#include <osg/LineSegment>
#include <osg/Vec4>

namespace SG
{

using namespace std;

//========================================================================
// LineDrawer.
//========================================================================

class LineDrawer 
{
public:
   LineDrawer(osg::Group *top_node);
   ~LineDrawer();

   ///
   /// Draw a LineSegment with a particular color.
   ///@return an integer describing the line number so it can be removed later.
   ///
   unsigned int draw_line(const osg::LineSegment *ls, const osg::Vec4 &color);

   ///
   /// Remove a LineSegment previously drawn.
   ///
   void remove_line(unsigned int id);

   ///
   /// Clear all drawed lines.
   ///
   void clear_lines();

private:
   unsigned int last_id;
   osg::ref_ptr<osg::Group> top_node;
   osg::ref_ptr<osg::Group> symbology3d;

   std::map< unsigned int, osg::ref_ptr<osg::Geode> > lase_lines;
   std::map< unsigned int, osg::ref_ptr<osg::Geometry> > geometries_by_line;
   std::map< unsigned int, osg::ref_ptr<osg::Vec3Array> > line_vertices_by_line;
};

}; // end namespace SG

#endif
