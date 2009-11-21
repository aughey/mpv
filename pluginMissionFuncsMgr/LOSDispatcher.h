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


#ifndef _LOSDISPATCHER_H_
#define _LOSDISPATCHER_H_

#include <list>
#include <map>

#include <CigiLosSegReqV3_2.h>
#include <CigiLosVectReqV3_2.h>

#include "AllCigi.h"
#include "Referenced.h"
#include "LOSRequest.h"
#include "LOSResponse.h"
#include "CoordinateConverter.h"
#include "Entity.h"
#include "MissionFunctionsWorker.h"

//=========================================================
//! 
//! 
class LOSDispatcher : public mpv::Referenced
{
public:
	
	boost::signal<void (mpv::RefPtr<mpv::LOSRequest>)> newLOSRequest;
	
	//=========================================================
	//! General Constructor
	//! 
	LOSDispatcher();
	
	void setCoordConverter( mpv::CoordinateConverter *cc )
	{
		coordinateConverter = cc;
	}
	
	void setEntityContainer( mpv::EntityContainer *container )
	{
		allEntities = container;
	}
	
	void setTerrainMaterialOverride( unsigned int materialOverride )
	{
		terrainMaterialOverride = true;
		terrainMaterialOverrideCode = materialOverride;
	}
	void setEntityMaterialOverride( unsigned int materialOverride )
	{
		entityMaterialOverride = true;
		entityMaterialOverrideCode = materialOverride;
	}
	
	void processRequest( CigiLosSegReqV3_2 *packet );
	void processRequest( CigiLosVectReqV3_2 *packet );

	void registerWorker( mpv::MissionFunctionsWorker *worker );
	
	void sendResponses( CigiOutgoingMsg *outgoing );
	
protected:

	class RequestEntry
	{
	public:
		mpv::RefPtr< mpv::LOSRequest > request;
		std::list< mpv::LOSResponseList > responses;
	};
	typedef std::map< int, RequestEntry > RequestEntryMap;

	//=========================================================
	//! General Destructor
	//! 
	virtual ~LOSDispatcher();
	
	void queueResponse( int requestID, mpv::LOSResponseList &responses );
	
	void sendResponse( 
		CigiOutgoingMsg *outgoing, 
		mpv::LOSRequest *request, 
		mpv::LOSResponse *response, int numResponses );

	void sendExtendedResponse( 
		CigiOutgoingMsg *outgoing, 
		mpv::LOSRequest *request, 
		mpv::LOSResponse *response, int numResponses );

	void sendMissResponse( CigiOutgoingMsg *outgoing, int requestID );

	mpv::CoordinateConverter *coordinateConverter;
	
	mpv::EntityContainer *allEntities;
	
	RequestEntryMap requests;
	
	unsigned int numWorkers;

	bool terrainMaterialOverride;
	unsigned int terrainMaterialOverrideCode;
	bool entityMaterialOverride;
	unsigned int entityMaterialOverrideCode;
};

#endif
