/** <pre>
 *  MPV coordinate conversion manager plugin
 *  Copyright (c) 2008 Andrew Sampson
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
 *  2008-07-06 Andrew Sampson
 *      Initial release
 *  
 *  
 *  </pre>
 */


#ifndef _PLUGINCOORDINATECONVERSIONMGR_H_
#define _PLUGINCOORDINATECONVERSIONMGR_H_

#include <string>
#include <map>

#include "RefPtr.h"
#include "Plugin.h"
#include "DefFileGroup.h"
#include "TerrainContainer.h"
#include "CoordinateConverter.h"
#include "CoordinateConverterProxy.h"

using namespace mpv;

//=========================================================
//! This plugin acts a broker for CoordinateConverters.  Plugins that provide 
//! CoordinateConverters will add those converters to the map provided by 
//! this plugin.  Plugins that require coordinate conversion will use the 
//! coordinate converter provided by this plugin, which acts as a proxy and 
//! insulates client plugins from the details of database changes.
//! 
class PluginCoordinateConversionMgr : public Plugin 
{
public:
	//=========================================================
	//! General Constructor
	//! 
	PluginCoordinateConversionMgr();

	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginCoordinateConversionMgr() throw();
	
	//=========================================================
	//! The per-frame processing that this plugin performs.
	//! This plugin's act() does ...
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );
	
	
private:
	
	//=========================================================
	//! Called when the database changes.  Updates the coordinate conversion 
	//! proxy with the current database parameters and an appropriate coord. 
	//! converter (provided that these are available).
	//! \param dbNum the new database ID
	//! 
	void databaseChanged( int dbNum );

	//=========================================================
	//! Pulls info from the configuration data.  Currently unused.
	//! 
	void processConfigData();

	//=========================================================
	//! The root of the def-file tree.  Retrieved from the blackboard.
	//! 
	DefFileGroup **configData;
	
	//=========================================================
	//! A container containing the known terrain databases.  Retrieved from the 
	//! blackboard.
	//!
	mpv::TerrainContainer *terrainDatabases;

	//=========================================================
	//! The loaded database number.  This is the number for the database 
	//! that is currently in memory.  Retrieved from the blackboard.
	//!
	int *LoadedDatabaseNumber;

	//=========================================================
	//! The commanded database number, as set by the Host.  Retrieved from 
	//! the blackboard.
	//!
	int *CommandedDatabaseNumber;

	typedef std::map< 
		CoordinateSystemPair, 
		RefPtr<CoordinateConverter> > CoordinateConverterMap;

	//=========================================================
	//! The coordinate converters.  Posted to the blackboard.
	//! 
	CoordinateConverterMap coordinateConverters;

	//=========================================================
	//! A coordinate converter that updates automatically when the terrain 
	//! database changes.  Uses converters found in coordinateConverters.  
	//! Posted to the blackboard.
	//! 
	RefPtr<CoordinateConverterProxy> coordinateConverterProxy;

	//=========================================================
	//! Variable used to track changes to LoadedDatabaseNumber
	//! 
	int previousLoadedDatabaseNumber;
};

#endif

