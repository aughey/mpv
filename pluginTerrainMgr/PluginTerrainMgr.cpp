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


#include <iostream>

#include "PluginTerrainMgr.h"
#include "MPVExceptions.h"


using namespace mpv;

EXPORT_DYNAMIC_CLASS( PluginTerrainMgr )

// ================================================
// PluginTerrainMgr
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginTerrainMgr::PluginTerrainMgr() : Plugin(),
	terrainDatabases( new TerrainContainer() )
{
	name_ = "PluginTerrainMgr";
	licenseInfo_.setLicense( LicenseInfo::LicenseGPL );
	licenseInfo_.setOrigin( "Boeing" );

	dependencies_.push_back( "PluginDefFileReader" );

	LoadedDatabaseNumber = NULL;
	CommandedDatabaseNumber = NULL;
	ReportedDatabaseNumber = NULL;
	DefaultDatabaseNumber = NULL;
	DefinitionData = NULL;
	
	isDatabaseLoadTransitionPending = false;
}


// ================================================
// ~PluginTerrainMgr
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginTerrainMgr::~PluginTerrainMgr() throw() 
{
	
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginTerrainMgr::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{
	case SystemState::BlackboardPost:
		// This state is for posting things to the blackboard and other 
		// initialization tasks
		bb_->put( "TerrainDatabases", terrainDatabases.get() );

		break;

	case SystemState::BlackboardRetrieve:
		// This state is for retrieving things from the blackboard

		bb_->get( "LoadedDatabaseNumber", LoadedDatabaseNumber );
		bb_->get( "CommandedDatabaseNumber", CommandedDatabaseNumber );
		bb_->get( "ReportedDatabaseNumber", ReportedDatabaseNumber );
		bb_->get( "DefaultDatabaseNumber", DefaultDatabaseNumber );

		bb_->get( "DefinitionData", DefinitionData );

		break;

	case SystemState::ConfigurationProcess:
		processConfigData();
		break;
	
	case SystemState::Reset:
		*CommandedDatabaseNumber = *DefaultDatabaseNumber;
		if( currentTerrain.valid() )
		{
			// unload the terrain
			currentTerrain->unload();
			currentTerrain = NULL;
		}
		*LoadedDatabaseNumber = -128;
		
		break;
	
	case SystemState::Operate:
	case SystemState::Debug:

		*ReportedDatabaseNumber = *LoadedDatabaseNumber;

		// FIXME - The CIGI standard states that the Host can force a database 
		// to be reloaded by specifying the currently-loaded database's ID in 
		// an IGCtrl packet.  This behavior *could* be implemented, but for the 
		// moment I will ignore DB reload requests. -ADS
		if( *CommandedDatabaseNumber != *LoadedDatabaseNumber && 
			*CommandedDatabaseNumber > 0 )
		{
			Terrain *commandedTerrain = terrainDatabases->findTerrain( *CommandedDatabaseNumber );
			if( commandedTerrain == NULL )
			{
				// the Host (or the user, through the config files) has 
				// requested a database that the MPV doesn't know about
				
				std::cerr << "PluginTerrainMgr::act - Warning: the database number was set to "
					<< *CommandedDatabaseNumber << ", which is not a known database.\n";

				stateContext.databaseLoadRequested = true;
			}
			else
			{
				// the Host has requested a database that exists
				
				Terrain::TerrainState terrainState = commandedTerrain->getState();
				
				if( terrainState == Terrain::Broken )
				{
					/*
					The Host has requested a database that we know is 
					not valid.  This indicates that the Host has requested 
					this database previously, and that the request failed.
					Furthermore, this means that the Host ignored the MPV's 
					response to the previous request.  The Host, then, is in 
					violation of the CIGI standard, and is poorly written.
					I shall shame the Host authors with my stinging words.
					*/
					static bool havePrintedErrorMessage = false;
					if( !havePrintedErrorMessage && *CommandedDatabaseNumber != *DefaultDatabaseNumber )
					{
						std::cerr << std::endl;
						std::cerr << "ERROR - Database " 
							<< *CommandedDatabaseNumber
							<< " was requested.  This terrain database was requested \n"
							<< "previously, and could not be loaded.  If this request came from a " 
							<< "Host \napplication, then that application has ignored the MPV's response "
							<< "to the \ninitial request for this database.  If the Host is in fact ignoring \n"
							<< "the IG's responses to database load requests, then the Host application \n"
							<< "does not adhere to the CIGI standard and needs to be fixed.\n";
						std::cerr << "This error will be reported once, ever.\n"; 
						std::cerr << std::endl;
							
						havePrintedErrorMessage = true;
					}
				}
				else
				{
					stateContext.databaseLoadRequested = true;
				}
			}
			if( stateContext.databaseLoadRequested )
			{
				// This plugin needs to differentiate the first frame in the 
				// DatabaseLoad state from subsequent frames.  The MPV kernel 
				// will change databaseLoadRequested, so this plugin can't rely 
				// on that variable for this purpose.  
				isDatabaseLoadTransitionPending = true;
			}
		}
		break;

	case SystemState::DatabaseLoad:
		
		*ReportedDatabaseNumber = -1 * *CommandedDatabaseNumber;

		if( isDatabaseLoadTransitionPending )
		{
			// first frame in the DatabaseLoad state
			
			if( currentTerrain.valid() )
			{
				// unload the previous terrain
				currentTerrain->unload();
			}
			
			// retrieve the commanded terrain and make it current
			currentTerrain = terrainDatabases->findTerrain( *CommandedDatabaseNumber );
			
			if( !currentTerrain.valid() )
			{
				// the Host (or the user, through the config files) has 
				// requested a database that the MPV doesn't know about
				currentTerrain = new Terrain;
				currentTerrain->setName( "Unknown database" );
				currentTerrain->setID( *CommandedDatabaseNumber );
				
				terrainDatabases->addTerrain( currentTerrain.get() );
			}
			
			std::cout << "PluginTerrainMgr - about to load terrain database \""
				<< currentTerrain->getName() << "\" (id " << currentTerrain->getID() << ")\n";
			// begin loading the current terrain
			currentTerrain->load();
			
			isDatabaseLoadTransitionPending = false;
		}
		else
		{
			switch( currentTerrain->getState() )
			{
			case Terrain::Loaded:
				// success
				*LoadedDatabaseNumber = currentTerrain->getID();
				stateContext.databaseLoadComplete = true;
				break;
			case Terrain::Broken:
				// fail
				// -128 indicates an error
				*LoadedDatabaseNumber = -128;
				stateContext.databaseLoadComplete = true;
				break;
			case Terrain::NotLoaded:
			case Terrain::Loading:
			default:
				// continue waiting
				break;
			}
		}
		
		break;

	default:
		break;
	}
	
}


// ================================================
// processConfigData
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginTerrainMgr::processConfigData()
{
	if( *DefinitionData == NULL )
		return;

	std::list<DefFileGroup*>::iterator iter = (*DefinitionData)->children.begin();
	DefFileGroup *databasesGroup = (*DefinitionData)->getGroupByURI( "/databases/" );

	if( databasesGroup == NULL )
	{
		std::cerr << "Error in PluginTerrainMgr - couldn't find /databases/ in configuration data\n";
		return;
	}

	std::list<DefFileGroup*>::iterator dter = databasesGroup->children.begin();
	for( ; dter != databasesGroup->children.end(); dter++ )
	{
		DefFileGroup *dbGroup = ( *dter );
		if( dbGroup->getName() == "database" )
		{
			handleDatabaseConfigGroup( dbGroup );
		}
	}
}


// ================================================
// handleDatabaseConfigGroup
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginTerrainMgr::handleDatabaseConfigGroup( DefFileGroup *group )
{
	int id = -1;
	CoordinateSystem coordSys = CoordSysUNKNOWN;
	
	// pick out a few mandatory fields before continuing
	
	// database id
	DefFileAttrib *attr = group->getAttribute( "id" );
	if( attr )
		id = attr->asInt();
	else
		return;

	// coordinate system used for this database
	attr = group->getAttribute( "coordinate_system" );
	if( attr )
		coordSys = stringToCoordinateSystem( attr->asString() );
	else
		return;

	Terrain *terrain = new Terrain();
	CoordSysParams coordSysParams;

	terrain->setID( id );
	coordSysParams.coordSys = coordSys;

	attr = group->getAttribute( "name" );
	if( attr )
		terrain->setName( attr->asString() );

	attr = group->getAttribute( "directory" );
	if( attr )
		terrain->setDirectoryname( attr->asString() );

	attr = group->getAttribute( "filename" );
	if( attr )
		terrain->setFilename( attr->asString() );

	attr = group->getAttribute( "offset" );
	if( attr )
	{
		std::cout << "Warning in PluginTerrainMgr - in terrain database config for \""
			<< terrain->getName() << "\" (id " << terrain->getID() << "): "
			<< "the \"offset\" attribute is no longer supported\n";
		
		/*std::vector<float> offset = attr->asFloats();
		if( offset.size() >= 3 )
		{
			dbParam->offset[0] = offset[0];
			dbParam->offset[1] = offset[1];
			dbParam->offset[2] = offset[2];
		}*/
	}

	switch( coordSysParams.coordSys )
	{
	case CoordSysTM:
		handleCoordSysSpecificParamsTM( coordSysParams, group );
		break;
	case CoordSysEasyTM:
		handleCoordSysSpecificParamsEasyTM( coordSysParams, group );
		break;
	case CoordSysUTM:
		handleCoordSysSpecificParamsUTM( coordSysParams, group );
		break;
	case CoordSysLCC:
		handleCoordSysSpecificParamsLCC( coordSysParams, group );
		break;
	default:
		break;
	}

	// finally, add the terrain to the terrain container
	terrain->setCoordSys( coordSysParams );
	terrainDatabases->addTerrain( terrain );
}


// ================================================
// 
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CoordinateSystem PluginTerrainMgr::stringToCoordinateSystem( 
	const std::string &coordSys )
{
	CoordinateSystem result = CoordSysUNKNOWN;

	if( coordSys == "gcc" || coordSys == "GCC" )
		result = CoordSysGCC;
	else if( coordSys == "tm" || coordSys == "TM" )
		result = CoordSysTM;
	else if( coordSys == "easy_tm" )
		result = CoordSysEasyTM;
	else if( coordSys == "utm" || coordSys == "UTM" )
		result = CoordSysUTM;
	else if( coordSys == "lcc" || coordSys == "LCC" )
		result = CoordSysLCC;

	return result;
}


// ================================================
// 
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginTerrainMgr::handleCoordSysSpecificParamsTM( CoordSysParams &coordSysParams, DefFileGroup *group )
{
	DefFileAttrib *attr = group->getAttribute( "central_meridian" );
	if( attr )
		coordSysParams.coordSysParams.tmParams.CentralMeridian = attr->asFloat();

	attr = group->getAttribute( "false_easting" );
	if( attr )
		coordSysParams.coordSysParams.tmParams.FalseEasting = attr->asFloat();

	attr = group->getAttribute( "false_northing" );
	if( attr )
		coordSysParams.coordSysParams.tmParams.FalseNorthing = attr->asFloat();

	attr = group->getAttribute( "scale_factor" );
	if( attr )
		coordSysParams.coordSysParams.tmParams.ScaleFactor = attr->asFloat();
	else
		coordSysParams.coordSysParams.tmParams.ScaleFactor = 0.9996;
}


// ================================================
// 
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginTerrainMgr::handleCoordSysSpecificParamsEasyTM( CoordSysParams &coordSysParams, DefFileGroup *group )
{
	DefFileAttrib *attr = group->getAttribute( "origin_latitude" );
	if( attr )
		coordSysParams.coordSysParams.easyTMParams.OriginLatitude = attr->asFloat();

	attr = group->getAttribute( "origin_longitude" );
	if( attr )
		coordSysParams.coordSysParams.easyTMParams.OriginLongitude = attr->asFloat();

	attr = group->getAttribute( "central_meridian" );
	if( attr )
		coordSysParams.coordSysParams.easyTMParams.CentralMeridian = attr->asFloat();
	else
		coordSysParams.coordSysParams.easyTMParams.CentralMeridian = 
			coordSysParams.coordSysParams.easyTMParams.OriginLongitude;

	attr = group->getAttribute( "scale_factor" );
	if( attr )
		coordSysParams.coordSysParams.easyTMParams.ScaleFactor = attr->asFloat();
	else
		coordSysParams.coordSysParams.easyTMParams.ScaleFactor = 0.9996;
}


// ================================================
// 
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginTerrainMgr::handleCoordSysSpecificParamsUTM( CoordSysParams &coordSysParams, DefFileGroup *group )
{
	DefFileAttrib *attr = group->getAttribute( "zone" );
	if( attr )
		coordSysParams.coordSysParams.utmParams.Zone = attr->asInt();
}


// ================================================
// 
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginTerrainMgr::handleCoordSysSpecificParamsLCC( CoordSysParams &coordSysParams, DefFileGroup *group )
{
	DefFileAttrib *attr = group->getAttribute( "latitude_origin" );
	if( attr )
		coordSysParams.coordSysParams.lccParams.LatOrig = attr->asFloat();

	attr = group->getAttribute( "longitude_origin" );
	if( attr )
		coordSysParams.coordSysParams.lccParams.LonOrig = attr->asFloat();

	attr = group->getAttribute( "equ_lat" );
	if( attr )
		coordSysParams.coordSysParams.lccParams.Lat1 = attr->asFloat();

	attr = group->getAttribute( "pol_lat" );
	if( attr )
		coordSysParams.coordSysParams.lccParams.Lat2 = attr->asFloat();

	attr = group->getAttribute( "linear_units" );
	if( attr )
		coordSysParams.coordSysParams.lccParams.Line = attr->asInt();

	attr = group->getAttribute( "orientation_units" );
	if( attr )
		coordSysParams.coordSysParams.lccParams.Orientation = attr->asInt();

	attr = group->getAttribute( "options" );
	if( attr )
		coordSysParams.coordSysParams.lccParams.Option = attr->asInt();
}


