/** <pre>
 *  Plugin for the MPV to add statistics displays, such as frame rate
 *  Copyright (c) 2007 Andrew Sampson
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
 *  2007-10-07 Andrew Sampson
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


#include "SolidColorScreen.h"


using namespace mpv;
using namespace mpvosg;

SolidColorScreen::SolidColorScreen( const osg::Vec4 &screenColor ) :
	OverlayScreen(),
	color( screenColor )
{
}


SolidColorScreen::~SolidColorScreen()
{
}


void SolidColorScreen::resetView( const View *viewParams )
{
	projectionMatrix = osg::Matrix::ortho2D( 0.0, 1.0, 0.0, 1.0 );
}


osg::Node *SolidColorScreen::getNode()
{
	if( !geode.valid() )
		createGeode();
	return geode.get();
}


// ================================================
// createGeode
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void SolidColorScreen::createGeode()
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
	colorarray->push_back( color );
	quad->setColorBinding( osg::Geometry::BIND_OVERALL );
	quad->setColorArray( colorarray );
	
	osg::Vec3Array *vertArray = new osg::Vec3Array( 4 );
	quad->setVertexArray( vertArray );
	
	(*vertArray)[0].set( 0., 0., 0. );
	(*vertArray)[1].set( 0., 1., 0. );
	(*vertArray)[2].set( 1., 1., 0. );
	(*vertArray)[3].set( 1., 0., 0. );

	quad->addPrimitiveSet( 
		new osg::DrawArrays(
			osg::PrimitiveSet::QUADS, 0, vertArray->size() ) );
	
}

