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
 *  2004-06-11 Andrew Sampson
 *      Initial release
 *  
 *  2008-07-26 Andrew Sampson
 *      Ported class EntitySubgraphWrapper to the new entity interface, and 
 *      renamed class to EntityImpOSG.  However, most of the code is from 
 *      SymbolImpOSG.
 *  
 *  </pre>
 */

#include <stdio.h>
#include <iostream>

#include <osg/Switch>
#include <osg/PositionAttitudeTransform>
#include <osg/StateSet>
#include <osg/Material>

#ifdef WIN32
#define snprintf _snprintf
#endif

#include "MiscOSG.h"
#include "SymbolSurfaceImpOSG.h"

#include "EntityImpOSG.h"


using namespace mpv;
using namespace mpvosg;
using namespace osg;

// statics
osg::Vec3 EntityImpOSG::xAxis( 1., 0., 0. );
osg::Vec3 EntityImpOSG::yAxis( 0., 1., 0. );
osg::Vec3 EntityImpOSG::zAxis( 0., 0., 1. );

// ================================================
// EntityImpOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
EntityImpOSG::EntityImpOSG( Entity *entity ) : EntityImp( entity )
{
	entityElement = NULL;
	ignoreAlpha = false;
	
	transform = new osg::PositionAttitudeTransform();
	switchNode = new osg::Switch();
	stateSet = transform->getOrCreateStateSet();
	switchNode->addChild( transform.get() );

	elementGroup = new osg::Group();
	elementGroup->setName( "Entity element group" );
	transform->addChild( elementGroup.get() );

	childEntityGroup = new osg::Group();
	childEntityGroup->setName( "Child entity group" );
	transform->addChild( childEntityGroup.get() );

	symbolSurfaceGroup = new osg::Group();
	symbolSurfaceGroup->setName( "Symbol surface group" );
	transform->addChild( symbolSurfaceGroup.get() );

	nameChanged( entity );
	typeChanged( entity );
	stateChanged( entity );
	parentChanged( entity );
	alphaChanged( entity );
	collisionDetectionEnabledChanged( entity );
	groundClampStateChanged( entity );
	positionDBChanged( entity );
	
	entity->nameChanged.connect( BIND_SLOT1( EntityImpOSG::nameChanged, this ) );
	entity->typeChanged.connect( BIND_SLOT1( EntityImpOSG::typeChanged, this ) );
	entity->stateChanged.connect( BIND_SLOT1( EntityImpOSG::stateChanged, this ) );
	entity->parentChanged.connect( BIND_SLOT1( EntityImpOSG::parentChanged, this ) );
	entity->alphaChanged.connect( BIND_SLOT1( EntityImpOSG::alphaChanged, this ) );
	entity->collisionDetectionEnabledChanged.connect( BIND_SLOT1( EntityImpOSG::collisionDetectionEnabledChanged, this ) );
	entity->groundClampStateChanged.connect( BIND_SLOT1( EntityImpOSG::groundClampStateChanged, this ) );
	entity->positionDBChanged.connect( BIND_SLOT1( EntityImpOSG::positionDBChanged, this ) );

	// check for symbol surfaces already attached to the entity and add them
	SymbolSurfaceContainer::SymbolSurfaceIteratorPair ssIterPair = entity->getSymbolSurfaces();
	SymbolSurfaceContainer::SymbolSurfaceMap::iterator ssIter;
	for( ssIter = ssIterPair.first; ssIter != ssIterPair.second; ssIter++ )
	{
		addedSymbolSurface( entity, ssIter->second.get() );
	}

	// arrange notification of symbol surface creation/destruction
	entity->addedSymbolSurface.connect( BIND_SLOT2( EntityImpOSG::addedSymbolSurface, this ) );
	entity->removedSymbolSurface.connect( BIND_SLOT2( EntityImpOSG::removedSymbolSurface, this ) );
	
	// handle existing child entities
	EntityContainer::EntityIteratorPair eIterPair = entity->getEntities();
	EntityContainer::EntityMap::iterator eIter;
	for( eIter = eIterPair.first; eIter != eIterPair.second; eIter++ )
	{
		addedChildEntity( entity, eIter->second.get() );
	}
	
	entity->addedEntity.connect( BIND_SLOT2( EntityImpOSG::addedChildEntity, this ) );
	entity->removedEntity.connect( BIND_SLOT2( EntityImpOSG::removedChildEntity, this ) );
}


// ================================================
// ~EntityImpOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
EntityImpOSG::~EntityImpOSG()
{
	// sever links with children
	childEntityGroup->removeChildren( 0, childEntityGroup->getNumChildren() );

	removeEntityElement();
}


// ================================================
// update
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void EntityImpOSG::update( double timeElapsed )
{
}


void EntityImpOSG::nameChanged( Entity * )
{
	// fixme - switch to strstream, include entity name in string
	char str[64];
	snprintf( str, 64, "Entity Subgraph entID %i typeID %i", 
		entity->getID(), entity->getType() );
	switchNode->setName( str );
}


void EntityImpOSG::typeChanged( Entity * )
{
	// FIXME - freak out, or delete scene graph, or something
}


void EntityImpOSG::stateChanged( Entity * )
{
	updateSwitch();
}


void EntityImpOSG::parentChanged( Entity * )
{
	
}


void EntityImpOSG::alphaChanged( Entity * )
{
	if( !ignoreAlpha )
	{
		unsigned char entAlpha = entity->getAlpha();
		osg::Material *material = (osg::Material *)stateSet->getAttribute( osg::StateAttribute::MATERIAL );
		
		// OpenGL ties material color and opacity together.  One can not be 
		// set without also setting the other.  In order to apply an entity's 
		// alpha (opacity), the material color must be set for that entity's 
		// subgraph.  
		// Many entity models use material color instead of texturing.  
		// Applying a material the subgraph (in order to set the entity's 
		// opacity) will clobber the colors too, making models that use 
		// material colors look funny.  We want to avoid this as much as 
		// possible.
		// With the above in mind, the goal is to only override the material 
		// color if we have to.  The material color will only be overridden 
		// if the entity is set to non-opaque.  If the entity ever becomes 
		// non-opaque, then returns to complete opacity, then the stateset 
		// will be cleaned up by removing the Material attribute and the 
		// blend setting.
		// 
		if( entAlpha == 255 )
		{
			// entity is opaque
			if( material != NULL )
			{
				// if the entity is opaque, and the material was previously 
				// set, then we should remove that setting
				stateSet->removeAttribute( osg::StateAttribute::MATERIAL );
				stateSet->removeMode( GL_BLEND );
			}
		}
		else
		{
			// entity is transparent to some degree
			if( material == NULL )
			{
				// material state attribute has not been created yet
				material = new osg::Material;
				stateSet->setMode( GL_BLEND, 
					osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON );
			}
			material->setAlpha( osg::Material::FRONT_AND_BACK, ((float)entAlpha) / 255.0 );

			stateSet->setAttributeAndModes( material, 
				osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON );
		}
	}
}


void EntityImpOSG::collisionDetectionEnabledChanged( Entity * )
{
	
}


void EntityImpOSG::groundClampStateChanged( Entity * )
{
	
}


void EntityImpOSG::positionDBChanged( GeodeticObject * )
{
	const CoordinateSet &coord = entity->getPositionDB();
/*std::cout << "Entity " << entity->getID() << " claims it is at " << 
coord.LatX << " " << 
coord.LonY << " " << 
coord.AltZ << std::endl;*/

	// entity position
	transform->setPosition( 
		osg::Vec3d( coord.LatX, coord.LonY, coord.AltZ ) );
	
	// entity orientation
	transform->setAttitude( osg::Quat( 
		osg::DegreesToRadians(coord.Roll), yAxis, 
		osg::DegreesToRadians(coord.Pitch), xAxis, 
		osg::DegreesToRadians(coord.Yaw * -1.0), zAxis   
	 ) );
}


void EntityImpOSG::addedChildEntity( EntityContainer *entity, Entity *childEntity )
{
	EntityImpOSG *entityImpOSG = getEntityImpOSGFromEntity( childEntity );
	if( entityImpOSG == NULL )
	{
		std::cerr << "EntityImpOSG::addedChildEntity - warning - The newly-added child entity either has no implementation object or has an implementation that isn't an EntityImpOSG.  Can't add new child to my scene graph.\n";
	}
	else
	{
		childEntityGroup->addChild( entityImpOSG->getTopNode() );
	}
}


void EntityImpOSG::removedChildEntity( EntityContainer *entity, Entity *childEntity )
{
	EntityImpOSG *entityImpOSG = getEntityImpOSGFromEntity( childEntity );
	if( entityImpOSG == NULL )
	{
		std::cerr << "EntityImpOSG::removedChildEntity - warning - The newly-removed child entity either has no implementation object or has an implementation that isn't an EntityImpOSG.  Can't remove child from my scene graph.\n";
	}
	else
	{
		childEntityGroup->removeChild( entityImpOSG->getTopNode() );
	}
}


osg::Node *EntityImpOSG::getTopNode()
{
	return switchNode.get();
}


// ================================================
// setEntityElement
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void EntityImpOSG::setEntityElement( EntityElement *newElement )
{
	// delete existing element 
	removeEntityElement();
	
	entityElement = newElement;
	if( entityElement != NULL )
	{
		elementGroup->addChild( entityElement->getTopNode() );
	}
}


// ================================================
// removeEntityElement
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void EntityImpOSG::removeEntityElement()
{
	elementGroup->removeChildren( 0, elementGroup->getNumChildren() );

	if( entityElement != NULL )
	{
		delete entityElement;
		entityElement = NULL;
	}
}


// ================================================
// updateSwitch
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void EntityImpOSG::updateSwitch()
{
	if( entity->getState() == Entity::Active ) {
		switchNode->setAllChildrenOn();
	} else {
		switchNode->setAllChildrenOff();
	}
}


// ================================================
// addedSymbolSurface
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void EntityImpOSG::addedSymbolSurface( 
	SymbolSurfaceContainer *, SymbolSurface *surface )
{
	// retrieve nodes from symbol surface, attach them to the entity subgraph
	
	// sanity check? container == entity
	
	if( surface->getAttachState() != SymbolSurface::Entity )
	{
std::cerr << "Debug warning - in EntityImpOSG::addedSymbolSurface, symbol surface is not a entity-attached surface\n";
		return;
	}
	
	SymbolSurfaceImpOSG *impOSG = SymbolSurfaceImpOSG::getSymbolSurfaceImpOSGFromSymbolSurface( surface );
	if( impOSG == NULL )
	{
		std::cerr << "Error - in EntityImpOSG::addedSymbolSurface, symbol surface \n"
			<< "has no implementation object.  It is not ready to be attached to the scene!\n";
		return;
	}
	
std::cerr << "EntityImpOSG::addedSymbolSurface - Adding a new entity-attached symbol surface to the scene\n";
	if( !symbolSurfaceGroup->addChild( impOSG->getTopNode() ) )
	{
		std::cerr << "Error - in EntityImpOSG::addedSymbolSurface, symbol surface \n"
			<< "top node couldn't be added to scene!\n";
	}
}


// ================================================
// removedSymbolSurface
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void EntityImpOSG::removedSymbolSurface( 
	SymbolSurfaceContainer *, SymbolSurface *surface )
{
	// retrieve nodes from symbol surface, remove them from the entity subgraph
	
	// sanity check? container == entity
	
	SymbolSurfaceImpOSG *impOSG = SymbolSurfaceImpOSG::getSymbolSurfaceImpOSGFromSymbolSurface( surface );
	if( impOSG == NULL )
	{
		std::cerr << "Error - in EntityImpOSG::removedSymbolSurface, symbol surface \n"
			<< "has no implementation object.  It can't be removed from the scene!\n";
		return;
	}
	
std::cerr << "EntityImpOSG::removedSymbolSurface - Removing a entity-attached symbol surface from the scene\n";
	if( !symbolSurfaceGroup->removeChild( impOSG->getTopNode() ) )
	{
		std::cerr << "Error - in EntityImpOSG::removedSymbolSurface, symbol surface \n"
			<< "top node couldn't be removed from scene!\n";
	}
}


// ================================================
// getEntityImpOSGFromEntity
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
EntityImpOSG *EntityImpOSG::getEntityImpOSGFromEntity( Entity *entity )
{
	EntityImpOSG *entityImpOSG = NULL;
	
	if( entity == NULL )
		return NULL;
	
	Entity::EntityImpIteratorPair iterPair = entity->getImplementations();
	Entity::EntityImpList::iterator iter;
	for( iter = iterPair.first; iter != iterPair.second; iter++ )
	{
		// RTTI will (hopefully) tell us if imp is an EntityImpOSG or 
		// something else.
		entityImpOSG = dynamic_cast<EntityImpOSG*>(iter->get());
		if( entityImpOSG != NULL )
		{
			break;
		}
	}

	return entityImpOSG;
}


