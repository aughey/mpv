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

#ifndef OSGPARTICLE_ANGULARACCELOPERATOR_
#define OSGPARTICLE_ANGULARACCELOPERATOR_ 1

#include "Export.h"

#include "ModularProgram.h"
#include "Operator.h"
#include "Particle.h"

#include <osg/CopyOp>
#include <osg/Object>
#include <osg/Vec3>

namespace osgParticleHPS
{

    /**    An operator class that applies a constant angular acceleration to 
     *     the particles.
     */
    class OSGPARTICLE_EXPORT AngularAccelOperator: public Operator {
    public:
        inline AngularAccelOperator();
        inline AngularAccelOperator(const AngularAccelOperator &copy, const osg::CopyOp &copyop = osg::CopyOp::SHALLOW_COPY);

        META_Object(osgParticleHPS, AngularAccelOperator);

        /// Get the angular acceleration vector.
        inline const osg::Vec3 &getAngularAcceleration() const;
        
        /// Set the angular acceleration vector.
        inline void setAngularAcceleration(const osg::Vec3 &v);
        
        /// Apply the angular acceleration to a particle. Do not call this method manually.
        inline void operate(Particle *P, double dt);
        
        /// Perform some initializations. Do not call this method manually.
        inline void beginOperate(Program *prg);

    protected:
        virtual ~AngularAccelOperator() {}
        AngularAccelOperator &operator=(const AngularAccelOperator &) { return *this; }        

    private:
        osg::Vec3 angular_accel_;
        osg::Vec3 xf_angular_accel_;
    };

    // INLINE FUNCTIONS
    
    inline AngularAccelOperator::AngularAccelOperator()
    : Operator(), angular_accel_(0, 0, 0)
    {
    }
    
    inline AngularAccelOperator::AngularAccelOperator(const AngularAccelOperator &copy, const osg::CopyOp &copyop)
    : Operator(copy, copyop), angular_accel_(copy.angular_accel_)
    {
    }

    inline const osg::Vec3 &AngularAccelOperator::getAngularAcceleration() const
    {
        return angular_accel_;
    }

    inline void AngularAccelOperator::setAngularAcceleration(const osg::Vec3 &v)
    {
        angular_accel_ = v;
    }

    inline void AngularAccelOperator::operate(Particle *P, double dt)
    {
        P->addAngularVelocity(xf_angular_accel_ * dt);
    }
    
    inline void AngularAccelOperator::beginOperate(Program *prg)
    {
        if (prg->getReferenceFrame() == ModularProgram::RELATIVE_TO_PARENTS) {
            xf_angular_accel_ = prg->rotateLocalToWorld(angular_accel_);
        } else {
            xf_angular_accel_ = angular_accel_;
        }
    }

}


#endif
