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
 *  FILENAME:   PluginHTTPD.h
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


#ifndef PLUGIN_HTTPD_H
#define PLUGIN_HTTPD_H

#include <list>

#include "Plugin.h"
#include "Entity.h"
#include "DefFileGroup.h"

#include "ViewerServer.h"
#include "EntityListHandler.h"
#include "DefTreeHandler.h"

//=========================================================
//! This plugin is responsible for running the embedded http server.
//! 
class PluginHTTPD : public Plugin 
{
public:

	//=========================================================
	//! General Constructor
	//! 
	PluginHTTPD();

	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginHTTPD() throw();
	
	//=========================================================
	//! Used by the kernel to coordinate system initialization, and to 
	//! notify plugins when a reset event has occurred.
	//! \param stage - The current stage of initialization
	//! 
	virtual void init( InitializationStates stage );
	
	//=========================================================
	//! Called by the kernel once per-plugin, per-frame.
	//! 
	virtual void act( void );
	
	//=========================================================
	//! Called by the kernel when the program is about to exit.
	//! 
	virtual void Shutdown( ShutdownStates stage );
	
	
private:
	//=========================================================
	//! The master entity list. Retrieved from the blackboard.
	//! 
	std::list<Entity*> *entityList;
	
	//=========================================================
	//! Map containing the view parameters (used for frustum setup,
	//! entity-following, etc).  Retrieved from the blackboard.
	//! 
	std::map< int, ViewParams * > *viewParamsMap;

	//=========================================================
	//! The root of the def-file tree.  Retrieved from the blackboard.
	//! 
	DefFileGroup **defFileData;
	
	//=========================================================
	//! The EHS server object.  Responds to http requests, serves web pages, 
	//! etc.
	//! 
	ViewerServer server;
	
	//=========================================================
	//! Another EHS server object.  Handles page requests in the 
	//! "entityList" virtual directory.
	//! 
	EntityListHandler entityListHandler;
	
	//=========================================================
	//! Another EHS server object.  Handles page requests in the 
	//! "defTree" virtual directory.
	//! 
	DefTreeHandler defTreeHandler;
	
	//=========================================================
	//! If true, indicates that the ViewerServer is running (ie StartServer
	//! has been called)
	//! 
	bool serverIsRunning;
	
	//=========================================================
	//! A runing total of the number of frames rendered; used to prevent the 
	//! server from running every frame.
	//! 
	int frameCount;
};


#endif

