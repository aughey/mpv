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

#ifndef OSGPARTICLE_FLUIDFRICTIONOPERATOR_
#define OSGPARTICLE_FLUIDFRICTIONOPERATOR_ 1

#include "Export.h"
#include "Operator.h"

#include <osg/CopyOp>
#include <osg/Object>
#include <osg/Math>

namespace osgParticleHPS
{

    class Particle;    

    /**    An operator that simulates the friction of a fluid.
        By using this operator you can let the particles move in a fluid of a given <I>density</I>
        and <I>viscosity</I>. There are two functions to quickly setup the parameters for pure water
        and air. You can decide whether to compute the forces using the particle's physical 
        radius or another value, by calling the <CODE>setOverrideRadius()</CODE> method.
    */
    class OSGPARTICLE_EXPORT FluidFrictionOperator: public Operator {
    public:
        
        FluidFrictionOperator();
        FluidFrictionOperator(const FluidFrictionOperator &copy, const osg::CopyOp &copyop = osg::CopyOp::SHALLOW_COPY);
        
        META_Object(osgParticleHPS, FluidFrictionOperator);
        
        /// Get the density of the fluid.
        inline float getFluidDensity() const;
        
        /// Set the density of the fluid.
        inline void setFluidDensity(float d);
        
        /// Get the viscosity of the fluid.
        inline float getFluidViscosity() const;
        
        /// Set the viscosity of the fluid.
        inline void setFluidViscosity(float v);
        
        /// Get the overriden radius value.
        inline float getOverrideRadius() const;        
        
        /// Set the overriden radius value (pass 0 if you want to use particle's radius).
        inline void setOverrideRadius(float r);
        
        /// Set the fluid parameters as for air (20�C temperature).
        inline void setFluidToAir();
        
        /// Set the fluid parameters as for pure water (20�C temperature).
        inline void setFluidToWater();
        
        /// Apply the friction forces to a particle. Do not call this method manually.
        void operate(Particle *P, double dt);
        
        /// Perform some initializations. Do not call this method manually.
        inline void beginOperate(Program *prg);
        
    protected:
        virtual ~FluidFrictionOperator() {}
        FluidFrictionOperator &operator=(const FluidFrictionOperator &) { return *this; }
        
    private:
        float A_;
        float B_;
        float density_;
        float viscosity_;
        float ovr_rad_;
        Program *current_program_;
    };
    
    // INLINE FUNCTIONS
    
    inline float FluidFrictionOperator::getFluidDensity() const
    {
        return density_;
    }

    inline float FluidFrictionOperator::getFluidViscosity() const
    {
        return viscosity_;
    }
    
    inline void FluidFrictionOperator::setFluidDensity(float d)
    {
        density_ = d;
        B_ = 0.2f * osg::PI * density_;
    }

    inline void FluidFrictionOperator::setFluidViscosity(float v)
    {
        viscosity_ = v;
        A_ = 6 * osg::PI * viscosity_;
    }
    
    inline void FluidFrictionOperator::setFluidToAir()
    {
        setFluidViscosity(1.8e-5f);
        setFluidDensity(1.2929f);
    }

    inline void FluidFrictionOperator::setFluidToWater()
    {
        setFluidViscosity(1.002e-3f);
        setFluidDensity(1.0f);
    }
    
    inline float FluidFrictionOperator::getOverrideRadius() const
    {
        return ovr_rad_;
    }
    
    inline void FluidFrictionOperator::setOverrideRadius(float r)
    {
        ovr_rad_ = r;
    }
    
    inline void FluidFrictionOperator::beginOperate(Program *prg)
    {
        current_program_ = prg;
    }

}


#endif
