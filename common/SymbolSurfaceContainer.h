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
 *  2008-01-24 Andrew Sampson, Boeing
 *      Initial release
 *
 * </pre>
 */

#ifndef SYMBOL_SURFACE_CONTAINER_H
#define SYMBOL_SURFACE_CONTAINER_H

#include <utility>
#include <map>

#include "Referenced.h"
#include "SymbolSurface.h"
#include "MPVCommonTypes.h"

namespace mpv
{

//=========================================================
//! Implements a container for symbol surfaces.  Currently, CIGI specifies 
//! that symbol surfaces can be attached to Views and Entities.  Thus, 
//! the View and Entity classes will inherit from this class.
//! 
class MPVCMN_SPEC SymbolSurfaceContainer : virtual public Referenced
{
public:

	boost::signal<void (SymbolSurfaceContainer*, SymbolSurface*)> addedSymbolSurface;
	boost::signal<void (SymbolSurfaceContainer*, SymbolSurface*)> removedSymbolSurface;
	
	typedef std::map< int, RefPtr<SymbolSurface> > SymbolSurfaceMap;
	typedef std::pair< SymbolSurfaceMap::iterator, SymbolSurfaceMap::iterator > SymbolSurfaceIteratorPair;

	//=========================================================
	//! Constructor
	//! 
	SymbolSurfaceContainer();

	//=========================================================
	//! Flags each symbol surface in this container as 
	//! destroyed.
	//! 
	void flagAllSurfacesAsDestroyed();
	
	//=========================================================
	//! Adds the specified symbol surface to this container.
	//! \param surface - the surface to add
	//! 
	void addSymbolSurface( SymbolSurface *surface );
	
	//=========================================================
	//! Removes the specified symbol surface to this container.  
	//! \param surface - the surface to remove
	//! 
	void removeSymbolSurface( SymbolSurface *surface );
	
	//=========================================================
	//! Returns the begin and end iterators for the symbol surface container.
	//! \return the begin and end iterators for the symbol surface container.
	//! 
	SymbolSurfaceIteratorPair getSymbolSurfaces()
	{
		return SymbolSurfaceIteratorPair( symbolSurfaces.begin(), symbolSurfaces.end() );
	}
	
	//=========================================================
	//! Searches the container for the given symbol surface.
	//! \return a pointer to the requested SymbolSurface, or NULL if not found
	//! 
	SymbolSurface *findSymbolSurface( int surfaceID )
	{
		SymbolSurfaceMap::iterator mapIter = 
			symbolSurfaces.find( surfaceID );
		if( mapIter != symbolSurfaces.end() )
		{
			return mapIter->second.get();
		}
		return NULL;
	}
	
#if 0
	//=========================================================
	//! Returns the underlying map data structure.  This should only be 
	//! called in very rare situations... I didn't plan on allowing 
	//! direct access to this variable.
	//! \return a pointer to the underlying map data structure
	//! 
	SymbolSurfaceMap *getSymbolSurfaceMap()
	{
		return &symbolSurfaces;
	}
#endif
	
protected:
	
	//=========================================================
	//! Destructor
	//! 
	virtual ~SymbolSurfaceContainer();
	
	//=========================================================
	//! The symbol surfaces attached to this object.
	//! 
	SymbolSurfaceMap symbolSurfaces;

	//=========================================================
	//! Callback; notification of state change for a surface
	//! 
	void surfaceChangedState( SymbolSurface *surface );
};

}
#endif
