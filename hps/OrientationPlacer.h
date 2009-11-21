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
 *  FILENAME:   OrientationPlacer.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This file defines the OrientationPlacer class.
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

#ifndef ORIENTATION_PLACER_H
#define ORIENTATION_PLACER_H

#ifdef WIN32
#define _USE_MATH_DEFINES
#endif
#include <math.h>  // for M_PI

#include "Export.h"

#include "Placer.h"
#include "Particle.h"

#include <osg/CopyOp>
#include <osg/Object>
#include <osg/Vec3>

namespace osgParticleHPS {

//=========================================================
//! A particle placer that only sets the orientation.
//! 
class OSGPARTICLE_EXPORT OrientationPlacer: public Placer {
public:

	//=========================================================
	//! General Constructor
	//! 
	inline OrientationPlacer();

	//=========================================================
	//! Copy Constructor
	//! 
	inline OrientationPlacer(const OrientationPlacer &copy, const osg::CopyOp &copyop = osg::CopyOp::SHALLOW_COPY);

	META_Object(osgParticleHPS, OrientationPlacer);

	//=========================================================
	//! Sets the min and max values for the range of initial orientations.
	//! The actual values used for a particle's initial orientation  
	//! will be a random value from the range mins_ to maxes_.
	//! \param mins - the minimum angles
	//! \param maxes - the maximum angles
	//! 
	inline void setRanges(const osg::Vec3 &mins, const osg::Vec3 &maxes);
	
	//=========================================================
	//! gives particle P an initial orientation
	//! \param P - the particle to modify
	//! 
	inline void place(Particle *P) const;

protected:

	//=========================================================
	//! General Destructor
	//! 
	virtual ~OrientationPlacer() {}

	//=========================================================
	//! (Hidden) assignment operator
	//! \param f - unused
	//! \return nothing
	//! 
	OrientationPlacer &operator=(const OrientationPlacer &f) { return *this; }		

private:

	//=========================================================
	//! Minimum orientation values;  the actual values used for a particle 
	//! will be a random value from the range mins_ to maxes_.
	//! 
	osg::Vec3 mins_;

	//=========================================================
	//! Maximum orientation values;  the actual values used for a particle 
	//! will be a random value from the range mins_ to maxes_.
	//! 
	osg::Vec3 maxes_;
};

// INLINE FUNCTIONS

// ================================================
// OrientationPlacer
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
inline OrientationPlacer::OrientationPlacer()
: Placer(), mins_(-M_PI, -M_PI, -M_PI), maxes_(M_PI, M_PI, M_PI)
{
}

// ================================================
// OrientationPlacer
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
inline OrientationPlacer::OrientationPlacer(const OrientationPlacer &copy, const osg::CopyOp &copyop)
: Placer(copy, copyop), mins_(copy.mins_), maxes_(copy.maxes_)
{
}

// ================================================
// setRanges
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
inline void OrientationPlacer::setRanges(const osg::Vec3 &mins, const osg::Vec3 &maxes)
{
	mins_ = mins;
	maxes_ = maxes;
}

// ================================================
// place
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
inline void OrientationPlacer::place(Particle *P) const
{
	P->setAngle(rangev3(mins_, maxes_).get_random());
}
	
}

#endif
