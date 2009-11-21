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

#ifndef OSGPARTICLE_MODULAREMITTER_
#define OSGPARTICLE_MODULAREMITTER_ 1

#include "Export.h"
#include "Emitter.h"
#include "Particle.h"
#include "RandomRateCounter.h"
#include "Placer.h"
#include "PointPlacer.h"
#include "Shooter.h"
#include "RadialShooter.h"
#include "ParticleSystem.h"

#include <osg/ref_ptr>
#include <osg/CopyOp>
#include <osg/Object>
#include <osg/Node>
#include <osg/NodeVisitor>

namespace osgParticleHPS
{

    /**    An emitter class that holds three objects to control the creation of particles.
        These objects are a <I>counter</I>, a <I>placer</I> and a <I>shooter</I>.
        The counter controls the number of particles to be emitted at each frame; 
        the placer must initialize the particle's position vector, while the shooter initializes 
        its velocity vector.
        You can use the predefined counter/placer/shooter classes, or you can create your own.
    */
    class OSGPARTICLE_EXPORT ModularEmitter: public Emitter {
    public:
        ModularEmitter();
        ModularEmitter(const ModularEmitter &copy, const osg::CopyOp &copyop = osg::CopyOp::SHALLOW_COPY);
        
        META_Node(osgParticleHPS,ModularEmitter);
        
        /// Get the counter object.
        inline Counter *getCounter();
        
        /// Get the const Counter object.
        inline const Counter *getCounter() const;
        
        /// Set the Counter object.
        inline void setCounter(Counter *c);
        
        /// Get the Placer object.
        inline Placer *getPlacer();
        
        /// Get the const Placer object.        
        inline const Placer *getPlacer() const;
        
        /// Set the Placer object.
        inline void setPlacer(Placer *p);
        
        /// Get the Shooter object.
        inline Shooter *getShooter();
        
        /// Get the const Shooter object.
        inline const Shooter *getShooter() const;
        
        /// Set the Shooter object.
        inline void setShooter(Shooter *s);        

    protected:
        virtual ~ModularEmitter() {}
        ModularEmitter &operator=(const ModularEmitter &) { return *this; }
        
        void emit(double dt);
        
    private:
        osg::ref_ptr<Counter> counter_;
        osg::ref_ptr<Placer> placer_;
        osg::ref_ptr<Shooter> shooter_;
    };
    
    // INLINE FUNCTIONS
    
    inline Counter *ModularEmitter::getCounter()
    {
        return counter_.get();
    }
    
    inline const Counter *ModularEmitter::getCounter() const
    {
        return counter_.get();
    }

    inline void ModularEmitter::setCounter(Counter *c)
    {
        counter_ = c;
    }

    inline Placer *ModularEmitter::getPlacer()
    {
        return placer_.get();
    }

    inline const Placer *ModularEmitter::getPlacer() const
    {
        return placer_.get();
    }

    inline void ModularEmitter::setPlacer(Placer *p)
    {
        placer_ = p;
    }

    inline Shooter *ModularEmitter::getShooter()
    {
        return shooter_.get();
    }

    inline const Shooter *ModularEmitter::getShooter() const
    {
        return shooter_.get();
    }

    inline void ModularEmitter::setShooter(Shooter *s)
    {
        shooter_ = s;
    }
    
}

#endif
