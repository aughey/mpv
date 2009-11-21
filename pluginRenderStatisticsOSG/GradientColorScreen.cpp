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
 *  2008-03-15 Andrew Sampson
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


#include "GradientColorScreen.h"


using namespace mpv;
using namespace mpvosg;

GradientColorScreen::GradientColorScreen( 
	const osg::Vec4 &newcolorA, const osg::Vec4 &newcolorB, 
	GradientColorScreen::GradientType newgradient ) :
	OverlayScreen(),
	colorA( newcolorA ),
	colorB( newcolorB ),
	gradient( newgradient )
{
}


GradientColorScreen::~GradientColorScreen()
{
}


void GradientColorScreen::resetView( const View *viewParams )
{
	projectionMatrix = osg::Matrix::ortho2D( 0.0, 1.0, 0.0, 1.0 );
}


osg::Node *GradientColorScreen::getNode()
{
	if( !geode.valid() )
		createGeode();
	return geode.get();
}


// ================================================
// createGeode
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void GradientColorScreen::createGeode()
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
	quad->setColorBinding( osg::Geometry::BIND_PER_VERTEX );
	quad->setColorArray( colorarray );

	if( gradient == Vertical )
	{
		colorarray->push_back( colorA );
		colorarray->push_back( colorA );
		colorarray->push_back( colorB );
		colorarray->push_back( colorB );
	}
	else if( gradient == Horizontal )
	{
		colorarray->push_back( colorA );
		colorarray->push_back( colorB );
		colorarray->push_back( colorB );
		colorarray->push_back( colorA );
	}

	
	osg::Vec3Array *vertArray = new osg::Vec3Array( 4 );
	quad->setVertexArray( vertArray );
	
	(*vertArray)[0].set( 0., 0., 0. );
	(*vertArray)[1].set( 1., 0., 0. );
	(*vertArray)[2].set( 1., 1., 0. );
	(*vertArray)[3].set( 0., 1., 0. );

	quad->addPrimitiveSet( 
		new osg::DrawArrays(
			osg::PrimitiveSet::QUADS, 0, vertArray->size() ) );
	
}

