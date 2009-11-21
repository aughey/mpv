/** <pre>
 *  MPV Mission Functions manager plugin
 *  Copyright (c) 2007 General Dynamics Land Systems
 *  Copyright (c) 2008 The Boeing Company
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
 *  2008-08-30 Andrew Sampson
 *      Initial release.  Plugin is based in part on the GDLS 
 *      pluginMissionFuncsOSG.
 *  
 *  
 *  </pre>
 */


#include "PluginMissionFuncsMgr.h"


EXPORT_DYNAMIC_CLASS( PluginMissionFuncsMgr )

// ================================================
// PluginMissionFuncsMgr
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginMissionFuncsMgr::PluginMissionFuncsMgr() : Plugin(), 
	hoatRequestProc( this ),
	losSegmentRequestProc( this ),
	losVectorRequestProc( this )
{
	name_ = "PluginMissionFuncsMgr";
	licenseInfo_.setLicense( LicenseInfo::LicenseLGPL );
	licenseInfo_.setOrigin( "GDLS+Boeing" );
	
	dependencies_.push_back( "PluginDefFileReader" );
	dependencies_.push_back( "PluginCoordinateConversionMgr" );
	dependencies_.push_back( "PluginEntityMgr" );
	
	// bb lookups
	DefFileData = NULL;
	OmsgPtr = NULL;
	ImsgPtr = NULL;
	allEntities = NULL;
	coordinateConverter = NULL;
	
	haveCalledSetup = false;
	hoatDispatcher = new HOATDispatcher();
	losDispatcher = new LOSDispatcher();
}


// ================================================
// ~PluginMissionFuncsMgr
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginMissionFuncsMgr::~PluginMissionFuncsMgr() throw() 
{
	
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginMissionFuncsMgr::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{
	case SystemState::BlackboardPost:
		bb_->put( "MissionFunctionsWorkers", &workers );
//		bb_->put( "NewHOTRequestSignal", &hoatDispatcher->newHOTRequest );
//		bb_->put( "NewLOSRequestSignal", &losDispatcher->newLOSRequest );
		
		break;

	case SystemState::BlackboardRetrieve:
		bb_->get("DefinitionData", DefFileData);

		bb_->get( "CigiOutgoingMsg", OmsgPtr );
		bb_->get( "CigiIncomingMsg", ImsgPtr );
		
		bb_->get( "AllEntities", allEntities );
		
		bb_->get( "CoordinateConverterProxy", coordinateConverter );
		
		ImsgPtr->RegisterEventProcessor( CIGI_HAT_HOT_REQ_PACKET_ID_V3_2, &hoatRequestProc );
		ImsgPtr->RegisterEventProcessor( CIGI_LOS_SEG_REQ_PACKET_ID_V3_2, &losSegmentRequestProc );
		ImsgPtr->RegisterEventProcessor( CIGI_LOS_VECT_REQ_PACKET_ID_V3_2, &losVectorRequestProc );
		break;

	case SystemState::ConfigurationProcess:
		if( !haveCalledSetup )
		{
			setup();
			haveCalledSetup = true;
		}
		getConfig();
		break;

	case SystemState::Operate:
	case SystemState::Debug:
		hoatDispatcher->sendResponses( OmsgPtr );
		losDispatcher->sendResponses( OmsgPtr );
		break;

	case SystemState::DatabaseLoad:
	case SystemState::Reset:
	case SystemState::Shutdown:
		// fixme - flush all requests
		break;

	default:
		break;
	}
}


// ================================================
// getConfig
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginMissionFuncsMgr::getConfig( void ) 
{
	DefFileGroup *root = *DefFileData;
	if(!root) return;

	DefFileGroup *mission_functions_group = root->getGroupByURI( "/mission_functions/" );
	if(!mission_functions_group) return;

	DefFileAttrib *attr = 0;

	attr = mission_functions_group->getAttribute("override_entity_material");
	if( attr )
	{
		losDispatcher->setEntityMaterialOverride( attr->asInt() );
	}

	attr = mission_functions_group->getAttribute("override_terrain_material");
	if( attr )
	{
		hoatDispatcher->setTerrainMaterialOverride( attr->asInt() );
		losDispatcher->setTerrainMaterialOverride( attr->asInt() );
	}

}


// ================================================
// setup
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginMissionFuncsMgr::setup()
{
	// some one-time setup stuff occurs here
	
	hoatDispatcher->setCoordConverter( coordinateConverter );
	losDispatcher->setCoordConverter( coordinateConverter );
	
	hoatDispatcher->setEntityContainer( allEntities );
	losDispatcher->setEntityContainer( allEntities );
	
	// for each of the workers posted to the blackboard, register it with the 
	// dispatchers
	std::list< mpv::RefPtr< mpv::MissionFunctionsWorker > >::iterator iter;
	for( iter = workers.begin(); iter != workers.end(); iter++ )
	{
		hoatDispatcher->registerWorker( iter->get() );
		losDispatcher->registerWorker( iter->get() );
	}
}



