/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2004 The Boeing Company
 *  Copyright (c) 2009 Andrew Sampson
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
 *  2009-06-19 Andrew Sampson
 *      Copied file from EntityImpOSG to EntityImpS11n.  Adapted for use in 
 *      S11n subsystem.
 *  
 *  </pre>
 */

#ifndef _ENTITYIMPS11N_H_
#define _ENTITYIMPS11N_H_

#include <string>
#include <list>

#include "BindSlot.h"
#include "Entity.h"

#include "DoubleNode.h"
#include "DoubleVectorNode.h"
#include "FloatNode.h"
#include "GroupNode.h"
#include "IntNode.h"
#include "StringNode.h"


namespace mpvs11n
{

//=========================================================
//! This class encapsulates the scene graph nodes used to visually represent 
//! an Entity.  This class also contains the functions for placing those nodes 
//! in the scene graph, and for updating their state according to the state of 
//! the entity.
//! 
class EntityImpS11n : public mpv::EntityImp
{
public:
	
	//=========================================================
	//! General Constructor
	//! 
	EntityImpS11n( mpv::Entity *entity );

	//=========================================================
	//! Called once per frame.  
	//! 
	virtual void update( double timeElapsed );

	void nameChanged( mpv::Entity *entity );

	void typeChanged( mpv::Entity *entity );

	void stateChanged( mpv::Entity *entity );

	void parentChanged( mpv::Entity *entity );

	void alphaChanged( mpv::Entity *entity );

	void collisionDetectionEnabledChanged( mpv::Entity *entity );

	void groundClampStateChanged( mpv::Entity *entity );

	void positionGDCChanged( mpv::GeodeticObject *entity );

	void positionDBChanged( mpv::GeodeticObject *entity );

	void addedChildEntity( mpv::EntityContainer *entity, mpv::Entity *childEntity );
	
	void removedChildEntity( mpv::EntityContainer *entity, mpv::Entity *childEntity );

	mpvs11n::Node *getTopNode();
	
	//=========================================================
	//! Given a Entity, this method performs various casts to extract 
	//! the implementation object (of type EntityImpS11n).  Relies on 
	//! RTTI.  Returns NULL if the entity's implementation object is not 
	//! the correct type, or does not exist.
	//! \param entity - the Entity from which to retrieve the implementation
	//! \return a pointer to the implementation object, or NULL on error
	//!
	static EntityImpS11n *getEntityImpS11nFromEntity( mpv::Entity *entity );

protected:
	//=========================================================
	//! General Destructor
	//! 
	~EntityImpS11n();

	mpv::RefPtr<mpvs11n::GroupNode> topGroup;
	
	mpv::RefPtr<mpvs11n::IntNode> id;
	mpv::RefPtr<mpvs11n::StringNode> name;
	mpv::RefPtr<mpvs11n::IntNode> type;
	mpv::RefPtr<mpvs11n::StringNode> state;
	mpv::RefPtr<mpvs11n::IntNode> parentID;
	mpv::RefPtr<mpvs11n::IntNode> alpha;
	mpv::RefPtr<mpvs11n::IntNode> inheritAlpha;
	mpv::RefPtr<mpvs11n::IntNode> collisionDetectionEnabled;
	mpv::RefPtr<mpvs11n::StringNode> groundClampState;
	mpv::RefPtr<mpvs11n::DoubleVectorNode> positionGDC;
	mpv::RefPtr<mpvs11n::DoubleVectorNode> orientationGDC;
	mpv::RefPtr<mpvs11n::DoubleVectorNode> positionDB;
	mpv::RefPtr<mpvs11n::DoubleVectorNode> orientationDB;

	//=========================================================
	//! Attachment point for child entities
	//! 
	mpv::RefPtr<mpvs11n::GroupNode> childEntityGroup;
	
	//=========================================================
	//! Attachment point for symbol surfaces
	//! 
	mpv::RefPtr<mpvs11n::GroupNode> symbolSurfaceGroup;

	void addedSymbolSurface( mpv::SymbolSurfaceContainer *, mpv::SymbolSurface *surface );
	void removedSymbolSurface( mpv::SymbolSurfaceContainer *, mpv::SymbolSurface *surface );

};

}
#endif

