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
 *  FILENAME:   FollowOperator.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This file defines the FollowOperator class.
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


#include "FollowOperator.h"

#include "MultiEmitter.h" // for convertRotMtxToEuler

// ================================================
// FollowOperator
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
FollowOperator::FollowOperator() 
	: osgParticleHPS::Operator() 
{
	firstTimeThrough_ = true;
	excludeRoll_ = false;
	excludePitch_ = false;
	excludeYaw_ = false;
}


// ================================================
// beginOperate
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void FollowOperator::beginOperate( osgParticleHPS::Program *program )
{
	currentPosition_ = 
		program->transformLocalToWorld( osg::Vec3( 0., 0., 0. ) );

	rotationMtx_ = program->getLocalToWorldMatrix();
	rotationMtx_.setTrans( osg::Vec3( 0., 0., 0. ) );
	convertRotMtxToEuler( rotationMtx_, eulerAngle_ );

	if( firstTimeThrough_ )
	{
		lastPosition_ = currentPosition_;
		lastRotationMtxInv_.invert( rotationMtx_ );
		firstTimeThrough_ = false;
	}
	
	positionDelta_ = currentPosition_ - lastPosition_;

}


// ================================================
// operate
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void FollowOperator::operate(osgParticleHPS::Particle *P, double dt)
{
	osg::Vec3 particlePosition = P->getPosition();
	osg::Vec3 particleLocalPosition = particlePosition - lastPosition_;
	
	// "un-rotate" the particle
	particleLocalPosition = lastRotationMtxInv_ * particleLocalPosition;
	
	// rotate the particle with the new orientation
	particleLocalPosition = rotationMtx_ * particleLocalPosition;
	
	// translate the particle to the new position
	P->setPosition( currentPosition_ + particleLocalPosition );
	
	osg::Vec3 newAngle = P->getAngle();
	
	if( !excludeRoll_ )
		newAngle[0] = eulerAngle_[0];
	if( !excludePitch_ )
		newAngle[1] = eulerAngle_[1];
	if( !excludeYaw_ )
		newAngle[2] = eulerAngle_[2];
	
	P->setAngle( newAngle );
}


// ================================================
// endOperate
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void FollowOperator::endOperate()
{
	
	lastPosition_ = currentPosition_;
	lastRotationMtxInv_.invert( rotationMtx_ );
	
}


