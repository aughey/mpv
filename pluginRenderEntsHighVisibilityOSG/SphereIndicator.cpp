/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2004 The Boeing Company
 *  
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *  
 *  
 *  FILENAME:   SphereIndicator.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class defines a node that creates a sphere, and causes that sphere 
 *  to fade as the distance from the camera decreases (yes, it disappears 
 *  when it gets closer).
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  04/20/2005 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <math.h>

#include <osgUtil/CullVisitor>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Depth>
#include <osg/StateSet>
#include <osg/Material>

#include <osg/MatrixTransform>
#include <osg/Projection>

#include "SphereIndicator.h"


using namespace osg;




// ================================================
// SphereIndicator
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
SphereIndicator::SphereIndicator()
{
	init();
}


// ================================================
// SphereIndicator
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
SphereIndicator::SphereIndicator( const osg::Vec3 &color )
{
	init();
	sphereColor = color;
}


// ================================================
// SphereIndicator
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
SphereIndicator::SphereIndicator(const SphereIndicator& dn,const CopyOp& copyop):
		Group(dn,copyop)
{
}


// ================================================
// traverse
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void SphereIndicator::traverse( NodeVisitor &nv )
{

	if( nv.getTraversalMode() == NodeVisitor::TRAVERSE_ACTIVE_CHILDREN )
	{
		// The opacity adjustment should only be done once per frame, when the 
		// cull visitor traverses this node.
		
		float dist = nv.getDistanceToEyePoint( osg::Vec3(0,0,0), true );
		
		if( dist < fadeDistNear )
		{
			// if the camera is closer than fadeDistNear, turn off the 
			// children entirely
			return;
		}
		
		if( dist > fadeDistFar )
		{
			// do not modify the alpha if the camera is farther away than 
			// fadeDistFar

			shapeDrawable->setColor( osg::Vec4( sphereColor, alphaMax ) );
		}
		else
		{
			float alpha = alphaMax * 
				((dist - fadeDistNear) / (fadeDistFar - fadeDistNear));
		
			shapeDrawable->setColor( osg::Vec4( sphereColor, alpha ) );
		}
		
		shapeDrawable->dirtyDisplayList();
		
	}
	
	Group::traverse( nv );
}


// ================================================
// init
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void SphereIndicator::init()
{
	fadeDistNear = 3000.0;
	fadeDistFar = 8000.0;
	
	alphaMin = 0.0;
	alphaMax = 0.5;
	
	// the default sphere color is green
	sphereColor.set( 0, 1, 0 );
	
	createSphere();
}


// ================================================
// createSphere
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void SphereIndicator::createSphere()
{
	osg::Geode *geode = new osg::Geode;
	geode->setName( "High Visibility Entity Indicator" );
	
	// set some limit on the poly count, etc
	osg::TessellationHints *tessHints = new osg::TessellationHints;
	tessHints->setDetailRatio( 0.25 );
	tessHints->setCreateBackFace( false );
	
	osg::Sphere *shape = new osg::Sphere( osg::Vec3(0,0,0), 150.0 );
	shapeDrawable = new osg::ShapeDrawable( shape, tessHints );
	geode->addDrawable( shapeDrawable.get() );
	
	shapeDrawable->setColor( osg::Vec4( sphereColor, alphaMax ) );
	
	osg::StateSet *stateset = geode->getOrCreateStateSet();

	// don't want lighting
	stateset->setMode( GL_LIGHTING, 
		osg::StateAttribute::OVERRIDE|osg::StateAttribute::OFF );

	// definitely want blending (EntitySubgraphWrapper will enable blending 
	// too, but it can't hurt to make sure...)
	stateset->setMode( GL_BLEND, osg::StateAttribute::ON );

	// don't want clockwise polygons
	stateset->setMode( GL_CULL_FACE, StateAttribute::ON );

	// transparent stuff goes in the last render bin
	stateset->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );

	addChild( geode );
}

