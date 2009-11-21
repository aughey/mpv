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
 *  FILENAME:   PluginSymbologyMgr.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  maintain state for CIGI symbology.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  2008-01-10 Andrew Sampson
 *      Initial Release.
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#ifndef PLUGIN_SYMBOLOGYMGR_H
#define PLUGIN_SYMBOLOGYMGR_H

#include <string>
#include <map>

#include "RefPtr.h"
#include "AllCigi.h"
#include "Plugin.h"
#include "DefFileParser.h"
#include "Symbol.h"
#include "SymbolSurface.h"
#include "SymbolContainer.h"
#include "SymbolSurfaceContainer.h"
#include "View.h"
#include "Entity.h"
#include "EntityContainer.h"


using namespace mpv;
//using namespace SymbologyMgr;

//=========================================================
//! This plugin listens for symbology-related packets and maintains 
//! state for the symbols.  It is analogous to the Entity Manager plugin.
//! 
class PluginSymbologyMgr : public Plugin 
{
public:
	//=========================================================
	//! General Constructor
	//! 
	PluginSymbologyMgr();

	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginSymbologyMgr() throw();
	
	//=========================================================
	//! The per-frame processing that this plugin performs.
	//! This plugin's act() does ...
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );
	
	
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
	//! The CigiIGSession class for registering user defined packets.
	//! Retrieved from the blackboard.
	//! 
	CigiIGSession *IGSession;

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
	//! A std::map of the parameters for each of the views that this instance 
	//! of MPV will display.  Retrieved from the blackboard.
	//! 
	std::map< int, RefPtr<View> > *viewMap;
	
	//=========================================================
	//! An entity container, containing all the entities.  
	//! Retrieved from the blackboard.
	//! 
	mpv::EntityContainer *allEntities;
	
	//=========================================================
	//! A symbol container.  Posted to the blackboard.
	//! 
	RefPtr<SymbolContainer> symbols;
	
	//=========================================================
	//! A symbol surface container.  Posted to the blackboard.
	//! 
	RefPtr<SymbolSurfaceContainer> symbolSurfaces;

	//=========================================================
	//! Called by act() when in Operate or Debug states.  Updates all the 
	//! active symbols, performs miscellaneous housekeeping, etc.
	//! 
	void operate();
	
	//=========================================================
	//! Retrieves some preferences from the configuration data
	//! 
	void processConfigData();
	
	//=========================================================
	//! Processes a given symbol.  This includes updating the symbol, 
	//! reparenting it (if needed), etc.
	//! \param symbol - the symbol to update
	//! 
	void processSymbol( Symbol *symbol );

	//=========================================================
	//! Creates a new symbol and adds it to the system.  If a symbol already 
	//! exists with the same id, then one of two actions will be performed:
	//! - if the symbol type for the existing symbol matches the type that has 
	//!   been requested, then the existing symbol instance will be returned
	//! - if the existing symbol's type does not match, then the old symbol is 
	//!   destroyed (along with its children) and replaced with a new symbol.
	//! \param id - the symbol ID to use for the new symbol
	//! \param type - the expected SymbolType 
	//! \return a pointer to a newly-created symbol, or an existing symbol
	//! 
	template <class T> T * createSymbol( Cigi_uint16 id, Symbol::SymbolType type )
	{
		{
		// hold on to a reference until signal emission is complete
		RefPtr<Symbol> symbol = symbols->findSymbol( id );
		if( symbol.valid() )
		{
			if( symbol->getType() == type )
			{
				// Symbol exists, and its type matches the expected type.
				return dynamic_cast<T*>( symbol.get() );
			}
			else
			{
				// Symbol exists, but its type does not match the expected type.
				// The CIGI standard says that, in situations like this, the 
				// existing symbol is destroyed (along with its children) 
				// and the new symbol takes its place.
				
				// setting the state is enough; the symbol will be automatically 
				// removed from surfaces, other symbols, the plugin, etc.
				symbol->setState( Symbol::Destroyed );
			}
		}
		}
		
		T *result = new T;
		result->setID( id );
		symbols->addSymbol( result );
		
		return result;
	}

	//=========================================================
	//! Moves the symbol with the given ID to a different parent (or no parent).
	//! \param symbolID - the ID of the symbol to reparent
	//! \param isChild - if true, symbol will be placed as a child under some 
	//!        symbol; if false, symbol will be set up as a top-level symbol
	//! \param newParentID - the ID of the symbol that should be the new parent
	//! 
	void reparentSymbol( int symbolID, bool isChild, int newParentID );

	//=========================================================
	//! Adds a symbol to a symbol surface.
	//! \param symbol - pointer to the symbol to add
	//! \param surfaceID - the ID of the symbol surface that the symbol 
	//!        should be added to
	//! 
	void addSymbolToSurface( Symbol *symbol, int surfaceID );

	//=========================================================
	//! Removes a symbol from a symbol surface.
	//! \param symbol - pointer to the symbol to remove
	//! \param surfaceID - the ID of the symbol surface that the symbol 
	//!        should be removed from
	//! 
	void removeSymbolFromSurface( Symbol *symbol, int surfaceID );

	//=========================================================
	//! Adds a new surface to the plugin's symbolSurfaces container.
	//! \param surface - the surface to add
	//! 
	void addSymbolSurface( SymbolSurface *surface );
	
	//=========================================================
	//! Attaches a surface to the view or entity that it is associated with.
	//! The surface container identity is retrieved from the surface, and 
	//! hence must be valid.
	//! \param surface - the surface to attach
	//! 
	void attachSymbolSurface( SymbolSurface *surface );

	//=========================================================
	//! Detaches a surface from the view or entity that it is associated with.
	//! The surface container identity is retrieved from the surface, and 
	//! hence must be valid.
	//! \param surface - the surface to detach
	//! 
	void detachSymbolSurface( SymbolSurface *surface );

	//=========================================================
	//! Retrieves the symbol surface container for a given symbol surface.
	//! Used by attachSymbolSurface() and detachSymbolSurface().
	//! \param surface - the surface to query
	//! \return the surface container that the surface is attached to, or NULL
	//! 
	SymbolSurfaceContainer *getSurfaceContainer( SymbolSurface *surface );

	//=========================================================
	//! Creates a copy of an existing symbol and adds it to the system.
	//! Called when a symbol clone packet is received.
	//! \param sourceID - the ID of the symbol to copy
	//! \param destinationID - the ID for the resulting symbol
	//! \param sourceIsIGDefined - if true, indicates that the sourceID is an 
	//!     IG-defined symbol template, rather than an existing instanced symbol
	//! 
	void cloneSymbol( int sourceID, int destinationID, bool sourceIsIGDefined );

	//=========================================================
	//! This class processes Symbology Surface Definition packets
	//! 
	class SymbologySurfaceDefP : public CigiBaseEventProcessor  
	{
	public:

		//=========================================================
		//! General Constructor
		//! 
		SymbologySurfaceDefP( PluginSymbologyMgr *_plugin )
			: plugin( _plugin ) {}

		//=========================================================
		//! General Destructor
		//! 
		virtual ~SymbologySurfaceDefP() {}

		//=========================================================
		//! Callback; processes a packet
		//! 
		virtual void OnPacketReceived( CigiBasePacket *Packet );

	private:

		//=========================================================
		//! Pointer to parent container, passed in from plugin
		//! 
		PluginSymbologyMgr *plugin;

	};

	//=========================================================
	//! Callback object for handling symbology control packets.
	//! 
	SymbologySurfaceDefP symbologySurfaceDefProc;

	//=========================================================
	//! This class processes Symbology Control packets
	//! 
	class SymbologyCtrlP : public CigiBaseEventProcessor  
	{
	public:

		//=========================================================
		//! General Constructor
		//! 
		SymbologyCtrlP( PluginSymbologyMgr *_plugin )
			: plugin( _plugin ) {}

		//=========================================================
		//! General Destructor
		//! 
		virtual ~SymbologyCtrlP() {}

		//=========================================================
		//! Callback; processes a packet
		//! 
		virtual void OnPacketReceived( CigiBasePacket *Packet );

	private:

		//=========================================================
		//! Pointer to parent container, passed in from plugin
		//! 
		PluginSymbologyMgr *plugin;

	};

	//=========================================================
	//! Callback object for handling symbology control packets.
	//! 
	SymbologyCtrlP symbologyCtrlProc;

	//=========================================================
	//! This class processes Short Symbology Control packets
	//! 
	class ShortSymbologyCtrlP : public CigiBaseEventProcessor  
	{
	public:

		//=========================================================
		//! General Constructor
		//! 
		ShortSymbologyCtrlP( PluginSymbologyMgr *_plugin )
			: plugin( _plugin ) {}

		//=========================================================
		//! General Destructor
		//! 
		virtual ~ShortSymbologyCtrlP() {}

		//=========================================================
		//! Callback; processes a packet
		//! 
		virtual void OnPacketReceived( CigiBasePacket *Packet );

	private:

		//=========================================================
		//! Pointer to parent container, passed in from plugin
		//! 
		PluginSymbologyMgr *plugin;

	};

	//=========================================================
	//! Callback object for handling short symbology control packets.
	//! 
	ShortSymbologyCtrlP shortSymbologyCtrlProc;

	//=========================================================
	//! This class processes Text Symbol Definition packets
	//! 
	class TextSymbolDefinitionP : public CigiBaseEventProcessor  
	{
	public:

		//=========================================================
		//! General Constructor
		//! 
		TextSymbolDefinitionP( PluginSymbologyMgr *_plugin )
			: plugin( _plugin ) {}

		//=========================================================
		//! General Destructor
		//! 
		virtual ~TextSymbolDefinitionP() {}

		//=========================================================
		//! Callback; processes a packet
		//! 
		virtual void OnPacketReceived( CigiBasePacket *Packet );

	private:

		//=========================================================
		//! Pointer to parent container, passed in from plugin
		//! 
		PluginSymbologyMgr *plugin;

	};

	//=========================================================
	//! Callback object for handling text symbol definition packets.
	//! 
	TextSymbolDefinitionP textSymbolDefinitionProc;

	//=========================================================
	//! This class processes Circle Symbol Definition packets
	//! 
	class CircleSymbolDefinitionP : public CigiBaseEventProcessor  
	{
	public:

		//=========================================================
		//! General Constructor
		//! 
		CircleSymbolDefinitionP( PluginSymbologyMgr *_plugin )
			: plugin( _plugin ) {}

		//=========================================================
		//! General Destructor
		//! 
		virtual ~CircleSymbolDefinitionP() {}

		//=========================================================
		//! Callback; processes a packet
		//! 
		virtual void OnPacketReceived( CigiBasePacket *Packet );

	private:

		//=========================================================
		//! Pointer to parent container, passed in from plugin
		//! 
		PluginSymbologyMgr *plugin;

	};

	//=========================================================
	//! Callback object for handling circle symbol definition packets.
	//! 
	CircleSymbolDefinitionP circleSymbolDefinitionProc;

	//=========================================================
	//! This class processes Line Symbol Definition packets
	//! 
	class LineSymbolDefinitionP : public CigiBaseEventProcessor  
	{
	public:

		//=========================================================
		//! General Constructor
		//! 
		LineSymbolDefinitionP( PluginSymbologyMgr *_plugin )
			: plugin( _plugin ) {}

		//=========================================================
		//! General Destructor
		//! 
		virtual ~LineSymbolDefinitionP() {}

		//=========================================================
		//! Callback; processes a packet
		//! 
		virtual void OnPacketReceived( CigiBasePacket *Packet );

	private:

		//=========================================================
		//! Pointer to parent container, passed in from plugin
		//! 
		PluginSymbologyMgr *plugin;

	};

	//=========================================================
	//! Callback object for handling line symbol definition packets.
	//! 
	LineSymbolDefinitionP lineSymbolDefinitionProc;

	//=========================================================
	//! This class processes Symbol Clone Definition packets
	//! 
	class SymbolCloneDefinitionP : public CigiBaseEventProcessor  
	{
	public:

		//=========================================================
		//! General Constructor
		//! 
		SymbolCloneDefinitionP( PluginSymbologyMgr *_plugin )
			: plugin( _plugin ) {}

		//=========================================================
		//! General Destructor
		//! 
		virtual ~SymbolCloneDefinitionP() {}

		//=========================================================
		//! Callback; processes a packet
		//! 
		virtual void OnPacketReceived( CigiBasePacket *Packet );

	private:

		//=========================================================
		//! Pointer to parent container, passed in from plugin
		//! 
		PluginSymbologyMgr *plugin;

	};

	//=========================================================
	//! Callback object for handling symbol-clone definition packets.
	//! 
	SymbolCloneDefinitionP symbolCloneDefinitionProc;

};



#endif

