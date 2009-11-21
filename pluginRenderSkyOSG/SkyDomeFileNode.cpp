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
 *  FILENAME:   SkyDomeFileNode.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class defines a sky dome node, loaded from an external file.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  08/20/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#include <algorithm>
#include <stdlib.h>
#include <math.h>

#include <osgDB/ReadFile>

#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>
#include <osg/Depth>
#include <osg/StateSet>

#include "SkyDomeFileNode.h"
#include "MoveSkyTransform.h"


// ================================================
// SkyDomeFileNode
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
SkyDomeFileNode::SkyDomeFileNode()
{
}


// ================================================
// SkyDomeFileNode
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
SkyDomeFileNode::SkyDomeFileNode( const char *filename, const osg::Vec3 &ypr, bool isTransparent )
{
	osg::Node *skydome = NULL;

	if( filename != NULL )
		skydome = osgDB::readNodeFile( filename );

	if( skydome != NULL )
	{
		osg::Transform *epxform = new MoveEarthySkyWithEyePointTransform;
		osg::MatrixTransform *mxform = new osg::MatrixTransform;
		
		// Turn off culling for the skydome.  This is important; with culling enabled, 
		// the sky might disappear when the eyepoint moves away from the origin.
		epxform->setCullingActive(false);
		
		epxform->addChild( mxform );
		osg::Matrix mtx;
		mtx.makeScale( SKY_DOME_RADIUS, SKY_DOME_RADIUS, SKY_DOME_RADIUS );
		mtx *= osg::Matrix::rotate( osg::Quat( 
		ypr[2], osg::Vec3(0.,1.,0.), 
		ypr[1], osg::Vec3(1.,0.,0.), 
		ypr[0] * -1.0, osg::Vec3(0.,0.,1.) ) );
		mxform->setMatrix( mtx );
		mxform->addChild( skydome );

		osg::StateSet *dstate = new osg::StateSet;
		dstate->setMode( GL_LIGHTING, osg::StateAttribute::OFF|osg::StateAttribute::OVERRIDE );
		dstate->setMode( GL_CULL_FACE, osg::StateAttribute::ON );
		if( isTransparent )
		{
			dstate->setMode( GL_BLEND, osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE );
			dstate->setMode( GL_ALPHA_TEST, osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE );
		}
		// clear the depth to the far plane.
		osg::Depth* depth = new osg::Depth;
		depth->setFunction(osg::Depth::ALWAYS);
		depth->setRange(1.0,1.0);   
		dstate->setAttributeAndModes(depth,osg::StateAttribute::ON );
	
		dstate->setRenderBinDetails(-1,"RenderBin");
	
		epxform->setStateSet( dstate );
		addChild( epxform );
	}
}


// ================================================
// SkyDomeFileNode
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
SkyDomeFileNode::SkyDomeFileNode(const SkyDomeFileNode& dn,const osg::CopyOp& copyop):
		osg::Group(dn,copyop)
{
//fixme - may need to impl copy for [???]
}


