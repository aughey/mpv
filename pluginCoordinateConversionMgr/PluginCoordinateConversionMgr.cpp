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


#include <iostream>
#include <string.h> // for memcpy

#include "PluginCoordinateConversionMgr.h"


EXPORT_DYNAMIC_CLASS( PluginCoordinateConversionMgr )

// ================================================
// PluginCoordinateConversionMgr
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginCoordinateConversionMgr::PluginCoordinateConversionMgr() : Plugin()
{
	name_ = "PluginCoordinateConversionMgr";

	licenseInfo_.setLicense( LicenseInfo::LicenseLGPL );
	licenseInfo_.setOrigin( "AndrewSampson" );

	dependencies_.push_back( "PluginDefFileReader" );
	dependencies_.push_back( "PluginTerrainMgr" );

	configData = NULL;
	terrainDatabases = NULL;
	LoadedDatabaseNumber = NULL;
	CommandedDatabaseNumber = NULL;
	
	coordinateConverterProxy = new CoordinateConverterProxy();
	previousLoadedDatabaseNumber = -128;
}


// ================================================
// ~PluginCoordinateConversionMgr
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginCoordinateConversionMgr::~PluginCoordinateConversionMgr() throw() 
{
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginCoordinateConversionMgr::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{
	case SystemState::BlackboardPost:
		// This state is for posting things to the blackboard and other 
		// initialization tasks
		
		bb_->put( "CoordinateConverterMap", &coordinateConverters );
		
		bb_->put( "CoordinateConverterProxy", (CoordinateConverter *)coordinateConverterProxy.get() );

		break;

	case SystemState::BlackboardRetrieve:
		// This state is for retrieving things from the blackboard

		bb_->get( "DefinitionData", configData );

		bb_->get( "TerrainDatabases", terrainDatabases );

		bb_->get( "LoadedDatabaseNumber", LoadedDatabaseNumber );
		bb_->get( "CommandedDatabaseNumber", CommandedDatabaseNumber );

		break;

	case SystemState::ConfigurationProcess:
		processConfigData();
		break;

	case SystemState::Operate:
	case SystemState::Debug:
	case SystemState::DatabaseLoad:
	case SystemState::Reset:
		
		// Check to see if the loaded database has changed.  If so, 
		// then update the coordinate conversion proxy.
		if( previousLoadedDatabaseNumber != *LoadedDatabaseNumber )
		{
			databaseChanged( *LoadedDatabaseNumber );
			previousLoadedDatabaseNumber = *LoadedDatabaseNumber;
		}
		
		break;

	default:
		break;
	}
	
}


// ================================================
// processConfigData
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginCoordinateConversionMgr::processConfigData()
{
	DefFileGroup *root = *configData;

	if(root == NULL)
	{
		return;
	}

	
}


// ================================================
// databaseChanged
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginCoordinateConversionMgr::databaseChanged( int dbNum )
{
	Terrain *terrain = terrainDatabases->findTerrain( dbNum );
	if( terrain == NULL )
	{
		// error - couldn't find entry for database
		std::cerr << "Error in PluginCoordinateConversionMgr - couldn't find entry for database " 
			<< dbNum << std::endl;
	}
	else
	{
		const CoordSysParams &coordSysParams = terrain->getCoordSys();
		CoordinateSystemPair coordSystemPair( CoordSysGDC, coordSysParams.coordSys );
		CoordinateConverterMap::iterator convIter = coordinateConverters.find( coordSystemPair );
		if( convIter == coordinateConverters.end() )
		{
			// error - couldn't find an appropriate CoordinateConverter
			throw MPVException( std::string( "Error in PluginCoordinateConversionMgr" ) 
				+ " - couldn't find a coordinate converter for database \"" 
				+ terrain->getName() + "\"" );
		}
		else
		{
			coordinateConverterProxy->setDatabase( convIter->second.get(), terrain );
		}
	}
}

