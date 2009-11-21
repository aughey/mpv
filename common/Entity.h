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
 *  FILENAME:   Entity.h
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
 *  
 *  2008-07-06 Andrew Sampson
 *      Rewrote most of class.  Now uses XLObject, signals/slots, inherits 
 *      from various *Container classes, and bears a striking resemblance 
 *      to Symbol.
 *  
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <list>
#include <string>
#include <vector>

#include <CigiOutgoingMsg.h>
#include <CigiEntityCtrlV3.h>

#include "GeodeticObject.h"
#include "AnimationContainer.h"
#include "ArticulationContainer.h"
#include "ComponentContainer.h"
#include "EntityContainer.h"
#include "SymbolSurfaceContainer.h"
#include "Mtx4.h"
#include "Vect3.h"
#include "DefFileGroup.h"

#include "MPVCommonTypes.h"

#if defined(_MSC_VER)
   #pragma warning(push)
   #pragma warning(disable : 4251)
#endif


namespace mpv
{

// forward declaration; defined at bottom of file
class EntityImp;


//=========================================================
//! This class encapsulates the Entity functionality and data
//!
class MPVCMN_SPEC Entity :
			public GeodeticObject,
			public AnimationContainer,
			public ArticulationContainer,
			public ComponentContainer,
			public EntityContainer,
			public SymbolSurfaceContainer
{
public:

	//=========================================================
	//! The enumeration for the Entity State flag
	//!
	enum EntityState
	{
		Standby = CigiBaseEntityCtrl::Standby,
		Active = CigiBaseEntityCtrl::Active,
		Remove = CigiBaseEntityCtrl::Remove
	};

	//=========================================================
	//! The enumeration for the Ground/Ocean Clamp flag
	//!
	enum GroundClampState
	{
		NoClamp = CigiBaseEntityCtrl::NoClamp,
		AltClamp = CigiBaseEntityCtrl::AltClamp,
		AltAttClamp = CigiBaseEntityCtrl::AltAttClamp
	};

	typedef std::list< RefPtr<EntityImp> > EntityImpList;
	typedef std::pair< EntityImpList::iterator, EntityImpList::iterator > EntityImpIteratorPair;

public:

	boost::signal<void (Entity*)> nameChanged;
	boost::signal<void (Entity*)> typeChanged;
	boost::signal<void (Entity*)> stateChanged;
	boost::signal<void (Entity*)> parentChanged;
	boost::signal<void (Entity*)> alphaChanged;
	boost::signal<void (Entity*)> collisionDetectionEnabledChanged;
	boost::signal<void (Entity*)> groundClampStateChanged;
	boost::signal<void (Entity*)> animationFinished;

	//=========================================================
	//! General Constructor
	//!
	Entity();

	virtual void update( double timeElapsed );

	//=========================================================
	//! Gets the Entity ID
	//! \return The Entity ID.
	//!
	Cigi_uint16 getID() const { return id; }

	//=========================================================
	//! Gets the entity name - a textual description of the entity type,
	//! such as "F-18 E"
	//! \return The entity name.
	//!
	const std::string &getName() const { return name; }

	//=========================================================
	//! Gets the Entity Type
	//! \return The Entity Type.
	//!
	Cigi_uint16 getType() const { return type; }

	EntityState getState() const { return state; }

	bool getFullControlPacketRecvd() const { return fullControlPacketRecvd; }

	Cigi_uint16 getParentID() const { return parentID; }

	//=========================================================
	//! Gets the pointer to this entity's parent.  May be NULL.
	//! \return The pointer to the parent of this entity.
	//!
	Entity *getParent() const { return parent; }

	//=========================================================
	//! Gets a pointer to this entity's top-level parent.  If this entity 
	//! is a top-level entity, then the returned value will be a pointer to 
	//! this entity.  The return value should never be NULL.
	//! \return the top-level entity that contains this entity OR 
	//!         if this entity is top-level, this entity
	//!
	Entity *getTopLevelParent();

	bool getIsChild() const { return isChild; }

	Cigi_uint8 getAlpha() const { return alpha; }

	bool getInheritAlpha() const { return inheritAlpha; }

	bool getCollisionDetectionEnabled() const { return collisionDetectionEnabled; }

	GroundClampState getGroundClampState() const { return groundClampState; }
	
	//=========================================================
	//! Gets the configuration data associated with this entity's type.  
	//! The same information is available via the MPV blackboard; it is 
	//! available here purely for the sake of convenience.
	//! All entities of the same type will have the same pointer value 
	//! for their config group.
	//! \return pointer to the config data for this entity's type, or NULL 
	//!         if such data is not available
	//! 
	DefFileGroup *getConfig() const { return config; }

	//=========================================================
	//! Returns a matrix which will transform points in this Entity's 
	//! coordinate system into the coordinate system of its parent.  For 
	//! top-level entities, the parent coordinate system is the terrain 
	//! database.
	Mtx4 getRelativeTransform() const;

	//=========================================================
	//! Returns a matrix which will transform points in this Entity's 
	//! coordinate system into terrain database coordinates.
	Mtx4 getAbsoluteTransform() const;
	
	//=========================================================
	//! Similar to getRelativeTransform(), but includes only the 
	//! orientation transformation.
	Mtx4 getRelativeTransformOrientationOnly() const;

	//=========================================================
	//! Similar to getAbsoluteTransform(), but includes only the 
	//! orientation transformation.
	Mtx4 getAbsoluteTransformOrientationOnly() const;
	

	//=========================================================
	//! Sets the Entity ID
	//! \param newID - The new Entity ID.
	//!
	void setID( Cigi_uint16 newID );

	//=========================================================
	//! Sets the entity name - a textual description of the entity type,
	//! such as "F-18 E"
	//! \param newName - The new entity name.
	//!
	void setName( const std::string &newName );

	void setType( Cigi_uint16 newType );

	void setState( EntityState newState );

	void setFullControlPacketRecvd( bool truth );

	//=========================================================
	//! Sets the Entity's parent.  Do not call this function outside of
	//! the entity manager plugin.
	//! \param newIsChild - boolean indicating whether this entity should be a child entity
	//! \param newParentID - entity id for the new parent
	//! \param newParent - The pointer to the parent of this entity.
	//!
	void setParent( bool newIsChild, Cigi_uint16 newParentID, Entity *newParent );

	void setAlpha( Cigi_uint8 newAlpha );

	void setInheritAlpha( bool newInheritAlpha );

	void setCollisionDetectionEnabled( bool newCollisionDetectionEnabled );

	void setGroundClampState( GroundClampState newGroundClampState );

	//=========================================================
	//! Sets the configuration data associated with this entity's type.  
	//! Do not call this function outside of the entity manager plugin.
	//! 
	void setConfig( DefFileGroup *newConfig );

	void addImplementation( EntityImp *newImp );
	
	EntityImpIteratorPair getImplementations()
	{
		return EntityImpIteratorPair( imps.begin(), imps.end() );
	}
	
protected:
	//=========================================================
	//! General Destructor
	//!
	virtual ~Entity();

	//=========================================================
	//! A callback method, called when this entity's parent's alpha changes
	//! 
	void parentAlphaChanged( Entity * );
	
	//=========================================================
	//! A callback method, called when this entity's default animation 
	//! (anim #0) stops playing.
	//! 
	void animationStoppedPlaying( Animation * );

	//=========================================================
	//! Uniquely identifies the specific entity instance<br>
	//! In CIGI, the numbering convention is as follows:<br>
	//!   0 - The ownship<br>
	//!   All other values - An Application defined entity<br>
	//! <br>
	//! This convention is not strictly enforced, however.  For example,
	//! it is entirely possible that the Host could specify that the
	//! eyepoint follow some entity instance other than 0.
	//!
	Cigi_uint16 id;

	//=========================================================
	//! A textual description of the entity type, such as "F-18 E".
	//!
	std::string name;

	//=========================================================
	//! Specifies the type of entity. 
	//! The meaning of this value is system-specific, ie type 100 might be
	//! an F-15 in one simulation, but might be a helicopter in another
	//! simulation.  This sort of thing is defined through the configuration
	//! files.  The Host and the IG must "agree" on the meaning of these values.
	//!
	Cigi_uint16 type;

	//=========================================================
	//! The entity's state.  This variable controls the entity's 
	//! lifetime.
	//!
	EntityState state;

	//=========================================================
	//! In order for a entity to be placed on-screen, 
	//! a full Entity Control packet must be sent to the IG for that entity.  
	//! It is not enough to send just a 
	//! Short Entity Control for that entity; doing so would leave several 
	//! entity attributes uninitialized.  For that reason, this variable 
	//! exists to keep track of whether a full Entity Control has been sent 
	//! for this entity.
	bool fullControlPacketRecvd;

	//=========================================================
	//! If this is an attached entity,
	//!   this specifies to which entity this entity is attached.
	//!
	Cigi_uint16 parentID;

	//=========================================================
	//! If this is an attached entity,
	//!   this points to the parent entity object.
	//!
	Entity *parent;

	//=========================================================
	//! A boolean indicating the attachment status of the entity.  
	//! If true, this is a child entity.  
	//! If false, this is an independent "top level" entity.
	//!
	bool isChild;

	//=========================================================
	//! Specifies the visibility/transparency of the entity.  
	//! 0 - transparent : 255 - Fully visible
	Cigi_uint8 alpha;

	//=========================================================
	//! A boolean indicating whether the entity's alpha should be used, or 
	//! if the entity's parent's alpha should be used instead.  Ignored if 
	//! the entity is a top-level entity.
	//!
	bool inheritAlpha;

	//=========================================================
	//! Indicates whether collision detection is enabled for this 
	//! entity.
	//!
	bool collisionDetectionEnabled;

	//=========================================================
	//! Indicates the kind of clamping to be performed on the entity.
	//!
	GroundClampState groundClampState;
	
	//=========================================================
	//! The configuration data associated with this entity's type.  
	//! All entities of the same type will have the same pointer value 
	//! for their config group.
	//! This variable will be NULL if the config data for this entity's type 
	//! is not available.
	//! Note that this variable isn't reference counted.  That may need to 
	//! change.
	//! 
	DefFileGroup *config;

	//=========================================================
	//! Implementation objects for this Entity.  
	//!
	EntityImpList imps;

};



//=========================================================
//! An implementation class, which complements Entity.
//! Those seeking to do interesting things with Entities should 
//! inherit from this class.
//! 
class MPVCMN_SPEC EntityImp : public Referenced
{
public:
	//=========================================================
	//! General Constructor
	//! 
	EntityImp( Entity *_entity ) : 
		Referenced(), 
		entity( _entity )
	{}
	

	virtual void update( double timeElapsed ) {}

protected:
	//=========================================================
	//! General Destructor
	//! 
	virtual ~EntityImp() {}
	
	//=========================================================
	//! Pointer to the owning Entity.  
	//!
	Entity *entity;
};


}

#if defined(_MSC_VER)
   #pragma warning(pop)
#endif

#endif
