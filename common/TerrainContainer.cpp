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

#include "TerrainContainer.h"

using namespace mpv;


TerrainContainer::TerrainContainer() : Referenced()
{
	
}


TerrainContainer::~TerrainContainer()
{
	// when a TerrainContainer is deleted, all Terrains attached to it are deleted
//	flagAllTerrainsAsDestroyed();
}


void TerrainContainer::updateTerrains( double timeElapsed )
{
	TerrainMap::iterator iter = terrains.begin();
	for( ; iter != terrains.end(); iter++ )
	{
		iter->second->update( timeElapsed );
	}
}


/*void TerrainContainer::flagAllTerrainsAsDestroyed()
{
	while( !terrains.empty() )
	{
		// hold on to a reference until signal emission is complete
		RefPtr<Terrain> terrain( terrains.begin()->second );

		terrain->setState( Terrain::Destroyed );
		// note - callback terrainChangedState will handle removal of terrain 
		// from the map
	}
}*/


void TerrainContainer::addTerrain( Terrain *terrain )
{
	if( terrain != NULL )
	{
//		terrain->stateChanged.connect( SLOT( this, TerrainContainer::terrainChangedState ) );
		terrains[terrain->getID()] = terrain;

		addedTerrain( this, terrain );
	}
}


void TerrainContainer::removeTerrain( Terrain *terrain )
{
	TerrainMap::iterator iter = terrains.find( terrain->getID() );
	if( iter != terrains.end() )
	{
		// hold on to a reference until signal emission is complete
		RefPtr<Terrain> terrainReference( terrain );
		
//		terrain->stateChanged.disconnect( SLOT( this, TerrainContainer::terrainChangedState ) );
		terrains.erase( iter );
		
		removedTerrain( this, terrain );
	}
}


Terrain *TerrainContainer::findOrCreateTerrain( int terrainID )
{
	Terrain *result = findTerrain( terrainID );
	if( result == NULL )
	{
		result = new Terrain;
		result->setID( terrainID );
		addTerrain( result );
	}
	return result;
}


/*void TerrainContainer::terrainChangedState( Terrain *terrain )
{
	if( terrain->getState() == Terrain::Destroyed )
		removeTerrain( terrain );
}*/

