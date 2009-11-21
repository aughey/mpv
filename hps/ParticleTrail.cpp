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
 *  FILENAME:   ParticleTrail.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  represent a particle.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  06/22/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
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

#include "ParticleTrail.h"


int osgParticleHPS::ParticleTrail::shapeType = osgParticleHPS::Particle::reserveNewShapeType();

// ================================================
// ParticleTrail
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
osgParticleHPS::ParticleTrail::ParticleTrail()
:   osgParticleHPS::Particle(),
	trailType_(RIBBON),
	trailShape_( Particle::QUAD ),
	timestampForLastNewPoint_(0.),
	timeBetweenPoints_(0.1),
	maxNumPointsToRecord_(40),
	stretchTexture(true),
	totalHistoryPoints(0)
{
	setShape( (osgParticleHPS::Particle::Shape)shapeType );
}


// ================================================
// ParticleTrail
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
osgParticleHPS::ParticleTrail::ParticleTrail( const ParticleTrail &other )
:   osgParticleHPS::Particle( other ),
	trailType_( other.trailType_ ),
	trailShape_( other.trailShape_ ),
	positions_(),
	timestampForLastNewPoint_( 0. ),
	timeBetweenPoints_( other.timeBetweenPoints_ ),
	maxNumPointsToRecord_( other.maxNumPointsToRecord_ ),
	stretchTexture( other.stretchTexture ),
	totalHistoryPoints( other.totalHistoryPoints )
{
}


// ================================================
// update
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
bool osgParticleHPS::ParticleTrail::update(double dt)
{

	bool result = osgParticleHPS::Particle::update(dt);

	// find out if it is time to commit a new point to the trail
	if( positions_.empty() || 
		timestampForLastNewPoint_ + timeBetweenPoints_ < t0_ )
	{
		// commit a new point to the trail
		HPSTrailPoint p;
// not needed		p.set( xpos, angle_, t0_ );
		positions_.push_front( p );
		timestampForLastNewPoint_ = t0_;
		totalHistoryPoints++;
	}
	
	// find out if it is time to remove old points from the trail
	if( positions_.size() > maxNumPointsToRecord_ )
	{
		positions_.pop_back();
	}
	
	if( !positions_.empty() )
	{
		positions_[0].set( position_, angle_, t0_ );
	}

	return result;
}


// ================================================
// render
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void osgParticleHPS::ParticleTrail::render(const osg::Vec3 &xpos, const osg::Vec3 &px, const osg::Vec3 &py ) const
{

	// set the color for the particle
	glColor4f( current_color_.x(), 
	           current_color_.y(), 
	           current_color_.z(), 
	           current_color_.w() );

//std::cout << "ParticleTrail::render - ratio " << (t0_ - timestampForLastNewPoint_) / timeBetweenPoints_ << std::endl;
//std::cout << "ParticleTrail::render - xpos " << xpos << std::endl;
//std::cout << "ParticleTrail::render - num points in trail: " << positions_.size() << std::endl;

//std::cout << "roll is " << osg::RadiansToDegrees( angle_[0] ) << std::endl;

	if( !positions_.empty() )
	{
		
		switch( trailType_ )
		{
		case BUILT_IN_SHAPE:
			// cast away const-ness (sigh...)
			((osgParticleHPS::ParticleTrail*)this)->renderBuiltInShape( px, py );
			break;
		case LINE:
			renderLine();
			break;
		case RIBBON:
			renderRibbon();
			break;
		case CROSS_RIBBON:
			renderCrossRibbon();
			break;
		case DEBUG:
			renderDebug( current_size_.x() );
			break;
		default:
			break;
		}
	}
	
}


// ================================================
// copy
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
osgParticleHPS::Particle * osgParticleHPS::ParticleTrail::copy() const
{
	osgParticleHPS::ParticleTrail *result = new ParticleTrail( *this );
//std::cout << "ParticleTrail::copy - copy's drawable " << (unsigned int)result->drawable_.get() << " valid " << (unsigned int)result->drawable_.valid() << std::endl;
	return result;
}


// ================================================
// reset
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void osgParticleHPS::ParticleTrail::reset()
{
	Particle::reset();
	positions_.clear();
	totalHistoryPoints = 0;
}


void osgParticleHPS::ParticleTrail::renderBuiltInShape( 
	const osg::Vec3 &px, const osg::Vec3 &py )
{

//fixme - tweak (and save off):
// current_color_
// current_size_
// orientation

	float increment = 1. / maxNumPointsToRecord_;
	float t = 0.;
	const osgParticleHPS::rangev4 &colorRange = getColorRange();
	const osgParticleHPS::Interpolator *colorInterp = getColorInterpolator();
	const osgParticleHPS::rangev3 &sizeRange = getSizeRange();
	const osgParticleHPS::Interpolator *sizeInterp = getSizeInterpolator();
	
	setShape( trailShape_ );
	Particle::beginRender();
	
	std::deque< HPSTrailPoint >::iterator iter;
	for( iter = positions_.begin(); iter != positions_.end(); iter++ )
	{
		osg::Vec4 interpColor = colorInterp->interpolate( t, colorRange );
		osg::Vec3 thisPosSize = sizeInterp->interpolate( t, sizeRange );
		
//		glScalef( thisPosSize.x(), thisPosSize.y(), thisPosSize.z() );
//		glColor4f( interpColor[0], interpColor[1], interpColor[2], interpColor[3] );

		Particle::render( iter->pos, px, py );

		t += increment;
	}
	
	Particle::endRender();
	setShape( (osgParticleHPS::Particle::Shape)shapeType );
}


void osgParticleHPS::ParticleTrail::renderLine() const
{
	int numPoints = positions_.size();

	float t = 0.;
	float tIncrement = 1. / maxNumPointsToRecord_;
	const osgParticleHPS::rangev4 &colorRange = getColorRange();
	const osgParticleHPS::Interpolator *colorInterp = getColorInterpolator();

	// This offset will give the ribbon texture a much smoother feel.
	// Without it, the texture would "pulse" and jump as the points at the 
	// tail end of the ribbon are removed.
	float sTexCoordOffset = 1. / (float)numPoints;
	sTexCoordOffset *= (t0_ - timestampForLastNewPoint_) / timeBetweenPoints_;

	glBegin( GL_LINE_STRIP );
	
	int i = 0;
	for( ; i < numPoints; i++ )
	{
		const HPSTrailPoint &point = positions_[i];
		osg::Vec4 interpColor = colorInterp->interpolate( t, colorRange );
		t += tIncrement;

		float sTexCoord;

		if( stretchTexture )
		{
			sTexCoord = (float)i / (float)numPoints;
			if( i > 0 )
				sTexCoord += sTexCoordOffset;
		}
		else
		{
			if( totalHistoryPoints%2 )
				sTexCoord = (i%2) ? 0.0 : 1.0;
			else
				sTexCoord = (i%2) ? 1.0 : 0.0;
		}

		glColor4f( interpColor[0], interpColor[1], interpColor[2], interpColor[3] );

		glTexCoord2f( sTexCoord, 0.5 );
		glVertex3fv( point.pos.ptr() );
	}
	
	glEnd();
}


void osgParticleHPS::ParticleTrail::renderRibbon() const
{
	osg::Matrix rotMtx;
	osg::Vec3 ptA, ptB;
	osg::Vec3 xaxis( 1., 0., 0. );
	osg::Vec3 yaxis( 0., 1., 0. );
	osg::Vec3 zaxis( 0., 0., 1. );
	int numPoints = positions_.size();

	float t = 0.;
	float tIncrement = 1. / maxNumPointsToRecord_;
	const osgParticleHPS::rangev4 &colorRange = getColorRange();
	const osgParticleHPS::Interpolator *colorInterp = getColorInterpolator();
	const osgParticleHPS::rangev3 &sizeRange = getSizeRange();
	const osgParticleHPS::Interpolator *sizeInterp = getSizeInterpolator();

	// This offset will give the ribbon texture a much smoother feel.
	// Without it, the texture would "pulse" and jump as the points at the 
	// tail end of the ribbon are removed.
	float sTexCoordOffset = 1. / (float)numPoints;
	sTexCoordOffset *= (t0_ - timestampForLastNewPoint_) / timeBetweenPoints_;

	glBegin( GL_QUAD_STRIP );
	int i = 0;
	for( ; i < numPoints; i++ )
	{
		const HPSTrailPoint &point = positions_[i];
		osg::Vec4 interpColor = colorInterp->interpolate( t, colorRange );
		osg::Vec3 thisPosSize = sizeInterp->interpolate( t, sizeRange );
		t += tIncrement;
		float sTexCoord;

		if( stretchTexture )
		{
			sTexCoord = (float)i / (float)numPoints;
			if( i > 0 )
				sTexCoord += sTexCoordOffset;
		}
		else
		{
			if( totalHistoryPoints%2 )
				sTexCoord = (i%2) ? 0.0 : 1.0;
			else
				sTexCoord = (i%2) ? 1.0 : 0.0;
		}

		rotMtx.makeRotate( 
			point.rpy[0], yaxis,
			point.rpy[1], xaxis,
			point.rpy[2], zaxis
			 );
		ptA = osg::Vec3(thisPosSize.x(), 0., 0.) * rotMtx;
		ptB = ptA * -1.;
		ptA += point.pos;
		ptB += point.pos;

		glColor4f( interpColor[0], interpColor[1], interpColor[2], interpColor[3] );

		glTexCoord2f( sTexCoord, 1. );
		glVertex3fv( ptA.ptr() );

		glTexCoord2f( sTexCoord, 0. );
		glVertex3fv( ptB.ptr() );

	}

	glEnd();
}


void osgParticleHPS::ParticleTrail::renderCrossRibbon() const
{
	osg::Matrix rotMtx;
	osg::Vec3 ptA, ptB;
	osg::Vec3 xaxis( 1., 0., 0. );
	osg::Vec3 yaxis( 0., 1., 0. );
	osg::Vec3 zaxis( 0., 0., 1. );
	int numPoints = positions_.size();

	float t = 0.;
	float tIncrement = 1. / maxNumPointsToRecord_;
	const osgParticleHPS::rangev4 &colorRange = getColorRange();
	const osgParticleHPS::Interpolator *colorInterp = getColorInterpolator();
	const osgParticleHPS::rangev3 &sizeRange = getSizeRange();
	const osgParticleHPS::Interpolator *sizeInterp = getSizeInterpolator();

	// This offset will give the ribbon texture a much smoother feel.
	// Without it, the texture would "pulse" and jump as the points at the 
	// tail end of the ribbon are removed.
	float sTexCoordOffset = 1. / (float)numPoints;
	sTexCoordOffset *= (t0_ - timestampForLastNewPoint_) / timeBetweenPoints_;

	glBegin( GL_QUAD_STRIP );
	int i;
	for( i = 0; i < numPoints; i++ )
	{
		const HPSTrailPoint &point = positions_[i];
		osg::Vec4 interpColor = colorInterp->interpolate( t, colorRange );
		osg::Vec3 thisPosSize = sizeInterp->interpolate( t, sizeRange );
		t += tIncrement;
		float sTexCoord;

		if( stretchTexture )
		{
			sTexCoord = (float)i / (float)numPoints;
			if( i > 0 )
				sTexCoord += sTexCoordOffset;
		}
		else
		{
			if( totalHistoryPoints%2 )
				sTexCoord = (i%2) ? 0.0 : 1.0;
			else
				sTexCoord = (i%2) ? 1.0 : 0.0;
		}

		rotMtx.makeRotate( 
			point.rpy[0], yaxis,
			point.rpy[1], xaxis,
			point.rpy[2], zaxis
			 );
		ptA = osg::Vec3(thisPosSize.x(), 0., 0.) * rotMtx;
		ptB = ptA * -1.;
		ptA += point.pos;
		ptB += point.pos;

		glColor4f( interpColor[0], interpColor[1], interpColor[2], interpColor[3] );

		glTexCoord2f( sTexCoord, 1. );
		glVertex3fv( ptA.ptr() );

		glTexCoord2f( sTexCoord, 0. );
		glVertex3fv( ptB.ptr() );

	}
	glEnd();
	
	t = 0.0;
	
	glBegin( GL_QUAD_STRIP );
	for( i = 0; i < numPoints; i++ )
	{
		const HPSTrailPoint &point = positions_[i];
		osg::Vec4 interpColor = colorInterp->interpolate( t, colorRange );
		osg::Vec3 thisPosSize = sizeInterp->interpolate( t, sizeRange );
		t += tIncrement;
		float sTexCoord;

		if( stretchTexture )
		{
			sTexCoord = (float)i / (float)numPoints;
			if( i > 0 )
				sTexCoord += sTexCoordOffset;
		}
		else
		{
			if( totalHistoryPoints%2 )
				sTexCoord = (i%2) ? 0.0 : 1.0;
			else
				sTexCoord = (i%2) ? 1.0 : 0.0;
		}

		rotMtx.makeRotate( 
			point.rpy[0], yaxis,
			point.rpy[1], xaxis,
			point.rpy[2], zaxis
			 );
		ptA = osg::Vec3(0., 0., thisPosSize.z()) * rotMtx;
		ptB = ptA * -1.;
		ptA += point.pos;
		ptB += point.pos;

		glColor4f( interpColor[0], interpColor[1], interpColor[2], interpColor[3] );

		glTexCoord2f( sTexCoord, 1. );
		glVertex3fv( ptA.ptr() );

		glTexCoord2f( sTexCoord, 0. );
		glVertex3fv( ptB.ptr() );

	}

	glEnd();
}

void osgParticleHPS::ParticleTrail::renderDebug( float scale ) const
{
	osg::Matrix rotMtx;
	osg::Vec3 ptA, ptB;
	osg::Vec3 xaxis( 1., 0., 0. );
	osg::Vec3 yaxis( 0., 1., 0. );
	osg::Vec3 zaxis( 0., 0., 1. );

	glBegin( GL_LINES );
	int i;
	int numPoints = positions_.size();
	for( i = 0; i < numPoints; i++ )
	{
		const HPSTrailPoint &point = positions_[i];
		float sTexCoord = (float)i / (float)numPoints;
		rotMtx.makeRotate( 
			point.rpy[0], yaxis,
			point.rpy[1], xaxis,
			point.rpy[2], zaxis
			 );

		ptA = osg::Vec3(scale, 0., 0.) * rotMtx;
		ptA += point.pos;

		glVertex3fv( point.pos.ptr() );
		glVertex3fv( ptA.ptr() );

		ptA = osg::Vec3(0., 0., scale) * rotMtx;
		ptA += point.pos;

		glVertex3fv( point.pos.ptr() );
		glVertex3fv( ptA.ptr() );
	}
	glEnd();

}

