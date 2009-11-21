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
 *  FILENAME:   ScaleOperator.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This file defines the ScaleOperator class.
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


#include "ScaleOperator.h"

// ================================================
// ScaleOperator
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ScaleOperator::ScaleOperator() 
		: osgParticleHPS::Operator() 
{
	begin_.set( 1., 1., 1. );
	end_.set( 1., 1., 1. );
}


// ================================================
// operate
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ScaleOperator::operate(osgParticleHPS::Particle *P, double dt)
{
	// this operator does not act on vanilla Particles, only HPSParticles
	if( P->getShape() != (osgParticleHPS::Particle::Shape)-1 )
		return;
	
	osgParticleHPS::HPSParticle *hpsp = (osgParticleHPS::HPSParticle *)P;
	
	float partAge = hpsp->getAge();
	float partLifeTime = hpsp->getLifeTime();
	
	// this operator does not act on immortal particles
	if( partLifeTime <= 0.0 )
		return;
	
	osg::Vec3 temp = end_ - begin_;
	temp *= partAge / partLifeTime;
	temp += begin_;
	
	hpsp->setScale( temp );
}

// ================================================
// setRange
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ScaleOperator::setRange( const osg::Vec3 &begin, const osg::Vec3 &end ) 
{
	begin_ = begin;
	end_ = end;
}
