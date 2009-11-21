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

#ifndef OSGPARTICLE_FORCEOPERATOR_
#define OSGPARTICLE_FORCEOPERATOR_ 1

#include "Export.h"

#include "ModularProgram.h"
#include "Operator.h"
#include "Particle.h"

#include <osg/CopyOp>
#include <osg/Object>
#include <osg/Vec3>

namespace osgParticleHPS
{

    /**    An operator that applies a constant force to the particles.
        Remember that if the mass of particles is expressed in <U>kg</U> and the lengths are
        expressed in <U>meters</U>, then the force should be expressed in <U>Newtons</U>.
    */
    class ForceOperator: public Operator {
    public:
        inline ForceOperator();
        inline ForceOperator(const ForceOperator &copy, const osg::CopyOp &copyop = osg::CopyOp::SHALLOW_COPY);
        
        META_Object(osgParticleHPS, ForceOperator);
        
        /// Get the force vector.
        inline const osg::Vec3 &getForce() const;
        
        /// Set the force vector.
        inline void setForce(const osg::Vec3 &f);
        
        /// Apply the force to a particle. Do not call this method manually.
        inline void operate(Particle *P, double dt);
        
        /// Perform some initialization. Do not call this method manually.
        inline void beginOperate(Program *prg);
        
    protected:
        virtual ~ForceOperator() {};
        ForceOperator &operator=(const ForceOperator &) { return *this; }        
        
    private:
        osg::Vec3 force_;
        osg::Vec3 xf_force_;
    };
    
    // INLINE FUNCTIONS
    
    inline ForceOperator::ForceOperator()
    : Operator(), force_(0, 0, 0)
    {        
    }
    
    inline ForceOperator::ForceOperator(const ForceOperator &copy, const osg::CopyOp &copyop)
    : Operator(copy, copyop), force_(copy.force_)
    {
    }
    
    inline const osg::Vec3 &ForceOperator::getForce() const
    {
        return force_;
    }
    
    inline void ForceOperator::setForce(const osg::Vec3 &v)
    {
        force_ = v;
    }
    
    inline void ForceOperator::operate(Particle *P, double dt)
    {
        P->addVelocity(xf_force_ * (P->getMassInv() * dt));
    }
    
    inline void ForceOperator::beginOperate(Program *prg)
    {
        if (prg->getReferenceFrame() == ModularProgram::RELATIVE_TO_PARENTS) {
            xf_force_ = prg->rotateLocalToWorld(force_);
        } else {
            xf_force_ = force_;
        }
    }
}

#endif
