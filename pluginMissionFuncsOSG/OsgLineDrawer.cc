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

#include "OsgLineDrawer.h"

#include <osg/Geode>

namespace SG
{

using namespace std;

//========================================================================
// LineDrawer.
//========================================================================
//------------------------------------------------------------------------
// LineDrawer.
//------------------------------------------------------------------------

LineDrawer::LineDrawer(osg::Group* top_node) : top_node(top_node)
{
   last_id = 0;

   symbology3d = new osg::Group();
   symbology3d->setName("3D Symbology Node");
   symbology3d->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
   top_node->addChild(symbology3d.get());
}

//------------------------------------------------------------------------
// ~LineDrawer.
//------------------------------------------------------------------------

LineDrawer::~LineDrawer()
{
   clear_lines();
}

//------------------------------------------------------------------------
// clear_lines.
//------------------------------------------------------------------------

void LineDrawer::clear_lines()
{
   std::map< unsigned int, osg::ref_ptr<osg::Geode> >::iterator itr;
   for(itr = lase_lines.begin(); itr != lase_lines.end(); ++itr)
   {
      symbology3d->removeChild(itr->second.get());
   }
   top_node->removeChild(symbology3d.get());

   lase_lines.clear();
   geometries_by_line.clear();
   line_vertices_by_line.clear();
}

//------------------------------------------------------------------------
// draw_line.
//------------------------------------------------------------------------

unsigned int LineDrawer::draw_line(const osg::LineSegment *ls, const osg::Vec4 &color)
{
   ++last_id;

   // Create vertices
   osg::Vec3Array *line_vertices = new osg::Vec3Array();
   line_vertices->push_back(ls->start());
   line_vertices->push_back(ls->end());
   line_vertices_by_line[last_id] = line_vertices;
   // Create LINES
   osg::DrawElementsUInt *line_prim = new osg::DrawElementsUInt(osg::PrimitiveSet::LINES, 0);
   line_prim->push_back(0);
   line_prim->push_back(1);
   // Initialize colors
   osg::Vec4Array *colors = new osg::Vec4Array;
   colors->push_back( color );
   // Create geometry
   osg::Geometry *geometry = new osg::Geometry();
   geometry->setColorBinding(osg::Geometry::BIND_OVERALL);
   geometry->addPrimitiveSet(line_prim);
   geometry->setColorArray(colors);
   geometry->setVertexArray(line_vertices);
   geometries_by_line[last_id] = geometry;

   osg::Geode *lase_line = new osg::Geode();
   lase_line->addDrawable(geometry);
   lase_lines[last_id] = lase_line;

   symbology3d->addChild(lase_line);
   return last_id;
}

//------------------------------------------------------------------------
// remove_line.
//------------------------------------------------------------------------

void LineDrawer::remove_line(unsigned int id)
{
   std::map< unsigned int, osg::ref_ptr<osg::Geode> >::iterator itr;
   itr = lase_lines.find(id);
   if(itr != lase_lines.end())
   {
      symbology3d->removeChild(itr->second.get());

      lase_lines.erase(id);
      geometries_by_line.erase(id);
      line_vertices_by_line.erase(id);
   }
}

//------------------------------------------------------------------------

}; // end namespace SG

