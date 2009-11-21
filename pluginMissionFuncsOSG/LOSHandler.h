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

#ifndef LOS_HANDLER_H
#define LOS_HANDLER_H

#include <vector>
#include <map>

#include "MissionFunctionsWorker.h"

#include "OsgIntersectionFinder.h"
#include "OsgLineDrawer.h"

//=========================================================
//! Handles incoming LOSRequests and interfaces with
//! the scenegraph.  Generates LOSResponses.
//! 
class LOSHandler : public mpv::MissionFunctionsWorker
{
public:

	//=========================================================
	//! LOSHandler requires a reference to the intersection finder.
	//! LineDrawing is optional.
	//! 
	LOSHandler( SG::OsgIntersectionFinder &oif );

	//=========================================================
	//! Heartbeat
	//! 
	void heartbeat();

	//=========================================================
	//! Set the line drawer (optional).
	//! @drawer    The LineDrawer to use or 0 if none is to be used.
	//!
	void setLineDrawer(SG::LineDrawer *drawer); 

	//=========================================================
	//! Set the ownship entity ID (optional).  If the line drawer is
	//! used when a LOS return intersects this entityID the color of
	//! the line will be red.
	//!
	void setOwnshipEntityID(Cigi_uint16 ownshipEntityID) { this->ownshipEntityID = ownshipEntityID; }

	//=========================================================
	//! Set the maximum number of lase lines to be visible at any
	//! particular time.  Using 0 here implies infinite lines.  To
	//! disable visual lase lines use setLineDrawer().
	//!
	void setMaxLaseLines(unsigned int num) { this->maxLaseLines = num; }

	//=========================================================
	//! Set the maximum number of frames a lase line should be rendered
	//! for.  If 0 is specified here lase lines will have no timeout.
	//!
	void setLaseTimeoutFrames(unsigned int num_frames) { this->laseTimeoutFrames = num_frames; }

protected:

	// Note - not overriding processHOTRequest()

	virtual void processLOSRequest( mpv::RefPtr<mpv::LOSRequest> request );

	//=========================================================
	//! General Destructor
	//! 
	virtual ~LOSHandler();

	SG::OsgIntersectionFinder &scene_oif;
	SG::LineDrawer *lineDrawer;

	Cigi_uint16 ownshipEntityID;

	unsigned int localFrame;
	unsigned int maxLaseLines;
	unsigned int laseTimeoutFrames;
	std::map<unsigned int, unsigned int> laseLinesByRemoveFrame;

	//=========================================================
	//! Determines if a particular intersection hit an entity
	//! by searching for MPV node names in the nodepath of an
	//! intersection.
	//! @param nodepath       The path of nodes for this intersection.
	//! @param entity_id      If we hit an entity, this is it's ID.
	//! @param entity_type_id If we hit an entity, this is it's type ID.
	//! @return true          IFF it is clear from the nodepath we hit an entity. 
	//!
	bool getEntityDetails(osg::NodePath const &nodepath,
			Cigi_uint16 &entity_id, Cigi_uint16 &entity_type_id) const;

   //=========================================================
	//! Determine the Line Color of the LOS visualization
	//! @param intersections A vector of OSG Intersections from the scenegraph.
	//! @param color         A vector filled with the normalized RGBA color to use.
	//!
	void determineLineColor(std::vector<SG::OsgIntersection> const &intersections,
			osg::Vec4 &color) const;

	//=========================================================
	//! Draw a line to the first intersection in the SG
	//! @param intersections A vector of OSG Intersections from the scenegraph.
	//! @param lineseg       The test segment
	//!
	void drawToFirstIntersection(std::vector<SG::OsgIntersection> const &intersections,
			ISECT::LineSeg const &test_segment);

	//=========================================================
	//! Translates entity coordinates into world coordinates.
	//! @param entity        The entity to use for the conversion.
	//! @param entity_offset Coordinates in the entity's coordinate system
	//! @return The position of the entity in world (database) space.
	//!
//	osg::Vec3d entityToWorld(Entity const *entity, osg::Vec3d const &entity_offset) const;

	//=========================================================
	//! Remove Lase Lines in excess of max_lase_lines.
	//!
	void removeExcessLaseLines();

	//=========================================================
	//! Remove Expired Lase Lines (lines existing longer than lase_timeout_frames).
	//!
	void removeExpiredLaseLines(unsigned int cur_local_frame);

};

#endif
