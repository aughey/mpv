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

#ifndef OSGPARTICLE_SECTORPLACER_
#define OSGPARTICLE_SECTORPLACER_ 1

#include "Export.h"

#include "CenteredPlacer.h"
#include "Particle.h"
#include "range.h"

#include <osg/CopyOp>
#include <osg/Object>
#include <osg/Vec3>
#include <osg/Math>

namespace osgParticleHPS
{

    /**     A sector-shaped particle placer.
        This placer sets the initial position of incoming particle by choosing a random position 
        within a circular sector; this sector is defined by three parameters: a <I>center point</I>, 
        which is inherited directly from <CODE>osgParticle::CenteredPlacer</CODE>, a range of values 
        for <I>radius</I>, and a range of values for the <I>central angle</I> (sometimes called    <B>phi</B>).        
    */
    class OSGPARTICLE_EXPORT SectorPlacer: public CenteredPlacer {
    public:
        inline SectorPlacer();
        inline SectorPlacer(const SectorPlacer &copy, const osg::CopyOp &copyop = osg::CopyOp::SHALLOW_COPY);
        
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

        META_Object(osgParticleHPS, SectorPlacer);
        
        /// Place a particle. Do not call it manually.
        inline void setPosition(Particle *P) const;

    protected:
        virtual ~SectorPlacer() {}
        SectorPlacer &operator=(const SectorPlacer &) { return *this; }        
        
    private:
        rangef rad_range_;
        rangef phi_range_;
    };

    // INLINE FUNCTIONS
    
    inline SectorPlacer::SectorPlacer()
    : CenteredPlacer(), rad_range_(0, 1), phi_range_(0, osg::PI*2)
    {
    }
    
    inline SectorPlacer::SectorPlacer(const SectorPlacer &copy, const osg::CopyOp &copyop)
    : CenteredPlacer(copy, copyop), rad_range_(copy.rad_range_), phi_range_(copy.phi_range_)
    {
    }
    
    inline const rangef &SectorPlacer::getRadiusRange() const
    {
        return rad_range_;
    }

    inline const rangef &SectorPlacer::getPhiRange() const
    {
        return phi_range_;
    }

    inline void SectorPlacer::setRadiusRange(const rangef &r)
    {
        rad_range_ = r;
    }
    
    inline void SectorPlacer::setRadiusRange(float r1, float r2)
    {
        rad_range_.minimum = r1;
        rad_range_.maximum = r2;
    }
    
    inline void SectorPlacer::setPhiRange(const rangef &r)
    {
        phi_range_ = r;
    }
    
    inline void SectorPlacer::setPhiRange(float r1, float r2)
    {
        phi_range_.minimum = r1;
        phi_range_.maximum = r2;
    }

    inline void SectorPlacer::setPosition(Particle *P) const
    {
        float rad = rad_range_.get_random();
        float phi = phi_range_.get_random();
        
        osg::Vec3 pos(
            getCenter().x() + rad * cosf(phi), 
            getCenter().y() + rad * sinf(phi), 
            getCenter().z());
        
        P->setPosition(pos);
    }


}

#endif
