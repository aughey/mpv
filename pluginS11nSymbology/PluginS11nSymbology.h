/** <pre>
 *  MPV Serialization Framework
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
 *  2009-09-06 Andrew Sampson
 *      Initial release.  SymbolObserver and SymbolSurfaceObserver are from 
 *      PluginRenderSymbologyOSG.
 *
 * </pre>
 */

#ifndef PLUGIN_S11N_SYMBOLOGY_H
#define PLUGIN_S11N_SYMBOLOGY_H

#include <list>

#include "Referenced.h"
#include "Plugin.h"
#include "Symbol.h"
#include "SymbolSurface.h"
#include "SymbolSurfaceContainer.h"

#include "GroupNode.h"


class PluginS11nSymbology : public Plugin 
{
public:

	//=========================================================
	//! General Constructor
	//! 
	PluginS11nSymbology();

	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginS11nSymbology() throw();
	
	//=========================================================
	//! The per-frame processing that this plugin performs
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );
	
	
private:
	//=========================================================
	//! The root node of the serialization tree.
	//! Retrieved from the blackboard.
	//!
	mpvs11n::GroupNode *rootNode;

	//=========================================================
	//! A SymbolContainer, containing all the active symbols.  
	//! Retrieved from the blackboard.
	//! 
	mpv::SymbolContainer *symbols;

	//=========================================================
	//! A SymbolSurfaceContainer, containing all the active symbol surfaces.  
	//! Retrieved from the blackboard.
	//! 
	mpv::SymbolSurfaceContainer *symbolSurfaces;

	//=========================================================
	//! The node under which the symbol surfaces are serialized.
	//! Attached to the root node of the serialization tree.
	//!
	mpv::RefPtr<mpvs11n::GroupNode> symbolSurfacesNode;


	//! An observer class, with methods that are invoked when SymbolSurfaces 
	//! are added/removed.
	//! Slots must be part of classes inheriting from Referenced (well, 
	//! actually boost::signals::trackable).  Plugins do not inherit from 
	//! Referenced (they can't, because they're not reference-counted), 
	//! hence the need for this class.
	class SymbolSurfaceObserver : public mpv::Referenced
	{
	public:
		SymbolSurfaceObserver( PluginS11nSymbology *_plugin ) : 
			mpv::Referenced(), 
			plugin( _plugin )
		{}
		
		//! Adds a SymbolSurfaceImp to newly-created surfaces.  Also adds 
		//! that imp to symbolSurfacesNode.
		//! \param surface - the newly-added surface
		void addedSymbolSurface( mpv::SymbolSurfaceContainer *, mpv::SymbolSurface *surface );
		//! Removes SymbolSurfaceImps from symbolSurfacesNode for surfaces 
		//! that are being destroyed.
		//! \param surface - the surface being destroyed
		void removedSymbolSurface( mpv::SymbolSurfaceContainer *, mpv::SymbolSurface *surface );
		
	protected:
		virtual ~SymbolSurfaceObserver() {}
		
		PluginS11nSymbology *plugin;
	};
	
	mpv::RefPtr<SymbolSurfaceObserver> surfaceObserver;


	//! An observer class, with methods that are invoked when Symbols 
	//! are added/removed.
	//! Slots must be part of classes inheriting from Referenced (well, 
	//! actually boost::signals::trackable).  Plugins do not inherit from 
	//! Referenced (they can't, because they're not reference-counted), 
	//! hence the need for this class.
	class SymbolObserver : public mpv::Referenced
	{
	public:
		SymbolObserver( PluginS11nSymbology *_plugin ) : 
			mpv::Referenced(), 
			plugin( _plugin )
		{}
		
		//! Adds a SymbolImpS11n to newly-created symbols.
		//! \param symbol - the newly-added symbol
		void addedSymbol( mpv::SymbolContainer *, mpv::Symbol *symbol );
		
	protected:
		virtual ~SymbolObserver() {}
		
		PluginS11nSymbology *plugin;
	};
	
	mpv::RefPtr<SymbolObserver> symbolObserver;
};



#endif

