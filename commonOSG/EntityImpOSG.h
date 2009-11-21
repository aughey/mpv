/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2004 The Boeing Company
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
 *  2004-05-28 Andrew Sampson
 *      Initial release
 *  
 *  2008-07-26 Andrew Sampson
 *      Ported class EntitySubgraphWrapper to the new entity interface, and 
 *      renamed class to EntityImpOSG.  However, most of the code is from 
 *      SymbolImpOSG.
 *  
 *  </pre>
 */

#ifndef _ENTITYIMPOSG_H_
#define _ENTITYIMPOSG_H_

#include <string>
#include <list>

#include <osg/ref_ptr>
#include <osg/Group>
#include <osg/Vec3>

#include "BindSlot.h"
#include "Entity.h"
#include "EntityElement.h"
#include "MPVCommonOSGTypes.h"


namespace mpvosg
{

//=========================================================
//! This class encapsulates the scene graph nodes used to visually represent 
//! an Entity.  This class also contains the functions for placing those nodes 
//! in the scene graph, and for updating their state according to the state of 
//! the entity.
//! 
class MPVCMNOSG_SPEC EntityImpOSG : public mpv::EntityImp
{
public:
	
	//=========================================================
	//! General Constructor
	//! 
	EntityImpOSG( mpv::Entity *entity );

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

	void positionDBChanged( mpv::GeodeticObject *entity );

	void addedChildEntity( mpv::EntityContainer *entity, mpv::Entity *childEntity );
	
	void removedChildEntity( mpv::EntityContainer *entity, mpv::Entity *childEntity );

	osg::Node *getTopNode();
	
	//=========================================================
	//! Returns the Entity associated with this EntityImpOSG
	//! \return The Entity associated with this EntityImpOSG
	//! 
	mpv::Entity * getEntity() { return entity; }
	
	//=========================================================
	//! Sets whether this entity's alpha value should be ignored
	//! \param newVal - the new value for ignoreAlpha
	//! 
	void setIgnoreAlpha( bool newVal )
	{
		ignoreAlpha = newVal;
	}
	
	//=========================================================
	//! Returns the ignoreAlpha flag.
	//! \return the value for ignoreAlpha
	//! 
	bool getIgnoreAlpha( )
	{
		return ignoreAlpha;
	}
	
	void setEntityElement( EntityElement *newElement );
	void removeEntityElement();

	//=========================================================
	//! Given a Entity, this method performs various casts to extract 
	//! the implementation object (of type EntityImpOSG).  Relies on 
	//! RTTI.  Returns NULL if the entity's implementation object is not 
	//! the correct type, or does not exist.
	//! \param entity - the Entity from which to retrieve the implementation
	//! \return a pointer to the implementation object, or NULL on error
	//!
	static EntityImpOSG *getEntityImpOSGFromEntity( mpv::Entity *entity );

protected:
	//=========================================================
	//! General Destructor
	//! 
	~EntityImpOSG();

	//=========================================================
	//! Convenience variable
	//! 
	static osg::Vec3 xAxis;
	//=========================================================
	//! Convenience variable
	//! 
	static osg::Vec3 yAxis;
	//=========================================================
	//! Convenience variable
	//! 
	static osg::Vec3 zAxis;

	//=========================================================
	//! A pointer to an EntityElement.  This object is the product of some 
	//! EntityElementFactory, and contains scene graph nodes.  Those nodes 
	//! need to be added to this entity's subgraph.
	//! 
	EntityElement *entityElement;

	//=========================================================
	//! True indicates that this entity's alpha should be ignored.
	//! 
	bool ignoreAlpha;
	
	osg::ref_ptr<osg::PositionAttitudeTransform> transform;
	osg::ref_ptr<osg::Switch> switchNode;
	osg::ref_ptr<osg::StateSet> stateSet;
	
	//=========================================================
	//! Attachment point for entity elements
	//! 
	osg::ref_ptr<osg::Group> elementGroup;
	
	//=========================================================
	//! Attachment point for child entities
	//! 
	osg::ref_ptr<osg::Group> childEntityGroup;
	
	//=========================================================
	//! Attachment point for symbol surfaces
	//! 
	osg::ref_ptr<osg::Group> symbolSurfaceGroup;

	void updateSwitch();

	void addedSymbolSurface( mpv::SymbolSurfaceContainer *, mpv::SymbolSurface *surface );
	void removedSymbolSurface( mpv::SymbolSurfaceContainer *, mpv::SymbolSurface *surface );

};

}
#endif

