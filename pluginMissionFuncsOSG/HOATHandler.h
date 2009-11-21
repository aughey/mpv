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

#ifndef HOAT_HANDLER_H
#define HOAT_HANDLER_H

#include <vector>
#include <map>

#include "MissionFunctionsWorker.h"

#include "OsgIntersectionFinder.h"
#include "OsgLineDrawer.h"


//=========================================================
//! Handles HOTRequests and interfaces with the scenegraph.  
//! 
class HOATHandler : public mpv::MissionFunctionsWorker
{
public:

	//=========================================================
	//! HOATHandler requires a reference to the intersection finder
	//! 
	HOATHandler( SG::OsgIntersectionFinder &terrain_oif );

protected:

	virtual void processHOTRequest( mpv::RefPtr<mpv::HOTRequest> request );
	
	// Note - not overriding processLOSRequest()

	//=========================================================
	//! General Destructor
	//! 
	virtual ~HOATHandler();

	SG::OsgIntersectionFinder &terrain_oif;
	
	static double HighestExpectedEntityAltitudeMSL;
	static double HighestPointOnEarthMSL;
	static double LowestPointOnEarthMSL;
	static double MaxRangeAboveHATTestPoint;
	static double MaxRangeBelowHATTestPoint;

};

#endif
