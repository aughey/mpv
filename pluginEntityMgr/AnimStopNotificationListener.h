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


#ifndef _ANIMSTOPNOTIFICATIONLISTENER_H_
#define _ANIMSTOPNOTIFICATIONLISTENER_H_

#include "AllCigi.h"
#include "Entity.h"
#include "EntityContainer.h"

//=========================================================
//! 
//! 
class AnimStopNotificationListener : public mpv::Referenced
{
public:
	//=========================================================
	//! General Constructor
	//! 
	AnimStopNotificationListener( mpv::EntityContainer *_entities );
	
	void setOutgoingMessageBuffer( CigiOutgoingMsg *_outgoing );
	
protected:
	//=========================================================
	//! General Destructor
	//! 
	virtual ~AnimStopNotificationListener();
	
	void entityAdded( mpv::EntityContainer *, mpv::Entity *entity );
	void animationStoppedPlaying( mpv::Entity *entity );
	
	mpv::RefPtr<mpv::EntityContainer> entities;
	CigiOutgoingMsg *outgoing;
	
};

#endif
