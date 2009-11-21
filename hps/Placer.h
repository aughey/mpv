/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2003 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/
//osgParticle - Copyright (C) 2002 Marco Jez

#ifndef OSGPARTICLE_PLACER_
#define OSGPARTICLE_PLACER_ 1

#include "Export.h"
#include "Particle.h"

#include <osg/Vec3>
#include <osg/CopyOp>
#include <osg/Object>

namespace osgParticleHPS
{

    /**    An abstract base class for implementing <I>particle placers</I>. A placer is an object which take
        a particle as input, and places it somewhere by setting its position vector. Placer objects are
        used by the <CODE>ModularEmitter</CODE> class as part of the particle emission process.
    */
    class OSGPARTICLE_EXPORT Placer: public osg::Object {
    public:
        inline Placer();
        inline Placer(const Placer &copy, const osg::CopyOp &copyop = osg::CopyOp::SHALLOW_COPY);

        virtual const char *libraryName() const { return "osgParticleHPS"; }
        virtual const char *className() const { return "Placer"; }
        virtual bool isSameKindAs(const osg::Object *obj) const { return dynamic_cast<const Placer *>(obj) != 0; }


		//=========================================================
		//! Sets the min and max values for the range of initial orientations.
		//! The actual values used for a particle's initial orientation  
		//! will be a random value from the range minOrientation_ to maxOrientation_.
		//! \param mins - the minimum angles
		//! \param maxes - the maximum angles
		//! 
		inline void setOrientationRanges(const osg::Vec3 &mins, const osg::Vec3 &maxes);

		/// Place a particle. 
		inline virtual void place(Particle *P) const;
		
		/// Set a particle's position. Must be implemented in descendant classes.
		virtual void setPosition( Particle *P ) const = 0;
		
		/// Set a particle's orientation.
		inline virtual void setOrientation( Particle *P ) const;

    protected:
        ~Placer() {}
        Placer &operator=(const Placer &) { return *this; }

		//=========================================================
		//! Minimum orientation values;  the actual values used for a particle 
		//! will be a random value from the range minOrientation_ to maxOrientation_.
		//! Units are radians.  Order is roll, pitch, yaw.
		//! 
		osg::Vec3 minOrientation_;

		//=========================================================
		//! Maximum orientation values;  the actual values used for a particle 
		//! will be a random value from the range minOrientation_ to maxOrientation_.
		//! Units are radians.  Order is roll, pitch, yaw.
		//! 
		osg::Vec3 maxOrientation_;
    };
    
    // INLINE FUNCTIONS
    
    inline Placer::Placer()
    : osg::Object(), 
	  minOrientation_(0,0,0), maxOrientation_(0,0,0)
    {
    }
    
    inline Placer::Placer(const Placer &copy, const osg::CopyOp &copyop)
    : osg::Object(copy, copyop),
	  minOrientation_(copy.minOrientation_), maxOrientation_(copy.maxOrientation_)
    {
    }

	inline void Placer::setOrientationRanges(const osg::Vec3 &mins, const osg::Vec3 &maxes)
	{
		minOrientation_ = mins;
		maxOrientation_ = maxes;
	}

	inline void Placer::place(Particle *P) const
	{
		setPosition( P );
		setOrientation( P );
	}

	inline void Placer::setOrientation( Particle *P ) const
	{
		P->setAngle(rangev3(minOrientation_, maxOrientation_).get_random());
	}
}

#endif
