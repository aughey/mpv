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
 *  FILENAME:   StarDomeNode.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class creates the star field and updates the transform associated 
 *  with it.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  2006-02-25 Andrew Sampson
 *      Initial Release.
 *  
 *  2007-12-09 Andrew Sampson
 *      Merged the StarSystem class into StarDomeNode.  Reworked the mechanism 
 *      for attenuating star intensity during the day.
 *  
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#include <cassert>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <math.h>

#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Light>
#include <osg/StateSet>
#include <osg/BlendFunc>
#include <osg/BlendColor>
#include <osg/Geometry>
#include <osg/Geode>

#include "StarDomeNode.h"
#include "SkyParameters.h"


// ================================================
// StarDomeNode
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
StarDomeNode::StarDomeNode() : osg::Group()
{
}


// ================================================
// StarDomeNode
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
StarDomeNode::StarDomeNode(const StarDomeNode& dn,const osg::CopyOp& copyop):
		osg::Group(dn,copyop)
{
//fixme - may need to impl copy for [???]
	std::cerr << "StarDomeNode: Error - Copy constructor is not implemented\n";
}


// ================================================
// ~StarDomeNode
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
StarDomeNode::~StarDomeNode()
{
}


// ================================================
// init
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void StarDomeNode::init()
{
	
	// the stars will be placed just outside of the sky dome
	float m_RenderDistance = SKY_DOME_RADIUS + (SKY_DOME_RADIUS * 0.06);

	osg::ref_ptr<StarCatalog> _catalog = new StarCatalog();
	assert(_catalog.valid());
	int numStars = _catalog->_stars.size();
	//std::cout << "Using " << _catalog->m_Source << std::endl;
	//std::cout << "Magnitude " << _catalog->m_MagnitudeCutoff << " cutoff, " << numStars << " stars\n";

	osg::ref_ptr<osg::Vec3Array> _positionList = new osg::Vec3Array(numStars);
	osg::ref_ptr<osg::Vec3Array> _normalList = new osg::Vec3Array(numStars);
	osg::ref_ptr<osg::Vec4Array> _colorList = new osg::Vec4Array(numStars);

	float phi, theta, costheta;
	for( int i=0; i < numStars; i++ )
	{
		const StarCatalog::Star &star = _catalog->_stars[i];
		osg::Vec3 &pos = (*_positionList)[i];
		osg::Vec3 &normal = (*_normalList)[i];
		
		// some trig to position the star on the star sphere
		phi = star.Ra;
		theta = star.De;
		costheta = cos(theta);
		pos[0] = m_RenderDistance*cos(phi)*costheta;
		pos[1] = m_RenderDistance*sin(phi)*costheta;
		pos[2] = m_RenderDistance*sin(theta);

		// the normal points toward the center of the star sphere
		normal = pos * -1.0;
		normal.normalize();

		// the star color needs some tweaking
		(*_colorList)[i] = desaturateStarColor( star.I, star.r, star.g, star.b );
	}
	
	starField = new osg::Geometry;
	starField->setVertexArray( _positionList.get() );
	starField->setNormalArray( _normalList.get() );
	starField->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );
	starField->setColorArray( _colorList.get() );
	starField->setColorBinding( osg::Geometry::BIND_PER_VERTEX );
	starField->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::POINTS, 0, numStars ) );

	osg::StateSet *star_state = starField->getOrCreateStateSet();

	// Lighting is used to control overall brightness of the star field.
// FIXME? - turn off all other lights!  only light 0 should be active.
// FIXME? - turn off global ambient?
	star_state->setMode( GL_LIGHTING, osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE );
	star_state->setMode( GL_LIGHT0, osg::StateAttribute::ON );
	
	osg::Geode *starDomeGeode = new osg::Geode();
	starDomeGeode->addDrawable( starField.get() );

	starFieldTransform = new osg::MatrixTransform();
	starFieldTransform->addChild( starDomeGeode );

	addChild( starFieldTransform.get() );
}


// ================================================
// updateStars
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void StarDomeNode::updateStars( 
	double latitude, double localMeanSiderealTime, float skyIntensity ) 
{
	if( !starField.valid() || !starFieldTransform.valid() )
		return;
	
	// update the orientation of the star dome
	
	// 4.712389 rad = 270 deg
	float rz = 4.712389 - localMeanSiderealTime;
	// 1.5707963 rad = 90 deg
	float rx = - 1.5707963 + latitude;
	
	starFieldTransform->setMatrix( osg::Matrix::rotate( 
		rz, osg::Vec3( 0, 0, 1 ),
		rx, osg::Vec3( 1, 0, 0 ), 
		0., osg::Vec3( 0, 1, 0 ) ) );
	
	// update the brightness of the star dome

	float alpha = skyIntensity;
	if (alpha < 0.0) alpha = 0.0;
	if (alpha > 1.0) alpha = 1.0;
	alpha = 1.0 - alpha;
	
	// cubing alpha causes the values to fall off faster
	alpha *= alpha * alpha;

//	// I've found that the stars are too bright near dawn and dusk
//	alpha -= 0.1;
//	if( alpha < 0. ) alpha = 0.;

	osg::StateSet *star_state = starField->getOrCreateStateSet();

	osg::Light *lightStateAttr = 
		(osg::Light *)star_state->getAttribute( osg::StateAttribute::LIGHT );
	if( lightStateAttr == NULL ) {
		lightStateAttr = new osg::Light;
	}
	// put light 0 at the origin when the star dome is drawn
	lightStateAttr->setLightNum( 0 );
	lightStateAttr->setPosition( osg::Vec4( 0.0, 0.0, 0.0, 1.0 ) );
	// Change the intensity of light 0 according to "alpha".  This will dim 
	// the stars during the day.
	lightStateAttr->setAmbient( osg::Vec4( 0.0, 0.0, 0.0, 1.0 ) );
	lightStateAttr->setSpecular( osg::Vec4( 0.0, 0.0, 0.0, 1.0 ) );
	lightStateAttr->setDiffuse( osg::Vec4( alpha, alpha, alpha, 1.0 ) );
	star_state->setAttributeAndModes( lightStateAttr, osg::StateAttribute::ON );
}


// ================================================
// desaturateStarColor
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
osg::Vec4 StarDomeNode::desaturateStarColor( float I, float r, float g, float b )
{
	// original author said:
	//    FIXME this all needs *extensive* cleaning.  just hacking for now...
	
	// C is used to reduce star color saturation as an
	// ad hoc approximation to human color vision response
	// at low light levels
	float C = I - 0.8;
	if (C > 1.0) C = 1.0;
	if (C < 0.0) C = 0.0;
// further desaturation...
//		C *= C;
//		C *= C;
	if (I > 1.0) I = 1.0;

	osg::Vec4 result( r, g, b, I );

	float avg = (0.213*result[0]+0.715*result[1]+0.072*result[2]);
	result[0] = result[0] + (1.0-C) * (avg-result[0]);
	result[1] = result[1] + (1.0-C) * (avg-result[1]);
	result[2] = result[2] + (1.0-C) * (avg-result[2]);

	// this increases the apparent intensity of strongly colored
	// stars by reducing saturation (also ad hoc).
//	avg = (0.213*result[0]+0.715*result[1]+0.072*result[2]);
//	if( avg > 0.0 )
//	{
//		result[0] /= avg;
//		result[1] /= avg;
//		result[2] /= avg;
//	}

	if (result[0] > 1.0) result[0] = 1.0;
	if (result[1] > 1.0) result[1] = 1.0;
	if (result[2] > 1.0) result[2] = 1.0;
	
	// apply the intensity
	result[0] *= result[3];
	result[1] *= result[3];
	result[2] *= result[3];
	result[3] = 1.0;
	
	return result;
}
