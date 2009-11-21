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

#ifndef OSGPARTICLE_ACCELOPERATOR_
#define OSGPARTICLE_ACCELOPERATOR_ 1

#include "Export.h"

#include "ModularProgram.h"
#include "Operator.h"
#include "Particle.h"

#include <osg/CopyOp>
#include <osg/Object>
#include <osg/Vec3>

namespace osgParticleHPS
{

    /**    An operator class that applies a constant acceleration to the particles.
    */
    class OSGPARTICLE_EXPORT AccelOperator: public Operator {
    public:
        inline AccelOperator();
        inline AccelOperator(const AccelOperator &copy, const osg::CopyOp &copyop = osg::CopyOp::SHALLOW_COPY);

        META_Object(osgParticleHPS, AccelOperator);

        /// Get the acceleration vector.
        inline const osg::Vec3 &getAcceleration() const;
        
        /// Set the acceleration vector.
        inline void setAcceleration(const osg::Vec3 &v);
        
        /** Quickly set the acceleration vector to the gravity on earth (0, 0, -9.81).
            The acceleration will be multiplied by the <CODE>scale</CODE> parameter.
        */
        inline void setToGravity(float scale = 1);
        
        /// Apply the acceleration to a particle. Do not call this method manually.
        inline void operate(Particle *P, double dt);
        
        /// Perform some initializations. Do not call this method manually.
        inline void beginOperate(Program *prg);

    protected:
        virtual ~AccelOperator() {}
        AccelOperator &operator=(const AccelOperator &) { return *this; }        

    private:
        osg::Vec3 accel_;
        osg::Vec3 xf_accel_;
    };

    // INLINE FUNCTIONS
    
    inline AccelOperator::AccelOperator()
    : Operator(), accel_(0, 0, 0)
    {
    }
    
    inline AccelOperator::AccelOperator(const AccelOperator &copy, const osg::CopyOp &copyop)
    : Operator(copy, copyop), accel_(copy.accel_)
    {
    }

    inline const osg::Vec3 &AccelOperator::getAcceleration() const
    {
        return accel_;
    }

    inline void AccelOperator::setAcceleration(const osg::Vec3 &v)
    {
        accel_ = v;
    }
    
    inline void AccelOperator::setToGravity(float scale)
    {
        accel_.set(0, 0, -9.80665f * scale);
    }

    inline void AccelOperator::operate(Particle *P, double dt)
    {
        P->addVelocity(xf_accel_ * dt);
    }
    
    inline void AccelOperator::beginOperate(Program *prg)
    {
        if (prg->getReferenceFrame() == ModularProgram::RELATIVE_TO_PARENTS) {
            xf_accel_ = prg->rotateLocalToWorld(accel_);
        } else {
            xf_accel_ = accel_;
        }
    }

}


#endif
