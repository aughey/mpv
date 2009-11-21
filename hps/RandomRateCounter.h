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

#ifndef OSGPARTICLE_RANDOMRATECOUNTER_
#define OSGPARTICLE_RANDOMRATECOUNTER_ 1

#include "Export.h"

#include "VariableRateCounter.h"

#include <osg/CopyOp>
#include <osg/Object>

namespace osgParticleHPS
{

    class OSGPARTICLE_EXPORT RandomRateCounter: public VariableRateCounter {
    public:
        inline RandomRateCounter();
        inline RandomRateCounter(const RandomRateCounter &copy, const osg::CopyOp &copyop = osg::CopyOp::SHALLOW_COPY);
        
        META_Object(osgParticleHPS, RandomRateCounter);
    
        /// Return the number of particles to be created in this frame
        inline int numParticlesToCreate(double dt) const;
        
    protected:
        virtual ~RandomRateCounter() {}
        
        mutable float np_;
    };
    
    // INLINE FUNCTIONS
    
    inline RandomRateCounter::RandomRateCounter()
    : VariableRateCounter(), np_(0)
    {
    }
    
    inline RandomRateCounter::RandomRateCounter(const RandomRateCounter &copy, const osg::CopyOp &copyop)
    : VariableRateCounter(copy, copyop), np_(copy.np_)
    {
    }
    
    inline int RandomRateCounter::numParticlesToCreate(double dt) const
    {
        np_ += dt * getRateRange().get_random();
        int n = static_cast<int>(np_);
        np_ -= n;
        return n;
    }    

}


#endif
