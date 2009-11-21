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
 *  FILENAME:   PluginSyncedRandomNumbers.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  generate pseudo-random numbers in a synchronized manner.
 *  
 *  MODIFICATION NOTES:
 *  DATE	  NAME										  SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  05/06/2004 Andrew Sampson							  MPV_CR_DR_1
 *  Initial Release.
 *
 *  2007-07-21 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#include <iostream>

#include "PluginSyncedRandomNumbers.h"


EXPORT_DYNAMIC_CLASS( PluginSyncedRandomNumbers )

// ================================================
// PluginSyncedRandomNumbers
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginSyncedRandomNumbers::PluginSyncedRandomNumbers() : Plugin() 
{
	name_ = "PluginSyncedRandomNumbers";
	licenseInfo_.setLicense( LicenseInfo::LicenseGPL );
	licenseInfo_.setOrigin( "Boeing" );

	ImsgPtr = NULL;
	frameNumber = 0;
	
	igCtrlP.setFrameNumber( &frameNumber );
	randGen.setSeed( &frameNumber );
	randGenMax = MPV_RAND_MAX;
}


// ================================================
// ~PluginSyncedRandomNumbers
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginSyncedRandomNumbers::~PluginSyncedRandomNumbers() throw() 
{
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginSyncedRandomNumbers::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{
	case SystemState::BlackboardPost:
		// This state is for posting things to the blackboard and other 
		// initialization tasks
		bb_->put( "RandomNumberGenerator", &randGen );
		bb_->put( "RandomNumberGeneratorMax", &randGenMax );
		break;

	case SystemState::BlackboardRetrieve:
		// This state is for retrieving things from the blackboard

		bb_->get( "CigiIncomingMsg", ImsgPtr );

		if( ImsgPtr != NULL )
		{
			ImsgPtr->RegisterEventProcessor(
				CIGI_IG_CTRL_PACKET_ID_V3_2,
				(CigiBaseEventProcessor *) &igCtrlP);
		}

		break;

	default:
		break;
	}
	
}

