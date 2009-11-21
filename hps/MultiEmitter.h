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
 *  FILENAME:   MultiEmitter.h
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

#ifndef MULTIEMITTER_H
#define MULTIEMITTER_H

#include <vector>

#include "Export.h"
#include "Emitter.h"
#include "Particle.h"
#include "RandomRateCounter.h"
#include "Placer.h"
#include "PointPlacer.h"
#include "Shooter.h"
#include "RadialShooter.h"
#include "ParticleSystem.h"

#include <osg/ref_ptr>
#include <osg/CopyOp>
#include <osg/Object>
#include <osg/Node>
#include <osg/NodeVisitor>
#include <osg/Vec3>
#include <osg/Matrix>
#include <osg/Quat>

// define a new reference frame
#define RELATIVE_TO_TZERO 100

//=========================================================
//! Extracts euler angles (roll, pitch, yaw) from a rotation matrix.
//! \param m - source rotation matrix
//! \param euler - vector to place the results in
//! 
void convertRotMtxToEuler( const osg::Matrix &m, osg::Vec3 &euler ) ;
	

namespace osgParticleHPS
{

//=========================================================
//! This class is a particle emitter.  It differs from the stock 
//! ModularEmitter in that it allows for multiple Placers and Shooters.
//! This functionality is really only useful in one situation: the placers 
//! that come with OSG do not set the initial orientation.  By adding two 
//! Placers to a MultiEmitter, one of which changes the particle's initial 
//! location and another which changes the particle's initial orientation, 
//! the user can create particles with initial orientations AND still be able 
//! to reuse the existing OSG Placers.
//! 
class OSGPARTICLE_EXPORT MultiEmitter: public Emitter {
public:

	//=========================================================
	//! General Constructor
	//! 
	MultiEmitter();

	//=========================================================
	//! Copy Constructor
	//! 
	MultiEmitter(const MultiEmitter &copy, const osg::CopyOp &copyop = osg::CopyOp::SHALLOW_COPY);
	
	META_Node(osgParticleHPS,MultiEmitter);
	
	//=========================================================
	//! returns the Counter 
	//! \return the Counter 
	//! 
	inline Counter *getCounter();
	
	//=========================================================
	//! returns the Counter 
	//! \return the Counter 
	//! 
	inline const Counter *getCounter() const;
	
	//=========================================================
	//! sets the Counter
	//! \param c - the new Counter
	//! 
	inline void setCounter(Counter *c);
	
	//=========================================================
	//! returns the number of Placers
	//! \return the number of Placers
	//! 
	int getNumPlacers() { return placers_.size(); }
	
	//=========================================================
	//! returns the Placer at position i
	//! \param i - the index of the Placer to retrieve
	//! \return the Placer at position i, or NULL
	//! 
	inline Placer *getPlacer( int i );
	
	//=========================================================
	//! returns the Placer at position i
	//! \param i - the index of the Placer to retrieve
	//! \return the Placer at position i, or NULL
	//! 
	inline const Placer *getPlacer( int i ) const;
	
	//=========================================================
	//! adds a placer to the emitter
	//! \param p - the placer to add
	//! 
	inline void addPlacer(Placer *p);
	
	//=========================================================
	//! removes the placer at position i
	//! \param i - the index of the Placer to remove
	//! 
	void removePlacer( int i ) { placers_.erase( placers_.begin() + i ); }
	
	//=========================================================
	//! returns the number of Shooters
	//! \return the number of Shooters
	//! 
	int getNumShooters() { return shooters_.size(); }
	
	//=========================================================
	//! returns the Shooter at position i
	//! \param i - the index of the Shooter to retrieve
	//! \return the Shooter at position i, or NULL
	//! 
	inline Shooter *getShooter( int i );
	
	//=========================================================
	//! returns the Shooter at position i
	//! \param i - the index of the Shooter to retrieve
	//! \return the Shooter at position i, or NULL
	//! 
	inline const Shooter *getShooter( int i ) const;
	
	//=========================================================
	//! adds a Shooter to the emitter
	//! \param s - the Shooter to add
	//! 
	inline void addShooter(Shooter *s);		

	//=========================================================
	//! removes the Shooter at position i
	//! \param i - the index of the Shooter to remove
	//! 
	void removeShooter( int i ) { shooters_.erase( shooters_.begin() + i ); }

	//=========================================================
	//! Enables or disables additive velocity.  If additive velocity is 
	//! enabled, the velocity of the emitter will be added to the initial 
	//! velocity of the particles it creates.  Additive velocity is enabled 
	//! by default.  You should turn it off for particle systems like 
	//! smoke trails and puffy explosions, etc.  You should leave it enabled 
	//! for effects like tracers.
	//! \param enable - set to true to enable additive velocity, false to disable
	//! 
	void setAdditiveVelocity( bool enable ) { additiveVelocity_ = enable; }
	
	//=========================================================
	//! Because ParticleProcessor won't call our process() function (ie emit()) 
	//! unless the currentTime >= our start time, in some situations we need 
	//! to give ParticleProcessor a fake start time of 0 (so that emit() is 
	//! called for every frame).  This additional delay is the "real" start 
	//! time, if you will.
	//! Note that this mechanism doesn't interfere with the behavior of 
	//! startTime, so you can choose to ignore this function and everything 
	//! will work as expected.  The only time when this mechanism is truly 
	//! needed is when reference frame is RELATIVE_TO_TZERO.  In this case, we 
	//! need to find out what the emitter's position is at T0 and store it.
	//! The only way to get the position at T0 is to be active at T0, hence 
	//! the need for this mechanism.
	//! 
	void setAdditionalDelay( double addlDelay ) { additionalDelay_ = addlDelay; }
	
protected:

	//=========================================================
	//! General Destructor
	//! 
	virtual ~MultiEmitter() {}

	//=========================================================
	//! (Hidden) assignment operator
	//! \param f - unused
	//! \return nothing
	//! 
	MultiEmitter &operator=(const MultiEmitter &f) { return *this; }
	
	//=========================================================
	//! Calls the counter's numParticlesToCreate, and if particles are to be 
	//! created this frame, they are created/initialized by the ParticleSystem 
	//! and then further manipulated by the Placers and Shooters.
	//! \param dt - time in seconds since last frame
	//! 
	void emit(double dt);

private:

	//=========================================================
	//! the Counter
	//! 
	osg::ref_ptr<Counter> counter_;

	//=========================================================
	//! a list of Placers
	//! 
	std::vector<osg::ref_ptr<Placer> > placers_;

	//=========================================================
	//! a list of Shooters
	//! 
	std::vector<osg::ref_ptr<Shooter> > shooters_;

	//=========================================================
	//! the time recorded when emit was last called
	//! 
	double prevTime_;

	//=========================================================
	//! This emitter will start emitting particles at which ever comes later: 
	//!  currentTime >= startTime
	//!   OR 
	//!  currentTime >= additionalDelay_
	//! 
	//! additionalDelay_ defaults to 0, so everything behaves 
	//! as expected if you don't change it.
	//! See the comments near setAdditionalDelay for more info.
	//! 
	double additionalDelay_;

	//=========================================================
	//! the last recorded position of this emitter; used when calculating 
	//! the velocity
	//! 
	osg::Vec3 prevPosition_;

	//=========================================================
	//! the first recorded position of this emitter; used when in 
	//! RELATIVE_TO_TZERO mode
	//! 
	osg::Matrix tZeroMatrix_;

	//=========================================================
	//! If additive velocity is enabled, the velocity of the emitter will be
	//! added to the initial velocity of the particles it creates.  Additive
	//! velocity is enabled by default.  You should turn it off for particle
	//! systems like smoke trails and puffy explosions, etc.  You should leave
	//! it enabled for effects like tracers.
	//! 
	bool additiveVelocity_;

	//=========================================================
	//! Convenience variable
	//! 
	osg::Vec3 xaxis;
	//=========================================================
	//! Convenience variable
	//! 
	osg::Vec3 yaxis;
	//=========================================================
	//! Convenience variable
	//! 
	osg::Vec3 zaxis;
};

// INLINE FUNCTIONS

// ================================================
// getCounter
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
inline Counter *MultiEmitter::getCounter()
{
	return counter_.get();
}

// ================================================
// getCounter
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
inline const Counter *MultiEmitter::getCounter() const
{
	return counter_.get();
}

// ================================================
// setCounter
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
inline void MultiEmitter::setCounter(Counter *c)
{
	counter_ = c;
}

// ================================================
// getPlacer
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
inline Placer *MultiEmitter::getPlacer( int i )
{
	return placers_[i].get();
}

// ================================================
// getPlacer
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
inline const Placer *MultiEmitter::getPlacer( int i ) const
{
	return placers_[i].get();
}

// ================================================
// addPlacer
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
inline void MultiEmitter::addPlacer(Placer *p)
{
	placers_.push_back( p );
}

// ================================================
// getShooter
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
inline Shooter *MultiEmitter::getShooter( int i )
{
	return shooters_[i].get();
}

// ================================================
// getShooter
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
inline const Shooter *MultiEmitter::getShooter( int i ) const
{
	return shooters_[i].get();
}

// ================================================
// addShooter
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
inline void MultiEmitter::addShooter(Shooter *s)
{
	shooters_.push_back( s );
}

}

#endif
