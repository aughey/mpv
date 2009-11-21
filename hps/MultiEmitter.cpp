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
 *  FILENAME:   MultiEmitter.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This file defines the MultiEmitter class.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  05/28/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#include <math.h>
#include <iostream>

#include <osg/Matrix>

#include "MultiEmitter.h"

using namespace osgParticleHPS;
using namespace std;

// ================================================
// MultiEmitter
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
osgParticleHPS::MultiEmitter::MultiEmitter()
:	Emitter(),
	counter_(new RandomRateCounter),
	additiveVelocity_(true),
	xaxis( 1., 0., 0. ),
	yaxis( 0., 1., 0. ),
	zaxis( 0., 0., 1. )
{
	placers_.push_back( new PointPlacer );
	shooters_.push_back( new RadialShooter );
	prevTime_ = 700000.0; // needs to be some large number
	additionalDelay_ = 0.0;
}
	

// ================================================
// MultiEmitter
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
osgParticleHPS::MultiEmitter::MultiEmitter(const MultiEmitter &copy, const osg::CopyOp &copyop)
:	Emitter(copy, copyop)
/*,
	counter_(static_cast<Counter *>(copyop(copy.counter_.get()))), 
	placer_(static_cast<Placer *>(copyop(copy.placer_.get()))), 
	shooter_(static_cast<Shooter *>(copyop(copy.shooter_.get())))*/
{
	std::cerr 
	<< "MultiEmitter : you shouldn't try to copy MultiEmitter objects, as \n"
	<< "this copy constructor is not complete.\n";
}


// ================================================
// emit
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void osgParticleHPS::MultiEmitter::emit(double dt) 
{
	// update the timestamp, position, tZeroMatrix_, velocity, etc
	double currentTime = getCurrentTime();
	osg::Vec3 currentPosition = transformLocalToWorld( osg::Vec3( 0., 0., 0. ) );
	osg::Vec3 velocity = currentPosition - prevPosition_;
	velocity /= dt;
	prevPosition_ = currentPosition;
	if( currentTime < prevTime_ )
	{
		// if time has been "rewound", it means that either this is the 
		// first time this emitter's emit() has been called, or that the 
		// particle system has been reset to time 0
		
		// store position at T0
		tZeroMatrix_ = getLocalToWorldMatrix();
		
		// velocity will have meaningless values at T0; initialize to 0
		velocity.set( 0., 0., 0. );
	}
	prevTime_ = currentTime;
	
	// "additional delay" stuff...
	if( currentTime < additionalDelay_ )
		return;
	
	int n = counter_->numParticlesToCreate(dt);
	for (int i=0; i<n; ++i) {
		Particle *P = 
			getParticleSystem()->createParticle();
		if( P ) {
			for( std::vector<osg::ref_ptr<Placer> >::iterator pIter = 
				placers_.begin(); pIter != placers_.end(); pIter ++ )
			{
				(*pIter)->place(P);
			}
			for( std::vector<osg::ref_ptr<Shooter> >::iterator sIter = 
				shooters_.begin(); sIter != shooters_.end(); sIter ++ )
			{
				(*sIter)->shoot(P);
			}

			if( getReferenceFrame() == 
				(osgParticleHPS::ParticleProcessor::ReferenceFrame)RELATIVE_TO_TZERO )
			{
				P->transformPositionVelocity( tZeroMatrix_ );
			}
			else if( getReferenceFrame() == RELATIVE_TO_PARENTS ) 
			{
				P->transformPositionVelocity(getLocalToWorldMatrix());
			}

			// not sure if this works for RELATIVE_TO_ABSOLUTE mode...
			// don't really care, either, since we don't use that mode
			if( additiveVelocity_ )
				P->addVelocity( velocity );

			if( getParticleSystem()->getParticleAlignment() == 
				ParticleSystem::FIXED )
			{
				// next, rotate the particle to match the orientation of the 
				// emitter
	
				osg::Vec3 particleAngle = P->getAngle();
				osg::Vec3 temp;
	
				osg::Matrix m1, m2, m3;
	
				if( getReferenceFrame() == 
					(osgParticleHPS::ParticleProcessor::ReferenceFrame)RELATIVE_TO_TZERO )
				{
					// copy the emitter's T0 orientation to a rotation matrix
					m1 = tZeroMatrix_;
					m1.setTrans( osg::Vec3( 0., 0., 0. ) );
				}
				else
				{
					// convert the emitter's current orientation to a rotation matrix
					m1 = getLocalToWorldMatrix();
					m1.setTrans( osg::Vec3( 0., 0., 0. ) );
				}
				
				// convert the particle orientation to a rotation matrix
				m2.makeRotate( 
					particleAngle[0], yaxis,
					particleAngle[1], xaxis,
					particleAngle[2], zaxis
					 );
	
				m3 = m2 * m1;
				
				convertRotMtxToEuler( m3, temp );
	
				P->setAngle( temp );
			}
		}
	}
}


#define ARCCOS_SANITY( theta ) \
if( theta > 0.9999 ) theta = 0.9999;\
if( theta < -0.9999 ) theta = -0.9999;

// ================================================
// convertRotMtxToEuler
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void convertRotMtxToEuler( const osg::Matrix &m, osg::Vec3 &euler ) 
{
	osg::Vec3 yaxis( 0., 1., 0. ), zaxis( 0., 0., 1. );
	float heading, pitch, roll;
	
	osg::Vec3 headingv, pitchv, hpnormalv, v;

	pitchv = m.preMult( yaxis );
	headingv = pitchv;
	// zero-out the z component
	headingv[2] = 0.;
	// normalize
	headingv.normalize();
	
	/////////////////////////////////////////////////////////////////
	// HEADING
	/////////////////////////////////////////////////////////////////
	
	// the dot product of the two vectors will get us the cosine of the 
	// angle between them
	float cosH = headingv * yaxis; // dot product
	ARCCOS_SANITY( cosH )
	heading = acos(cosH);
	
	// the cross product of the two vectors will get us the vector normal 
	// to them
	v = headingv ^ yaxis; // cross product
	if( v[2] > 0. )
	{
		heading *= -1.;
	}
	
	
	/////////////////////////////////////////////////////////////////
	// PITCH
	/////////////////////////////////////////////////////////////////
	
	float cosP = pitchv * headingv; // dot product
	ARCCOS_SANITY( cosP )
	pitch = acos(cosP);

	if( pitchv[2] < 0. )
	{
		pitch *= -1.;
	}


	/////////////////////////////////////////////////////////////////
	// ROLL
	/////////////////////////////////////////////////////////////////
	
	hpnormalv = pitchv ^ headingv;
	hpnormalv.normalize();
	// If pitch is negative, hpnormalv will point in the "wrong" direction.
	// In this situation, we'll negate hpnormalv.  If we didn't do this, 
	// we'd get roll values that change sign as pitch changes from positive to 
	// negative or vice versa (yuck).
	if( pitch < 0. )
		hpnormalv *= -1.;
		
	osg::Vec3 zm;
	zm = m.preMult( zaxis );

/*
cout << "vv color " << "zm" << " 0.5 0.5 1.0\n";
cout << "vv set zm " << zm  << endl;
cout << "vv color pitchv 1 0 1\n";
cout << "vv set pitchv " << pitchv  << endl;
cout << "vv color headingv 1 .5 1\n";
cout << "vv set headingv " << headingv  << endl;
cout << "vv color " << "hpnormalv" << " 1. 1. 1.\n";
cout << "vv set hpnormalv " << hpnormalv << endl;
*/
	
	float cosR = zm * hpnormalv;
	ARCCOS_SANITY( cosR )
	roll = acos(cosR);
	// if the normal is roughly coincident with pitchv, then we need 
	// to negate the sign of the roll angle
	osg::Vec3 rollplanenormalv = zm ^ hpnormalv;
	rollplanenormalv.normalize();
//cout << "vv color " << "rollplanenormalv" << " .3 .3 .3\n";
//cout << "vv set rollplanenormalv " << rollplanenormalv << endl;
	if( pitchv * rollplanenormalv > 0. )
	{
		roll *= -1.;
	}
	
	euler.set( roll, pitch, heading );
//cout << "r p h " << euler << endl;
}


