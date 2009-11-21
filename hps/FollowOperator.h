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
 *  FILENAME:   FollowOperator.h
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


#ifndef FOLLOW_OPERATOR_H
#define FOLLOW_OPERATOR_H

#include <vector>
#include <string>

#include <osg/Group>
#include <osg/Geode>
#include <osg/Texture2D>
#include <osg/Geometry>
#include <osg/Vec3>
#include <osgDB/ReadFile>

#include "Export.h"

#include "Operator.h"
#include "range.h"

#include "Particle.h"

//=========================================================
//! This Operator modifies the position of the particles.  
//! All of the particles that this operator acts on will be translated by the 
//! same amount that this operator's Program has been moved during the last 
//! frame.
//! 
class OSGPARTICLE_EXPORT FollowOperator: public osgParticleHPS::Operator {
public:

	//=========================================================
	//! General Constructor
	//! 
	FollowOperator() ;

	//=========================================================
	//! Copy Constructor
	//! 
	FollowOperator(const FollowOperator &copy, const osg::CopyOp &copyop = osg::CopyOp::SHALLOW_COPY)
		: osgParticleHPS::Operator(copy, copyop) {}

	META_Object(osgParticleHPS, FollowOperator);

	//=========================================================
	//! calculates the distance that this operator's Program has been 
	//! moved during the last frame
	//! \param program - the program that this operator belongs to
	//! 
	virtual void beginOperate( osgParticleHPS::Program *program );

	//=========================================================
	//! modifies the scale vector of particle P
	//! \param P - the particle to modify
	//! \param dt - time in seconds since last frame
	//! 
	virtual void operate(osgParticleHPS::Particle *P, double dt);
	
	//=========================================================
	//! prepares the operator for the next frame
	//! 
	virtual void endOperate();

	//=========================================================
	//! If this function is called with true as the parameter, 
	//! the operator will not modify the particles' roll value.
	//! \param exclude - boolean indicating whether this rotation should be 
	//! excluded
	//! 
	void setExcludeRoll( bool exclude ) { excludeRoll_ = exclude; }
	
	//=========================================================
	//! If this function is called with true as the parameter, 
	//! the operator will not modify the particles' pitch value.
	//! \param exclude - boolean indicating whether this rotation should be 
	//! excluded
	//! 
	void setExcludePitch( bool exclude ) { excludePitch_ = exclude; }
	
	//=========================================================
	//! If this function is called with true as the parameter, 
	//! the operator will not modify the particles' yaw value.
	//! \param exclude - boolean indicating whether this rotation should be 
	//! excluded
	//! 
	void setExcludeYaw( bool exclude ) { excludeYaw_ = exclude; }
	
protected:

	//=========================================================
	//! General Destructor
	//! 
	virtual ~FollowOperator() {}

private:
	
	//=========================================================
	//! the position of this operator during the current frame
	//! 
	osg::Vec3 currentPosition_;
	
	//=========================================================
	//! the position of this operator during the previous frame
	//! 
	osg::Vec3 lastPosition_;
	
	//=========================================================
	//! the distance that this operator's Program has been 
	//! moved during the last frame
	//! 
	osg::Vec3 positionDelta_;

	//=========================================================
	//! the rotation matrix for the orientation of this operator's Program
	//! 
	osg::Matrix rotationMtx_;
	
	//=========================================================
	//! the inverse of the rotation matrix for the previous frame
	//! 
	osg::Matrix lastRotationMtxInv_;
	
	//=========================================================
	//! the orientation of this operator's Program, in Euler angles
	//! 
	osg::Vec3 eulerAngle_;

	//=========================================================
	//! indicates whether the beginOperate function has been called yet
	//! 
	bool firstTimeThrough_;
	
	//=========================================================
	//! indicates whether the operator should modify roll rotations 
	//! 
	bool excludeRoll_;
	
	//=========================================================
	//! indicates whether the operator should modify pitch rotations 
	//! 
	bool excludePitch_;
	
	//=========================================================
	//! indicates whether the operator should modify yaw rotations 
	//! 
	bool excludeYaw_;
};

#endif

