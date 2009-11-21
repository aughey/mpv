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

#ifdef WIN32
#define _USE_MATH_DEFINES
#endif

#include "LOSHandler.h"

#include <osg/io_utils>

#include <CigiLosRespV3.h>

#include <iostream>
#include <iterator>

using namespace mpv;

// ================================================
// LOSHandler
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
LOSHandler::LOSHandler( SG::OsgIntersectionFinder &oif ) :
   MissionFunctionsWorker(),
   scene_oif(oif)
{
   this->lineDrawer = 0;
   this->ownshipEntityID = 0;

   this->localFrame = 0;
   this->maxLaseLines = 1;
   this->laseTimeoutFrames = 60;
}

// ================================================
// ~LOSHandler
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
LOSHandler::~LOSHandler()
{
}

// ================================================
// determineLineColor
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void LOSHandler::determineLineColor(std::vector<SG::OsgIntersection> const &intersections,
      osg::Vec4 &color) const
{
   color.set(0.0f, 1.0f, 0.0f, 1.0f); // GREEN

   if(intersections.size() > 0)
   {
      Cigi_uint16 entity_id = 0;
      Cigi_uint16 entity_type_id;
      bool hit_entity = getEntityDetails(intersections[0].hit.getNodePath(), entity_id, entity_type_id);
      if(hit_entity && entity_id == ownshipEntityID)
      {
         // Hit ourselves
         color.set(1.0, 0.0, 0.0, 1.0); // RED
      }
      else if(hit_entity)
      {
         // Hit another entity
         color.set(1.0f, 1.0f, 0.0f, 1.0f); // YELLOW
      }
   }
}

// ================================================
//
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void LOSHandler::processLOSRequest( mpv::RefPtr<mpv::LOSRequest> request )
{
   std::vector<SG::OsgIntersection> intersections;
   LOSResponseList responses;

   //scene_oif.set_minimum_alpha(cmd.minimum_alpha);

   ISECT::LineSeg test_segment;
   test_segment.start.set(
      request->start[0],
      request->start[1],
      request->start[2] );
   test_segment.end.set(
      request->end[0],
      request->end[1],
      request->end[2] );

   scene_oif.find_intersections_along_segment(intersections, test_segment);

   //
   // Send in reverse order so that a potentially buggy host implementation
   // gets the shortest range last.
   //
   std::vector<SG::OsgIntersection>::reverse_iterator itr;
   for(itr = intersections.rbegin(); itr != intersections.rend(); ++itr)
   {
      LOSResponse *response = new LOSResponse();

      Cigi_uint16 entity_id, entity_type_id;
      bool hit_entity = getEntityDetails(itr->hit.getNodePath(), entity_id, entity_type_id);
      if(hit_entity)
      {
         response->entityID = entity_id;
      }
      else
      {
         response->entityID = -1;
      }

      osg::Vec3 wipt = itr->hit.getWorldIntersectPoint();
      osg::Vec3 wipn = itr->hit.getWorldIntersectNormal();

      response->hitLocation.Set( wipt.x(), wipt.y(), wipt.z() );
      response->normal.Set( wipn.x(), wipn.y(), wipn.z() );

      responses.push_back( response );
   }

   //
   // Optionally, draw a line to represent the LOS.
   //
   if(lineDrawer)
   {
      drawToFirstIntersection(intersections, test_segment);
   }

   // finally, emit signal
   finishedLOSRequest( request->id, responses );
}

// ================================================
// drawToFirstIntersection
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void LOSHandler::drawToFirstIntersection(std::vector<SG::OsgIntersection> const &intersections,
      ISECT::LineSeg const &test_segment)
{
   if(lineDrawer)
   {
      //
      // Determine the LineSegment start/endpoints
      //
      osg::ref_ptr<osg::LineSegment> ls;
      osg::Vec3 a(test_segment.start.x, test_segment.start.y, test_segment.start.z);
      if(intersections.empty())
      {
         // If we hit nothing draw the test segment itself
         osg::Vec3 b(test_segment.end.x, test_segment.end.y, test_segment.end.z);
         ls = new osg::LineSegment(a, b);
      }
      else
      {
         // Draw the segment from min_range to first intersection.
         ls = new osg::LineSegment(a, intersections[0].hit.getWorldIntersectPoint());
      }

      osg::Vec4 color;
      determineLineColor(intersections, color);

      //
      // Draw the new line replacing the old.
      //
      unsigned int newid = lineDrawer->draw_line(ls.get(), color);

      // Set expiry time
      laseLinesByRemoveFrame[newid] = localFrame + laseTimeoutFrames;
      removeExcessLaseLines();
   }
}

/*// ================================================
// entityToWorld
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
osg::Vec3d LOSHandler::entityToWorld(Entity const *entity, osg::Vec3d const &entity_offset) const
{
   osg::Vec3d const entity_pos_db(entity->GetdbXPos(), entity->GetdbYPos(), entity->GetdbZPos());

   osg::Quat quaternion;
   quaternion.makeRotate(
         entity->GetdbRoll() * M_PI/180.0, osg::Vec3d(0,1,0),
         entity->GetdbPitch() * M_PI/180.0, osg::Vec3d(1,0,0),
         entity->GetdbYaw() * M_PI/180.0, osg::Vec3d(0,0,-1));

   osg::Vec3d world_pos = quaternion * entity_offset;
   world_pos += entity_pos_db;

   return world_pos;
}*/

// ================================================
// getEntityDetails
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
bool LOSHandler::getEntityDetails(osg::NodePath const &nodepath,
      Cigi_uint16 &entity_id, Cigi_uint16 &entity_type_id) const
{
   if(nodepath.size() >= 3)
   {
      // Get the subgraph wrapper node name two levels below the root
      std::string nodename = nodepath[2]->getName();
      std::string::size_type start_loc = nodename.find("Entity Subgraph");
      if (start_loc != std::string::npos)
      {
         std::string::size_type id_loc = nodename.find("entID ");
         std::string::size_type type_loc = nodename.find("typeID ");
         if(id_loc == std::string::npos || type_loc == std::string::npos)
         {
            return false;
         }
         std::string entID = nodename.substr( id_loc+6, type_loc);
         std::string typeID = nodename.substr( type_loc+7);

         entity_id = atoi(entID.c_str());
         entity_type_id = atoi(typeID.c_str());
         return true;
      }
   }
   return false;
}

// ================================================
// heartbeat
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void LOSHandler::heartbeat()
{
   localFrame++;
   if(lineDrawer && laseTimeoutFrames)
   {
      removeExpiredLaseLines(localFrame);
   }
}

// ================================================
// removeExcessLaseLines
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void LOSHandler::removeExcessLaseLines()
{
   if(lineDrawer && maxLaseLines > 0)
   {
      while(laseLinesByRemoveFrame.size() > maxLaseLines)
      {
         std::map<unsigned int, unsigned int>::iterator itr = laseLinesByRemoveFrame.begin();
         if(itr == laseLinesByRemoveFrame.end()) return;

         unsigned int const &line_id = itr->first;

         // Remove excess line
         lineDrawer->remove_line(line_id);
         laseLinesByRemoveFrame.erase(itr);
      }
   }
}

// ================================================
// removeExpiredLaseLines
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void LOSHandler::removeExpiredLaseLines(unsigned int cur_local_frame)
{
   if(lineDrawer)
   {
      std::map<unsigned int, unsigned int>::iterator itr;
      for(itr = laseLinesByRemoveFrame.begin();
            itr != laseLinesByRemoveFrame.end(); )
      {
         unsigned int const &line_id = itr->first;
         unsigned int const &expiry_frame = itr->second;

         if(cur_local_frame > expiry_frame)
         {
            // Remove expired line
            lineDrawer->remove_line(line_id);
            laseLinesByRemoveFrame.erase(itr++);
         }
         else
         {
            // All lase lines past this point are not expired
            return;
         }
      }
   }
}

// ================================================
// setLineDrawer
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void LOSHandler::setLineDrawer(SG::LineDrawer *drawer)
{
   if(lineDrawer && drawer==0)
   {
      // If they want to get rid of the line drawer clear all lines
      lineDrawer->clear_lines();
   }
   this->lineDrawer = drawer;
}

