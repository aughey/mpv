/** <pre>
 *  The MPV Entity Manager plugin
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
 *  2008-08-24 Andrew Sampson
 *      Initial release.  Replaces the old ProcArtPart class.  Uses the 
 *      new Articulation interface.
 *  
 *  </pre>
 */


#ifndef _PROCARTPART_H_
#define _PROCARTPART_H_

#include "AllCigi.h"
#include "Entity.h"
#include "EntityContainer.h"

//=========================================================
//! Processes ArtPart Control packets
//! 
class ProcArtPart : public CigiBaseEventProcessor
{
public:
	//=========================================================
	//! General Constructor
	//! 
	ProcArtPart( mpv::EntityContainer *entContainer );
	
	//=========================================================
	//! General Destructor
	//! 
	virtual ~ProcArtPart();
	
	//=========================================================
	//! Callback; processes a packet
	//! \param Packet the packet to process; should be an ArtPart Control
	//! 
	virtual void OnPacketReceived( CigiBasePacket *Packet );

private:
	
	//=========================================================
	//! Pointer to entity container, passed in from plugin
	//! 
	mpv::EntityContainer *allEntities;
};

#endif
