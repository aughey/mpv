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

#ifndef OSGPARTICLE_SEGMENTPLACER_
#define OSGPARTICLE_SEGMENTPLACER_ 1

#include "Export.h"

#include "Placer.h"
#include "Particle.h"

#include <osg/CopyOp>
#include <osg/Object>
#include <osg/Vec3>

namespace osgParticleHPS {

    /**    A segment-shaped particle placer.
        To use this placer you have to define a segment, by setting its two vertices (<B>A</B> and <B>B</B>);
        when an emitter requests a <CODE>SegmentPlacer</CODE> to place a particle, the position is chosen randomly
        within that segment.
    */
    class OSGPARTICLE_EXPORT SegmentPlacer: public Placer {
    public:
        inline SegmentPlacer();
        inline SegmentPlacer(const SegmentPlacer &copy, const osg::CopyOp &copyop = osg::CopyOp::SHALLOW_COPY);

        META_Object(osgParticleHPS, SegmentPlacer);

        /// get vertex <B>A</B>.
        inline const osg::Vec3 &getVertexA() const;
        
        /// Set vertex <B>A</B> of the segment as a vector.
        inline void setVertexA(const osg::Vec3 &v);
        
        /// Set vertex <B>A</B> of the segment as three floats.
        inline void setVertexA(float x, float y, float z);
        
        /// get vertex <B>B</B>.
        inline const osg::Vec3 &getVertexB() const;
        
        /// Set vertex <B>B</B> of the segment as a vector.
        inline void setVertexB(const osg::Vec3 &v);
        
        /// Set vertex <B>B</B> of the segment as three floats.
        inline void setVertexB(float x, float y, float z);
        
        /// Set both vertices.
        inline void setSegment(const osg::Vec3 &A, const osg::Vec3 &B);
        
        /// Place a particle. This method is called by <CODE>ModularEmitter</CODE>, do not call it manually.
        inline void setPosition(Particle *P) const;

    protected:
        virtual ~SegmentPlacer() {}
        SegmentPlacer &operator=(const SegmentPlacer &) { return *this; }        

    private:
        osg::Vec3 A_;
        osg::Vec3 B_;
    };

    // INLINE FUNCTIONS
    
    inline SegmentPlacer::SegmentPlacer()
    : Placer(), A_(-1, 0, 0), B_(1, 0, 0)
    {
    }
    
    inline SegmentPlacer::SegmentPlacer(const SegmentPlacer &copy, const osg::CopyOp &copyop)
    : Placer(copy, copyop), A_(copy.A_), B_(copy.B_)
    {
    }

    inline const osg::Vec3 &SegmentPlacer::getVertexA() const
    {
        return A_;
    }

    inline const osg::Vec3 &SegmentPlacer::getVertexB() const
    {
        return B_;
    }

    inline void SegmentPlacer::setSegment(const osg::Vec3 &A, const osg::Vec3 &B)
    {
        A_ = A;
        B_ = B;
    }

    inline void SegmentPlacer::setPosition(Particle *P) const
    {
        P->setPosition(rangev3(A_, B_).get_random());
    }
    
    inline void SegmentPlacer::setVertexA(const osg::Vec3 &v)
    {
        A_ = v;
    }
    
    inline void SegmentPlacer::setVertexA(float x, float y, float z)
    {
        A_.set(x, y, z);
    }
    
    inline void SegmentPlacer::setVertexB(const osg::Vec3 &v)
    {
        B_ = v;
    }

    inline void SegmentPlacer::setVertexB(float x, float y, float z)
    {
        B_.set(x, y, z);
    }    

}

#endif
