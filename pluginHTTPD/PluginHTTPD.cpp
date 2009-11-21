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
 *  FILENAME:   PluginHTTPD.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  run a small embedded http server.
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
#include <iostream>

#include "PluginHTTPD.h"

EXPORT_DYNAMIC_CLASS( PluginHTTPD )

// ================================================
// PluginHTTPD
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginHTTPD::PluginHTTPD() : Plugin() 
{
	name_ = "PluginHTTPD";
	licenseInfo_.setLicense( LicenseInfo::LicenseGPL );
	licenseInfo_.setOrigin( "Boeing" );
	dependencies_.push_back( "PluginEntityMgr" );
	dependencies_.push_back( "PluginViewMgr" );

	entityList = NULL;
	viewParamsMap = NULL;
	defFileData = NULL;
	serverIsRunning = false;
	frameCount = 0;
}


// ================================================
// ~PluginHTTPD
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginHTTPD::~PluginHTTPD() throw() 
{
}


// ================================================
// init
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginHTTPD::init( InitializationStates stage ) 
{
	switch( stage ) {

	case isPost:
		// stage 1 is for posting things to the blackboard and other 
		// initialization tasks
		
		break;

	case isRetrieve:
		// stage 2 is for retrieving things from the blackboard

		// get the entity list from the BB
		if( bb_->contains( "EntityList" ) ) {
			entityList = (std::list<Entity*> *)bb_->get( "EntityList" );
		}

		// get the view parameters
		if( bb_->contains( "ViewParamsMap" ) ) {
			viewParamsMap = (std::map< int, ViewParams *> *)bb_->get( "ViewParamsMap" );
		}

		// get the configuration info
		if( bb_->contains( "DefinitionData" ) )
			defFileData = (DefFileGroup **)(bb_->get( "DefinitionData" ));

		break;

	case isInit:
		break;

	default:  // isRead, isParse, and isSave are not used
		break;
	}
	
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginHTTPD::act( void ) 
{
	if( !serverIsRunning )
	{
		EHSServerParameters esp;
		
		esp["port"] = "8080";
		esp["mode"] = "singlethreaded";
		
		server.StartServer( esp );
		serverIsRunning = true;
		
		server.RegisterEHS( &entityListHandler, "entityList" );
		server.RegisterEHS( &defTreeHandler, "defTree" );
		
		entityListHandler.setEntityList( entityList );
		entityListHandler.setViewParamsMap( viewParamsMap );
		defTreeHandler.setDefTree( defFileData );
	}
	
	// the server's request-handling code is run only once every N frames
	if( (frameCount % 20) == 0 )
	{
		// the server will check for requests, send responses, and return
		server.HandleData();
	}
	frameCount++;
}


// ================================================
// Shutdown
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginHTTPD::Shutdown( ShutdownStates stage ) 
{
	if( serverIsRunning )
	{
		server.StopServer();
		serverIsRunning = false;
	}
}


