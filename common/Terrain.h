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


#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "MPVCommonTypes.h"
#include "ComponentContainer.h"
#include "CoordSysParams.h"

namespace mpv
{

// forward declaration; defined at bottom of file
class TerrainImp;


//=========================================================
//! 
//! 
class MPVCMN_SPEC Terrain : public ComponentContainer
{
public:
	
	enum TerrainState
	{
		NotLoaded,
		Loading,
		Loaded,
		Broken
	};
	
	
	typedef std::list< RefPtr<TerrainImp> > TerrainImpList;
	typedef std::pair< TerrainImpList::iterator, TerrainImpList::iterator > TerrainImpIteratorPair;

//public:
	//=========================================================
	//! General Constructor
	//! 
	Terrain();
	
	void load();
	
	void unload();
	
	void update( double timeElapsed );
	
	TerrainState getState();

	int getID() const { return id; }
	const std::string &getName() const { return name; }
	const std::string &getFilename() const { return filename; }
	const std::string &getDirectoryname() const { return directoryname; }
	const CoordSysParams &getCoordSys() const { return coordSys; }
	
	void setID( int newID );
	void setName( const std::string &newName );
	void setFilename( const std::string &newFilename );
	void setDirectoryname( const std::string &newDirectoryname );
	void setCoordSys( const CoordSysParams &newCoordSys );
	
	void addImplementation( TerrainImp *newImp );
	
	TerrainImpIteratorPair getImplementations()
	{
		return TerrainImpIteratorPair( imps.begin(), imps.end() );
	}
	
protected:
	//=========================================================
	//! General Destructor
	//! 
	virtual ~Terrain();
	
	//! The CIGI database ID for this terrain
	int id;
	
	//! A descriptive, human-readable name
	std::string name;
	
	//! The filename for the database file.  This string should not include 
	//! the path.
	std::string filename;
	
	//! The absolute path name for the directory containing the database
	std::string directoryname;
	
	//! Parameters which define the coordinate system for this database
	CoordSysParams coordSys;

	//! Implementation objects for this Terrain.  
	TerrainImpList imps;
};



//=========================================================
//! An implementation class, which complements Terrain.
//! 
class MPVCMN_SPEC TerrainImp : public Referenced
{
public:
	//=========================================================
	//! Constructor
	//! \param _terrain - the Terrain object that this TerrainImp is 
	//!        associated with
	//! 
	TerrainImp( Terrain *_terrain ) : 
		Referenced(), 
		terrain( _terrain ),
		state( Terrain::NotLoaded )
	{}
	
	//! Performs loading of the terrain database for this imp.
	//! Sets state accordingly.
	//! If loading is performed in a parallel thread, then this method should 
	//! set state to Loading, kick off the loading thread, and return.  The 
	//! state should be updated to either Loaded or Broken once loading is 
	//! finished.  If you're worried about concurrency issues re. state, then 
	//! you can override getState() and wrap access to state in a mutex.
	virtual void load() = 0;

	//! Removes this imp's terrain database from memory.
	//! Sets state accordingly.
	virtual void unload() = 0;
	
	virtual void update( double timeElapsed ) {}

	virtual Terrain::TerrainState getState() { return state; }

protected:
	//=========================================================
	//! General Destructor
	//! 
	virtual ~TerrainImp() {}
	
	//=========================================================
	//! Pointer to the owning Terrain.  
	//!
	Terrain *terrain;

	Terrain::TerrainState state;

};


}

#endif
