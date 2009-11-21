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
 *  FILENAME:   TextureAnimOperator.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This file defines the TextureAnimOperator class.
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


#ifndef TEXTURE_ANIM_OPERATOR_H
#define TEXTURE_ANIM_OPERATOR_H

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

#include "ParticleSystem.h"
#include "Particle.h"

//=========================================================
//! This Operator changes the Texture applied to a particle.
//! It operates only on HPSParticles.
//! Animations are constructed by adding images via the 'addFrame' function.
//! The rate at which the images are changed is controlled via 'setFramePeriod'.
//! Control over the behavior when an animation reaches the end of its 
//! list of images is provided through setLoop and setHoldLastFrame.
//! 
class OSGPARTICLE_EXPORT TextureAnimOperator: public osgParticleHPS::Operator {
public:
	//=========================================================
	//! General Constructor
	//! 
	TextureAnimOperator() ;

	//=========================================================
	//! Copy Constructor
	//! 
	TextureAnimOperator(const TextureAnimOperator &copy, const osg::CopyOp &copyop = osg::CopyOp::SHALLOW_COPY)
		: osgParticleHPS::Operator(copy, copyop) {}

	META_Object(osgParticleHPS, TextureAnimOperator);

	//=========================================================
	//! 
	//! \param program - the program that this operator belongs to
	//! 
	virtual void beginOperate( osgParticleHPS::Program *program );

	//=========================================================
	//! changes the texture applied to particle P
	//! \param P - the particle to modify
	//! \param dt - time in seconds since last frame
	//! 
	void operate(osgParticleHPS::Particle *P, double dt);
	
	//=========================================================
	//! Sets the amount of delay between frames.  
	//! Examples: 0.03333 roughly corresponds to 30 fps, 0.01666 to 60 fps
	//! \param period - time in seconds between frames
	//! 
	void setFramePeriod( float period ) { period_ = period; }
	
	//=========================================================
	//! Sets whether the animation should start at the beginning when it 
	//! reaches the end.
	//! \param loop - determines whether the animation should start at the 
	//!     beginning when it reaches the end
	//! 
	void setLoop( bool loop ) { loop_ = loop; }

	//=========================================================
	//! Sets whether the last frame in the animation should be displayed when 
	//! it reaches the end.  This setting is ignored if looping is enabled.
	//! \param hold_last_frame - determines whether the last frame in the 
	//!     animation should be displayed when it reaches the end
	//! 
	void setHoldLastFrame( bool hold_last_frame ) { hold_last_frame_ = hold_last_frame; }
	
	//=========================================================
	//! Adds a frame to the end of the animation.
	//! \param filename - the filename of the image to add
	//! 
	void addFrame( std::string &filename );
	
protected:

	//=========================================================
	//! General Destructor
	//! 
	virtual ~TextureAnimOperator() {}

private:
	
	//=========================================================
	//! the list of textures, one for every frame of the animation
	//! 
	std::vector<osg::ref_ptr<osg::Texture2D> > textures_;

	//=========================================================
	//! the amount of time between frames
	//! 
	float period_;
	
	//=========================================================
	//! determines whether the animation loops for the life of the particle, 
	//! or stops after the animation has played once
	//! 
	bool loop_;
	
	//=========================================================
	//! determines whether, when an animation is finished, the last frame
	//! is applied, or no texture is applied.  This variable is only checked 
	//! when the loop variable is false.
	//! 
	bool hold_last_frame_;
	
	//=========================================================
	//! points to osg::StateSet attached to ParticleSystem generating the particles
	//! used to update the current animation texture
	//! 
	osg::ref_ptr<osg::StateSet> stateset_;
};

#endif

