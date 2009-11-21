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
 *  FILENAME:   ViewerServer.cpp
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

#include "ViewerServer.h"

#ifdef WIN32
#define snprintf _snprintf
#endif

using std::ostringstream;

ViewerServer::ViewerServer()
{
}

ResponseCode ViewerServer::HandleRequest( HttpRequest * ipoHttpRequest,
										  HttpResponse * ipoHttpResponse )
{
	
	ostringstream oss;
	
	oss << "<html><head><title>MPV Embedded HTTP Server</title></head>\n";
	oss << "<body>\n";
	oss << "<center>\n";

	oss << "<a href=\"entityList/\">Entity List</a><br><br>\n";
	oss << "<a href=\"defTree/\">\"Def\" Tree (configuration info)</a><br><br>\n";
	
	
	oss << "</center>\n";
	oss << "</body>\n";
	oss << "</html>\n";
	
	std::string bodyText = oss.str();
	ipoHttpResponse->SetBody( bodyText.c_str(), bodyText.length() );

	return HTTPRESPONSECODE_200_OK;
}

