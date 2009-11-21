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
 *  FILENAME:   PluginManager.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *   manage the plugins.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  03/29/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#ifndef _PLUGIN_MANAGER_INCLUDED_
#define _PLUGIN_MANAGER_INCLUDED_

#include <list>
#include <string>

#include "SystemState.h"
#include "StateContext.h"
#include "Plugin.h"
#include "Blackboard.h"
#include "MPVTimer.h"


//=========================================================
//! A structure for the plugin list.<br>
//!   This contains pointers to the specific plugin object,
//!   the creator routine, the destroyer routine, the
//!   dynamic library handle, and the name.
//!
typedef struct {
	Plugin * plugin;
	std::string name;
	bool wasLoadedImplicitly;
} PluginWrap;



//=========================================================
//! The class manages the plugins.
//!
class PluginManager 
{
public:


	//==> Management

	//=========================================================
	//! General Constructor
	//!
	PluginManager() ;

	//=========================================================
	//! General Destructor
	//!
	~PluginManager() ;


	//==> management methods


	//=========================================================
	//! Sets the pointer to the blackboard
	//! \param bb - A pointer to the blackboard
	//!
	void setBlackboard( Blackboard *bb ) ;

	//=========================================================
	//! Loads all the specified and dependency identified plugins
	//!
	void loadPlugins( void ) ;

	//=========================================================
	//! The per-frame plugin processing
	//!
	void act( SystemState::ID state, StateContext &stateContext );

	//=========================================================
	//! Closes and deletes all plugins
	//!
	void closeAllPlugins( void ) ;
	
	
private:


	//=========================================================
	//! Loads and initializes a specific plugin
	//! \param pluginName - The name of the plugin to load.
	//!
	bool loadAndInitPlugin( const char * pluginName ) ;

	//=========================================================
	//! Removes list items from list1 that are in list 2 and returns
	//!  the result
	//! \param list1 - The first list.
	//! \param list2 - The second list.
	//!
	//! \return The resulting "unique items from list 1" list
	//!
	std::list<std::string> subtractList( 
		std::list<std::string> &list1, std::list<std::string> &list2 ) ;

	//=========================================================
	//! Sorts the plugin list
	//! \param pluginOrderList - The list of plugins.
	//!
	void sortPluginList( std::list<std::string> &pluginOrderList );

	//=========================================================
	//! Loads the plugin configuration file
	//! \param filename - The plugin configuration file's name.
	//!
	void loadConfig( std::string filename ) ;

	//=========================================================
	//! Sets up the pluginTimer, and a sub-timer for each of the plugins
	//!
	void initTimers();

	//=========================================================
	//! Prints a list of plugins
	//!
	void printPlugins() ;


	//=========================================================
	//! A pointer to the blackboard object
	//!
	Blackboard *bb_;

	//=========================================================
	//! The list of plugins
	//!
	std::list<PluginWrap> pluginList;

	//=========================================================
	//! An unsorted list of plugins; used during the plugin-loading process
	//!
	std::list<PluginWrap> unsortedPluginList;

	//=========================================================
	//! A list of loaded plugins
	//!
	std::list<std::string> loadedPlugins;

	//=========================================================
	//! A list of plugins that are to be loaded
	//!
	std::list<std::string> pendingPlugins;

	//=========================================================
	//! The main execution timer for the plugin manager
	//!
	MPVTimer *pluginsTimer;

	//=========================================================
	//! A pointer to the system list of execution timers; retrieved 
	//! from the blackboard
	//!
	std::list<MPVTimer *> *timerList;
	
	//=========================================================
	//! a boolean indicating whether the plugin timers are enabled
	//!
	bool timersEnabled;


};

#endif    // _PLUGIN_MANAGER_INCLUDED_

