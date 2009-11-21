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

#include <iostream>
#include <iterator>

#include <osg/io_utils>

#include "Vect3.h"

#include "HOATHandler.h"

using namespace mpv;

//! The highest altitude that a HAT test could conceivably be made from.
//! The U2 and SR71 spy planes are claimed to have a service ceiling of 26000m, 
//! so this value should be OK.
double HOATHandler::HighestExpectedEntityAltitudeMSL = 30000.0;
//! The altitude of Mt. Everest, the highest point on earth, in meters MSL
double HOATHandler::HighestPointOnEarthMSL = 8848.0;
//! The altitude of the Mariana Trench, the lowest point on earth, in meters MSL
double HOATHandler::LowestPointOnEarthMSL = -11000.0;

//! The maximum reasonable range above a HAT test point
double HOATHandler::MaxRangeAboveHATTestPoint = 
	HOATHandler::HighestPointOnEarthMSL + 
	-HOATHandler::LowestPointOnEarthMSL;
//! The maximum reasonable range below a HAT test point
double HOATHandler::MaxRangeBelowHATTestPoint = 
	HOATHandler::LowestPointOnEarthMSL - 
	HOATHandler::HighestExpectedEntityAltitudeMSL;


// ================================================
// HOATHandler
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
HOATHandler::HOATHandler( SG::OsgIntersectionFinder &terrain_oif ) : 
	MissionFunctionsWorker(), 
	terrain_oif(terrain_oif)
{
}

// ================================================
// ~HOATHandler
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
HOATHandler::~HOATHandler()
{
}

// ================================================
// processHOTRequest
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void HOATHandler::processHOTRequest( mpv::RefPtr<mpv::HOTRequest> request )
{
	std::vector<SG::OsgIntersection> intersections;
	HOTResponseList responses;
	
	mpv::Vect3 startPoint = request->up * MaxRangeAboveHATTestPoint + 
	                        request->location;
	mpv::Vect3 endPoint   = request->up * MaxRangeBelowHATTestPoint + 
	                        request->location;
	
	terrain_oif.find_intersections_along_segment( intersections, ISECT::LineSeg(
		ISECT::Point( startPoint[0], startPoint[1], startPoint[2] ), 
		ISECT::Point( endPoint[0], endPoint[1], endPoint[2] ) ) );

	unsigned int size = intersections.size();
	for( unsigned int i = 0; i < size; i++ )
	{
		SG::OsgIntersection const &oi = intersections[ i ];
		HOTResponse *response = new HOTResponse();
		
		osg::Vec3 wipt = oi.hit.getWorldIntersectPoint();
		osg::Vec3 wipn = oi.hit.getWorldIntersectNormal();
		
		response->hitLocation.Set( wipt.x(), wipt.y(), wipt.z() );
		response->normal.Set( wipn.x(), wipn.y(), wipn.z() );
		
		responses.push_back( response );
	}
	
	// finally, emit signal
	finishedHOTRequest( request->id, responses );
}

