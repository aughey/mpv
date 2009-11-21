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


#ifndef _HOATDISPATCHER_H_
#define _HOATDISPATCHER_H_

#include <list>
#include <map>

#include <CigiHatHotReqV3_2.h>

#include "AllCigi.h"
#include "Referenced.h"
#include "HOTRequest.h"
#include "HOTResponse.h"
#include "CoordinateConverter.h"
#include "Entity.h"
#include "MissionFunctionsWorker.h"

//=========================================================
//! 
//! 
class HOATDispatcher : public mpv::Referenced
{
public:
	
	boost::signal<void (mpv::RefPtr<mpv::HOTRequest>)> newHOTRequest;
	
	//=========================================================
	//! General Constructor
	//! 
	HOATDispatcher();
	
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
	
	void processRequest( CigiHatHotReqV3_2 *packet );

	void registerWorker( mpv::MissionFunctionsWorker *worker );
	
	void sendResponses( CigiOutgoingMsg *outgoing );
	
protected:
	
	class RequestEntry
	{
	public:
		mpv::RefPtr< mpv::HOTRequest > request;
		std::list< mpv::HOTResponseList > responses;
	};
	typedef std::map< int, RequestEntry > RequestEntryMap;

	//=========================================================
	//! General Destructor
	//! 
	virtual ~HOATDispatcher();
	
	void queueResponse( int requestID, mpv::HOTResponseList &responses );
	
	void sendResponse( 
		CigiOutgoingMsg *outgoing, 
		mpv::HOTRequest *request, 
		mpv::HOTResponse *response, int numResponses );

	void sendExtendedResponse( 
		CigiOutgoingMsg *outgoing, 
		mpv::HOTRequest *request, 
		mpv::HOTResponse *response, int numResponses );

	void sendMissResponse( CigiOutgoingMsg *outgoing, int requestID );

	double normalize_angle(double a) const;
	double normalize_angle_positive(double a) const;
	double clamp_pitch( double a ) const;

	mpv::CoordinateConverter *coordinateConverter;
	
	mpv::EntityContainer *allEntities;
	
	RequestEntryMap requests;
	
	unsigned int numWorkers;

	bool terrainMaterialOverride;
	unsigned int terrainMaterialOverrideCode;
};

#endif
