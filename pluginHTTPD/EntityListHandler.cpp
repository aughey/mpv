/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2004 The Boeing Company
 *  
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *  
 *  
 *  FILENAME:   EntityListHandler.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class responds to http client requests; it inherits from EHS and 
 *  overrides the HandleRequest method.  The class can report on entity status, 
 *  and can handle some http-form requests.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  07/01/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>

#include "EntityListHandler.h"

#ifdef WIN32
#define snprintf _snprintf
#endif

using std::ostringstream;

EntityListHandler::EntityListHandler()
{
	entityList = NULL;
	viewParamsMap = NULL;
}

ResponseCode EntityListHandler::HandleRequest( HttpRequest * ipoHttpRequest,
										  HttpResponse * ipoHttpResponse )
{
	if( ipoHttpRequest->oFormValueMap[ "targetEnt" ].sBody.length() > 0 )
	{
		// the user has decided to change the target entity
		std::string targetEntString = 
			ipoHttpRequest->oFormValueMap[ "targetEnt" ].sBody;
		int targetEntInt = atoi( targetEntString.c_str() );
		
		if( viewParamsMap )
		{
			// for each view, set the view's target entity
			std::map< int, ViewParams * >::iterator vpmIter = 
				viewParamsMap->begin();
			for( ; vpmIter != viewParamsMap->end(); vpmIter++ )
			{
				ViewParams *vp = (*vpmIter).second;
				vp->entityID = targetEntInt;
			}
			
		}
	}

	if( 
	ipoHttpRequest->oFormValueMap[ "viewOffX" ].sBody.length() > 0 &&
	ipoHttpRequest->oFormValueMap[ "viewOffY" ].sBody.length() > 0 &&
	ipoHttpRequest->oFormValueMap[ "viewOffZ" ].sBody.length() > 0 &&
	ipoHttpRequest->oFormValueMap[ "viewOffH" ].sBody.length() > 0 &&
	ipoHttpRequest->oFormValueMap[ "viewOffP" ].sBody.length() > 0 &&
	ipoHttpRequest->oFormValueMap[ "viewOffR" ].sBody.length() > 0 
	 )
	{
		// the user has decided to change the view offset
		float viewOffX = atof( ipoHttpRequest->oFormValueMap[ "viewOffX" ].sBody.c_str() );
		float viewOffY = atof( ipoHttpRequest->oFormValueMap[ "viewOffY" ].sBody.c_str() );
		float viewOffZ = atof( ipoHttpRequest->oFormValueMap[ "viewOffZ" ].sBody.c_str() );
		float viewOffH = atof( ipoHttpRequest->oFormValueMap[ "viewOffH" ].sBody.c_str() );
		float viewOffP = atof( ipoHttpRequest->oFormValueMap[ "viewOffP" ].sBody.c_str() );
		float viewOffR = atof( ipoHttpRequest->oFormValueMap[ "viewOffR" ].sBody.c_str() );
		
		if( viewParamsMap )
		{
			std::map< int, ViewParams * >::iterator vpmIter = 
				viewParamsMap->begin();
			for( ; vpmIter != viewParamsMap->end(); vpmIter++ )
			{
				ViewParams *vp = (*vpmIter).second;
				vp->viewOffset[0] = viewOffY;
				vp->viewOffset[1] = viewOffX;
				vp->viewOffset[2] = viewOffZ * -1.;
				vp->viewRotate[0] = viewOffH;
				vp->viewRotate[1] = viewOffP;
				vp->viewRotate[2] = viewOffR;
			}
			
		}
	}
	
	
	ostringstream oss;
	
	oss << "<html><head><title>MPV Embedded HTTP Server - Entity List</title></head>\n";
	
	oss << "<center>Entity List<br><br>\n";
	
	oss << "<form method=GET action=\"/\">\n";
	oss << "<table width=100% border=1>\n";
	
	if( entityList ) 
	{
		// heading at top of table
		oss << "<tr>\n"
		<< "<td>ID</td>\n"
		<< "<td>Type</td>\n"
		<< "<td>Parent's ID</td>\n"
		<< "<td>X position</td>\n"
		<< "<td>Y position</td>\n"
		<< "<td>Z position</td>\n"
		<< "<td>Yaw</td>\n"
		<< "<td>Pitch</td>\n"
		<< "<td>Roll</td>\n"
		<< "</tr>\n";

		std::list<Entity*>::iterator eIter = entityList->begin();
		for( ; eIter != entityList->end(); eIter++ )
		{
			Entity *ent = (*eIter);

			oss << "<tr>\n";

			oss << "<td>";
			oss << "<input type=submit name=\"targetEnt\" value=" << ent->GetEntityID() << " >";
			oss << "</td>\n" ;
			oss << "<td>" << ent->GetEntityType() << "</td>\n" ;
			oss << "<td>" << ent->GetParentID() << "</td>\n" ;
			oss << "<td>" << ent->GetdbXPos() << "</td>\n" ;
			oss << "<td>" << ent->GetdbYPos() << "</td>\n" ;
			oss << "<td>" << ent->GetdbZPos() << "</td>\n" ;
			oss << "<td>" << ent->GetdbYaw() << "</td>\n" ;
			oss << "<td>" << ent->GetdbPitch() << "</td>\n" ;
			oss << "<td>" << ent->GetdbRoll() << "</td>\n" ;

			oss << "</tr>\n";
		}
	}

	oss << "</table><br><br>\n";
	oss << "</form>\n";
	
	// view offset form
	if( viewParamsMap )
	{
		std::map< int, ViewParams * >::iterator vpmIter = 
			viewParamsMap->begin();
		ViewParams *vp = (*vpmIter).second;
		
		oss << "<form> "
		<< "View Offset<br>"
		<< "X: <input type=text name=viewOffX value=\"" << vp->viewOffset[1] << "\">"
		<< "Y: <input type=text name=viewOffY value=\"" << vp->viewOffset[0] << "\">"
		<< "Z: <input type=text name=viewOffZ value=\"" << vp->viewOffset[2]*-1. << "\">"
		<< "<br>"
		<< "H: <input type=text name=viewOffH value=\"" << vp->viewRotate[0] << "\">"
		<< "P: <input type=text name=viewOffP value=\"" << vp->viewRotate[1] << "\">"
		<< "R: <input type=text name=viewOffR value=\"" << vp->viewRotate[2] << "\">"
		<< "<br>"
		<< "<input type=submit>"
		<< " </form>\n";
	}
	
	oss << "</center>\n";
	
	oss << "</html>\n";
	
	std::string bodyText = oss.str();
	ipoHttpResponse->SetBody( bodyText.c_str(), bodyText.length() );

	return HTTPRESPONSECODE_200_OK;
}


void EntityListHandler::setEntityList( std::list<Entity*> *elist )
{
	entityList = elist;
}

void EntityListHandler::setViewParamsMap( std::map< int, ViewParams * > *vpm )
{
	viewParamsMap = vpm;
}

