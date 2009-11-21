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
 *  FILENAME:   SpherePlacer.h
 *  LANGUAGE:   C++
 *  CLASS:	  UNCLASSIFIED
 *  PROJECT:	Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This file defines the SpherePlacer class.
 *  
 *  MODIFICATION NOTES:
 *  DATE	 NAME								SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  08/26/2004 Andrew Sampson					   MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */

// Note - most of this file was taken from the osgParticle SectorPlacer and 
// RadialShooter classes. -ads

#ifndef SPHERE_PLACER_H
#define SPHERE_PLACER_H

#include "Export.h"

#include <math.h>  // for M_PI

#include "Placer.h"
#include "Particle.h"

#include <osg/CopyOp>
#include <osg/Object>
#include <osg/Vec3>

namespace osgParticleHPS {

//=========================================================
//! A particle placer 
//! 
class OSGPARTICLE_EXPORT SpherePlacer: public CenteredPlacer {
public:

	//=========================================================
	//! General Constructor
	//! 
	inline SpherePlacer();

	//=========================================================
	//! Copy Constructor
	//! 
	inline SpherePlacer(const SpherePlacer &copy, const osg::CopyOp &copyop = osg::CopyOp::SHALLOW_COPY);

	META_Object(osgParticleHPS, SpherePlacer);

	/// Get the range of possible values for radius.
	inline const rangef &getRadiusRange() const;
	
	/// Set the range of possible values for radius.
	inline void setRadiusRange(const rangef &r);
	
	/// Set the range of possible values for radius.
	inline void setRadiusRange(float r1, float r2);
	
	/// Get the range of possible values for the central angle.
	inline const rangef &getPhiRange() const;		
	
	/// Set the range of possible values for the central angle.
	inline void setPhiRange(const rangef &r);
	
	/// Set the range of possible values for the central angle.
	inline void setPhiRange(float r1, float r2);	

	/// Get the range of possible values for <B>theta</B> angle.
	inline const rangef &getThetaRange() const;
	
	/// Set the range of possible values for <B>theta</B> angle.
	inline void setThetaRange(const rangef &r);
	
	/// Set the range of possible values for <B>theta</B> angle.
	inline void setThetaRange(float r1, float r2);
		
	//=========================================================
	//! gives particle P an initial position
	//! \param P - the particle to modify
	//! 
	inline void setPosition(Particle *P) const;

protected:

	//=========================================================
	//! General Destructor
	//! 
	virtual ~SpherePlacer() {}

	//=========================================================
	//! (Hidden) assignment operator
	//! \param f - unused
	//! \return nothing
	//! 
	SpherePlacer &operator=(const SpherePlacer &f) { return *this; }		

private:

	//=========================================================
	//! 
	rangef rad_range_;
	rangef phi_range_;
	rangef theta_range_;
};

// INLINE FUNCTIONS


inline SpherePlacer::SpherePlacer()
: CenteredPlacer(), rad_range_(0, 1), phi_range_(0, osg::PI*2), theta_range_(0, 0.5f*osg::PI_4)
{
}

inline SpherePlacer::SpherePlacer(const SpherePlacer &copy, const osg::CopyOp &copyop)
: CenteredPlacer(copy, copyop), rad_range_(copy.rad_range_), phi_range_(copy.phi_range_)
{
}

inline const rangef &SpherePlacer::getRadiusRange() const
{
	return rad_range_;
}

inline const rangef &SpherePlacer::getPhiRange() const
{
	return phi_range_;
}

inline const rangef &SpherePlacer::getThetaRange() const
{
	return phi_range_;
}

inline void SpherePlacer::setRadiusRange(const rangef &r)
{
	rad_range_ = r;
}

inline void SpherePlacer::setRadiusRange(float r1, float r2)
{
	rad_range_.minimum = r1;
	rad_range_.maximum = r2;
}

inline void SpherePlacer::setPhiRange(const rangef &r)
{
	phi_range_ = r;
}

inline void SpherePlacer::setPhiRange(float r1, float r2)
{
	phi_range_.minimum = r1;
	phi_range_.maximum = r2;
}

inline void SpherePlacer::setThetaRange(const rangef &r)
{
	theta_range_ = r;
}

inline void SpherePlacer::setThetaRange(float r1, float r2)
{
	theta_range_.minimum = r1;
	theta_range_.maximum = r2;
}

inline void SpherePlacer::setPosition(Particle *P) const
{
	float rad = rad_range_.get_random();
	float phi = phi_range_.get_random();
	float theta = theta_range_.get_random();
	
	osg::Vec3 pos(
		getCenter().x() + rad * sinf(theta) * cosf(phi),
		getCenter().y() + rad * sinf(theta) * sinf(phi),
		getCenter().z() + rad * cosf(theta) );
	
	P->setPosition(pos);
}


}

#endif
