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

#ifndef OSGPARTICLE_VARIABLERATECOUNTER_
#define OSGPARTICLE_VARIABLERATECOUNTER_ 1

#include "Export.h"

#include "Counter.h"
#include "range.h"

#include <osg/CopyOp>
#include <osg/Object>

namespace osgParticleHPS
{

    class OSGPARTICLE_EXPORT VariableRateCounter: public Counter {
    public:
        inline VariableRateCounter();
        inline VariableRateCounter(const VariableRateCounter &copy, const osg::CopyOp &copyop = osg::CopyOp::SHALLOW_COPY);
        
        virtual const char *libraryName() const { return "osgParticleHPS"; }
        virtual const char *className() const { return "VariableRateCounter"; }
        virtual bool isSameKindAs(const osg::Object *obj) const { return dynamic_cast<const VariableRateCounter *>(obj) != 0; }
        
        inline const rangef &getRateRange() const;
        inline void setRateRange(const rangef &r);
        inline void setRateRange(float minrange, float maxrange);
        
    protected:
        virtual ~VariableRateCounter() {}
        
    private:
        rangef rate_range_;
    };
    
    // INLINE FUNCTIONS
    
    inline VariableRateCounter::VariableRateCounter()
    : Counter(), rate_range_(1, 1)
    {
    }
    
    inline VariableRateCounter::VariableRateCounter(const VariableRateCounter &copy, const osg::CopyOp &copyop)
    : Counter(copy, copyop), rate_range_(copy.rate_range_)
    {
    }
    
    inline const rangef &VariableRateCounter::getRateRange() const
    {
        return rate_range_;
    }
    
    inline void VariableRateCounter::setRateRange(const rangef &r)
    {
        rate_range_ = r;
    }

    inline void VariableRateCounter::setRateRange(float minrange, float maxrange)
    {
        rate_range_.set(minrange, maxrange);
    }

}


#endif
