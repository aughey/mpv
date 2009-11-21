/** <pre>
 *  PROJECTNAME
 *  Copyright (c) 2008 AUTHORNAME
 *  
 *  This component is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; specifically 
 *  version 2.1 of the License.
 *  
 *  This software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *  
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this software; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *  
 *  
 *  2008-09-01 AUTHORNAME
 *      Initial release
 *  
 *  
 *  </pre>
 */


#ifdef WIN32
#define _USE_MATH_DEFINES
#endif

#include <math.h>
#include <iostream>

#include <CigiHatHotRespV3_2.h>
#include <CigiHatHotXRespV3_2.h>

#include "BindSlot.h"

#include "HOATDispatcher.h"

using namespace mpv;

// ================================================
// HOATDispatcher
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
HOATDispatcher::HOATDispatcher() : Referenced(),
	coordinateConverter( NULL ), 
	allEntities( NULL ),
	numWorkers( 0 ),
	terrainMaterialOverride( false ),
	terrainMaterialOverrideCode( 0 )
{
	
}


// ================================================
// ~HOATDispatcher
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
HOATDispatcher::~HOATDispatcher() 
{
	
}


void HOATDispatcher::processRequest( CigiHatHotReqV3_2 *packet )
{
	if( numWorkers == 0 )
	{
		std::cout << "HOATDispatcher::processRequest - no workers; discarding HOT request " 
			<< packet->GetHatHotID() << std::endl;
		return;
	}
	
	RefPtr<HOTRequest> request = new HOTRequest();
	
	request->id = packet->GetHatHotID();
	request->type = packet->GetReqType();
	request->hostFrameNumber = 0; // FIXME
	
	if( packet->GetSrcCoordSys() == CigiBaseHatHotReq::Entity )
	{
		// entity-relative
		
		Entity *entity = allEntities->findEntity( packet->GetEntityID() );
		if( entity == NULL )
		{
			std::cout << "HOATDispatcher::processRequest - entity " 
				<< packet->GetEntityID() << " doesn't exist; discarding HOT request " 
				<< packet->GetHatHotID() << std::endl;
			return;
		}
		
		Mtx4 entityTransform = entity->getAbsoluteTransform();
		Vect3 requestPosition = 
			entityTransform * 
			Vect3( packet->GetYoff(), 
			       packet->GetXoff(), 
			       -packet->GetZoff() );
		CoordinateSet requestDBCoords;
		requestDBCoords.LatX = requestPosition[0];
		requestDBCoords.LonY = requestPosition[1];
		requestDBCoords.AltZ = requestPosition[2];

		// the coordinates of the test point at MSL don't need to be calculated 
		// for HAT-only requests
		if( request->type == CigiBaseHatHotReq::HOT || 
			request->type == CigiBaseHatHotReq::Extended )
		{
			// need to find db coordinate of test location, but at MSL
			// 1) db -> gdc
			// 2) gdc.alt = 0
			// 3) gdc -> db
			
			CoordinateSet gdc, db;
			coordinateConverter->performReverseConversion( requestDBCoords, gdc );
			gdc.AltZ = 0.0;
			coordinateConverter->performConversion( gdc, db );

			request->locationMSL.Set( db.LatX, db.LonY, db.AltZ );
		}

		// HAT and Extended requests use the altitude provided by the Host 
		// for the test reference point.  (HOT requests use MSL instead.)
		if( request->type == CigiBaseHatHotReq::HAT || 
			request->type == CigiBaseHatHotReq::Extended )
		{
			request->location = requestPosition;
		}
		// HOT requests use the test point at MSL for the test reference 
		// point.
		else
		{
			request->location = request->locationMSL;
		}

		//fixme - geocentric db's (ent will prolly get an "up" eventually, so just pull value from there)
		request->up.Set( 0., 0., 1. );
		
		if( packet->GetUpdatePeriod() != 0 )
			std::cout << "HOATDispatcher::processRequest - warning/fixme - continuous HOT/HAT requests are not supported (HOT request " 
				<< packet->GetHatHotID() << ")\n";
		
	}
	else
	{
		// geodetic coordinates - need to perform coordinate conversion
		
		CoordinateSet gdc, db;
		gdc.LatX = packet->GetLat();
		gdc.LonY = packet->GetLon();

		// the coordinates of the test point at MSL don't need to be calculated 
		// for HAT-only requests
		if( request->type == CigiBaseHatHotReq::HOT || 
			request->type == CigiBaseHatHotReq::Extended )
		{
			gdc.AltZ = 0.0;

			coordinateConverter->performConversion( gdc, db );

			request->locationMSL.Set( db.LatX, db.LonY, db.AltZ );
		}
		
		// HAT and Extended requests use the altitude provided by the Host 
		// for the test reference point.  (HOT requests use MSL instead.)
		if( request->type == CigiBaseHatHotReq::HAT || 
			request->type == CigiBaseHatHotReq::Extended )
		{
			gdc.AltZ = packet->GetAlt();

			coordinateConverter->performConversion( gdc, db );

			request->location.Set( db.LatX, db.LonY, db.AltZ );
		}
		// HOT requests use the test point at MSL for the test reference 
		// point.
		else
		{
			request->location = request->locationMSL;
		}
		
		//fixme - geocentric db's
		request->up.Set( 0., 0., 1. );
		
		if( packet->GetUpdatePeriod() != 0 )
			std::cout << "HOATDispatcher::processRequest - warning - Host sent nonsensical HOT/HAT request: requested continuous responses on a geodetic position (HOT request " 
				<< packet->GetHatHotID() << ")\n";
	}
	

	RequestEntryMap::iterator iter = requests.find( request->id );
	if( iter != requests.end() )
	{
		std::cout << "HOATDispatcher::processRequest - warning - HOT request ID " 
			<< packet->GetHatHotID() << " is still active.  \n"
			<< "\tDiscarding previous request with same ID.  Expect strange behavior.\n";
	}
	RequestEntry entry;
	entry.request = request.get();
	requests[request->id] = entry;
	
	// finally, emit signal
	newHOTRequest( request );
}


void HOATDispatcher::registerWorker( mpv::MissionFunctionsWorker *worker )
{
	worker->connectToNewHOTRequestSignal( &newHOTRequest );
	worker->finishedHOTRequest.connect( BIND_SLOT2( HOATDispatcher::queueResponse, this ) );
	numWorkers++;
}


void HOATDispatcher::queueResponse( int requestID, mpv::HOTResponseList &responses )
{
	RequestEntry &entry = requests[requestID];
	// makes a copy of the list, for later use in sendResponses()
	entry.responses.push_back( responses );
}


void HOATDispatcher::sendResponses( CigiOutgoingMsg *outgoing )
{
	std::list<RequestEntryMap::iterator> completedRequests;
	
	RequestEntryMap::iterator requestIter;
	for( requestIter = requests.begin(); requestIter != requests.end(); requestIter++ )
	{
		int requestID = requestIter->first;
		HOTRequest *request = requestIter->second.request.get();
		std::list< mpv::HOTResponseList > &workerResponseList = requestIter->second.responses;
		
		// if, for this request, we've received a response list from every 
		// worker, then this request is finished and the responses should be 
		// sent
		if( workerResponseList.size() >= numWorkers )
		{
			int numResponses = 0;
		
			// need to total the responses
			std::list< mpv::HOTResponseList >::iterator workerIter;
			for( workerIter = workerResponseList.begin(); workerIter != workerResponseList.end(); workerIter++ )
			{
				numResponses += workerIter->size();
			}

			if( numResponses > 0 )
			{
				std::list< mpv::HOTResponseList >::iterator workerIter;
				for( workerIter = workerResponseList.begin(); workerIter != workerResponseList.end(); workerIter++ )
				{
					mpv::HOTResponseList::iterator responseIter;
					for( responseIter = workerIter->begin(); responseIter != workerIter->end(); responseIter++ )
					{
						if( request->type == CigiBaseHatHotReq::Extended )
							sendExtendedResponse( outgoing, request, responseIter->get(), numResponses );
						else
							sendResponse( outgoing, request, responseIter->get(), numResponses );
					}
				}
			}
			else
			{
				sendMissResponse( outgoing, requestID );
			}
			
			// request has been handled
			completedRequests.push_back( requestIter );
		}
	}
	
	std::list<RequestEntryMap::iterator>::iterator completedIter;
	for( completedIter = completedRequests.begin(); completedIter != completedRequests.end(); completedIter++ )
	{
		requests.erase( *completedIter );
	}
	completedRequests.clear();
}


void HOATDispatcher::sendResponse( 
	CigiOutgoingMsg *outgoing, 
	HOTRequest *request, 
	HOTResponse *response, int numResponses )
{
	CigiHatHotRespV3_2 packet;
	
	packet.SetHatHotID( request->id );
	packet.SetValid( true );
	packet.SetHostFrame( request->hostFrameNumber & 0x0f );
	
	if( request->type == CigiBaseHatHotReq::HOT )
	{
		// HOT
		packet.SetReqType( CigiBaseHatHotResp::HOT );
		
		Vect3 delta = response->hitLocation - request->locationMSL;
		//FIXME - negative heights must be handled!!!
		double hot = delta.mag();
		
		packet.SetHot( hot );
	}
	else
	{
		// HAT
		packet.SetReqType( CigiBaseHatHotResp::HAT );
		
		Vect3 delta = request->location - response->hitLocation;
		//FIXME - negative heights must be handled!!!
		double hat = delta.mag();
		
		packet.SetHat( hat );
	}
	
	*outgoing << packet;
}


void HOATDispatcher::sendExtendedResponse( 
	CigiOutgoingMsg *outgoing, 
	HOTRequest *request, 
	HOTResponse *response, int numResponses )
{
	CigiHatHotXRespV3_2 packet;
	
	packet.SetHatHotID( request->id );
	packet.SetValid( true );
	packet.SetHostFrame( request->hostFrameNumber & 0x0f );
	
	Vect3 delta = response->hitLocation - request->locationMSL;
	//FIXME - negative heights must be handled!!!
	double hot = delta.mag();
	packet.SetHot( hot );
	
	delta = request->location - response->hitLocation;
	//FIXME - negative heights must be handled!!!
	double hat = delta.mag();
	packet.SetHat( hat );
	
	if( terrainMaterialOverride )
		packet.SetMaterial( terrainMaterialOverrideCode );
	else
		packet.SetMaterial( response->materialCode );
	
	// fixme - does this work w/ geocentric?
	double heading = 180.0/M_PI * normalize_angle( atan2(response->normal[1], response->normal[0]) );
	double pitch   = 180.0/M_PI * normalize_angle( atan2(response->normal[1], response->normal[2]) );

	// Convert database heading/pitch into geodetic
	CoordinateSet lru_pos;
	lru_pos.LatX  = response->hitLocation[0];
	lru_pos.LonY  = response->hitLocation[1];
	lru_pos.AltZ  = response->hitLocation[2];
	lru_pos.Yaw   = heading;
	lru_pos.Pitch = pitch;
	lru_pos.Roll  = 0.0;

	CoordinateSet gd_pos;
	coordinateConverter->performReverseConversion( lru_pos, gd_pos );

	packet.SetNormAz( normalize_angle( gd_pos.Yaw ) );
	packet.SetNormEl( clamp_pitch( gd_pos.Pitch ) );
	
	*outgoing << packet;
}


void HOATDispatcher::sendMissResponse( CigiOutgoingMsg *outgoing, int requestID )
{
	RequestEntryMap::iterator iter = requests.find( requestID );
	
	if( iter == requests.end() )
	{
		// hmm, that's weird... btw, should throw an exception or something
		return;
	}
	
	RequestEntry &requestEntry = iter->second;
	RefPtr<HOTRequest> request = requestEntry.request;
	
	if( request->type == CigiBaseHatHotReq::Extended )
	{
		CigiHatHotXRespV3_2 packet;

		packet.SetHatHotID( request->id );
		packet.SetValid( false );
		packet.SetHostFrame( request->hostFrameNumber & 0x0f );
		
		*outgoing << packet;
	}
	else
	{
		CigiHatHotRespV3_2 packet;

		packet.SetHatHotID( request->id );
		packet.SetValid( false );
		packet.SetHostFrame( request->hostFrameNumber & 0x0f );
		packet.SetReqType( request->type == CigiBaseHatHotReq::HOT 
			? CigiBaseHatHotResp::HOT 
			: CigiBaseHatHotResp::HAT );
		
		*outgoing << packet;
	}
}


// ================================================
// normalize_angle
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
double HOATDispatcher::normalize_angle(double a) const
{
	double n = fmod(a, 2*M_PI);

	if(n >= M_PI) n -= 2*M_PI;
	else if(n < -M_PI) n += 2*M_PI;
	return n;
}

// ================================================
// normalize_angle_positive
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
double HOATDispatcher::normalize_angle_positive(double a) const
{
	double n = normalize_angle(a);
	if(n < 0) n += 2*M_PI;
	return n;
}

// ================================================
// clamp_pitch
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
double HOATDispatcher::clamp_pitch( double a ) const
{
	a = std::max( a, -90.0 );
	a = std::min( a,  90.0 );
	return a;
}


