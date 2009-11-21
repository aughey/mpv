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


#include <iostream>
#include <string>
#if WIN32
   #include <Windows.h> // for Sleep()!
#else
   #include <unistd.h> // for sleep()
#endif
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>

#include "TerrainImpOSG.h"

#include "MoveDummyTerrainTransform.h"


using namespace mpv;

// ================================================
// TerrainImpOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
TerrainImpOSG::TerrainImpOSG( Terrain *_terrain, osg::Group *ap ) : 
	TerrainImp( _terrain ), 
	attachmentPoint( ap ), 
	loaderThread( this )
{
	
}


// ================================================
// ~TerrainImpOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
TerrainImpOSG::~TerrainImpOSG() 
{
	unload();
}


// ================================================
// load
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void TerrainImpOSG::load()
{
	unload();
	
	loaderMutex.lock();
	state = Terrain::Loading;
	
	loaderThread.start();
	loaderMutex.unlock();
}


// ================================================
// attachTerrainSubgraph
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void TerrainImpOSG::attachTerrainSubgraph( osg::Node *node )
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock( loaderMutex );
	
	terrainNode = node;
	if( terrainNode.valid() )
	{
		attachmentPoint->addChild( terrainNode.get() );
		// Get the pager and register the terrain node with it.
		// This is required for the PagedLOD stuff.
    /*
		osgDB::DatabasePager* pager =
			osgDB::Registry::instance()->getOrCreateDatabasePager();
		pager->registerPagedLODs(attachmentPoint.get());
    */

		state = Terrain::Loaded;
	}
	else
	{
		std::cerr << "TerrainImpOSG::load() - " 
			<< "could not load terrain database \""
			<< terrain->getName() << "\" (id " << terrain->getID() << ") : \n"
			<< "\tthere was an error while loading the following file: \n"
			<< "\t" << terrain->getDirectoryname() << "/" << terrain->getFilename()
			<< std::endl;
		
		terrainNode = createDummyTerrain();
		attachmentPoint->addChild( terrainNode.get() );

		state = Terrain::Broken;
	}
	
}


// ================================================
// unload
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void TerrainImpOSG::unload()
{
	// wait for in-progress load to complete
	while( getState() == Terrain::Loading ) // getState() does locking
	{
#if WIN32
    Sleep( 1000 );
#else
    sleep( 1 );
#endif
	}
	
	{
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock( loaderMutex );
	
	// clear various caches; this reduces the mpv's memory footprint 
	// following a database switch.  
	// Unfortunately there's still a leak somewhere, as the memory usage 
	// never returns to its base level
	osgDB::Registry::instance()->clearObjectCache();
	osgDB::Registry::instance()->clearArchiveCache();
	//osgDB::Registry::instance()->getOrCreateDatabasePager()->clear();
	
	if( terrainNode.valid() )
	{
		attachmentPoint->removeChild( terrainNode.get() );
		terrainNode = NULL;
	}
	state = Terrain::NotLoaded;
	}
}


// ================================================
// getState
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Terrain::TerrainState TerrainImpOSG::getState()
{
	loaderMutex.lock();
	Terrain::TerrainState result = state;
	loaderMutex.unlock();

	return result;
}



// ================================================
// createDummyTerrain
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
osg::Node *TerrainImpOSG::createDummyTerrain()
{
	std::cout << "TerrainImpOSG - creating dummy terrain\n";

	float xmin, xmax, ymin, ymax;
	float tilesize = 1000.0;
	float linewidth = 10.0; // actually half the linewidth, but whatever

	xmin = -20. * tilesize;
	xmax = 20. * tilesize;
	ymin = -20. * tilesize;
	ymax = 20. * tilesize;

	osg::Transform *terrainTransformNode = new MoveDummyTerrainTransform;

	// turn off culling for this terrain
	terrainTransformNode->setCullingActive(false);

	osg::Group *tileNode = createDummyTile( tilesize, linewidth );

	for( float x = xmin; x < xmax; x += tilesize ) {
		for( float y = ymin; y < ymax; y += tilesize ) {
			osg::PositionAttitudeTransform *transformNode = 
				new osg::PositionAttitudeTransform;
			transformNode->setPosition( osg::Vec3( x, y, 0.0f ) );
			transformNode->addChild( tileNode );
			terrainTransformNode->addChild( transformNode );
		}
		
	}

	return terrainTransformNode;
}


// ================================================
// createDummyTile
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
osg::Group *TerrainImpOSG::createDummyTile(
	float tilesize, float linewidth )
{
	
	osg::Group *tileNode = new osg::Group;
	osg::Geode *geode = new osg::Geode;
	osg::Vec3Array *normalArray = new osg::Vec3Array;
	normalArray->push_back( osg::Vec3( 0., 0., 1. ) );
	
	// GREEN TILE GEOMETRY ///////////////////////////////////////////
	osg::Geometry* geometry = new osg::Geometry();
	
	osg::Vec3 tileCoords[] =
	{
		osg::Vec3(0.0f, 0.0f, 0.0f),
		osg::Vec3(tilesize, 0.0f, 0.0f),
		osg::Vec3(tilesize, tilesize, 0.0f),
		osg::Vec3(0.0f, tilesize, 0.0f)
	};
	int numCoords = sizeof(tileCoords)/sizeof(osg::Vec3);
	
	osg::Vec3Array* vertices = new osg::Vec3Array(numCoords,tileCoords);

	osg::Vec4Array *colorarray = new osg::Vec4Array;
	colorarray->push_back( osg::Vec4( 0,0.5,0,1 ) );
	geometry->setColorBinding( osg::Geometry::BIND_OVERALL );
	geometry->setColorArray( colorarray );
	
	geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );
	geometry->setNormalArray( normalArray );
	
	geometry->setVertexArray(vertices);
	geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,numCoords));
	geometry->dirtyDisplayList();
	geometry->dirtyBound();

	geode->addDrawable(geometry);

	// LINES AROUND TILES ////////////////////////////////////////////
	geometry = new osg::Geometry();
	
	osg::Vec3 lineCoords[] =
	{
		osg::Vec3(linewidth * -1., tilesize + linewidth, 1.0f),
		osg::Vec3(linewidth, tilesize - linewidth, 1.0f),
		osg::Vec3(tilesize + linewidth, tilesize + linewidth, 1.0f),
		osg::Vec3(tilesize - linewidth, tilesize - linewidth, 1.0f),
		osg::Vec3(tilesize + linewidth, linewidth * -1., 1.0f),
		osg::Vec3(tilesize - linewidth, linewidth, 1.0f)
	};
	numCoords = sizeof(lineCoords)/sizeof(osg::Vec3);
	
	vertices = new osg::Vec3Array(numCoords,lineCoords);

	colorarray = new osg::Vec4Array;
	colorarray->push_back( osg::Vec4( 1,1,1,1 ) );
	geometry->setColorBinding( osg::Geometry::BIND_OVERALL );
	geometry->setColorArray( colorarray );
	
	geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );
	geometry->setNormalArray( normalArray );
	
	geometry->setVertexArray(vertices);
	geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP,0,numCoords));
	geometry->dirtyDisplayList();
	geometry->dirtyBound();

	geode->addDrawable(geometry);


	tileNode->addChild( geode );
	return tileNode;
}


TerrainImpOSG::LoaderThread::LoaderThread( TerrainImpOSG *imp ) : 
	OpenThreads::Thread(), 
	terrainImp( imp )
{
	
}

TerrainImpOSG::LoaderThread::~LoaderThread()
{
	
}

void TerrainImpOSG::LoaderThread::run()
{
	std::string filename = terrainImp->terrain->getDirectoryname() + "/" + terrainImp->terrain->getFilename();
	osg::Node *node = osgDB::readNodeFile( filename );
	terrainImp->attachTerrainSubgraph( node );
}

