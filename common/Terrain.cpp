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
 *  2008-09-29 Andrew Sampson
 *      Initial release
 *  
 *  </pre>
 */


#include "Terrain.h"


using namespace mpv;

// ================================================
// Terrain
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Terrain::Terrain() : ComponentContainer(),
	id( 0xffff )
{
	
}


// ================================================
// ~Terrain
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Terrain::~Terrain() 
{
	
}


void Terrain::load()
{
	TerrainImpList::iterator iter;
	for( iter = imps.begin(); iter != imps.end(); iter++ )
	{
		if( (*iter).valid() )
			(*iter)->load();
	}
}


void Terrain::unload()
{
	TerrainImpList::iterator iter;
	for( iter = imps.begin(); iter != imps.end(); iter++ )
	{
		if( (*iter).valid() )
			(*iter)->unload();
	}
}


void Terrain::update( double timeElapsed )
{
	TerrainImpList::iterator iter;
	for( iter = imps.begin(); iter != imps.end(); iter++ )
	{
		if( (*iter).valid() )
			(*iter)->update( timeElapsed );
	}
}


Terrain::TerrainState Terrain::getState()
{
	// If no implementation objects are registered for this Terrain, then 
	// there's nothing to do.
	if( imps.empty() )
	{
		return Loaded;
	}
	
	bool isNotLoaded = false;
	bool isLoading = false;
	bool isLoaded = false;
	bool isBroken = false;
	
	TerrainImpList::iterator iter;
	for( iter = imps.begin(); iter != imps.end(); iter++ )
	{
		if( (*iter).valid() )
		{
			TerrainState state = (*iter)->getState();
			if( state == NotLoaded )
				isNotLoaded = true;
			else if( state == Loading )
				isLoading = true;
			else if( state == Loaded )
				isLoaded = true;
			else if( state == Broken )
				isBroken = true;
		}
	}
	
	/*
	Truth table:
	
	notL Ling Led  Brk  out    notL Ling Led  Brk  out
	---- ---- ---- ---- ----   ---- ---- ---- ---- ----
	 0    0    0    0   Brk     1	 0    0    0   notL
	 0    0    0    1   Brk     1	 0    0    1   Brk
	 0    0    1    0   Led     1	 0    1    0   Led
	 0    0    1    1   Brk     1	 0    1    1   Brk
	 0    1    0    0   Ling    1	 1    0    0   Ling
	 0    1    0    1   Ling    1	 1    0    1   Ling
	 0    1    1    0   Ling    1	 1    1    0   Ling
	 0    1    1    1   Ling    1	 1    1    1   Ling
	
	The logic below is derived from the above table.
	*/
	
	if( isLoading )
		return Loading;
	
	if( isNotLoaded && !isLoading && !isLoaded && !isBroken )
		return NotLoaded;
	
	if( isLoaded && !isBroken )
		return Loaded;
	
	return Broken;
}


void Terrain::setID( int newID )
{
	id = newID;
}

void Terrain::setName( const std::string &newName )
{
	name = newName;
}

void Terrain::setFilename( const std::string &newFilename )
{
	filename = newFilename;
}

void Terrain::setDirectoryname( const std::string &newDirectoryname )
{
	directoryname = newDirectoryname;
}

void Terrain::setCoordSys( const CoordSysParams &newCoordSys )
{
	coordSys = newCoordSys;
}


void Terrain::addImplementation( TerrainImp *newImp )
{
	if( newImp != NULL )
		imps.push_back( newImp );
}

