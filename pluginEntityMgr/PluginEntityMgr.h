/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2008 The Boeing Company
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
 *  FILENAME:   PluginEntityMgr.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  maintain state for CIGI entities.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  04/04/2004 Greg Basler
 *      Initial Release.
 *
 *  2007-07-01 Andrew Sampson
 *      Ported to new plugin API
 *
 *  2008-07-07 Andrew Sampson
 *      Rewrote entity manager.  Mostly based on symbology mgr plugin.
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#ifndef _PLUGIN_ENTITY_MGR_H_
#define _PLUGIN_ENTITY_MGR_H_

#include <string>
#include <map>

#include <DynamicClass.hpp>

#include "RefPtr.h"
#include "AllCigi.h"
#include "Plugin.h"
#include "DefFileParser.h"
#include "Entity.h"
#include "EntityContainer.h"
#include "CoordinateConverter.h"

#include "EntityFactory.h"
#include "ProcEntityCtrl.h"
#include "ProcArtPart.h"
#include "ProcShortArtPart.h"
#include "ProcCompCtrl.h"
#include "ProcShortCompCtrl.h"
#include "EntityCoordinateConversionObserver.h"
#include "AnimStopNotificationListener.h"


using namespace mpv;

//=========================================================
//! This plugin listens for entity-related packets and maintains 
//! state for the entities.  
//! 
class PluginEntityMgr : public Plugin 
{
public:
	//=========================================================
	//! General Constructor
	//! 
	PluginEntityMgr();

protected:
	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginEntityMgr() throw();
	
public:
	//=========================================================
	//! The per-frame processing that this plugin performs.
	//! This plugin's act() does ...
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );
	
	//=========================================================
	//! Retrieves the entity with the given ID.
	//! \param entityID - the ID of the entity
	//! \return pointer to requested entity, or NULL if not found
	//! 
	Entity *findEntity( int entityID )
	{
		return allEntities->findEntity( entityID );
	}

	//=========================================================
	//! Creates a new entity and adds it to the system.  
	//! \param id - the entity instance ID to use for the new entity
	//! \param type - the entity type ID to use for the new entity
	//! \return a pointer to the newly-created entity
	//! 
	Entity *createEntity( int id, int type );

	//=========================================================
	//! Moves the entity with the given ID to a different parent (or no parent).
	//! \param entityID - the ID of the entity to reparent
	//! \param isChild - if true, entity will be placed as a child under some 
	//!        entity; if false, entity will be set up as a top-level entity
	//! \param newParentID - the ID of the entity that should be the new parent
	//! \return True indicates success, false indicates failure.  Failure could 
	//!         occur if, for example, the entity or parent entities don't exist
	//! 
	bool reparentEntity( int entityID, bool isChild, int newParentID );

	
private:
	//=========================================================
	//! The incoming message buffer.  Retrieved from the blackboard.
	//! 
	CigiIncomingMsg *ImsgPtr;

	//=========================================================
	//! The outgoing message buffer.  Retrieved from the blackboard.
	//! 
	CigiOutgoingMsg *OmsgPtr;

	//=========================================================
	//! The root of the def-file tree.  Retrieved from the blackboard.
	//! 
	DefFileGroup **DefFileData;
	
	//=========================================================
	//! The time elapsed, in seconds, during the previous frame.  
	//! Retrieved from the blackboard.
	//! 
	double *timeElapsedLastFrame;
	
	//=========================================================
	//! Coordinate converter, for converting entity positions from 
	//! GDC->database coordinates.  Used by conversionObserver.  
	//! Retrieved from the blackboard.
	//! 
	CoordinateConverter *coordinateConverter;
	
	//=========================================================
	//! An entity container, containing all the entities.  
	//! Posted to the blackboard.
	//! 
	mpv::RefPtr<EntityContainer> allEntities;
	
	//=========================================================
	//! An entity container, containing only the top-level entities.  
	//! Posted to the blackboard.
	//! 
	mpv::RefPtr<EntityContainer> topLevelEntities;
	
	//=========================================================
	//! A factory for creating entities.
	//! 
	EntityFactory factory;
	
	//=========================================================
	//! Performs automatic coordinate conversion on all entities in 
	//! topLevelEntities.  
	//! 
	mpv::RefPtr<EntityCoordinateConversionObserver> conversionObserver;
	
	//=========================================================
	//! Emits animation stop notification packets, when entity animations 
	//! stop playing.
	//! 
	mpv::RefPtr<AnimStopNotificationListener> animStopNotificationListener;
	
	//=========================================================
	//! Callback object for handling entity control packets.
	//! 
	ProcEntityCtrl entityCtrlProc;

	//=========================================================
	//! Callback object for handling artpart control packets.
	//! 
	ProcArtPart artPartProc;

	//=========================================================
	//! Callback object for handling short artpart control packets.
	//! 
	ProcShortArtPart shortArtPartProc;
	
	//=========================================================
	//! Callback object for handling component control packets.
	//! 
	ProcCompCtrl compCtrlProc;

	//=========================================================
	//! Callback object for handling short component control packets.
	//! 
	ProcShortCompCtrl shortCompCtrlProc;

	//=========================================================
	//! Called by act() when in Operate or Debug states.  Updates all the 
	//! active entities, performs miscellaneous housekeeping, etc.
	//! 
	void operate();
	
	//=========================================================
	//! Retrieves some preferences from the configuration data
	//! 
	void processConfigData();
	
	//=========================================================
	//! Processes a given entity.  This includes updating the entity, 
	//! reparenting it (if needed), etc.
	//! \param entity - the entity to update
	//! 
	void processEntity( Entity *entity );


};


#endif

