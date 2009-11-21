/** <pre>
 *  Copyright (c) 2007 Andrew Sampson
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
 *  The ThermalUpdraftOperator class implements a primitive thermal model. 
 *  It pushes particles upward in the middle, and sucks particles inward 
 *  around the base.
 *  
 *  Initial Release: 2007-11-04 Andrew Sampson
 * </pre>
 */

#include <iostream>
#include <algorithm>

#include "ThermalUpdraftOperator.h"
#include "ParticleProcessor.h"

// ================================================
// ThermalUpdraftOperator
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ThermalUpdraftOperator::ThermalUpdraftOperator() 
		: osgParticleHPS::Operator() 
{
	radius = 1.0;
	height = 6.0;
	newtons = 7.0;
}


// ================================================
// beginOperate
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ThermalUpdraftOperator::beginOperate( osgParticleHPS::Program *program )
{
	programPosition = 
		program->transformLocalToWorld( osg::Vec3( 0., 0., 0. ) );
}


// ================================================
// operate
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ThermalUpdraftOperator::operate(osgParticleHPS::Particle *P, double dt)
{

//nq	// Need to find the position of the particle, relative to the operator 
//nq	// origin, in polar coordinates.  

	// fixme - allow user to position thermal updraft relative to part sys origin?
	
	osg::Vec3 localVector( P->getPosition() - programPosition );
	bool underground = ( localVector[2] < 0.0 );
	float localZ = std::max( 0.0f, localVector[2] );

	osg::Vec3 localVectorXY( localVector[0], localVector[1], 0.0 );
	float localLengthXY = localVectorXY.normalize();
	
//std::cout << "particle locl at "
//<< localVector[0] << " "
//<< localVector[1] << " "
//<< localVector[2] << std::endl;
	float localLength = localVector.normalize();
//std::cout << "particle unit at "
//<< localVector[0] << " "
//<< localVector[1] << " "
//<< localVector[2] << std::endl;
	
	osg::Vec3 force;
	if( localLengthXY < radius )
	{
		// the central column of hot air moves upward and outward
		
		if( localZ < height * 0.3 )
			force.set( 0.0, 0.0, newtons );
		else
			force = localVector * newtons;

		// attenuate based on z
		force *= std::max( 0.0, 1.0 - (localZ / height) );
	}
	else
	{
		// outside of the central column, air gets sucked in
		force = localVectorXY * -1.0 * newtons;
		
		// attenuate based on xy distance
		// note - for denominator, (2R - R) is simplified to just R
//		force *= std::max( 0.0, 1.0 - ((localLengthXY - radius)/radius) );

		// attenuate based on z
		force *= std::max( 0.0, 1.0 - (localZ / (height * 0.3)) );
	}
	
	P->addVelocity( force * (P->getMassInv() * dt) );
	
	if( underground )
	{
		osg::Vec3 velocity = P->getVelocity();
		velocity[2] = std::max( 0.0f, velocity[2] );
		P->setVelocity( velocity );
	}
}

