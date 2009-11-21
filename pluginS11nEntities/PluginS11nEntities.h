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


#ifndef _PLUGINS11NENTITIES_H_
#define _PLUGINS11NENTITIES_H_

#include <string>

#include "RefPtr.h"
#include "Plugin.h"
#include "Entity.h"

#include "GroupNode.h"


//=========================================================
//! Manages the entities branch of the serialization graph.
//! 
class PluginS11nEntities : public Plugin 
{
public:
	//=========================================================
	//! General Constructor
	//! 
	PluginS11nEntities();

protected:
	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginS11nEntities() throw();
	
public:
	//=========================================================
	//! The per-frame processing that this plugin performs.
	//! This plugin's act() does ...
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );
	
	
private:
	
	void addedEntity( mpv::EntityContainer *, mpv::Entity *entity );
	
	void addedTopLevelEntity( mpv::EntityContainer *container, mpv::Entity *entity );
	
	void removedTopLevelEntity( mpv::EntityContainer *, mpv::Entity *entity );

	//=========================================================
	//! The root node of the serialization tree.
	//! Retrieved from the blackboard.
	//!
	mpvs11n::GroupNode *rootNode;

	//=========================================================
	//! An entity container, containing all the entities.  
	//! Retrieved from the blackboard.
	//! 
	mpv::EntityContainer *allEntities;
	
	//=========================================================
	//! An entity container, containing only the top-level entities.  
	//! Retrieved from the blackboard.
	//! 
	mpv::EntityContainer *topLevelEntities;

	//=========================================================
	//! The node under which the top level entities are serialized.
	//! Attached to the root node of the serialization tree.
	//!
	mpv::RefPtr<mpvs11n::GroupNode> entitiesNode;

};

#endif

