/** <pre>
 *  Plugin to add text labels to entities, for the MPV
 *  Copyright (c) 2006 Andrew Sampson
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
 *  Initial Release: 2006-03-19 Andrew Sampson
 *  
 * </pre>
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

#include "LabelNode.h"


using namespace osg;




// ================================================
// LabelNode
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
LabelNode::LabelNode()
{
	init();
}


// ================================================
// LabelNode
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
LabelNode::LabelNode(const LabelNode& dn,const CopyOp& copyop):
		Group(dn,copyop)
{
}


// ================================================
// traverse
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void LabelNode::traverse( NodeVisitor &nv )
{

	if( nv.getTraversalMode() == NodeVisitor::TRAVERSE_ACTIVE_CHILDREN )
	{
		// The opacity adjustment should only be done once per frame, when the 
		// cull visitor traverses this node.
		
		float dist = nv.getDistanceToEyePoint( osg::Vec3(0,0,0), true );
		
		if( dist > fadeDistFar )
		{
			// if the camera is farther away than fadeDistFar, turn off the 
			// children entirely
			return;
		}
		
		if( dist < fadeDistNear )
		{
			// do not modify the alpha if the camera is nearer than 
			// fadeDistNear

			textDrawable->setColor( osg::Vec4( textColor, alphaMax ) );
		}
		else
		{
			float alpha = alphaMax /* * 
				((dist - fadeDistNear) / (fadeDistFar - fadeDistNear)) */ ;
		
			textDrawable->setColor( osg::Vec4( textColor, alpha ) );
		}
		
		// don't need to dirty the display list, b/c display lists are disabled 
		// for textDrawable
	}
	
	Group::traverse( nv );
}


// ================================================
// setText
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void LabelNode::setText( const std::string &newText )
{
	textDrawable->setText( newText );
}


// ================================================
// setColor
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void LabelNode::setColor( const osg::Vec3 &color )
{
	textColor = color;
	// traverse() will take care of propagating this value to textDrawable
}


// ================================================
// setSize
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void LabelNode::setSize( float newSize )
{
	textDrawable->setCharacterSize( newSize );
}


// ================================================
// setPosition
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void LabelNode::setPosition( const osg::Vec3 &newPosition )
{
	textDrawable->setPosition( newPosition );
}


// ================================================
// init
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void LabelNode::init()
{
	fadeDistNear = 3000.0;
	fadeDistFar = 8000.0;
	
	alphaMin = 0.0;
	alphaMax = 0.9;
	
	// the default text color is green
	textColor.set( 0, 1, 0 );
	
	createText();
}


// ================================================
// createText
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void LabelNode::createText()
{
	osg::Geode *geode = new osg::Geode;
	geode->setName( "Entity Label" );
	
	textDrawable = new osgText::Text;
	textDrawable->setText( "" );
	textDrawable->setUseDisplayList( false );
	textDrawable->setFont( 0 );
	textDrawable->setCharacterSize( 0.2 );
	textDrawable->setPosition( osg::Vec3( 0., 0., 0. ) );
	textDrawable->setColor( osg::Vec4( textColor, alphaMax ) );
	textDrawable->setAlignment( osgText::Text::CENTER_BOTTOM );
	textDrawable->setAxisAlignment( osgText::Text::SCREEN );
	
	geode->addDrawable( textDrawable.get() );
	
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

