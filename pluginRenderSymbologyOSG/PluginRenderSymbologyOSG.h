/** <pre>
 * The MPV Symbology Plugin Collection
 * Copyright (c) 2007 General Dynamics Land Systems
 * Copyright (c) 2008 The Boeing Company
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *  FILENAME:   PluginRenderSymbologyOSG.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  render symbology.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME
 *  DESCRIPTION OF CHANGE........................
 *  
 *  2007-03-30 Philip Lowman, GDLS
 *      Initial release
 *  
 *  2007-09-26 Philip Lowman
 *      Modified factory member variable so that SymbologyFactory is published
 *      via new blackboard API instead of OSGSymbologyFactory.
 *
 *  2008-01-16 Andrew Sampson, Boeing
 *      Merged some code from GDLS symbology rendering into new plugin; part 
 *      of this class is from PluginRenderSymbologyOSG.  
 *  
 * </pre>
 */

#ifndef PLUGIN_RENDER_SYMBOLOGY_OSG_H
#define PLUGIN_RENDER_SYMBOLOGY_OSG_H

#include <list>
#include <osg/Group>
#include <osgText/Text>

#include "Referenced.h"
#include "Plugin.h"
#include "View.h"
#include "DefFileGroup.h"
#include "Symbol.h"
#include "SymbolSurface.h"
#include "SymbolImpOSG.h"


using namespace mpv;
using namespace mpvosg;

class PluginRenderSymbologyOSG : public Plugin 
{
public:

	//=========================================================
	//! General Constructor
	//! 
	PluginRenderSymbologyOSG();

	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginRenderSymbologyOSG() throw();
	
	//=========================================================
	//! The per-frame processing that this plugin performs
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );
	
	
private:
	//=========================================================
	//! The root node of the scene graph.  Retrieved from the blackboard.
	//! 
	osg::Group *rootNode;

	//=========================================================
	//! Configuration data.  Retrieved from the blackboard.
	//! 
	DefFileGroup **DefFileData;

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
	//! Map containing the view parameters (used for setting
	//! the matrix node to the proper display resolution).  Retrieved 
	//! from the blackboard.
	//!
	std::map< int, View * > *viewParamsMap;

	//=========================================================
	//! Get the configuration from the def files.
	//!
	void get_config();

	//=========================================================
	//! Called by act() when the system is in the Operate and Debug states.
	//! 
	//!
	void operate();

	//! An observer class, with methods that are invoked when SymbolSurfaces 
	//! are added/removed.
	//! Slots must be part of classes inheriting from Referenced (well, 
	//! actually boost::signals::trackable).  Plugins do not inherit from 
	//! Referenced (they can't, because they're not reference-counted), 
	//! hence the need for this class.
	class SymbolSurfaceObserver : public Referenced
	{
	public:
		SymbolSurfaceObserver() : 
			Referenced()
		{}
		
		//! Adds a SymbolSurfaceImp to newly-created surfaces.
		//! \param surface - the newly-added surface
		void addedSymbolSurface( SymbolSurfaceContainer *, SymbolSurface *surface );
		
	protected:
		virtual ~SymbolSurfaceObserver() {}
	};
	
	RefPtr<SymbolSurfaceObserver> surfaceObserver;


	//! An observer class, with methods that are invoked when Symbols 
	//! are added/removed.
	//! Slots must be part of classes inheriting from Referenced (well, 
	//! actually boost::signals::trackable).  Plugins do not inherit from 
	//! Referenced (they can't, because they're not reference-counted), 
	//! hence the need for this class.
	class SymbolObserver : public Referenced
	{
	public:
		SymbolObserver() : 
			Referenced()
		{}
		
		//! Adds a SymbolImpOSG to newly-created symbols.
		//! \param symbol - the newly-added symbol
		void addedSymbol( SymbolContainer *, Symbol *symbol );
		
	protected:
		virtual ~SymbolObserver() {}
	};
	
	RefPtr<SymbolObserver> symbolObserver;
};



#endif

