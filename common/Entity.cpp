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
 *  FILENAME:   Entity.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *   represent an entity.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  03/29/2004 Greg Basler                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#define _USE_MATH_DEFINES

#include <math.h>
#include <iostream>

#include "BindSlot.h"
#include "Entity.h"

using namespace mpv;


// ================================================
// Entity
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Entity::Entity() : 
	GeodeticObject(),
	AnimationContainer(),
	ArticulationContainer(),
	ComponentContainer(),
	EntityContainer(),
	SymbolSurfaceContainer()
{
	id = 0xffff;
	type = 0;
	state = Standby;
	parentID = 0xffff;
	parent = NULL;
	isChild = false;
	alpha = 0xff;
	inheritAlpha = false;
	collisionDetectionEnabled = false;
	groundClampState = NoClamp;
	config = NULL;
	
	// all entities have a built-in Animation
	Animation *defaultAnimation = new Animation;
	defaultAnimation->setID( 0 );
	defaultAnimation->setDirection( Animation::Forward );
	defaultAnimation->setLoopMode( Animation::OneShot );
	defaultAnimation->setState( Animation::Stop );
	defaultAnimation->animationFinished.connect( BIND_SLOT1( Entity::animationStoppedPlaying, this ) );
	addAnimation( defaultAnimation );
}


// ================================================
// ~Entity
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Entity::~Entity()
{
}


// ================================================
// update
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Entity::update( double timeElapsed )
{
	EntityImpList::iterator iter;
	for( iter = imps.begin(); iter != imps.end(); iter++ )
	{
		if( (*iter).valid() )
			(*iter)->update( timeElapsed );
	}

	updateAnimations( timeElapsed );
	updateArticulations( timeElapsed );
	updateComponents( timeElapsed );
	updateEntities( timeElapsed );
}


// ================================================
// getTopLevelParent
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Entity *Entity::getTopLevelParent()
{
	Entity *result = this;
	while( result->getIsChild() )
		result = result->getParent();
	
	return result;
}


// ================================================
// getRelativeTransform
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Mtx4 Entity::getRelativeTransform() const
{
	const CoordinateSet &coord = getPositionDB();
	Mtx4 entityTranslate, entityRotate;

	entityTranslate.setAsTranslate( Vect3( coord.LatX, coord.LonY, coord.AltZ ) );

	entityRotate.setAsRotate( 
		-1.0 * coord.Yaw * M_PI / 180.0, 
		coord.Pitch * M_PI / 180.0, 
		coord.Roll * M_PI / 180.0 );
	
	return entityTranslate * entityRotate;
}


// ================================================
// getAbsoluteTransform
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Mtx4 Entity::getAbsoluteTransform() const
{
	if( getIsChild() )
		return getParent()->getAbsoluteTransform() * getRelativeTransform();
	else
		return getRelativeTransform();
}


// ================================================
// getRelativeTransformOrientationOnly
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Mtx4 Entity::getRelativeTransformOrientationOnly() const
{
	const CoordinateSet &coord = getPositionDB();
	Mtx4 entityRotate;

	entityRotate.setAsRotate( 
		-1.0 * coord.Yaw * M_PI / 180.0, 
		coord.Pitch * M_PI / 180.0, 
		coord.Roll * M_PI / 180.0 );
	
	return entityRotate;
}


// ================================================
// getAbsoluteTransformOrientationOnly
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Mtx4 Entity::getAbsoluteTransformOrientationOnly() const
{
	if( getIsChild() )
		return getParent()->getAbsoluteTransformOrientationOnly() * getRelativeTransformOrientationOnly();
	else
		return getRelativeTransformOrientationOnly();
}


// ================================================
// setID
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Entity::setID( Cigi_uint16 newID )
{
	id = newID;
}


// ================================================
// setName
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Entity::setName( const std::string &newName )
{
	if( name != newName )
	{
		name = newName;
		nameChanged( this );
	}
}


// ================================================
// setType
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Entity::setType( Cigi_uint16 newType )
{
	if( type != newType )
	{
		type = newType;
		typeChanged( this );
	}
}


// ================================================
// setState
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Entity::setState( EntityState newState )
{
	if( state != newState )
	{
		state = newState;

		if( state == Remove )
		{
			// the CIGI standard states that when an entity is destroyed, all the 
			// ent's children are also destroyed
			flagAllEntitiesAsDestroyed();
		}

		stateChanged( this );
	}
}


// ================================================
// setFullControlPacketRecvd
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Entity::setFullControlPacketRecvd( bool truth )
{
	fullControlPacketRecvd = truth;
}


// ================================================
// setParent
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Entity::setParent( bool newIsChild, Cigi_uint16 newParentID, Entity *newParent )
{
	if( isChild != newIsChild || 
		parentID != newParentID || 
		parent != newParent )
	{
		// disconnect signals for old parent
		if( parent != NULL )
		{
			parent->alphaChanged.disconnect( BIND_SLOT1( Entity::parentAlphaChanged, this ) );
		}

		if( newIsChild )
		{
			// sanity checks
			if( newParent == NULL )
				throw std::string( "attempted to set parent entity to NULL when isChild set to true" );
			if( newParent->getID() != newParentID )
				throw std::string( "parentID and parent->getID() do not match" );
			Entity *testParent = newParent;
			while( testParent != NULL )
			{
				if( testParent == this || testParent->getID() == getID() )
					throw std::string( "attempted to attach an entity to itself" );
				testParent = testParent->getParent();
			}
			
			isChild = true;
			parentID = newParentID;
			parent = newParent;
			
			// connect signals for new parent
			parent->alphaChanged.connect( BIND_SLOT1( Entity::parentAlphaChanged, this ) );
		}
		else
		{
			isChild = false;
			parentID = 0xffff;
			parent = NULL;
		}
		parentChanged( this );
		
		// a new parent means a potential change in alpha
		alphaChanged( this );
	}
}


// ================================================
// setAlpha
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Entity::setAlpha( Cigi_uint8 newAlpha )
{
	if( alpha != newAlpha )
	{
		alpha = newAlpha;
		alphaChanged( this );
	}
}


// ================================================
// setInheritAlpha
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Entity::setInheritAlpha( bool newInheritAlpha )
{
	if( inheritAlpha != newInheritAlpha )
	{
		inheritAlpha = newInheritAlpha;
		alphaChanged( this );
	}
}


// ================================================
// setCollisionDetectionEnabled
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Entity::setCollisionDetectionEnabled( bool newCollisionDetectionEnabled )
{
	if( collisionDetectionEnabled != newCollisionDetectionEnabled )
	{
		collisionDetectionEnabled = newCollisionDetectionEnabled;
		collisionDetectionEnabledChanged( this );
	}
}


// ================================================
// setGroundClampState
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Entity::setGroundClampState( GroundClampState newGroundClampState )
{
	if( groundClampState != newGroundClampState )
	{
		groundClampState = newGroundClampState;
		groundClampStateChanged( this );
	}
}


// ================================================
// setConfig
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Entity::setConfig( DefFileGroup *newConfig )
{
	config = newConfig;
}


// ================================================
// addImplementation
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Entity::addImplementation( EntityImp *newImp )
{
	if( newImp != NULL )
		imps.push_back( newImp );
}



// ================================================
// parentAlphaChanged
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Entity::parentAlphaChanged( Entity * )
{
	if( inheritAlpha )
		alphaChanged( this );
}


// ================================================
// animationStoppedPlaying
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Entity::animationStoppedPlaying( Animation * )
{
	animationFinished( this );
}












#if 0
// ================================================
// ProcEntityCtrlPckt
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Entity::ProcEntityCtrlPckt( CigiEntityCtrlV3 *Packet )
{

	if( EntityID != Packet->GetEntityID() )
		return;

	if( EntityType != Packet->GetEntityType() )
	{
		EntityType = Packet->GetEntityType();
	}


	// Process items related to independent/child
	if( Packet->GetAttachState() == CigiBaseEntityCtrl::Detach )
	{
		IsChild = false;
		ParentID = 0;
		Parent = NULL;
		InheritAlpha = false;


		// Update absolute position
		CigiPos.LatX = Packet->GetLat();
		CigiPos.LonY = Packet->GetLon();
		CigiPos.Yaw = Packet->GetYaw();

		// Take Ground Clamp into consideration
		GrndClamp = ( GrndClampGrp )Packet->GetGrndClamp();
		if( GrndClamp == NoClamp )
			CigiPos.AltZ = Packet->GetAlt();
		else
			CigiPos.AltZ = 0.0;

		if( GrndClamp != AltAttClamp )
		{
			CigiPos.Pitch = Packet->GetPitch();
			CigiPos.Roll = Packet->GetRoll();
		}
		else
		{
			CigiPos.Pitch = 0.0;
			CigiPos.Roll = 0.0;
		}

		if( CoordCigi2DBase != NULL )
			CoordCigi2DBase( CigiPos, DBasePos );


	}
	else
	{
		IsChild = true;
		ParentID = Packet->GetParentID();
		// Parent pointer will be filled by manager
		InheritAlpha = ( Packet->GetInheritAlpha() != CigiBaseEntityCtrl::NoInherit );


		CigiPos.LatX = Packet->GetXoff();
		CigiPos.LonY = Packet->GetYoff();
		CigiPos.AltZ = Packet->GetZoff();
		DBasePos.LatX = CigiPos.LonY;
		DBasePos.LonY = CigiPos.LatX;
		DBasePos.AltZ = -CigiPos.AltZ;

		DBasePos.Yaw = CigiPos.Yaw = Packet->GetYaw();
		DBasePos.Pitch = CigiPos.Pitch = Packet->GetPitch();
		DBasePos.Roll = CigiPos.Roll = Packet->GetRoll();

		GrndClamp = NoClamp;  // Children cannot be ground clamped

	}

	// Get general data and flags
	Alpha = Packet->GetAlpha();

	EntityState = ( EntityStateGrp )Packet->GetEntityState();
	CollisionDetectEn = ( Packet->GetCollisionDetectEn() != CigiBaseEntityCtrl::Disable );

	// update the animation(s)
	std::list< Animation * >::iterator animIter;
	for( animIter = Animations.begin(); animIter != Animations.end(); animIter++ )
	{
		( *animIter )->ProcEntityCtrlPckt( Packet );
	}

	// Reset the Trajectory induced rates
	ExtrapData.XRate = ExtrapData.CmdXRate;
	ExtrapData.YRate = ExtrapData.CmdYRate;
	ExtrapData.ZRate = ExtrapData.CmdZRate;

	// Set updated flag
	PcktUpdated = true;

}


// ================================================
// ProcCCEntityCtrlPckt
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Entity::ProcCCEntityCtrlPckt( CigiConfClampEntityCtrlV3 *Packet )
{

	if( EntityID != Packet->GetEntityID() )
		return;


	// Process items related to independent/child
	IsChild = false;
	ParentID = 0;
	Parent = NULL;
	InheritAlpha = false;


	// Update absolute position
	CigiPos.LatX = Packet->GetLat();
	CigiPos.LonY = Packet->GetLon();
	CigiPos.Yaw = ( double )Packet->GetYaw();

	// These values are not applicable for conformal clamped entities
	CigiPos.AltZ = 0.0;
	CigiPos.Pitch = 0.0;
	CigiPos.Roll = 0.0;

	if( CoordCigi2DBase != NULL )
		CoordCigi2DBase( CigiPos, DBasePos );

	GrndClamp = AltAttClamp;

	// The following values are not affected by this packet:
	// Alpha, Entity State, Collision Detection, all the animation controls.

	// Reset the Trajectory induced rates
	ExtrapData.XRate = ExtrapData.CmdXRate;
	ExtrapData.YRate = ExtrapData.CmdYRate;
	ExtrapData.ZRate = ExtrapData.CmdZRate;

	// Set updated flag
	PcktUpdated = true;

}


// ================================================
// ProcTrajectoryPckt
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Entity::ProcTrajectoryPckt( CigiTrajectoryDefV3 *Packet )
{

	if( EntityID != Packet->GetEntityID() )
		return;

	// All Accel is in meters per sec per sec
	ExtrapData.AccelX = Packet->GetAccelX();
	ExtrapData.AccelY = Packet->GetAccelY();
	ExtrapData.AccelZ = Packet->GetAccelZ();

	ExtrapData.RetardationRate = Packet->GetRetardationRate();

	ExtrapData.TermVel = Packet->GetTermVel();  // meters per sec

	TrajEn = true;
	Interpol = true;

}


// ================================================
// ProcRatePckt
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Entity::ProcRatePckt( CigiRateCtrlV3 *Packet )
{

	if( EntityID != Packet->GetEntityID() )
		return;

	if( Packet->GetApplyToArtPart() )
	{
		// Check each art part until found or end
		std::list<ArtPart*>::iterator pArt;
		for( pArt = ArtParts.begin();pArt != ArtParts.end();pArt++ )
		{
			if(( *pArt )->GetArtPartID() == Packet->GetArtPartID() )
			{
				( *pArt )->ProcRatePckt( Packet );
				break;
			}
		}
	}
	else
	{
		// Meters/sec
		ExtrapData.CmdXRate = ExtrapData.XRate = Packet->GetXRate();
		ExtrapData.CmdYRate = ExtrapData.YRate = Packet->GetYRate();
		ExtrapData.CmdZRate = ExtrapData.ZRate = Packet->GetZRate();

		// Degrees/sec
		ExtrapData.RollRate = Packet->GetRollRate();
		ExtrapData.PitchRate = Packet->GetPitchRate();
		ExtrapData.YawRate = Packet->GetYawRate();
	}

	RateEn = true;
	Interpol = true;

}


// ================================================
// ProcPositionReq
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Entity::ProcPositionReq( CigiPositionReqV3 &Packet, CigiOutgoingMsg &Omsg )
{
	bool Valid = false;
	CigiPositionRespV3 Resp;

	if( Packet.GetObjectClass() == CigiBasePositionReq::Entity )
	{

		Resp.SetObjectID( Packet.GetObjectID(), false );
		Resp.SetArtPartID( Packet.GetArtPartID(), false );
		Resp.SetObjectClass(
		    ( CigiBasePositionResp::ObjectClassGrp )Packet.GetObjectClass(),
		    false );
		Resp.SetCoordSys(
		    ( CigiBasePositionResp::CoordSysGrp )Packet.GetCoordSys(),
		    false );

		if( Packet.GetCoordSys() == CigiBasePositionReq::Geodetic )
		{
			if( !IsChild )
			{
				Resp.SetLatOrXoff( CigiPos.LatX, false );
				Resp.SetLonOrYoff( CigiPos.LonY, false );
				Resp.SetAltOrZoff( CigiPos.AltZ, false );
				Resp.SetYaw(( float )CigiPos.Yaw, false );
				Resp.SetPitch(( float )CigiPos.Pitch, false );
				Resp.SetRoll(( float )CigiPos.Roll, false );

				Valid = true;
			}
			// If a geodetic coord position request for a child entity
			//   is received, the entity manager will process it
			//   because the manager will have to go through all the
			//   parents and develop a complete transformation
			//   matrix.
		}
		else if( Packet.GetCoordSys() == CigiBasePositionReq::ParentEntity )
		{
			if( IsChild )
			{
				Resp.SetLatOrXoff( CigiPos.LatX, false );
				Resp.SetLonOrYoff( CigiPos.LonY, false );
				Resp.SetAltOrZoff( CigiPos.AltZ, false );
				Resp.SetYaw(( float )CigiPos.Yaw, false );
				Resp.SetPitch(( float )CigiPos.Pitch, false );
				Resp.SetRoll(( float )CigiPos.Roll, false );

				Valid = true;
			}
		}
	}
	else if( Packet.GetCoordSys() == CigiBasePositionReq::Submodel )   // Articulated part
	{
		// Only respond to Submodel coordinate system requests for
		//   an articulated response
		ProcArtPartPositionReq( Packet, Omsg );
	}

	if( Valid )
		Omsg << Resp;
}


// ================================================
// Act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Entity::Act( float dTime )
{

	if( PcktUpdated )
	{
		PcktUpdated = false;
	}
	else if( Interpol )
	{
		// interpolate position if not updated this frame
		// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

		dTimeh = dTime;

		// Independent and Child entities are handled differently
		if( !IsChild )
		{

			if( GrndClamp != NoClamp )
			{
				ExtrapData.ZRate = 0.0;
				ExtrapData.AccelZ = 0.0;
			}

			if( GrndClamp == AltAttClamp )
			{
				ExtrapData.PitchRate = 0.0;
				ExtrapData.RollRate = 0.0;
			}

			if( RateEn || TrajEn )
				Extrapolate( CigiPos, DBasePos, ExtrapData, dTimeh, TrajEn );

			if( GrndClamp != NoClamp )
				DBasePos.AltZ = 0.0;

			if( GrndClamp == AltAttClamp )
			{
				DBasePos.Pitch = 0.0;
				DBasePos.Roll = 0.0;
			}

		}
		else
		{
			if( RateEn || TrajEn )
				ExtrapChild( CigiPos, DBasePos, ExtrapData, dTimeh, TrajEn );
		}

	}

	// Call Act on each art part
	std::list<ArtPart*>::iterator pArt;
	for( pArt = ArtParts.begin();pArt != ArtParts.end();pArt++ )
	{
		( *pArt )->Act( dTime );
	}

	// call Act on each component control
	std::list<CompCtrl*>::iterator ccIter;
	for( ccIter = CompCtrls.begin(); ccIter != CompCtrls.end(); ccIter++ )
	{
		( *ccIter )->Act();
	}

	// Call Act on each animation
	if( Animations.empty() )
	{
		AllAnimationsAreFinished = false;
	}
	else
	{
		AllAnimationsAreFinished = true;
		std::list< Animation * >::iterator animIter = Animations.begin();
		for( ; animIter != Animations.end(); animIter++ )
		{
			( *animIter )->act( dTime );
			AllAnimationsAreFinished =
			    AllAnimationsAreFinished && ( *animIter )->getIsFinished();
		}
	}
	if( !AllAnimationsAreFinished )
		AnimationStopNotificationSent = false;
}

#endif
