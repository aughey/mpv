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
 *  FILENAME:   DefTreeHandler.cpp
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
 *  01/19/2005 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#include <fcntl.h> // needed for open() and read()
#include <unistd.h> // needed for fstat()
#include <sys/stat.h> // needed for fstat()

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>

#include "DefTreeHandler.h"

#ifdef WIN32
#define snprintf _snprintf
#endif

using std::ostringstream;

DefTreeHandler::DefTreeHandler()
{
	defTree = NULL;
}

ResponseCode DefTreeHandler::HandleRequest( HttpRequest * ipoHttpRequest,
										  HttpResponse * ipoHttpResponse )
{
printf( "DefTreeHandler::HandleRequest uri: %s\n", ipoHttpRequest->sUri.c_str() );
	if( ipoHttpRequest->sUri.length() > 0 )
	{
		return sendExternalFile( ipoHttpResponse, 
			std::string( "/junk/html-tree-widget/" ) + ipoHttpRequest->sUri );
	}
	else
	{
		return sendHTMLPage( ipoHttpResponse );
	}
	

	return HTTPRESPONSECODE_200_OK;
}


void DefTreeHandler::setDefTree( DefFileGroup **dT )
{
	defTree = dT;
}


ResponseCode DefTreeHandler::sendExternalFile( 
	HttpResponse * ipoHttpResponse, const std::string &filename )
{
	int fd = open( filename.c_str(), O_RDONLY );
	
	if( fd != -1 )
	{
		struct stat fileStat;
		int status = fstat( fd, &fileStat );
		
		if( status == 0 )
		{
			unsigned char *fileBuffer = 
				(unsigned char *)malloc( (int)fileStat.st_size + 1 );
			memset( fileBuffer, 0, (int)fileStat.st_size + 1 );

printf( "file length: %i\n", (int)fileStat.st_size );
			
			read( fd, fileBuffer, (int)fileStat.st_size );
			
			ipoHttpResponse->SetBody( (char*)fileBuffer, (int)fileStat.st_size );
			free( fileBuffer );
			close( fd );
			return HTTPRESPONSECODE_200_OK;
		}
		else
		{
			printf( "fstat returned error: %i\n", status );
			close( fd );
			return sendErrorFileNotFound( ipoHttpResponse, filename );
		}
	}
	else
	{
		printf( "file could not be opened\n" );
		return sendErrorFileNotFound( ipoHttpResponse, filename );
	}
	
	return HTTPRESPONSECODE_500_INTERNALSERVERERROR;
}


ResponseCode DefTreeHandler::sendHTMLPage( HttpResponse * ipoHttpResponse )
{
	jsVariableNameCounter = 0;
	
	ostringstream oss;

	oss << "<html><head>";
	oss << "<title>MPV Embedded HTTP Server - Def Tree</title>\n";

	oss << "<!-- The xtree script file -->\n"
		<< "<script src=\"xtree.js\"></script>\n"
		<< "\n"
		<< "<!-- Modify this file to change the way the tree looks -->\n"
		<< "<link type=\"text/css\" rel=\"stylesheet\" href=\"xtree.css\">\n"
		<< "\n"
		<< "<style>\n"
		<< "	body { background: white; color: black; }\n"
		<< "	input { width: 120px; }\n"
		<< "</style>\n";
	
	oss << "</head>";
	oss << "<body>\n";
//	oss << "<center>\n";
	
	
oss << "<img src=\"images/file.png\"><br><br>\n";

	int rootID = jsVariableNameCounter;
	oss << "<script>\n"
		<< "if (document.getElementById) {\n"
		<< "\tvar v" << rootID << " = new WebFXTree('Root');\n"
		<< "\tv" << rootID << ".setBehavior('classic');\n";

	recurseThroughDefTree( rootID, *defTree, oss );

	oss << "\tdocument.write(v" << rootID << ");\n"
		<< "}\n"
		<< "</script>\n";

//	oss << "</center>\n";
	oss << "</body>\n";
	oss << "</html>\n";
		
	std::string bodyText = oss.str();
	ipoHttpResponse->SetBody( bodyText.c_str(), bodyText.length() );

	return HTTPRESPONSECODE_200_OK;
}


ResponseCode DefTreeHandler::sendErrorFileNotFound( 
	HttpResponse * ipoHttpResponse, const std::string &filename )
{
		ostringstream oss;
	
		oss << "<html><head><title>FILE NOT FOUND</title></head>\n";
		oss << "<body>\n";
		oss << "<center>\n";
		
		oss << "ERROR - FILE NOT FOUND<br><br>\n";
		
		
		oss << "</center>\n";
		oss << "</body>\n";
		oss << "</html>\n";
			
		std::string bodyText = oss.str();
		ipoHttpResponse->SetBody( bodyText.c_str(), bodyText.length() );
		
		return HTTPRESPONSECODE_404_NOTFOUND;
}

void DefTreeHandler::recurseThroughDefTree( 
	int myID, DefFileGroup *myGroup, ostringstream &oss )
{
	std::list<DefFileGroup *>::iterator groupIter = myGroup->children.begin();
	for( ; groupIter != myGroup->children.end(); groupIter++ )
	{
		DefFileGroup *childGroup = *groupIter;
		int childID = ++jsVariableNameCounter;
		oss << "\tvar v" << childID << " = new WebFXTreeItem('" 
			<< childGroup->getName() << "');\n";
		oss << "\tv" << myID << ".add(v" << childID << ");\n";
		recurseThroughDefTree( childID, childGroup, oss );
	}

	std::list<DefFileAttrib *>::iterator attrIter = myGroup->attributes.begin();
	for( ; attrIter != myGroup->attributes.end(); attrIter++ )
	{
		DefFileAttrib *attrib = *attrIter;
		oss << "\tv" << myID << ".add(new WebFXTreeItem('" 
			<< attrib->getName() << "'));\n";
	}
}

