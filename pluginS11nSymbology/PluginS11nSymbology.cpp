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

#include <cassert>
#include <iostream>

#include "MPVExceptions.h"
#include "SymbolText.h"
#include "SymbolCircle.h"
#include "SymbolLine.h"

#include "SymbolImpS11n.h"
#include "SymbolSurfaceImpS11n.h"

#include "SymbolTextImpS11n.h"
#include "SymbolCircleImpS11n.h"
#include "SymbolLineImpS11n.h"

#include "PluginS11nSymbology.h"

using namespace mpv;
using namespace mpvs11n;

EXPORT_DYNAMIC_CLASS( PluginS11nSymbology )

// ================================================
// PluginS11nSymbology
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginS11nSymbology::PluginS11nSymbology() : Plugin()
{
	name_ = "PluginS11nSymbology";

	licenseInfo_.setLicense( LicenseInfo::LicenseLGPL );
	licenseInfo_.setOrigin( "AndrewSampson" );

	dependencies_.push_back( "PluginS11nRoot" );
	dependencies_.push_back( "PluginSymbologyMgr" );

	rootNode = NULL;
	symbols = NULL;
	symbolSurfaces = NULL;

	symbolSurfacesNode = new mpvs11n::GroupNode( "symbol_surfaces" );
	surfaceObserver = new SymbolSurfaceObserver( this );
	symbolObserver = new SymbolObserver( this );
}

// ================================================
// ~PluginS11nSymbology
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginS11nSymbology::~PluginS11nSymbology() throw()
{
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginS11nSymbology::act( SystemState::ID state, StateContext &stateContext )
{
	switch ( state )
	{
	case SystemState::BlackboardRetrieve:

		bb_->get( "SerializationRootNode", rootNode );
		rootNode->addChild( symbolSurfacesNode.get() );

		bb_->get( "Symbols", symbols );
		bb_->get( "SymbolSurfaces", symbolSurfaces );

		// connect to signals, for notification regarding:
		// - addition and removal of symbol surfaces
		symbolSurfaces->addedSymbolSurface.connect( BIND_SLOT2( SymbolSurfaceObserver::addedSymbolSurface, surfaceObserver.get() ) );
		symbolSurfaces->removedSymbolSurface.connect( BIND_SLOT2( SymbolSurfaceObserver::removedSymbolSurface, surfaceObserver.get() ) );
		// - addition of new symbols
		symbols->addedSymbol.connect( BIND_SLOT2( SymbolObserver::addedSymbol, symbolObserver.get() ) );

		break;

	default:
		break;
	}

}


// ================================================
// addedSymbolSurface
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginS11nSymbology::SymbolSurfaceObserver::addedSymbolSurface( 
	SymbolSurfaceContainer *, SymbolSurface *surface )
{
	if( surface == NULL )
	{
		// error - why is this surface NULL?
		return;
	}

	// sanity check
	if( surface->getState() == SymbolSurface::Destroyed )
		return;

	SymbolSurfaceImpS11n *implementation = SymbolSurfaceImpS11n::getSymbolSurfaceImpS11nFromSymbolSurface( surface );
	if( implementation == NULL )
	{
//std::cerr << "PluginS11nSymbology::SymbolSurfaceObserver::addedSymbolSurface - Adding a new symbol surf imp to surface " << surface->getID() << std::endl;
		implementation = new SymbolSurfaceImpS11n( surface );
		surface->addImplementation( implementation );
		
		plugin->symbolSurfacesNode->addChild( implementation->getTopNode() );
	}

}


// ================================================
// removedSymbolSurface
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginS11nSymbology::SymbolSurfaceObserver::removedSymbolSurface( 
	SymbolSurfaceContainer *, SymbolSurface *surface )
{
	if( surface == NULL )
	{
		// error - why is this surface NULL?
		return;
	}

	// sanity check
	if( surface->getState() == SymbolSurface::Destroyed )
		return;

	SymbolSurfaceImpS11n *implementation = SymbolSurfaceImpS11n::getSymbolSurfaceImpS11nFromSymbolSurface( surface );
	if( implementation != NULL )
	{
//std::cerr << "PluginS11nSymbology::SymbolSurfaceObserver::removedSymbolSurface - Removing a symbol surf imp from surface " << surface->getID() << std::endl;
		plugin->symbolSurfacesNode->removeChild( implementation->getTopNode() );
	}

}


// ================================================
// addedSymbol
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginS11nSymbology::SymbolObserver::addedSymbol( 
	SymbolContainer *, Symbol *symbol )
{
	if( symbol == NULL )
	{
		// error - why is this symbol NULL?
		return;
	}

	// sanity check
	if( symbol->getState() == Symbol::Destroyed )
		return;

	SymbolImpS11n *implementation = SymbolImpS11n::getSymbolImpS11nFromSymbol( symbol );
	if( implementation == NULL )
	{
//std::cerr << "PluginS11nSymbology::SymbolObserver::addedSymbol - Adding a new symbol imp to symbol " << symbol->getID() << std::endl;
		switch( symbol->getType() )
		{
		case Symbol::Text:
			symbol->addImplementation( new SymbolTextImpS11n( static_cast<SymbolText*>(symbol) ) );
			break;
		case Symbol::Circle:
			symbol->addImplementation( new SymbolCircleImpS11n( static_cast<SymbolCircle*>(symbol) ) );
			break;
		case Symbol::Line:
			symbol->addImplementation( new SymbolLineImpS11n( static_cast<SymbolLine*>(symbol) ) );
			break;
		default:
			break;
		}
	}

}

