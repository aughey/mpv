/** <pre>
 *  MPV Serialization Framework - Entities plugin
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
 *  2008-01-11 Andrew Sampson
 *      Initial release
 *  
 *  
 *  </pre>
 */


#include <iostream>

#include "PluginS11nEntities.h"
#include "EntityImpS11n.h"

using namespace mpv;
using namespace mpvs11n;

EXPORT_DYNAMIC_CLASS( PluginS11nEntities )

// ================================================
// PluginS11nEntities
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginS11nEntities::PluginS11nEntities() : Plugin()
{
	name_ = "PluginS11nEntities";

	licenseInfo_.setLicense( LicenseInfo::LicenseLGPL );
	licenseInfo_.setOrigin( "AndrewSampson" );

	dependencies_.push_back( "PluginS11nRoot" );
	dependencies_.push_back( "PluginEntityMgr" );

	rootNode = NULL;
	allEntities = NULL;
	topLevelEntities = NULL;
	
	entitiesNode = new mpvs11n::GroupNode( "entities" );
}


// ================================================
// ~PluginS11nEntities
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginS11nEntities::~PluginS11nEntities() throw()
{
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginS11nEntities::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{
	case SystemState::BlackboardRetrieve:
		
		bb_->get( "SerializationRootNode", rootNode );
		rootNode->addChild( entitiesNode.get() );
		
		bb_->get( "AllEntities", allEntities );
		bb_->get( "TopLevelEntities", topLevelEntities );
		
		// set up notification for:
		// - entity creation 
		allEntities->addedEntity.connect( BIND_SLOT2( PluginS11nEntities::addedEntity, this ) );
		// - entity addition/removal from the top-level container
		topLevelEntities->addedEntity.connect( BIND_SLOT2( PluginS11nEntities::addedTopLevelEntity, this ) );
		topLevelEntities->removedEntity.connect( BIND_SLOT2( PluginS11nEntities::removedTopLevelEntity, this ) );

		break;

	default:
		break;
	}
	
}


// ================================================
// addedEntity
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginS11nEntities::addedEntity( EntityContainer *, Entity *entity )
{
	if( entity == NULL )
	{
		// error - why is this entity NULL?
		return;
	}

	// sanity check
	if( entity->getState() == Entity::Remove )
		return;

	EntityImpS11n *implementation = EntityImpS11n::getEntityImpS11nFromEntity( entity );
	if( implementation == NULL )
	{
std::cerr << "PluginS11nEntities::addedEntity - Adding a new entity imp to entity " << entity->getID() << std::endl;
		implementation = new EntityImpS11n( entity );
		entity->addImplementation( implementation );
	}

}

// ================================================
// addedTopLevelEntity
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginS11nEntities::addedTopLevelEntity( 
	EntityContainer *container, Entity *entity )
{
	if( entity == NULL )
	{
		// error - why is this entity NULL?
		return;
	}

	// sanity check
	if( entity->getState() == Entity::Remove )
		return;

	EntityImpS11n *implementation = EntityImpS11n::getEntityImpS11nFromEntity( entity );
	if( implementation == NULL )
	{
		// perhaps the entity was added to the top-level entities list before 
		// being added to the all-entities list?
		addedEntity( container, entity );
		implementation = EntityImpS11n::getEntityImpS11nFromEntity( entity );
	}
	
	entitiesNode->addChild( implementation->getTopNode() );
}


// ================================================
// removedTopLevelEntity
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginS11nEntities::removedTopLevelEntity( 
	EntityContainer *, Entity *entity )
{
	if( entity == NULL )
	{
		// error - why is this entity NULL?
		return;
	}

	// sanity check
	EntityImpS11n *implementation = EntityImpS11n::getEntityImpS11nFromEntity( entity );
	if( implementation == NULL )
	{
		// hmm... no EntityImpS11n attached to entity...
		return;
	}
	
	entitiesNode->removeChild( implementation->getTopNode() );
}


