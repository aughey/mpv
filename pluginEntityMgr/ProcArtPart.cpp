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


#include <iostream>

#include <CigiArtPartCtrlV3.h>

#include "ProcArtPart.h"

using namespace mpv;

// ================================================
// ProcArtPart
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ProcArtPart::ProcArtPart( mpv::EntityContainer *entContainer ) : 
	CigiBaseEventProcessor(),
	allEntities( entContainer )
{
	
}


// ================================================
// ~ProcArtPart
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ProcArtPart::~ProcArtPart() 
{
	
}


// ================================================
// OnPacketReceived
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ProcArtPart::OnPacketReceived( CigiBasePacket *Packet )
{
	CigiArtPartCtrlV3 *artPartCtrl = static_cast<CigiArtPartCtrlV3 *> (Packet);
	
	mpv::RefPtr<Entity> entity = allEntities->findEntity( artPartCtrl->GetEntityID() );
	if( !entity.valid() )
	{
		// entity not found!
		std::cout << "Warning - in ProcArtPart::OnPacketReceived() - entity " 
			<< artPartCtrl->GetEntityID() << " doesn't exist\n";
		return;
	}
	
	// check if articulation already exists; create one as necessary
	mpv::RefPtr<Articulation> articulation = 
		entity->findOrCreateArticulation( artPartCtrl->GetArtPartID() );
	if( articulation->getEntityID() != entity->getID() )
	{
		// articulation freshly created, needs to have entity ID set
		articulation->setEntityID( entity->getID() );
	}
	
	articulation->setEnabled( artPartCtrl->GetArtPartEn() );
	
	if( artPartCtrl->GetXOffEn() || artPartCtrl->GetYOffEn() || artPartCtrl->GetZOffEn() )
	{
		Vect3 offset = articulation->getOffset();
		if( artPartCtrl->GetXOffEn() )
			offset[0] = artPartCtrl->GetXOff();
		if( artPartCtrl->GetYOffEn() )
			offset[1] = artPartCtrl->GetYOff();
		if( artPartCtrl->GetZOffEn() )
			offset[2] = artPartCtrl->GetZOff();
		articulation->setOffset( offset );
	}
	
	if( artPartCtrl->GetRollEn() || artPartCtrl->GetPitchEn() || artPartCtrl->GetYawEn() )
	{
		Vect3 rotation = articulation->getRotation();
		if( artPartCtrl->GetYawEn() )
			rotation[0] = artPartCtrl->GetYaw();
		if( artPartCtrl->GetPitchEn() )
			rotation[1] = artPartCtrl->GetPitch();
		if( artPartCtrl->GetRollEn() )
			rotation[2] = artPartCtrl->GetRoll();
		articulation->setRotation( rotation );
	}
	
}



