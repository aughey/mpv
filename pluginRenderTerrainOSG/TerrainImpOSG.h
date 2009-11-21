/** <pre>
 *  MPV OSG terrain loading plugin
 *  Copyright (c) 2008 Andrew Sampson
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
 *  2008-10-12 Andrew Sampson
 *      Initial release.  Based on code from PluginRenderTerrainOSG.cpp
 *  
 *  
 *  </pre>
 */


#ifndef _TERRAINIMPOSG_H_
#define _TERRAINIMPOSG_H_

#include <OpenThreads/Mutex>
#include <OpenThreads/ScopedLock>
#include <OpenThreads/Thread>
#include <osg/Group>

#include "Terrain.h"

//=========================================================
//! A child class of TerrainImp, which implements loading of a terrain 
//! database via OSG.
//! 
class TerrainImpOSG : public mpv::TerrainImp
{
public:
	//=========================================================
	//! Constructor
	//! \param _terrain - the Terrain object that this TerrainImp is 
	//!        associated with
	//! \param ap - the attachment point for the terrain scene graph
	//! 
	TerrainImpOSG( mpv::Terrain *_terrain, osg::Group *ap );
	
	//! Performs loading of the terrain database for this imp.
	//! Sets state accordingly.
	virtual void load();

	//! Removes this imp's terrain database from memory.
	//! Sets state accordingly.
	virtual void unload();
	
	//! Returns state, and wraps access to state in a mutex.
	virtual mpv::Terrain::TerrainState getState();
	
	
protected:
	//=========================================================
	//! General Destructor
	//! 
	virtual ~TerrainImpOSG();
	
	//! called by terrain loading thread
	void attachTerrainSubgraph( osg::Node *node );
	
	//=========================================================
	//! Sets up the "dummy" terrain.  The dummy terrain consists of a large 
	//! flat green plane at sea level.  It is displayed when there is an 
	//! error loading the requested terrain.
	//! 
	osg::Node *createDummyTerrain();
	
	//=========================================================
	//! Convenience function, used by setupDummyTerrain
	//! \param tilesize - tile edge length
	//! \param linewidth - width of the white lines at the edges of tiles
	//! \return The newly created tile node.
	//! 
	osg::Group *createDummyTile(
		float tilesize = 1000.0, 
		float linewidth = 10.0 // actually half the linewidth, but whatever
	);
	
	//! the loaded scene graph
	osg::ref_ptr<osg::Node> terrainNode;

	//! the attachment point for the terrain scene graph
	osg::ref_ptr<osg::Group> attachmentPoint;
	
	//! mutex which controls access to this->state and attachmentPoint
	OpenThreads::Mutex loaderMutex;

	class LoaderThread : public OpenThreads::Thread
	{
	public:
		LoaderThread( TerrainImpOSG * );
		~LoaderThread();
		
		virtual void run();
		
	protected:
		TerrainImpOSG *terrainImp;
	};
	
	LoaderThread loaderThread;
};

#endif
