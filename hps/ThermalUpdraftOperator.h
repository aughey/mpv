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


#ifndef THERMAL_UPDRAFT_OPERATOR_H
#define THERMAL_UPDRAFT_OPERATOR_H

#include "Export.h"

#include <vector>
#include <string>

#include <osg/Vec3>

#include "Operator.h"
#include "range.h"

#include "Particle.h"

//=========================================================
//! This Operator 
//! 
class OSGPARTICLE_EXPORT ThermalUpdraftOperator: public osgParticleHPS::Operator {
public:

	//=========================================================
	//! General Constructor
	//! 
	ThermalUpdraftOperator();

	//=========================================================
	//! Copy Constructor
	//! 
	ThermalUpdraftOperator(const ThermalUpdraftOperator &copy, const osg::CopyOp &copyop = osg::CopyOp::SHALLOW_COPY)
		: osgParticleHPS::Operator(copy, copyop) {}

	META_Object(osgParticleHPS, ThermalUpdraftOperator);

	//=========================================================
	//! 
	//! \param program - the program that this operator belongs to
	//! 
	virtual void beginOperate( osgParticleHPS::Program *program );

	//=========================================================
	//! 
	//! \param P - the particle to modify
	//! \param dt - time in seconds since last frame
	//! 
	void operate(osgParticleHPS::Particle *P, double dt);
	
	//=========================================================
	//! Sets the maximum radius of this operator's influence
	//! \param newRadius - max range of the operator's influence
	//! 
	void setRadius( float newRadius ) { radius = fabs(newRadius); }
	
	//=========================================================
	//! Sets the maximum height of this operator's influence
	//! \param newHeight - max height of the operator's influence
	//! 
	void setHeight( float newHeight ) { height = fabs(newHeight); }
	
	//=========================================================
	//! Sets the maximum force applied by this operator
	//! \param newForce - maximum force that this operator will exert, in newtons
	//! 
	void setForce( float newForce ) { newtons = fabs(newForce); }
	
protected:

	//=========================================================
	//! General Destructor
	//! 
	virtual ~ThermalUpdraftOperator() {}

private:
	
	//=========================================================
//fixme - radius is now used as the radius of the central column of hot air
	//! The maximum radius of this operator's influence.  Units are meters.
	//! 
	float radius;
	
	//=========================================================
	//! The maximum height of this operator's influence.  Units are meters.
	//! 
	float height;
	
	//=========================================================
	//! The maximum force that this operator will exert.  Units are newtons.
	//! 
	float newtons;

	osg::Vec3 programPosition;
};

#endif

