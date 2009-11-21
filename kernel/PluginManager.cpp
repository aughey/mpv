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
 *  FILENAME:   PluginManager.cpp
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

#include <iostream>
#include <algorithm>
#include <math.h>

#include <DynamicLoader.hpp>
#include <LoaderException.hpp>

#include "Blackboard.h"
#include "DefFileParser.h"
#include "MPVExceptions.h"
#include "PluginManager.h"


// ================================================
// PluginManager
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginManager::PluginManager()
{
	bb_ = NULL;

	pluginsTimer = NULL;
	timerList = NULL;
	timersEnabled = true;

}


// ================================================
// ~PluginManager
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginManager::~PluginManager()
{

	// shut down the timers
	if( pluginsTimer != NULL )
	{
		delete pluginsTimer;
		pluginsTimer = NULL;
	}

	// shut down the plugins
	closeAllPlugins();
	
	pluginList.clear();
	loadedPlugins.clear();
	pendingPlugins.clear();
	unsortedPluginList.clear();
}


// ================================================
// setBlackboard
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginManager::setBlackboard( Blackboard *bb )
{
	bb_ = bb;
}


// ================================================
// loadPlugins
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginManager::loadPlugins( void )
{
	std::string *defFileDirectoryPtr = NULL;
	bb_->get( "DefFileDirectory", defFileDirectoryPtr );
	std::string defFileDirectory = *defFileDirectoryPtr;
	try
	{
		loadConfig( defFileDirectory + "plugins.def" );
	}
	catch( std::exception &e )
	{
		std::cerr << "Exception - " << e.what() << std::endl;
		std::cerr << "Error - the MPV kernel was unable to open the 'plugins.def' file.\n" 
			<< "If the def file is missing, then you will need to find a copy or create a new \n" 
			<< "one.  If the def file is not missing, then you probably forgot to set the \n" 
			<< "DEF_FILE_PATH environmental variable.  Just set the variable to the name of the \n" 
			<< "directory where you keep your MPV configuration files.\n";
		exit( 1 );
	}
	
	// make a copy of the pendingPlugins list; we will need this when 
	// sorting the pluginList
	std::list<std::string> pluginOrderList( pendingPlugins );

	// load plugins
	std::list<std::string>::iterator iter = pendingPlugins.begin();
	while( iter != pendingPlugins.end() ) 
	{
		if( !loadAndInitPlugin( (*iter).c_str() ) )
		{
			std::cerr << "Could not load plugin \"" << *iter << "\", exiting\n";
			exit( 1 );
		}
		else
		{
			loadedPlugins.push_back( *iter );
			pendingPlugins.erase( iter );
			iter = pendingPlugins.begin();
		}
	}

	// sort and populate the pluginList using pluginOrderList and 
	// unsortedPluginList
	sortPluginList( pluginOrderList );

	// clean up
	loadedPlugins.clear();
	pendingPlugins.clear();
	unsortedPluginList.clear();
	
	// print out the plugins that have been loaded
	printf( "The following plugins have been loaded:\n" );
	printPlugins();
	
	// set up the timers
	initTimers();
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginManager::act( SystemState::ID state, StateContext &stateContext )
{
	std::list<MPVTimer *> *subtimers = pluginsTimer->GetSubTimers();
	std::list<MPVTimer *>::iterator timerIter = subtimers->begin();

	if( timersEnabled )
		pluginsTimer->Begin();

	for( std::list<PluginWrap>::iterator pluginIter = pluginList.begin();
		pluginIter != pluginList.end(); pluginIter++ )
	{
		pluginIter->plugin->act( state, stateContext );

		if( timersEnabled && ( timerIter != subtimers->end() ) )
		{
			(*timerIter)->Time();
			timerIter++;
		}
	}

	if( timersEnabled )
	{
		pluginsTimer->End();
	}
}


// ================================================
// closeAllPlugins
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginManager::closeAllPlugins( void )
{
	// shut down the timers
	if(pluginsTimer != NULL)
	{
		// Delete and clear the subtimers
		pluginsTimer->ClearSubTimers();

		delete pluginsTimer;
		pluginsTimer = NULL;
	}

	/*
	PDL will perform the plugin-object destruction and plugin-library 
	closing in separate passes.  It is possible that a plugin has created 
	objects that are reference-counted (such as scene graph nodes) and are not 
	necessarily destroyed until *all* plugin objects are destroyed.  The 
	libraries must remain in memory until all objects are destroyed, so that 
	the objects' destructors remain valid.
	
	PDL performs this multi-stage shutdown under-the-hood; nothing extra 
	needs to be done here.
	*/
	PDL::DynamicLoader::Instance().Reset();
	
	pluginList.clear();
}


// PRIVATE /////////////////////////////////////////////////////////////////


// ================================================
// loadAndInitPlugin
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
bool PluginManager::loadAndInitPlugin( const char * pluginName )
{
	std::string pluginLibraryName = std::string( "lib" ) + pluginName;
	std::string pluginClassName = pluginName;
	
	PluginWrap pluginWrap;
	
	pluginWrap.plugin = NULL;
	pluginWrap.name = pluginClassName;//pluginLibraryName;
	pluginWrap.wasLoadedImplicitly = false;
	
	try
	{
		PDL::DynamicLoader &dynamicLoader = PDL::DynamicLoader::Instance();
		pluginWrap.plugin = dynamicLoader.GetClassInstance<Plugin>( 
			pluginLibraryName.c_str(), pluginClassName.c_str() );
	}
	catch( PDL::LoaderException &ex )
	{
		std::cerr << "There was an error while opening plugin file \""
			<< pluginLibraryName << "\".\n"
			<< "The reported error is: " << ex.what() << std::endl;
		return false;
	}
	
	if( pluginWrap.plugin == NULL )
	{
		std::cerr << "There was an error while creating the plugin object \""
			<< pluginClassName << "\".\n";
		return false;
	}

	pluginWrap.plugin->setBlackboard( bb_ );
	
	// Retrieve the dependencies from the plugin and merge them with the 
	// list of plugins that are to be loaded.
	std::list<std::string> depList = pluginWrap.plugin->getDependencies();

	// remove items from the dependency list that already exist in the 
	// loaded-plugins list
	depList = subtractList( depList, loadedPlugins );

	// remove items from the dependency list that already exist in the 
	// pending-plugins list
	depList = subtractList( depList, pendingPlugins );

	pendingPlugins.merge( depList );
	
	unsortedPluginList.push_back( pluginWrap );
	
	return true;
}


// ================================================
// subtractList
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
std::list<std::string> PluginManager::subtractList( 
	std::list<std::string> &list1, std::list<std::string> &list2 ) 
{
	std::list<std::string> outlist;

	// it is important that both lists are sorted
	list1.sort();
	list2.sort();

	std::set_difference( 
		list1.begin(), list1.end(), 
		list2.begin(), list2.end(), 
		std::insert_iterator<std::list<std::string> > ( outlist, outlist.begin() ) );
	return outlist;
}



// ================================================
// sortPluginList
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginManager::sortPluginList( std::list<std::string> &pluginOrderList )
{
	// we must move the contents of unsortedPluginList to pluginList, using 
	// pluginOrderList as our guide
	
	for( std::list<std::string>::iterator olIter = pluginOrderList.begin();
		olIter != pluginOrderList.end(); olIter++ )
	{
		std::list<PluginWrap>::iterator pwIter;

		// find the corresponding PluginWrap in unsortedPluginList
		for( pwIter = unsortedPluginList.begin();
			pwIter != unsortedPluginList.end(); pwIter++ )
		{
			if( (*pwIter).name == (*olIter) )
				break;
		}
		
		if( pwIter == unsortedPluginList.end() ) {
			printf( "There was a serious error while sorting the list "
					"of plugins.\nAttempting to continue.\n" );
			continue;
		}
		
		// remove the PW from unsortedPluginList and add it to pluginList
		pluginList.push_back( *pwIter );
		unsortedPluginList.erase( pwIter );
	}
	
	// for items remaining in unsortedPluginList, we need to set a flag to 
	// indicate that the plugin was loaded implicitly, and then add the 
	// PW to the end of pluginList
	std::list<PluginWrap>::iterator pwIter = unsortedPluginList.begin();
	while( pwIter != unsortedPluginList.end() )
	{
		(*pwIter).wasLoadedImplicitly = true;
		pluginList.push_back( *pwIter );
		pwIter = unsortedPluginList.erase( pwIter );
	}
}


// ================================================
// loadConfig
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginManager::loadConfig( std::string filename )
{
	DefFileParser parser;
	DefFileGroup *root = parser.parse( filename.c_str() );
	
	if( root == NULL )
	{
		throw MPVFileIOReadException( std::string( "Couldn't parse \"" ) + filename + "\"" );
		return;
	}

	for( std::list< DefFileGroup * >::iterator iter = root->children.begin();
		iter != root->children.end(); iter++ )
	{
		DefFileGroup *group = (*iter);
		if( group->getName() == "plugins" ) 
		{
			for( std::list< DefFileAttrib * >::iterator attrIter = 
				group->attributes.begin();
				attrIter != group->attributes.end(); attrIter++ )
			{
				if( (*attrIter)->getName() == "filename" ) {
					pendingPlugins.push_back( (*attrIter)->asString() );
				}
			}
		}
		else
		{
			// ignore non-plugins groups
		}
	}
	
	delete root;
}


// ================================================
// initTimers
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginManager::initTimers()
{
	// retrieve the timer list
	if( timerList == NULL )
	{
		if( bb_->contains( "TimerList" ) )
		{
			bb_->get( "TimerList", timerList );
		}
	}
	
	if( pluginsTimer == NULL )
	{
		GenerateID *genID = NULL;
		bb_->get( "GenerateID", genID );
		pluginsTimer = new MPVTimer( "PluginMgr", true, genID );
		timerList->push_back( pluginsTimer );
	}
	else
	{
		// Delete and clear the subtimers
		pluginsTimer->ClearSubTimers();
	}
	
	// for each plugin, create a timer and add it to the system
	for( std::list<PluginWrap>::iterator iter = pluginList.begin();
		iter != pluginList.end(); iter++ ) 
	{
		pluginsTimer->AddSubTimer( iter->plugin->name_ );
	}
}


// ================================================
// printPlugins
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginManager::printPlugins() 
{
	bool foundImplicitlyLoadedPlugin = false;
	for( std::list<PluginWrap>::iterator tempiter = pluginList.begin();
		tempiter != pluginList.end(); tempiter++ ) 
	{
		std::cout << "\t" << (*tempiter).name;
		if( (*tempiter).wasLoadedImplicitly )
		{
			std::cout << "   (implicit)";
			foundImplicitlyLoadedPlugin = true;
		}
		std::cout << std::endl;
	}
	
	if( foundImplicitlyLoadedPlugin )
	{
		std::cout 
		<< "\n"
		<< "** Note: The plugins marked with \"(implicit)\" were loaded implicitly \n"
		<< "** in order to satisfy a dependency.  \n"
		<< "** These plugins have been placed at the end of the plugin list.  \n"
		<< "** This means that the order in which the plugins are executed \n"
		<< "** may not match what you are expecting.  While this is not a \n"
		<< "** problem in most cases, this will sometimes cause the system to \n"
		<< "** behave strangely.  It is recommended that you add the plugin \n"
		<< "** to the list of plugins in your config file.  In doing so, you \n"
		<< "** maintain control over the order in which plugins are executed.\n"
		<< "** The sample \"plugins.def\" that comes with the MPV contains comments \n"
		<< "** explaining the recommended plugin ordering.\n"
		<< "\n";
	}
}

