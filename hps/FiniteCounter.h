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
 *  FILENAME:   FiniteCounter.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This file defines a new Counter class.
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


#ifndef FINITE_COUNTER
#define FINITE_COUNTER

#include <iostream>

#include "Export.h"

#include "Counter.h"
#include "range.h"
#include "Emitter.h"

#include <osg/CopyOp>
#include <osg/Object>

//=========================================================
//! This class is a Counter - it tells an Emitter how many particles to 
//! create for a give frame.  This counter is unique, in that it does not 
//! create particles continuously; instead, a number of particles is created 
//! the first frame that the counter is active, and no more particles are 
//! created after that.
//! 
class OSGPARTICLE_EXPORT FiniteCounter: public osgParticleHPS::Counter {
public:
	
	//=========================================================
	//! General Constructor
	//! 
	FiniteCounter(): osgParticleHPS::Counter() 
	{ 
		firstTimeThrough_ = true; 
		numParticlesRange_.set(1.,1.);
		emitter_ = NULL;
		emittersPrevTime_ = -1.;
	}

	//=========================================================
	//! Copy Constructor
	//! 
	FiniteCounter(const FiniteCounter &copy, const osg::CopyOp &copyop = osg::CopyOp::SHALLOW_COPY)
	: osgParticleHPS::Counter(copy, copyop) {  }
	
	META_Object(osgParticleHPS, FiniteCounter);

	//=========================================================
	//! Tells this counter which emitter it is a part of.
	//! \param e - The emitter that this counter belongs to
	//! 
	void setEmitter( osgParticleHPS::Emitter *e ) { emitter_ = e; }

	//=========================================================
	//! Retrieves the range of particles
	//! \return the range of particles
	//! 
	const osgParticleHPS::rangef &getNumParticlesRange() const { return numParticlesRange_; }
	
	//=========================================================
	//! Sets the range of particles to create
	//! \param r - The new range
	//! 
	void setNumParticlesRange(const osgParticleHPS::rangef &r)
	{
		numParticlesRange_ = r;
	}
	
	//=========================================================
	//! Sets the range of particles to create
	//! \param minrange - the minimum number of particles to create 
	//! \param maxrange - the maximum number of particles to create 
	//! 
	void setNumParticlesRange(float minrange, float maxrange)
	{
		numParticlesRange_.set(minrange, maxrange);
	}

	//=========================================================
	//! Tells the emitter how many particles to create.
	//! The first frame that this counter is active, a value in the range 
	//! numParticlesRange_ will be returned.  For all subsequent frames, 
	//! zero will be returned.
	//! \param dt - deltaT, the number of seconds since the last frame
	//! \return the number of particles to create
	//! 
	virtual int numParticlesToCreate(double dt) const 
	{
		if( emitter_ )
		{
			// if the emitter's current-time has jumped backwards, 
			// reset this counter
			if( emitter_->getCurrentTime() < emittersPrevTime_ )
			{
				firstTimeThrough_ = true;
			}
			emittersPrevTime_ = emitter_->getCurrentTime();
		}

		if( firstTimeThrough_ ) 
		{
			firstTimeThrough_ = false;
			int result = (int)( numParticlesRange_.get_random() + 0.5 );
			return result;
		}
		else
		{
			return 0;
		}
	}
	
protected:

	//=========================================================
	//! General Destructor
	//! 
	virtual ~FiniteCounter() {}
	
	//=========================================================
	//! The range for the number of particles to create on activation
	//! 
	osgParticleHPS::rangef numParticlesRange_;

	//=========================================================
	//! Stores whether this is the first frame or not
	//! 
	mutable bool firstTimeThrough_;
	
	//=========================================================
	//! A pointer to the Emitter that this counter belongs to
	//! 
	osgParticleHPS::Emitter *emitter_;
	
	//=========================================================
	//! The value returned by emitter_'s getCurrentTime, last time the 
	//! numParticlesToCreate function was called.  If this number ever 
	//! decreases, we know that the emitter has been reset, and that this 
	//! counter should reset itself as well.
	//! 
	mutable float emittersPrevTime_;
};


#endif
