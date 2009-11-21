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
 *  FILENAME:   PluginCigiManager.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  implement a simple manager.  A manager is equivalent to 
 *  the "Client" in Mantis nomenclature, ie it is a piece of software 
 *  that speaks directly with a Host, and passes messages back and forth 
 *  between the IG's and the Host.  In this manner, it acts as a proxy.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  07/27/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#include <stdio.h>
#include <ios>
#include <iostream>
#include <math.h>

#ifdef WIN32
#define snprintf _snprintf
#endif

#include "PluginCigiManager.h"

#ifdef FORWARD_MOTION_TRACKER_INFO
#include <osg/Matrix>
#include <osg/Vec3>
#include <CigiCompCtrlV3.h>
#include "AllCigi.h"

#endif




EXPORT_DYNAMIC_CLASS( PluginCigiManager )

// ================================================
// PluginCigiManager
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginCigiManager::PluginCigiManager() : Plugin() 
{
	name_ = "PluginCigiManager";
	licenseInfo_.setLicense( LicenseInfo::LicenseGPL );
	licenseInfo_.setOrigin( "Boeing" );
	
	dependencies_.push_back( "PluginDefFileReader" );
#ifdef FORWARD_MOTION_TRACKER_INFO
	dependencies_.push_back( "PluginMotionTrackerMgr" );

	trackerParamsMap = NULL;
#endif

	sendNetMessagesCB = NULL;

	h2iSignalProc.setIGConnectionList( &igConnections );
	
}


// ================================================
// ~PluginCigiManager
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginCigiManager::~PluginCigiManager() throw() 
{
	deleteIGConnections();
}


// ================================================
// init
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginCigiManager::init( InitializationStates stage ) 
{
	switch( stage ) {

	case isPost:
		// stage 1 is for posting things to the blackboard and other 
		// initialization tasks
		
		break;

	case isRetrieve:
		// stage 2 is for retrieving things from the blackboard

		if( bb_->contains( "CigiIncomingMsg" ) )
			HostIMsg = (CigiIncomingMsg *)(bb_->get( "CigiIncomingMsg" ));

		if( bb_->contains( "CigiOutgoingMsg" ) )
			HostOMsg = (CigiOutgoingMsg *)(bb_->get( "CigiOutgoingMsg" ));
		
		if( bb_->contains( "DefinitionData" ) )
			DefinitionData = (DefFileGroup **)(bb_->get( "DefinitionData" ));

		// get the pointer to the boolean which tells the kernel if it should 
		// send the message or if one of the plugins will take care of it
		if( bb_->contains( "ShouldKernelSendNetMessagesBool" ) ) {
			bool *shouldKernelSendMsgs = (bool*)bb_->get( "ShouldKernelSendNetMessagesBool" );
			if( *shouldKernelSendMsgs == false )
			{
				// "false" indicates that some other plugin has already laid claim 
				// to SendNetMessagesCB.  We do nothing in this situation.
			}
			else
			{
				// get the function pointer for sending SOF
				if( bb_->contains( "SendNetMessagesCB" ) ) {
					sendNetMessagesCB = (void (*)())bb_->get( "SendNetMessagesCB" );

					// this tells the kernel and the other plugins that *this* plugin will 
					// take care of sending SOF
					*shouldKernelSendMsgs = false;
				}
			}
		}
		
#ifdef FORWARD_MOTION_TRACKER_INFO
		// get the motion tracker parameters
		if( bb_->contains( "TrackerParamsMap" ) ) 
		{
			trackerParamsMap = (std::map< int, TrackerParams *> *)bb_->get( "TrackerParamsMap" );
		}
#endif
		
		if( HostIMsg != NULL )
		{
			HostIMsg->RegisterSignalProcessor( &h2iSignalProc );
		}
		
		if( HostOMsg != NULL )
		{
//			i2hSignalProc.setOutgoingMsg( HostOMsg );
		}
		
		
		
		break;
	
	case isParse:
		// this stage tell the plugin that it's time to (re-)read the contents 
		// of the def file tree.
		getDefFileData();
		break;
	

	case isInit:
		break;

	default:  // isRead and isSave are not used
		break;
	}
	
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginCigiManager::act( void ) 
{
	std::list< IGConnection* >::iterator iter;
	for( iter = igConnections.begin(); iter != igConnections.end(); iter++ )
	{
		IGConnection *igc = (*iter);
#ifdef FORWARD_MOTION_TRACKER_INFO
		if( trackerParamsMap != NULL )
		{
			// note - only checks first tracker
			TrackerParams *trackerParams = (*trackerParamsMap)[0];
			if( trackerParams != NULL )
			{
				// note - won't cope with translation, only rotation
				osg::Matrix trackerMtx( trackerParams->transformMtx );
				osg::Quat trackerOrientation;
				trackerMtx.get( trackerOrientation );
				
				CigiCompCtrlV3 viewCompCtrl;
				
				// this is a "view" component control
				viewCompCtrl.SetCompClassV3( CigiBaseCompCtrl::ViewV3 );
				// we'll use component 0 for head tracker stuff
				viewCompCtrl.SetCompID( 0 );
				// which view to act on?  we'll assume 0.
				viewCompCtrl.SetInstanceID( 0 );
				
				// we'll set the state to the tracker number
				viewCompCtrl.SetCompState( (short int)trackerParams->trackerID );
				
				viewCompCtrl.SetCompData( (float)trackerOrientation[0], 0 );
				viewCompCtrl.SetCompData( (float)trackerOrientation[1], 1 );
				viewCompCtrl.SetCompData( (float)trackerOrientation[2], 2 );
				viewCompCtrl.SetCompData( (float)trackerOrientation[3], 3 );
				
				// add the comp ctrl packet if we're set to not wait for the 
				// Host, or if we are set to wait and the Host has sent an IGCtrl
				if( !igc->waitForHost || igc->OmsgPtr->GetValidIGCtrlSOF() )
					(*igc->OmsgPtr) << viewCompCtrl;
			}
		}
#endif
		igc->act();
	}
	
}


// ================================================
// deleteIGConnections
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginCigiManager::deleteIGConnections( void )
{
	std::list< IGConnection* >::iterator iter;
	for( iter = igConnections.begin(); iter != igConnections.end(); iter++ )
	{
		IGConnection *igc = (*iter);
		igc->network.closeSocket();
		delete igc;
	}
	igConnections.clear();
}


// ================================================
// getDefFileData
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginCigiManager::getDefFileData( void )
{
	deleteIGConnections();
	
	if( !DefinitionData || !(*DefinitionData) )
		return;
	
	DefFileGroup *mgrDefGroup = (*DefinitionData)->getGroupByURI( "/manager/" );
	if( !mgrDefGroup )
		return;

	std::list< DefFileGroup * >::iterator iterG = mgrDefGroup->children.begin();
	for( ; iterG != mgrDefGroup->children.end(); iterG++ )
	{
		DefFileGroup *grp = (*iterG);
		if( grp->getName() == "ig_connection" )
		{
			std::string addr( "130.38.180.136" );
			int port_send = 8004;
			int port_rcv = 8005;
			bool isMaster = false;
			bool waitForHost = true;
			
			DefFileAttrib *attr;
			
			attr = grp->getAttribute( "ig_addr" );
			if( attr ) {
				addr = attr->asString();
			}
			
			attr = grp->getAttribute( "ig_port" );
			if( attr ) {
				port_send = attr->asInt();
			}
			
			attr = grp->getAttribute( "listen_port" );
			if( attr ) {
				port_rcv = attr->asInt();
			}
			
			attr = grp->getAttribute( "is_master" );
			if( attr ) {
				isMaster = attr->asInt();
			}
			
			attr = grp->getAttribute( "wait_for_host" );
			if( attr ) {
				waitForHost = attr->asInt();
			}
			
			IGConnection *igc = new IGConnection;
			igConnections.push_back( igc );
			igc->network.openSocket( 
				addr.c_str(), 
				port_send, 
				port_rcv );
			igc->init( HostOMsg, sendNetMessagesCB, isMaster );
			igc->waitForHost = waitForHost;
		}
	}
	
}




