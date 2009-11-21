/** <pre>
 *  The MPV Symbology Plugin Collection
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
 *  2008-01-28 Andrew Sampson, Boeing
 *      Initial release
 *
 * </pre>
 */

#include "BindSlot.h"
#include "SymbolSurfaceContainer.h"

using namespace mpv;


SymbolSurfaceContainer::SymbolSurfaceContainer() : Referenced()
{
	
}


SymbolSurfaceContainer::~SymbolSurfaceContainer()
{
	// The CIGI standard states that, when a view or entity is deleted, all the 
	// symbol surfaces associated with that object are deleted.  
	flagAllSurfacesAsDestroyed();
}


void SymbolSurfaceContainer::flagAllSurfacesAsDestroyed()
{
	while( !symbolSurfaces.empty() )
	{
		// hold on to a reference until signal emission is complete
		RefPtr<SymbolSurface> surface( symbolSurfaces.begin()->second );

		surface->setState( SymbolSurface::Destroyed );
		// note - callback surfaceChangedState will handle removal of surface 
		// from the map
	}
}


void SymbolSurfaceContainer::addSymbolSurface( SymbolSurface *surface )
{
	if( surface != NULL )
	{
		surface->stateChanged.connect( BIND_SLOT1( SymbolSurfaceContainer::surfaceChangedState, this ) );
		symbolSurfaces[ surface->getID() ] = surface;
		
		addedSymbolSurface( this, surface );
	}
}


void SymbolSurfaceContainer::removeSymbolSurface( SymbolSurface *surface )
{
	SymbolSurfaceMap::iterator iter = symbolSurfaces.find( surface->getID() );
	if( iter != symbolSurfaces.end() )
	{
		// hold on to a reference until signal emission is complete
		RefPtr<SymbolSurface> surfaceReference( surface );
		
		surface->stateChanged.disconnect( BIND_SLOT1( SymbolSurfaceContainer::surfaceChangedState, this ) );
		symbolSurfaces.erase( iter );
		
		removedSymbolSurface( this, surface );
	}
}



void SymbolSurfaceContainer::surfaceChangedState( SymbolSurface *surface )
{
	if( surface->getState() == SymbolSurface::Destroyed )
		removeSymbolSurface( surface );
}
