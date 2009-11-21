/** <pre>
 *  MPV entity manager plugin
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
 *  2008-11-14 Andrew Sampson
 *      Initial release
 *  
 *  
 *  </pre>
 */


#include <iostream>

#include <CigiAnimationStopV3.h>

#include "BindSlot.h"

#include "AnimStopNotificationListener.h"


using namespace mpv;

// ================================================
// AnimStopNotificationListener
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
AnimStopNotificationListener::AnimStopNotificationListener( 
	mpv::EntityContainer *_entities ) : 
	Referenced(), 
	entities( _entities ), 
	outgoing( NULL )
{
	entities->addedEntity.connect( BIND_SLOT2( AnimStopNotificationListener::entityAdded, this ) );
}


// ================================================
// ~AnimStopNotificationListener
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
AnimStopNotificationListener::~AnimStopNotificationListener() 
{
	
}


void AnimStopNotificationListener::setOutgoingMessageBuffer( CigiOutgoingMsg *_outgoing )
{
	outgoing = _outgoing;
}

void AnimStopNotificationListener::entityAdded( EntityContainer *, Entity *entity )
{
	if( entity != NULL )
	{
		entity->animationFinished.connect( BIND_SLOT1( AnimStopNotificationListener::animationStoppedPlaying, this ) );
		// don't need to worry about disconnecting... we want to recv 
		// animationFinished events for the entire lifetime of the 
		// entity object
	}
}


void AnimStopNotificationListener::animationStoppedPlaying( Entity *entity )
{
	CigiAnimationStopV3 animStop;
	animStop.SetEntityID( entity->getID() );
	(*outgoing) << animStop;
std::cout << "ANIMATION STOP EVENT!  Entity " << entity->getID() << std::endl;
}



