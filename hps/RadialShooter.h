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

#ifndef OSGPARTICLE_RADIALSHOOTER_
#define OSGPARTICLE_RADIALSHOOTER_ 1

#include "Export.h"

#include "Shooter.h"
#include "Particle.h"
#include "range.h"

#include <osg/CopyOp>
#include <osg/Object>
#include <osg/Math>

namespace osgParticleHPS
{

    /**    A shooter class that shoots particles radially.
        This shooter computes the velocity vector of incoming particles by choosing a
        random direction and a random speed. Both direction and speed are chosen within
        specified ranges. The direction is defined by two angles: <B>theta</B>, which
        is the angle between the velocity vector and the Z axis, and <B>phi</B>, which is
        the angle between the X axis and the velocity vector projected onto the X-Y plane.
    */
    class OSGPARTICLE_EXPORT RadialShooter: public Shooter {
    public:
        inline RadialShooter();
        inline RadialShooter(const RadialShooter &copy, const osg::CopyOp &copyop = osg::CopyOp::SHALLOW_COPY);

        META_Object(osgParticleHPS, RadialShooter);

        /// Get the range of possible values for <B>theta</B> angle.
        inline const rangef &getThetaRange() const;
        
        /// Set the range of possible values for <B>theta</B> angle.
        inline void setThetaRange(const rangef &r);
        
        /// Set the range of possible values for <B>theta</B> angle.
        inline void setThetaRange(float r1, float r2);
        
        /// Get the range of possible values for <B>phi</B> angle.
        inline const rangef &getPhiRange() const;
        
        /// Set the range of possible values for <B>phi</B> angle.
        inline void setPhiRange(const rangef &r);
        
        /// Set the range of possible values for <B>phi</B> angle.
        inline void setPhiRange(float r1, float r2);
        
        /// Get the range of possible values for initial speed of particles.
        inline const rangef &getInitialSpeedRange() const;        
        
        /// Set the range of possible values for initial speed of particles.
        inline void setInitialSpeedRange(const rangef &r);
        
        /// Set the range of possible values for initial speed of particles.
        inline void setInitialSpeedRange(float r1, float r2);

        /// Get the range of possible values for initial rotational speed of particles.
        inline const rangev3 &getInitialRotationalSpeedRange() const;        
        
        /// Set the range of possible values for initial rotational speed of particles.
        inline void setInitialRotationalSpeedRange(const rangev3 &r);
        
        /// Set the range of possible values for initial rotational speed of particles.
        inline void setInitialRotationalSpeedRange(const osg::Vec3 &r1, const osg::Vec3 &r2);

        /// Shoot a particle. Do not call this method manually.
        inline void shoot(Particle *P) const;

    protected:
        virtual ~RadialShooter() {}
        RadialShooter &operator=(const RadialShooter &) { return *this; }

    private:
        rangef theta_range_;
        rangef phi_range_;
        rangef speed_range_;
        rangev3 rot_speed_range_;
    };

    // INLINE FUNCTIONS

    inline RadialShooter::RadialShooter()
    :    Shooter(), 
        theta_range_(0, 0.5f*osg::PI_4), 
        phi_range_(0, 2*osg::PI), 
        speed_range_(10, 10),
        rot_speed_range_(osg::Vec3(0,0,0), osg::Vec3(0,0,0))
    {
    }

    inline RadialShooter::RadialShooter(const RadialShooter &copy, const osg::CopyOp &copyop)
    :    Shooter(copy, copyop), 
        theta_range_(copy.theta_range_), 
        phi_range_(copy.phi_range_), 
        speed_range_(copy.speed_range_),
        rot_speed_range_(copy.rot_speed_range_)
    {
    }
    
    inline const rangef &RadialShooter::getThetaRange() const
    {
        return theta_range_;
    }

    inline const rangef &RadialShooter::getPhiRange() const
    {
        return phi_range_;
    }

    inline const rangef &RadialShooter::getInitialSpeedRange() const
    {
        return speed_range_;
    }

    inline const rangev3 &RadialShooter::getInitialRotationalSpeedRange() const
    {
        return rot_speed_range_;
    }

    inline void RadialShooter::setThetaRange(const rangef &r)
    {
        theta_range_ = r;
    }

    inline void RadialShooter::setThetaRange(float r1, float r2)
    {
        theta_range_.minimum = r1;
        theta_range_.maximum = r2;
    }

    inline void RadialShooter::setPhiRange(const rangef &r)
    {
        phi_range_ = r;
    }

    inline void RadialShooter::setPhiRange(float r1, float r2)
    {
        phi_range_.minimum = r1;
        phi_range_.maximum = r2;
    }

    inline void RadialShooter::setInitialSpeedRange(const rangef &r)
    {
        speed_range_ = r;
    }

    inline void RadialShooter::setInitialSpeedRange(float r1, float r2)
    {
        speed_range_.minimum = r1;
        speed_range_.maximum = r2;
    }

    inline void RadialShooter::setInitialRotationalSpeedRange(const rangev3 &r)
    {
        rot_speed_range_ = r;
    }

    inline void RadialShooter::setInitialRotationalSpeedRange(const osg::Vec3 &r1, const osg::Vec3 &r2)
    {
        rot_speed_range_.minimum = r1;
        rot_speed_range_.maximum = r2;
    }

    inline void RadialShooter::shoot(Particle *P) const
    {
        float theta = theta_range_.get_random();
        float phi = phi_range_.get_random();
        float speed = speed_range_.get_random();
        osg::Vec3 rot_speed = rot_speed_range_.get_random();
        
        P->setVelocity(osg::Vec3(
            speed * sinf(theta) * cosf(phi),
            speed * sinf(theta) * sinf(phi),
            speed * cosf(theta)
            ));
            
        P->setAngularVelocity(rot_speed);
    }

}


#endif
