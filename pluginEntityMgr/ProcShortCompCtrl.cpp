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
 *      Initial release.  Replaces the old ProcShortCompCtrl class.  Uses the 
 *      new Component interface.
 *  
 *  </pre>
 */


#include <iostream>

#include <CigiShortCompCtrlV3_3.h>

#include "ProcShortCompCtrl.h"

using namespace mpv;

// ================================================
// ProcShortCompCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ProcShortCompCtrl::ProcShortCompCtrl( mpv::EntityContainer *entContainer ) :
	CigiBaseEventProcessor(),
	allEntities( entContainer )
{
	
}


// ================================================
// ~ProcShortCompCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ProcShortCompCtrl::~ProcShortCompCtrl() 
{
	
}


// ================================================
// OnPacketReceived
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ProcShortCompCtrl::OnPacketReceived( CigiBasePacket *Packet )
{
	CigiShortCompCtrlV3_3 *compCtrl = static_cast<CigiShortCompCtrlV3_3 *> (Packet);
	
	if( compCtrl->GetCompClassV3() != CigiBaseCompCtrl::EntityV3 )
		// not interested
		return;
	
	mpv::RefPtr<Entity> entity = allEntities->findEntity( compCtrl->GetInstanceID() );
	if( !entity.valid() )
	{
		// entity not found!
		std::cout << "Warning - in ProcShortCompCtrl::OnPacketReceived() - entity " 
			<< compCtrl->GetInstanceID() << " doesn't exist\n";
		return;
	}
	
	// check if component already exists; create one as necessary
	mpv::RefPtr<Component> component = 
		entity->findOrCreateComponent( compCtrl->GetCompID() );
	if( component->getInstanceID() != entity->getID() )
	{
		// component is freshly created, needs to have entity ID set
		component->setInstanceID( entity->getID() );
	}
	
	/*
	For now, packets are handled primarily by the Component itself.
	Ordinarily, I prefer not to do this; I opt for a more strict MVC approach, 
	i.e. a packet processor (Controller) that deals with all the CIGI stuff, 
	and objects (Model) that are manipulated but have no knowledge of where 
	the data comes from.  This is how Entity and ArtPart packets are handled.
	
	In the case of Components, however, the packet handling is complex enough 
	that I decided that it should be done in the Component class rather than 
	the packet processor.
	*/
	component->processPacket( compCtrl );
}
