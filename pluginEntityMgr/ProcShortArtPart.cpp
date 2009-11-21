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
 *      Initial release.  Replaces the old ProcShortArtPart class.  Uses the 
 *      new Articulation interface.
 *  
 *  </pre>
 */


#include <iostream>

#include "ProcShortArtPart.h"

using namespace mpv;

// ================================================
// ProcShortArtPart
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ProcShortArtPart::ProcShortArtPart( mpv::EntityContainer *entContainer ) :
	CigiBaseEventProcessor(),
	allEntities( entContainer )
{
	
}


// ================================================
// ~ProcShortArtPart
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ProcShortArtPart::~ProcShortArtPart() 
{
	
}


// ================================================
// OnPacketReceived
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ProcShortArtPart::OnPacketReceived( CigiBasePacket *Packet )
{
	CigiShortArtPartCtrlV3 *artPartCtrl = static_cast<CigiShortArtPartCtrlV3 *> (Packet);
	
	mpv::RefPtr<Entity> entity = allEntities->findEntity( artPartCtrl->GetEntityID() );
	if( !entity.valid() )
	{
		// entity not found!
		std::cout << "Warning - in ProcShortArtPart::OnPacketReceived() - entity " 
			<< artPartCtrl->GetEntityID() << " doesn't exist\n";
		return;
	}
	
	applySingleArticulationDOF( entity.get(), 
		artPartCtrl->GetArtPart1(), 
		artPartCtrl->GetArtPart1En(), 
		artPartCtrl->GetDofSelect1(), 
		artPartCtrl->GetDof1() );
	
	applySingleArticulationDOF( entity.get(), 
		artPartCtrl->GetArtPart2(), 
		artPartCtrl->GetArtPart2En(), 
		artPartCtrl->GetDofSelect2(), 
		artPartCtrl->GetDof2() );
}


void ProcShortArtPart::applySingleArticulationDOF( 
	mpv::Entity *entity, 
	Cigi_uint8 articulationID, 
	bool enabled, 
	CigiBaseShortArtPartCtrl::DofSelectGrp dofType, 
	float dofValue )
{
	// check if articulation already exists; create one as necessary
	mpv::RefPtr<Articulation> articulation = 
		entity->findOrCreateArticulation( articulationID );
	if( articulation->getEntityID() != entity->getID() )
	{
		// articulation freshly created, needs to have entity ID set
		articulation->setEntityID( entity->getID() );
	}
	
	articulation->setEnabled( enabled );
	
	bool offsetChanged = false;
	bool rotationChanged = false;
	Vect3 offset = articulation->getOffset();
	Vect3 rotation = articulation->getRotation();
	
	switch( dofType )
	{
	case CigiBaseShortArtPartCtrl::Xoff:
		offset[0] = dofValue;
		offsetChanged = true;
		break;

	case CigiBaseShortArtPartCtrl::Yoff:
		offset[1] = dofValue;
		offsetChanged = true;
		break;

	case CigiBaseShortArtPartCtrl::Zoff:
		offset[2] = dofValue;
		offsetChanged = true;
		break;

	case CigiBaseShortArtPartCtrl::Yaw:
		rotation[0] = dofValue;
		rotationChanged = true;
		break;

	case CigiBaseShortArtPartCtrl::Pitch:
		rotation[1] = dofValue;
		rotationChanged = true;
		break;

	case CigiBaseShortArtPartCtrl::Roll:
		rotation[2] = dofValue;
		rotationChanged = true;
		break;

	default:
		break;
	}
	
	if( offsetChanged )
		articulation->setOffset( offset );
	
	if( rotationChanged )
		articulation->setRotation( rotation );
}



