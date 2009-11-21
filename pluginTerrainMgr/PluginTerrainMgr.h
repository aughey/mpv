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
 *  04/04/2004 Andrew Sampson
 *      Initial Release.
 *  
 *  2008-10-05 Andrew Sampson
 *      Renamed plugin from PluginDatabaseMgr to PluginTerrainMgr.  Modified 
 *      to make use of Terrain class instead of old DatabaseParams class.
 *  
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#ifndef PLUGIN_TERRAIN_MGR_H
#define PLUGIN_TERRAIN_MGR_H

#include <string>

#include "Plugin.h"
#include "Terrain.h"
#include "TerrainContainer.h"
#include "DefFileParser.h"
#include "AllCigi.h"


//=========================================================
//! This plugin initiates database-changes and manages the various 
//! coordinate-system parameters associated with each database.
//!
class PluginTerrainMgr : public Plugin 
{
public:

	//=========================================================
	//! General Constructor
	//!
	PluginTerrainMgr();

	//=========================================================
	//! General Destructor
	//!
	virtual ~PluginTerrainMgr() throw();


	//=========================================================
	//! The per-frame processing that this plugin performs
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );

private:

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

	//=========================================================
	//! The reported database number; this is the number sent to the Host.  
	//! Retrieved from the blackboard.
	//!
	int *ReportedDatabaseNumber;

	//=========================================================
	//! The default database number; this is set by the user in the config 
	//! files.  Retrieved from the blackboard.
	//!
	int *DefaultDatabaseNumber;

	//=========================================================
	//! A pointer to a pointer to the configuration file data.  Retrieved 
	//! from the blackboard.
	//!
	DefFileGroup **DefinitionData;


	//=========================================================
	//! A terrain container, containing all Terrain objects.  This container 
	//! includes Terrain objects created on-the-fly as a result of the Host 
	//! requesting unknown databases.  Posted to the blackboard.
	//!
	mpv::RefPtr<mpv::TerrainContainer> terrainDatabases;


	//=========================================================
	//! 
	//!
	mpv::RefPtr<mpv::Terrain> currentTerrain;
	
	//! This plugin needs to differentiate the first frame in the 
	//! DatabaseLoad state from subsequent frames.  
	bool isDatabaseLoadTransitionPending;

	//=========================================================
	//! Loads and processes the database specific definition data
	//!   from the definition data construct
	//!
	void processConfigData();

	//=========================================================
	//! Creates a Terrain based on the contents of the specified 
	//! config data group.
	//! \param group - the config group to pull data from
	//!
	void handleDatabaseConfigGroup( DefFileGroup *group );

	//=========================================================
	//! Maps coordinate system strings to enums
	//! \param coordSys - string containing the coordinate system type
	//! \return an enum
	//!
	CoordinateSystem stringToCoordinateSystem( const std::string &coordSys );

	//=========================================================
	//! Pulls Transverse Mercator parameters from the config data structure
	//! \param dbParam - the parameters to populate
	//! \param group - config data group
	//!
	void handleCoordSysSpecificParamsTM( CoordSysParams &coordSysParams, DefFileGroup *group );

	//=========================================================
	//! Pulls Easy Transverse Mercator parameters from the config data structure
	//! \param dbParam - the parameters to populate
	//! \param group - config data group
	//!
	void handleCoordSysSpecificParamsEasyTM( CoordSysParams &coordSysParams, DefFileGroup *group );

	//=========================================================
	//! Pulls Universal Transverse Mercator parameters from the config data 
	//! structure
	//! \param dbParam - the parameters to populate
	//! \param group - config data group
	//!
	void handleCoordSysSpecificParamsUTM( CoordSysParams &coordSysParams, DefFileGroup *group );

	//=========================================================
	//! Pulls Lambert Conformal Conic parameters from the config data structure
	//! \param dbParam - the parameters to populate
	//! \param group - config data group
	//!
	void handleCoordSysSpecificParamsLCC( CoordSysParams &coordSysParams, DefFileGroup *group );

	//=========================================================
	//! Deletes all the entries in dbParamMap
	//!
	void clearAllDatabaseParams();

};



#endif

