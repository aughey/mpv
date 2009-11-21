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
 *  FILENAME:   ScaleOperator.h
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


#ifndef SCALE_OPERATOR_H
#define SCALE_OPERATOR_H

#include "Export.h"

#include <vector>
#include <string>

#include <osg/Group>
#include <osg/Geode>
#include <osg/Texture2D>
#include <osg/Geometry>
#include <osg/Vec3>
#include <osgDB/ReadFile>

#include "Operator.h"
#include "range.h"

#include "HPSParticleSystem.h"
#include "HPSParticle.h"

//=========================================================
//! This Operator modifies the scale vector of the particles.  
//! It operates only on HPSParticles.
//! Particles will start out with a scaling vector of 'begin' and, over 
//! the course of their lifetime, the scaling vector will be linearly 
//! interpolated until it equals 'end' at the end of the particle's life.
//! 
class OSGPARTICLE_EXPORT ScaleOperator: public osgParticleHPS::Operator {
public:

	//=========================================================
	//! General Constructor
	//! 
	ScaleOperator() ;

	//=========================================================
	//! Copy Constructor
	//! 
	ScaleOperator(const ScaleOperator &copy, const osg::CopyOp &copyop = osg::CopyOp::SHALLOW_COPY)
		: osgParticleHPS::Operator(copy, copyop) {}

	META_Object(osgParticleHPS, ScaleOperator);

	//=========================================================
	//! modifies the scale vector of particle P
	//! \param P - the particle to modify
	//! \param dt - time in seconds since last frame
	//! 
	void operate(osgParticleHPS::Particle *P, double dt);
	
	//=========================================================
	//! set the range for the scaling vector for the particles; 
	//! particles will start out with a scaling vector of 'begin' and, 
	//! over the course of their lifetime, the scaling vector will be linearly 
	//! interpolated until it equals 'end' at the end of the particle's life
	//! \param begin - initial scaling vector
	//! \param end - final scaling vector
	//! 
	void setRange( const osg::Vec3 &begin, const osg::Vec3 &end ) ;
	
protected:

	//=========================================================
	//! General Destructor
	//! 
	virtual ~ScaleOperator() {}

private:
	
	//=========================================================
	//! the scale vector for a particle at the beginning of its life
	//! 
	osg::Vec3 begin_;

	//=========================================================
	//! the scale vector for a particle at the end of its life
	//! 
	osg::Vec3 end_;
	
};

#endif

