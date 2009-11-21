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

#define _USE_MATH_DEFINES

#include <math.h>
#include <iostream>

#include <CigiLosRespV3_2.h>
#include <CigiLosXRespV3_2.h>

#include "BindSlot.h"
#include "LOSDispatcher.h"

using namespace mpv;

// ================================================
// LOSDispatcher
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
LOSDispatcher::LOSDispatcher() : Referenced(), 
	coordinateConverter( NULL ), 
	allEntities( NULL ),
	numWorkers( 0 ),
	terrainMaterialOverride( false ),
	terrainMaterialOverrideCode( 0 ),
	entityMaterialOverride( false ),
	entityMaterialOverrideCode( 0 )
{
	
}


// ================================================
// ~LOSDispatcher
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
LOSDispatcher::~LOSDispatcher() 
{
	
}


void LOSDispatcher::processRequest( CigiLosSegReqV3_2 *packet )
{
	if( numWorkers == 0 )
	{
		std::cout << "Error in LOSDispatcher::processRequest - no workers; discarding LOS request " 
			<< packet->GetLosID() << std::endl;
		return;
	}
	
	RefPtr<LOSRequest> request = new LOSRequest();
	
	request->id = packet->GetLosID();
	request->type = (packet->GetReqType() == CigiBaseLosSegReq::Basic) 
	                ? LOSRequest::Basic : LOSRequest::Extended;
	request->alphaThreshold = packet->GetAlphaThresh();
	request->materialMask = packet->GetMask();
	
	if( request->materialMask == 0 )
	{
		std::cout << "Warning in LOSDispatcher::processRequest - "
			<< "Host sent nonsensical LOS request: material mask set to 0 " 
			<< "(request " << request->id << ")\n";
	}
	
	request->requestedResponseCoordinateSystem = 
		(packet->GetResponseCoordSys() == CigiBaseLosSegReq::Geodetic)
			? LOSRequest::Geodetic : LOSRequest::Entity;
	request->hostFrameNumber = 0; // FIXME
	
	Entity *srcEntity = NULL;
	if( packet->GetSrcCoordSys() == CigiBaseLosSegReq::Entity )
	{
		// entity-relative
		
		srcEntity = allEntities->findEntity( packet->GetEntityID() );
		if( srcEntity == NULL )
		{
			std::cout << "Warning in LOSDispatcher::processRequest - source entity " 
				<< packet->GetEntityID() << " doesn't exist; discarding LOS request " 
				<< request->id << std::endl;
			return;
		}
		
		request->start = 
			srcEntity->getAbsoluteTransform() * 
			Vect3( packet->GetSrcYoff(), 
			       packet->GetSrcXoff(), 
			       -packet->GetSrcZoff() );
	}
	else
	{
		// geodetic coordinates - need to perform coordinate conversion
		
		CoordinateSet gdc, db;
		gdc.LatX = packet->GetSrcLat();
		gdc.LonY = packet->GetSrcLon();
		gdc.AltZ = packet->GetSrcAlt();

		coordinateConverter->performConversion( gdc, db );

		request->start.Set( db.LatX, db.LonY, db.AltZ );
	}
	
	request->origin = request->start;
	
	if( packet->GetDstCoordSys() == CigiBaseLosSegReq::Entity )
	{
		// entity-relative
		
		Entity *destEntity = NULL;
		
		if( packet->GetDestEntityIDValid() )
		{
			destEntity = allEntities->findEntity( packet->GetDestEntityID() );
			if( destEntity == NULL )
			{
				std::cout << "Warning in LOSDispatcher::processRequest - destination entity " 
					<< packet->GetDestEntityID() << " doesn't exist; discarding LOS request " 
					<< request->id << std::endl;
				return;
			}
		}
		else
		{
			// destination entity is source entity
			
			if( packet->GetSrcCoordSys() != CigiBaseLosSegReq::Entity )
			{
				std::cout << "Warning in LOSDispatcher::processRequest - "
					<< "Host sent nonsensical LOS request: destination coordinate was set " 
					<< "relative to source entity, but source coordinate wasn't an entity; "
					<< "discarding LOS request " 
					<< request->id << std::endl;
				return;
			}
			
			destEntity = srcEntity;
			if( destEntity == NULL )
			{
				// shouldn't reach this line; should have quit earlier 
				// when source entity couldn't be found
				std::cout << "Warning in LOSDispatcher::processRequest - " 
					<< "destination entity (also the source entity) doesn't exist; " 
					<< "discarding LOS request " 
					<< request->id << std::endl;
				return;
			}
		}
		
		request->end = 
			destEntity->getAbsoluteTransform() * 
			Vect3( packet->GetDstYoff(), 
			       packet->GetDstXoff(), 
			       -packet->GetDstZoff() );
	}
	else
	{
		// geodetic coordinates - need to perform coordinate conversion
		
		CoordinateSet gdc, db;
		gdc.LatX = packet->GetDstLat();
		gdc.LonY = packet->GetDstLon();
		gdc.AltZ = packet->GetDstAlt();

		coordinateConverter->performConversion( gdc, db );

		request->end.Set( db.LatX, db.LonY, db.AltZ );
	}
	
	RequestEntryMap::iterator iter = requests.find( request->id );
	if( iter != requests.end() )
	{
		std::cout << "Warning in LOSDispatcher::processRequest - LOS request ID " 
			<< request->id << " is still active.  \n"
			<< "\tDiscarding previous request with same ID.  Expect strange behavior.\n";
	}
	RequestEntry entry;
	entry.request = request.get();
	requests[request->id] = entry;
	
	// finally, emit signal
	newLOSRequest( request );
}


void LOSDispatcher::processRequest( CigiLosVectReqV3_2 *packet )
{
	if( numWorkers == 0 )
	{
		std::cout << "Error in LOSDispatcher::processRequest - no workers; discarding LOS request " 
			<< packet->GetLosID() << std::endl;
		return;
	}
	
	RefPtr<LOSRequest> request = new LOSRequest();
	
	request->id = packet->GetLosID();
	request->type = (packet->GetReqType() == CigiBaseLosVectReq::Basic) 
	                ? LOSRequest::Basic : LOSRequest::Extended;
	request->alphaThreshold = packet->GetAlphaThresh();
	request->materialMask = packet->GetMask();
	
	if( request->materialMask == 0 )
	{
		std::cout << "Warning in LOSDispatcher::processRequest - "
			<< "Host sent nonsensical LOS request: material mask set to 0 " 
			<< "(request " << request->id << ")\n";
	}
	
	request->requestedResponseCoordinateSystem = 
		(packet->GetResponseCoordSys() == CigiBaseLosVectReq::Geodetic)
			? LOSRequest::Geodetic : LOSRequest::Entity;
	request->hostFrameNumber = 0; // FIXME
	
	Vect3 requestPoint;
	Vect3 requestVector;
	
	if( packet->GetSrcCoordSys() == CigiBaseLosVectReq::Entity )
	{
		// entity-relative
		
		Entity *srcEntity = allEntities->findEntity( packet->GetEntityID() );
		if( srcEntity == NULL )
		{
			std::cout << "Warning in LOSDispatcher::processRequest - source entity " 
				<< packet->GetEntityID() << " doesn't exist; discarding LOS request " 
				<< request->id << std::endl;
			return;
		}
		
		if( srcEntity->getIsChild() )
		{
			std::cout << "Warning in LOSDispatcher::processRequest - "
				<< "LOS requests on child entities not implemented yet; "
				<< "discarding LOS request (child entity: " 
				<< srcEntity->getID() << ", LOS request " 
				<< request->id << ")\n";
			return;
		}

		const CoordinateSet &coord = srcEntity->getPositionDB();
		
		// fixme - this is all pretty hackish... need to revisit

		Mtx4 entityTranslate, entityRotate;

		entityTranslate.setAsTranslate( Vect3( coord.LatX, coord.LonY, coord.AltZ ) );

		entityRotate.setAsRotate( 
			-1.0 * coord.Yaw * M_PI / 180.0, 
			coord.Pitch * M_PI / 180.0, 
			coord.Roll * M_PI / 180.0 );


		Mtx4 requestTranslate, requestRotate;
		
		requestTranslate.setAsTranslate( Vect3( 
			packet->GetSrcYoff(), 
			packet->GetSrcXoff(), 
			-packet->GetSrcZoff() ) );
		
		requestRotate.setAsRotate( 
			-1.0 * packet->GetVectAz() * M_PI / 180.0,
			packet->GetVectEl() * M_PI / 180.0,
			0.0 );


		Mtx4 requestPointMtx = 
			entityTranslate * entityRotate * requestTranslate;
		Mtx4 requestVectorMtx = 
			entityRotate * requestRotate;

		
		Vect3 temp( 0, 0, 0 ); // entity origin
		requestPoint = requestPointMtx * temp;
		
		temp.Set( 0, 1, 0 ); // unit vector pointing forward
		requestVector = requestVectorMtx * temp;

	}
	else
	{
		// geodetic coordinates - need to perform coordinate conversion
		
		CoordinateSet gdc, db;
		gdc.LatX  = packet->GetSrcLat();
		gdc.LonY  = packet->GetSrcLon();
		gdc.AltZ  = packet->GetSrcAlt();
		gdc.Yaw   = packet->GetVectAz();
		gdc.Pitch = packet->GetVectEl();
		gdc.Roll  = 0.0;

		coordinateConverter->performConversion( gdc, db );

		requestPoint.Set( db.LatX, db.LonY, db.AltZ );
		
		//
		// Construct the unit vector based on the direction obtained
		//
		double azimuth_rads   = db.Yaw * M_PI / 180.0;
		double elevation_rads = db.Pitch * M_PI / 180.0; 
		// fixme - not geocentric-safe
		requestVector.Set(
				cos(elevation_rads)*sin(azimuth_rads),
				cos(elevation_rads)*cos(azimuth_rads),
				sin(elevation_rads) );
	}
	
	double minRange = packet->GetMinRange();
	double maxRange = packet->GetMaxRange();
	if( minRange < 0. )
	{
		std::cout << "Warning in LOSDispatcher::processRequest - "
			<< "Invalid minimum range: " << minRange << "; "
			<< "discarding LOS request " << request->id << std::endl;
		return;
	}
	if( maxRange < minRange )
	{
		std::cout << "Warning in LOSDispatcher::processRequest - "
			<< "Maximum range (" << maxRange 
			<< ") is smaller than minimum range (" << minRange << "); "
			<< "discarding LOS request " << request->id << std::endl;
		return;
	}
	
	request->start = requestVector * minRange + 
	                 requestPoint;
	request->end   = requestVector * maxRange + 
	                 requestPoint;
	
	request->origin = requestPoint;
	
	RequestEntryMap::iterator iter = requests.find( request->id );
	if( iter != requests.end() )
	{
		std::cout << "Warning in LOSDispatcher::processRequest - LOS request ID " 
			<< request->id << " is still active.  \n"
			<< "\tDiscarding previous request with same ID.  Expect strange behavior.\n";
	}
	RequestEntry entry;
	entry.request = request.get();
	requests[request->id] = entry;
	
	// finally, emit signal
	newLOSRequest( request );
}


void LOSDispatcher::registerWorker( mpv::MissionFunctionsWorker *worker )
{
	worker->connectToNewLOSRequestSignal( &newLOSRequest );
	worker->finishedLOSRequest.connect( BIND_SLOT2( LOSDispatcher::queueResponse, this ) );
	numWorkers++;
}


void LOSDispatcher::queueResponse( int requestID, mpv::LOSResponseList &responses )
{
	RequestEntry &entry = requests[requestID];
	// makes a copy of the list, for later use in sendResponses()
	entry.responses.push_back( responses );
}


void LOSDispatcher::sendResponses( CigiOutgoingMsg *outgoing )
{
	std::list<RequestEntryMap::iterator> completedRequests;
	
	RequestEntryMap::iterator requestIter;
	for( requestIter = requests.begin(); requestIter != requests.end(); requestIter++ )
	{
		int requestID = requestIter->first;
		LOSRequest *request = requestIter->second.request.get();
		std::list< mpv::LOSResponseList > &workerResponseList = requestIter->second.responses;
		
		// if, for this request, we've received a response list from every 
		// worker, then this request is finished and the responses should be 
		// sent
		if( workerResponseList.size() >= numWorkers )
		{
			int numResponses = 0;
		
			// need to total the responses
			std::list< mpv::LOSResponseList >::iterator workerIter;
			for( workerIter = workerResponseList.begin(); workerIter != workerResponseList.end(); workerIter++ )
			{
				numResponses += workerIter->size();
			}

			if( numResponses > 0 )
			{
				std::list< mpv::LOSResponseList >::iterator workerIter;
				for( workerIter = workerResponseList.begin(); workerIter != workerResponseList.end(); workerIter++ )
				{
					mpv::LOSResponseList::iterator responseIter;
					for( responseIter = workerIter->begin(); responseIter != workerIter->end(); responseIter++ )
					{
						if( request->type == LOSRequest::Extended )
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


void LOSDispatcher::sendResponse( 
	CigiOutgoingMsg *outgoing, 
	LOSRequest *request, 
	LOSResponse *response, int numResponses )
{
	CigiLosRespV3_2 packet;
	
	packet.SetLosID( request->id );
	packet.SetValid( true );
	packet.SetVisible( false );
	packet.SetHostFrame( request->hostFrameNumber & 0x0f );
	packet.SetRespCount( numResponses );
	
	Vect3 delta = response->hitLocation - request->origin;
	double range = delta.mag();
	packet.SetRange( range );
	
	if( response->entityID != -1 )
	{
		// hit an entity
		packet.SetEntityID( response->entityID );
		packet.SetEntityIDValid( true );
	}
	else
		packet.SetEntityIDValid( false );
	
	*outgoing << packet;
}


void LOSDispatcher::sendExtendedResponse( 
	CigiOutgoingMsg *outgoing, 
	LOSRequest *request, 
	LOSResponse *response, int numResponses )
{
	CigiLosXRespV3_2 packet;
	
	packet.SetLosID( request->id );
	packet.SetValid( true );
	packet.SetVisible( false );
	packet.SetHostFrame( request->hostFrameNumber & 0x0f );
	packet.SetRespCount( numResponses );
	
	Vect3 delta = response->hitLocation - request->origin;
	double range = delta.mag();
	packet.SetRange( range );
	packet.SetRangeValid( true );
	
	if( response->entityID != -1 )
	{
		// hit an entity
		packet.SetEntityID( response->entityID );
		packet.SetEntityIDValid( true );
	}
	else
		packet.SetEntityIDValid( false );
	
	if( request->requestedResponseCoordinateSystem == LOSRequest::Entity && 
		response->entityID != -1 )
	{
		// host requested entity hits to be returned in entity coordinates, 
		// and we hit an entity
		
		std::cout << "Fixme in LOSDispatcher::sendExtendedResponse - "
			<< "for LOS request ID " << request->id << ", Host requested that "
			<< "entity-hit responses be in entity coordinates.  That case "
			<< "isn't implemented in the MPV yet, so returning 0,0,0 in the "
			<< "offset fields.\n";
		packet.SetXoff( 0. );
		packet.SetYoff( 0. );
		packet.SetZoff( 0. );
	}
	else
	{
		// host requested entity hits to be returned in geodetic coordinates, 
		// or didn't hit an entity
		
		CoordinateSet db, gdc;
		db.LatX = response->hitLocation[0];
		db.LonY = response->hitLocation[1];
		db.AltZ = response->hitLocation[2];
		
		coordinateConverter->performReverseConversion( db, gdc );

		packet.SetLatitude( gdc.LatX );
		packet.SetLongitude( gdc.LonY );
		packet.SetAltitude( gdc.AltZ );
	}
	
	packet.SetRed(   (Cigi_uint8)( response->color[0] * 255.0 ) );
	packet.SetGreen( (Cigi_uint8)( response->color[1] * 255.0 ) );
	packet.SetBlue(  (Cigi_uint8)( response->color[2] * 255.0 ) );
	packet.SetAlpha( (Cigi_uint8)( response->color[3] * 255.0 ) );
	
	if( response->entityID != -1 )
	{
		// hit entity
		if( entityMaterialOverride )
			packet.SetMaterial( entityMaterialOverrideCode );
		else
			packet.SetMaterial( response->materialCode );
	}
	else
	{
		// hit terrain
		if( terrainMaterialOverride )
			packet.SetMaterial( terrainMaterialOverrideCode );
		else
			packet.SetMaterial( response->materialCode );
	}
	
	// fixme - normal
	
	*outgoing << packet;
}


void LOSDispatcher::sendMissResponse( CigiOutgoingMsg *outgoing, int requestID )
{
	RequestEntryMap::iterator iter = requests.find( requestID );
	
	if( iter == requests.end() )
	{
		// hmm, that's weird... btw, should throw an exception or something
		return;
	}
	
	RequestEntry &requestEntry = iter->second;
	RefPtr<LOSRequest> request = requestEntry.request;

	if( request->type == LOSRequest::Extended )
	{
		CigiLosXRespV3_2 packet;

		packet.SetLosID( request->id );
		packet.SetValid( false );
		packet.SetVisible( true );
		packet.SetHostFrame( request->hostFrameNumber & 0x0f );
		packet.SetRespCount( 1 );
		packet.SetEntityIDValid( false );
		packet.SetRange( 0. );
		packet.SetRangeValid( false );

		// In the case of a los miss, the hit location is supposed to 
		// contain the lat/lon of the ray endpoint.  
		// Commentary: I question how valuable that is.
		CoordinateSet db, gdc;
		db.LatX = request->end[0];
		db.LonY = request->end[1];
		db.AltZ = request->end[2];
		
		coordinateConverter->performReverseConversion( db, gdc );

		packet.SetLatitude( gdc.LatX );
		packet.SetLongitude( gdc.LonY );
		packet.SetAltitude( gdc.AltZ );
		
		*outgoing << packet;
	}
	else
	{
		CigiLosRespV3_2 packet;

		packet.SetLosID( request->id );
		packet.SetValid( false );
		packet.SetVisible( true );
		packet.SetHostFrame( request->hostFrameNumber & 0x0f );
		packet.SetRespCount( 1 );
		packet.SetEntityIDValid( false );
		packet.SetRange( 0. );
		
		*outgoing << packet;
	}
}


