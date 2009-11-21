/** <pre>
 *  Plugin for the MPV to add statistics displays, such as frame rate
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
 *  2008-03-16 Andrew Sampson
 *      Initial release
 *
 * </pre>
 */

/*
Note - this class was developed on my personal time, and I'd like to license it 
under the LGPL.  Other portions of this plugin were released under the GPL, 
and I haven't yet been given permission to LGPL those portions.
*/


#include <stdio.h>
#include <iostream>

#ifdef WIN32
#define snprintf _snprintf
#endif

#include <osg/StateSet>
#include <osg/Geometry>
#include <osg/Light>
#include <osg/Depth>


#include "GridLinesScreen.h"


using namespace mpv;
using namespace mpvosg;

GridLinesScreen::GridLinesScreen() :
	OverlayScreen()
{
}


GridLinesScreen::~GridLinesScreen()
{
}


void GridLinesScreen::resetView( const View *view )
{
	projectionMatrix = osg::Matrix::ortho2D( 0.0, 1.0, 0.0, 1.0 );
}


osg::Node *GridLinesScreen::getNode()
{
	if( !geode.valid() )
		createGeode();
	return geode.get();
}


// ================================================
// createGeode
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void GridLinesScreen::createGeode()
{
	geode = new osg::Geode();
	
	// turn lighting off 
	osg::StateSet* stateset = geode->getOrCreateStateSet();
	stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

#if 0
	// to ensure the hud appears on top we can either use osg::Depth to force the 
	// depth fragments to be placed at the front of the screen.
	stateset->setAttribute(new osg::Depth(osg::Depth::LESS,0.0,0.0001));
#else
	// or disable depth test, and make sure that the hud is drawn after everything 
	// else so that it always appears ontop.
	stateset->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
	stateset->setRenderBinDetails(11,"RenderBin");
#endif

	osg::Geometry *quad = new osg::Geometry;
	geode->addDrawable( quad );

	osg::Vec4Array *colorarray = new osg::Vec4Array;
	colorarray->push_back( osg::Vec4( 0,0,0,1 ) );
	colorarray->push_back( osg::Vec4( 1,1,1,1 ) );
	colorarray->push_back( osg::Vec4( 1,1,1,1 ) );
	quad->setColorBinding( osg::Geometry::BIND_PER_PRIMITIVE_SET );
	quad->setColorArray( colorarray );
	
	osg::Vec3Array *vertArray = new osg::Vec3Array();
	quad->setVertexArray( vertArray );
	
	// background quad
	vertArray->push_back( osg::Vec3( 0., 0., 0. ) );
	vertArray->push_back( osg::Vec3( 1., 0., 0. ) );
	vertArray->push_back( osg::Vec3( 1., 1., 0. ) );
	vertArray->push_back( osg::Vec3( 0., 1., 0. ) );

	quad->addPrimitiveSet( 
		new osg::DrawArrays(
			osg::PrimitiveSet::QUADS, 0, vertArray->size() ) );

	// grid lines
	unsigned int firstIndexForLines = vertArray->size();
	
	// vertical, center
	vertArray->push_back( osg::Vec3( 0.5, 0.0, 0. ) );
	vertArray->push_back( osg::Vec3( 0.5, 1.0, 0. ) );


	// diagonal
	vertArray->push_back( osg::Vec3( 0.5, 0.0, 0. ) );
	vertArray->push_back( osg::Vec3( 0.0, 1.0, 0. ) );


	// diagonal
	vertArray->push_back( osg::Vec3( 0.5, 0.0, 0. ) );
	vertArray->push_back( osg::Vec3( 1.0, 1.0, 0. ) );


	// diagonal
	vertArray->push_back( osg::Vec3( 1.0, 0.0, 0. ) );
	vertArray->push_back( osg::Vec3( 0.5, 1.0, 0. ) );


	// diagonal
	vertArray->push_back( osg::Vec3( 0.5, 1.0, 0. ) );
	vertArray->push_back( osg::Vec3( 0.0, 0.0, 0. ) );


	vertArray->push_back( osg::Vec3( 0.25, 0.0, 0. ) );
	vertArray->push_back( osg::Vec3( 0.25, 1.0, 0. ) );


	vertArray->push_back( osg::Vec3( 0.75, 0.0, 0. ) );
	vertArray->push_back( osg::Vec3( 0.75, 1.0, 0. ) );


	// horizontal, center
	vertArray->push_back( osg::Vec3( 0.0, 0.5, 0. ) );
	vertArray->push_back( osg::Vec3( 1.0, 0.5, 0. ) );


	vertArray->push_back( osg::Vec3( 0.125, 0.0, 0. ) );
	vertArray->push_back( osg::Vec3( 0.125, 1.0, 0. ) );


	vertArray->push_back( osg::Vec3( 0.375, 0.0, 0. ) );
	vertArray->push_back( osg::Vec3( 0.375, 1.0, 0. ) );


	vertArray->push_back( osg::Vec3( 0.625, 0.0, 0. ) );
	vertArray->push_back( osg::Vec3( 0.625, 1.0, 0. ) );


	vertArray->push_back( osg::Vec3( 0.875, 0.0, 0. ) );
	vertArray->push_back( osg::Vec3( 0.875, 1.0, 0. ) );


	vertArray->push_back( osg::Vec3( 0.0625, 0.0, 0. ) );
	vertArray->push_back( osg::Vec3( 0.0625, 1.0, 0. ) );


	vertArray->push_back( osg::Vec3( 0.1875, 0.0, 0. ) );
	vertArray->push_back( osg::Vec3( 0.1875, 1.0, 0. ) );


	vertArray->push_back( osg::Vec3( 0.3125, 0.0, 0. ) );
	vertArray->push_back( osg::Vec3( 0.3125, 1.0, 0. ) );


	vertArray->push_back( osg::Vec3( 0.4375, 0.0, 0. ) );
	vertArray->push_back( osg::Vec3( 0.4375, 1.0, 0. ) );


	vertArray->push_back( osg::Vec3( 0.5625, 0.0, 0. ) );
	vertArray->push_back( osg::Vec3( 0.5625, 1.0, 0. ) );


	vertArray->push_back( osg::Vec3( 0.6875, 0.0, 0. ) );
	vertArray->push_back( osg::Vec3( 0.6875, 1.0, 0. ) );


	vertArray->push_back( osg::Vec3( 0.8125, 0.0, 0. ) );
	vertArray->push_back( osg::Vec3( 0.8125, 1.0, 0. ) );


	vertArray->push_back( osg::Vec3( 0.9375, 0.0, 0. ) );
	vertArray->push_back( osg::Vec3( 0.9375, 1.0, 0. ) );


	vertArray->push_back( osg::Vec3( 0.510433070866, 0.0, 0. ) );
	vertArray->push_back( osg::Vec3( 0.510433070866, 1.0, 0. ) );


	vertArray->push_back( osg::Vec3( 0.489566929134, 0.0, 0. ) );
	vertArray->push_back( osg::Vec3( 0.489566929134, 1.0, 0. ) );


	vertArray->push_back( osg::Vec3( 0.0, 0.51854768154, 0. ) );
	vertArray->push_back( osg::Vec3( 1.0, 0.51854768154, 0. ) );


	vertArray->push_back( osg::Vec3( 0.0, 0.48145231846, 0. ) );
	vertArray->push_back( osg::Vec3( 1.0, 0.48145231846, 0. ) );


	vertArray->push_back( osg::Vec3( 0.0, 0.25, 0. ) );
	vertArray->push_back( osg::Vec3( 1.0, 0.25, 0. ) );


	vertArray->push_back( osg::Vec3( 0.0, 0.125, 0. ) );
	vertArray->push_back( osg::Vec3( 1.0, 0.125, 0. ) );


	vertArray->push_back( osg::Vec3( 0.0, 0.375, 0. ) );
	vertArray->push_back( osg::Vec3( 1.0, 0.375, 0. ) );


	vertArray->push_back( osg::Vec3( 0.0, 0.75, 0. ) );
	vertArray->push_back( osg::Vec3( 1.0, 0.75, 0. ) );


	vertArray->push_back( osg::Vec3( 0.0, 0.875, 0. ) );
	vertArray->push_back( osg::Vec3( 1.0, 0.875, 0. ) );


	vertArray->push_back( osg::Vec3( 0.0, 0.625, 0. ) );
	vertArray->push_back( osg::Vec3( 1.0, 0.625, 0. ) );


	quad->addPrimitiveSet( 
		new osg::DrawArrays(
			osg::PrimitiveSet::LINES, firstIndexForLines, vertArray->size() - firstIndexForLines ) );


	// border lines
	firstIndexForLines = vertArray->size();

	vertArray->push_back( osg::Vec3( 0.0104330708661, 0.0185476815398, 0. ) );
	vertArray->push_back( osg::Vec3( 0.989566929134, 0.0185476815398, 0. ) );
	vertArray->push_back( osg::Vec3( 0.989566929134, 0.98145231846, 0. ) );
	vertArray->push_back( osg::Vec3( 0.0104330708661, 0.98145231846, 0. ) );

	quad->addPrimitiveSet( 
		new osg::DrawArrays(
			osg::PrimitiveSet::LINE_LOOP, firstIndexForLines, vertArray->size() - firstIndexForLines ) );

}

