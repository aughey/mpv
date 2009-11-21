/** <pre>
 *  The MPV Entity Manager plugin
 *  Copyright (c) 2008 
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
 *  2008-07-13 Andrew Sampson
 *      Initial release.  Based on code extracted from PluginSymbologyMgr.
 *  
 *  
 *  </pre>
 */


#include "ProcEntityCtrl.h"
#include "PluginEntityMgr.h"
#include "CoordSet.h"

using namespace mpv;

// ================================================
// ProcEntityCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ProcEntityCtrl::ProcEntityCtrl( PluginEntityMgr *_plugin )
	: plugin( _plugin )
{
	
}


// ================================================
// ~ProcEntityCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ProcEntityCtrl::~ProcEntityCtrl() 
{
	
}


// ================================================
// OnPacketReceived
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ProcEntityCtrl::OnPacketReceived( CigiBasePacket *Packet )
{
	CigiEntityCtrlV3 *entityCtrl = static_cast<CigiEntityCtrlV3 *> (Packet);
	
	bool entityIsNew = false;
	
	mpv::RefPtr<Entity> entity = plugin->findEntity( entityCtrl->GetEntityID() );
	if( !entity.valid() )
	{
		// entity not found; create one
		entity = plugin->createEntity( 
			entityCtrl->GetEntityID(), entityCtrl->GetEntityType() );
		entityIsNew = true;
	}
	else
	{
		// entity found; make sure the type matches the contents of the packet
		if( entity->getType() != entityCtrl->GetEntityType() )
		{
			// fixme - print warning
			
			// Entity type does not match.  The CIGI standard says that if 
			// the Host tries to change the type for an entity, that entity 
			// is deleted (along with all its children, as per usual) and 
			// is replaced with a new entity of the given type.
			entity->setState( Entity::Remove );
			
			entity = plugin->createEntity( 
				entityCtrl->GetEntityID(), entityCtrl->GetEntityType() );
			entityIsNew = true;
		}
	}
	
	entity->setFullControlPacketRecvd( true );
	
	// entity state
	// signal handling in EntityContainer will take care of removing Removed 
	// entities from the plugin and parent entities.
	entity->setState( (Entity::EntityState)entityCtrl->GetEntityState() );
	if( entity->getState() == Entity::Remove )
	{
		// go no further
		return;
	}
	
	// entity hierarchy (it is important that this is processed 
	// before the alpha state is processed, as alpha is affected 
	// by the entity hierarchy)
	bool isChild = (entityCtrl->GetAttachState() == CigiBaseEntityCtrl::Attach);
	bool changedTopLevelStatus = ( entity->getIsChild() != isChild );
	bool changedParent =         ( entity->getIsChild() &&
	                             ( entity->getParentID() != entityCtrl->GetParentID() ) );
	if( changedTopLevelStatus || changedParent || entityIsNew )
	{
		plugin->reparentEntity( entity->getID(), isChild, entityCtrl->GetParentID() );
// FIXME - error handling - what if reparenting fails; for example, new parent doesn't exist?
	}
	
	// alpha
	entity->setInheritAlpha( entityCtrl->GetInheritAlpha() == CigiBaseEntityCtrl::Inherit );
	entity->setAlpha( entityCtrl->GetAlpha() );

	// collision detection
	entity->setCollisionDetectionEnabled( entityCtrl->GetCollisionDetectEn() == CigiBaseEntityCtrl::Enable );
	
	// clamping
	entity->setGroundClampState( (Entity::GroundClampState)entityCtrl->GetGrndClamp() );

	// transform
	CoordinateSet coordinate;
	coordinate.Yaw   = entityCtrl->GetYaw();
	coordinate.Pitch = entityCtrl->GetPitch();
	coordinate.Roll  = entityCtrl->GetRoll();
	if( isChild )
	{
		// child entities - set the database coordinate
		// Since the output is to be in database coordinates (rather than 
		// in CIGI entity coordinates) the input needs to be converted from 
		// forward-right-down to right-forward-up.
		coordinate.LatX  = entityCtrl->GetYoff();
		coordinate.LonY  = entityCtrl->GetXoff();
		coordinate.AltZ  = entityCtrl->GetZoff() * -1.0;
		entity->setPositionDB( coordinate );
	}
	else
	{
		// top-level entities - set the geodetic coordinate
		coordinate.LatX  = entityCtrl->GetLat();
		coordinate.LonY  = entityCtrl->GetLon();
		coordinate.AltZ  = entityCtrl->GetAlt();
		entity->setPositionGDC( coordinate );
	}
	
	// animation
	// retrieve (or create) the entity's default animation
	Animation *animation = entity->findOrCreateAnimation( 0 );
	animation->setState( (Animation::AnimationState)entityCtrl->GetAnimationState() );
	animation->setDirection( (Animation::AnimationDirection)entityCtrl->GetAnimationLoopMode() );
	animation->setLoopMode( (Animation::AnimationLoopMode)entityCtrl->GetAnimationLoopMode() );
}



