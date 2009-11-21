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
 *  2008-02-24 Andrew Sampson
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


#include "GridColorScreen.h"


using namespace mpv;
using namespace mpvosg;

int roundFloat( float f ) { return (int)(f + 0.5); }


// ================================================
// GridColorScreen
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
GridColorScreen::GridColorScreen() :
	OverlayScreen(),
	numHorizontalGridCells( 4 ),
	autoGridLayout( true )
{
}


// ================================================
// ~GridColorScreen
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
GridColorScreen::~GridColorScreen()
{
}


// ================================================
// resetView
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void GridColorScreen::resetView( const View *view )
{
	projectionMatrix = osg::Matrix::ortho2D( 0.0, 1.0, 0.0, 1.0 );
}


// ================================================
// getNode
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
osg::Node *GridColorScreen::getNode()
{
	if( !geode.valid() )
		createGeode();
	return geode.get();
}


// ================================================
// setNumGridCellsWide
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void GridColorScreen::setNumGridCellsWide( unsigned int numCells )
{
	if( numCells > 0 )
		numHorizontalGridCells = numCells;
	else
		numHorizontalGridCells = 1;
	autoGridLayout = false;
	// fixme - delete geode?
}


// ================================================
// applyAutomaticGridLayout
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void GridColorScreen::applyAutomaticGridLayout()
{
	autoGridLayout = true;
	// fixme - delete geode?
}


// ================================================
// addColor
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void GridColorScreen::addColor( const osg::Vec4 &color )
{
	colors.push_back( color );
	// fixme - delete geode?
}


// ================================================
// createGeode
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void GridColorScreen::createGeode()
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

	unsigned int numColors = colors.size();

	unsigned int numVerticalGridCells = 0;

	if( autoGridLayout )
	{
		// fixme - need to retrieve aspect ratio for screen
		float aspectRatio = 4. / 3.;
		
		/*
		Worked out a simple formula for dividing up the screen into roughly 
		square cells:
		
		sx = horizontal cell size
		sy = vertical cell size
		nx = num cells horizontally
		ny = num cells vertically
		
		required: 
		nx * ny >= numColors
		ideally, we fill the screen and have no empty cells:
		nx * ny = numColors
		
		known:
		(sx*nx) / (sy*ny) = aspectRatio
		
		ideally, we produce square cells:
		sx = sy
		
		continuing that optimism:
		nx / ny = aspectRatio
		
		substitution:
		ny = numColors / nx
		nx / (numColors / nx) = aspectRatio
		nx = sqrt( aspectRatio * numColors )
		*/
		
		numHorizontalGridCells = roundFloat( sqrtf( aspectRatio * (float)numColors ) );
		numVerticalGridCells = roundFloat( (float)numColors / (float)numHorizontalGridCells );
		
		while( numHorizontalGridCells * numVerticalGridCells < numColors )
		{
			// fixme - increment numHorizontalGridCells or numVerticalGridCells?
			// hmm... could check to see which would result in squarer cells, 
			// or which would result in fewer empty cells
			numVerticalGridCells++;
		}
	}
	else
	{
		numVerticalGridCells = numColors / numHorizontalGridCells;
		if( numColors % numHorizontalGridCells )
			numVerticalGridCells++;
	}
	
	float horizontalIncrement = 1.0 / numHorizontalGridCells;
	float verticalIncrement = 1.0 / numVerticalGridCells;
	float x = 0.0, xEnd;
	float y = 0.0, yEnd;

	for( unsigned int i = 0; i < numColors; i++ )
	{
		xEnd = x + horizontalIncrement;
		if( xEnd > 1.00001 )
		{
			// wrap back to left edge
			x = 0.0;
			xEnd = x + horizontalIncrement;
			// move to next row
			y += verticalIncrement;
		}
		yEnd = y + verticalIncrement;
		if( yEnd > 1.00001 )
		{
			// should never happen
			break;
		}
		
		addQuad( osg::Vec2( x, y ), osg::Vec2( xEnd, yEnd ), colors[i] );

		x += horizontalIncrement;
	}
}


// ================================================
// addQuad
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void GridColorScreen::addQuad( const osg::Vec2 &minCorner, const osg::Vec2 &maxCorner, 
	const osg::Vec4 &color )
{
	osg::Geometry *quad = new osg::Geometry;
	geode->addDrawable( quad );

	osg::Vec4Array *colorarray = new osg::Vec4Array;
	colorarray->push_back( color );
	quad->setColorBinding( osg::Geometry::BIND_OVERALL );
	quad->setColorArray( colorarray );
	
	osg::Vec3Array *vertArray = new osg::Vec3Array( 4 );
	quad->setVertexArray( vertArray );
	
	(*vertArray)[0].set( minCorner[0], minCorner[1], 0. );
	(*vertArray)[1].set( maxCorner[0], minCorner[1], 0. );
	(*vertArray)[2].set( maxCorner[0], maxCorner[1], 0. );
	(*vertArray)[3].set( minCorner[0], maxCorner[1], 0. );

	quad->addPrimitiveSet( 
		new osg::DrawArrays(
			osg::PrimitiveSet::QUADS, 0, vertArray->size() ) );
	
}

