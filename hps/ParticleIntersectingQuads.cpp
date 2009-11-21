/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2007 The Boeing Company
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
 *  FILENAME:   ParticleIntersectingQuads.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  represent a particle, formed by 2 or 3 intersecting quads.
 *  
 *  MODIFICATION NOTES:
 *  
 *  2007-11-17 Andrew Sampson
 *      Initial Release.
 *  
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#include <iostream>

#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Matrix>
#include <osg/GL>
#include <osg/BlendFunc>

#include "ParticleIntersectingQuads.h"


int osgParticleHPS::ParticleIntersectingQuads::shapeType = osgParticleHPS::Particle::reserveNewShapeType();

// ================================================
// ParticleIntersectingQuads
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
osgParticleHPS::ParticleIntersectingQuads::ParticleIntersectingQuads()
:   osgParticleHPS::Particle(),
	geometryType_(THREE_QUADS)
{
	setShape( (osgParticleHPS::Particle::Shape)shapeType );
}


// ================================================
// ParticleIntersectingQuads
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
osgParticleHPS::ParticleIntersectingQuads::ParticleIntersectingQuads( const ParticleIntersectingQuads &other )
:   osgParticleHPS::Particle( other ),
	geometryType_( other.geometryType_ )
{
}


// ================================================
// beginRender
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void osgParticleHPS::ParticleIntersectingQuads::beginRender() const
{
	glBegin( GL_QUADS );
}


// ================================================
// render
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void osgParticleHPS::ParticleIntersectingQuads::render(const osg::Vec3 &xpos, const osg::Vec3 &px, const osg::Vec3 &py ) const
{

	osg::Matrix matrix = 
		osg::Matrix::scale( 
			current_size_.x(), 
			current_size_.y(), 
			current_size_.z() ) * 
		osg::Matrix::rotate( 
			angle_[0], osg::Vec3(0, 1, 0), 
			angle_[1], osg::Vec3(1, 0, 0), 
			angle_[2], osg::Vec3(0, 0, 1) ) * 
		osg::Matrix::translate( xpos );

	// set the color for the particle
	glColor4f( current_color_.x(), 
	           current_color_.y(), 
	           current_color_.z(), 
	           current_color_.w() );

	switch( geometryType_ )
	{
	case THREE_QUADS:
		glTexCoord2f( 1.0f, 1.0f );
		glVertex3fv( (osg::Vec3( -0.5, 0.0, +0.5 ) * matrix).ptr() );
		glTexCoord2f( 0.0f, 1.0f );
		glVertex3fv( (osg::Vec3( +0.5, 0.0, +0.5 ) * matrix).ptr() );
		glTexCoord2f( 0.0f, 0.0f );
		glVertex3fv( (osg::Vec3( +0.5, 0.0, -0.5 ) * matrix).ptr() );
		glTexCoord2f( 1.0f, 0.0f );
		glVertex3fv( (osg::Vec3( -0.5, 0.0, -0.5 ) * matrix).ptr() );
		// fall through
	case TWO_QUADS:
		glTexCoord2f( 1.0f, 1.0f );
		glVertex3fv( (osg::Vec3( +0.5, +0.5, 0.0 ) * matrix).ptr() );
		glTexCoord2f( 0.0f, 1.0f );
		glVertex3fv( (osg::Vec3( -0.5, +0.5, 0.0 ) * matrix).ptr() );
		glTexCoord2f( 0.0f, 0.0f );
		glVertex3fv( (osg::Vec3( -0.5, -0.5, 0.0 ) * matrix).ptr() );
		glTexCoord2f( 1.0f, 0.0f );
		glVertex3fv( (osg::Vec3( +0.5, -0.5, 0.0 ) * matrix).ptr() );
		glTexCoord2f( 1.0f, 1.0f );
		glVertex3fv( (osg::Vec3( 0.0, +0.5, -0.5 ) * matrix).ptr() );
		glTexCoord2f( 0.0f, 1.0f );
		glVertex3fv( (osg::Vec3( 0.0, +0.5, +0.5 ) * matrix).ptr() );
		glTexCoord2f( 0.0f, 0.0f );
		glVertex3fv( (osg::Vec3( 0.0, -0.5, +0.5 ) * matrix).ptr() );
		glTexCoord2f( 1.0f, 0.0f );
		glVertex3fv( (osg::Vec3( 0.0, -0.5, -0.5 ) * matrix).ptr() );
		break;
	default:
		break;
	}

}


// ================================================
// endRender
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void osgParticleHPS::ParticleIntersectingQuads::endRender() const
{
	glEnd();
}


// ================================================
// copy
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
osgParticleHPS::Particle * osgParticleHPS::ParticleIntersectingQuads::copy() const
{
	osgParticleHPS::ParticleIntersectingQuads *result = new ParticleIntersectingQuads( *this );
//std::cout << "ParticleIntersectingQuads::copy - copy's drawable " << (unsigned int)result->drawable_.get() << " valid " << (unsigned int)result->drawable_.valid() << std::endl;
	return result;
}

