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
 *  FILENAME:   SkyDomeNode.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class defines a sky dome node.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  04/26/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#include <algorithm>
#include <stdlib.h>
#include <math.h>

#include <osgDB/ReadFile>
#include <osgUtil/CullVisitor>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Depth>
#include <osg/StateSet>

#include <osg/MatrixTransform>
#include <osg/Projection>
#include <osgText/Font>
#include <osgText/Text>

#include "SkyDomeNode.h"
#include "MoveSkyTransform.h"


using namespace osg;


/*
// DEBUGGING STUFF
osgText::Text *textObj = NULL;
osg::Node *textNode = NULL;*/



// ================================================
// SkyDomeNode
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
SkyDomeNode::SkyDomeNode()
{
	osg::ClearNode* clearNode = new osg::ClearNode;
//	clearNode->setRequiresClear(false); // we've got sky to do it.

	osg::Transform* xform = new MoveEarthySkyWithEyePointTransform;

	// Note - these comments, as well as some code in this file, were grabbed 
	// from one of the OSG example programs -ads

	// transform's value isn't known until in the cull traversal so its bounding
	// volume can't be determined, therefore culling will be invalid,
	// so switch it off, this cause all our paresnts to switch culling
	// off as well. But don't worry culling will be back on once underneath
	// this node or any other branch above this transform.
	xform->setCullingActive(false);


	xform->addChild( makeSky() );
	clearNode->addChild( xform );
	addChild( clearNode );

/*{
	// create the hud (debugging stuff)
	osg::Projection* projection = new osg::Projection;
	projection->setMatrix(osg::Matrix::ortho2D(0,1280,0,1024));
	
	osg::MatrixTransform* modelview_abs = new osg::MatrixTransform;
	modelview_abs->setReferenceFrame(osg::Transform::RELATIVE_TO_ABSOLUTE);
	modelview_abs->setMatrix(osg::Matrix::identity());
	
	osg::Geode* geode  = new osg::Geode;
	osgText::Text* text = new osgText::Text;
	text->setFont( 0 );  // default, icky font
	text->setColor( osg::Vec4(1.0f,1.0f,0.0f,1.0f) );
	text->setCharacterSize( 20.0 );
	text->setPosition(osg::Vec3(50.,50.,0.0f));
	geode->addDrawable(text);

	modelview_abs->addChild( geode );
	
	projection->addChild(modelview_abs);
	
	textNode = projection;
	textObj = text;

	addChild( projection );

}*/

}


// ================================================
// SkyDomeNode
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
SkyDomeNode::SkyDomeNode(const SkyDomeNode& dn,const CopyOp& copyop):
		Group(dn,copyop)
{
//fixme - may need to impl copy for [???]
}


// ================================================
// makeSky
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// Note - this function was grabbed from osghangglider's sky.cpp
Node *SkyDomeNode::makeSky( void )
{
	int i, j;
	float lev[] = { -20.0, -5.0, -1.0, 1.0, 15.0, 30.0, 60.0, 90.0  };
	float cc[][4] =
	{
		{ 0.70, 0.70, 0.90 },
		{ 0.70, 0.70, 0.90 },
		{ 0.55, 0.55, 0.80 },
		{ 0.40, 0.40, 0.70 },
		{ 0.20, 0.20, 0.60 },
		{ 0.10, 0.10, 0.60 },
		{ 0.10, 0.10, 0.60 },
		{ 0.10, 0.10, 0.60 },
	};
	float x, y, z;
	float alpha, theta;
	float radius = SKY_DOME_RADIUS;
	int nlev = sizeof( lev )/sizeof(float);
	int numWedges = 18;
	int numWedgesPlus1 = numWedges+1;
	float wedgeAngle = 360.0 / numWedges;

	Geometry *geom = new Geometry;

	Vec3Array& coords = *(new Vec3Array(numWedgesPlus1*nlev));
	Vec4Array& colors = *(new Vec4Array(numWedgesPlus1*nlev));
	Vec2Array& tcoords = *(new Vec2Array(numWedgesPlus1*nlev));
	
	
	int ci = 0;

	for( i = 0; i < nlev; i++ )
	{
		for( j = 0; j < numWedgesPlus1; j++ )
		{
			alpha = osg::DegreesToRadians(lev[i]);
			theta = osg::DegreesToRadians(wedgeAngle * (float)(j));

			x = radius * cosf( alpha ) * cosf( theta );
			y = radius * cosf( alpha ) * -sinf( theta );
			z = radius * sinf( alpha );

			coords[ci][0] = x;
			coords[ci][1] = y;
			coords[ci][2] = z;

			colors[ci][0] = cc[i][0];
			colors[ci][1] = cc[i][1];
			colors[ci][2] = cc[i][2];
			colors[ci][3] = 1.0;

			tcoords[ci][0] = (float)j/(float)numWedges;
			tcoords[ci][0] = (float)i/(float)(nlev-1);

			ci++;
		}


	}

	for( i = 0; i < nlev-1; i++ )
	{
		DrawElementsUShort* drawElements = new DrawElementsUShort(PrimitiveSet::TRIANGLE_STRIP);
		drawElements->reserve(numWedgesPlus1 * 2);

		for( j = 0; j <= numWedges; j++ )
		{
			drawElements->push_back((i+1)*numWedgesPlus1+j);
			drawElements->push_back((i+0)*numWedgesPlus1+j);
		}

		geom->addPrimitiveSet(drawElements);
	}
	
	geom->setVertexArray( &coords );
	geom->setTexCoordArray( 0, &tcoords );

	geom->setColorArray( &colors );
	geom->setColorBinding( Geometry::BIND_PER_VERTEX );

	StateSet *dstate = new StateSet;

	dstate->setMode( GL_LIGHTING, StateAttribute::OFF );
	dstate->setMode( GL_CULL_FACE, StateAttribute::ON );
	

	// clear the depth to the far plane.
	osg::Depth* depth = new osg::Depth;
	depth->setFunction(osg::Depth::ALWAYS);
	depth->setRange(1.0,1.0);   
	dstate->setAttributeAndModes(depth,StateAttribute::ON );

	dstate->setRenderBinDetails(-2,"RenderBin");

	geom->setStateSet( dstate );

	Geode *geode = new Geode;
	geode->addDrawable( geom );

	geode->setName( "Sky" );

	return geode;
}

