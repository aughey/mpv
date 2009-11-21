/** <pre>
 *  The MPV Common Library
 *  Copyright (c) 2008
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
 *  2008-10-05 Andrew Sampson
 *      Initial release
 *
 * </pre>
 */

#ifndef TERRAIN_CONTAINER_H
#define TERRAIN_CONTAINER_H

#include <utility>
#include <map>

#include "Referenced.h"
#include "MPVCommonTypes.h"
#include "Terrain.h"


namespace mpv
{

//=========================================================
//! Implements a container for Terrain objects.  It exists mostly to provide a 
//! place to put the new-terrain-object-created signal.
//! 
class MPVCMN_SPEC TerrainContainer : public virtual Referenced
{
public:

	boost::signal<void (TerrainContainer*, Terrain*)> addedTerrain;
	boost::signal<void (TerrainContainer*, Terrain*)> removedTerrain;
	
	typedef std::map< int, RefPtr<Terrain> > TerrainMap;
	typedef std::pair< TerrainMap::iterator, TerrainMap::iterator > TerrainIteratorPair;

	//=========================================================
	//! Constructor
	//! 
	TerrainContainer();

	//=========================================================
	//! Calls update() on the Terrain objects in this container
	//! 
	void updateTerrains( double timeElapsed );

	//=========================================================
	//! Flags each terrain in this container as 
	//! destroyed.
	//! 
//	void flagAllTerrainsAsDestroyed();
	
	//=========================================================
	//! Adds the specified terrain to this container.
	//! \param terrain - the terrain to add
	//! 
	void addTerrain( Terrain *terrain );
	
	//=========================================================
	//! Removes the specified terrain from this container.  
	//! \param terrain - the terrain to remove
	//! 
	void removeTerrain( Terrain *terrain );
	
	//=========================================================
	//! Returns the begin and end iterators for the terrain container.
	//! \return the begin and end iterators for the terrain container.
	//! 
	TerrainIteratorPair getTerrains()
	{
		return TerrainIteratorPair( terrains.begin(), terrains.end() );
	}
	
	//=========================================================
	//! Searches the container for the given terrain.
	//! \param terrainID the id of the terrain to search for
	//! \return a pointer to the requested Terrain, or NULL if not found
	//! 
	Terrain *findTerrain( int terrainID )
	{
		TerrainMap::iterator mapIter = 
			terrains.find( terrainID );
		if( mapIter != terrains.end() )
		{
			return mapIter->second.get();
		}
		return NULL;
	}
	
	//=========================================================
	//! Searches the container for the given terrain.  If a matching 
	//! terrain is not found, a new terrain is created and added.
	//! \param terrainID the id of the terrain to search for
	//! \return a pointer to the requested Terrain
	//! 
	Terrain *findOrCreateTerrain( int terrainID );

protected:
	
	//=========================================================
	//! Destructor
	//! 
	virtual ~TerrainContainer();
	
	//=========================================================
	//! The terrains attached to this object.
	//! 
	TerrainMap terrains;

	//=========================================================
	//! Callback; notification of state change for a terrain
	//! 
//	void terrainChangedState( Terrain *terrain );
};

}
#endif
