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

#ifndef OSGPARTICLE_EMITTER_
#define OSGPARTICLE_EMITTER_ 1

#include "Export.h"
#include "ParticleProcessor.h"
#include "Particle.h"

#include <osg/Object>
#include <osg/Node>
#include <osg/NodeVisitor>
#include <osg/CopyOp>

namespace osgParticleHPS
{

    /**    An abstract base class for particle emitters.
        Descendant classes must override the <CODE>emit()</CODE> method to generate new particles by
        calling the <CODE>ParticleSystem::createParticle()</CODE> method on the particle system associated
        to the emitter.
    */
    class OSGPARTICLE_EXPORT Emitter: public ParticleProcessor {
    public:
        Emitter();
        Emitter(const Emitter &copy, const osg::CopyOp &copyop = osg::CopyOp::SHALLOW_COPY);

        virtual const char *libraryName() const { return "osgParticleHPS"; }
        virtual const char *className() const { return "Emitter"; }
        virtual bool isSameKindAs(const osg::Object *obj) const { return dynamic_cast<const Emitter*>(obj) != 0; }
        virtual void accept(osg::NodeVisitor& nv) { if (nv.validNodeMask(*this)) { nv.pushOntoNodePath(this); nv.apply(*this); nv.popFromNodePath(); } }
        
    protected:
        virtual ~Emitter() {}
        Emitter &operator=(const Emitter &) { return *this; }
        
        inline void process(double dt);
        
        virtual void emit(double dt) = 0;
        
    private:        
    };

    // INLINE FUNCTIONS

    inline void Emitter::process(double dt)
    {
        emit(dt);
    }


}


#endif

