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

#ifndef OSGPARTICLE_PARTICLESYSTEMUPDATER_
#define OSGPARTICLE_PARTICLESYSTEMUPDATER_ 1

#include "Export.h"
#include "ParticleSystem.h"

#include <vector>

#include <osg/ref_ptr>
#include <osg/CopyOp>
#include <osg/Object>
#include <osg/Node>
#include <osg/NodeVisitor>

#include <osgUtil/CullVisitor>

namespace osgParticleHPS
{

    /**    A useful node class for updating particle systems automatically.
        When a <CODE>ParticleSystemUpdater</CODE> is traversed by a cull visitor, it calls the
        <CODE>update()</CODE> method on the specified particle systems. You should place this updater
        <U>AFTER</U> other nodes like emitters and programs.
    */
    class OSGPARTICLE_EXPORT ParticleSystemUpdater: public osg::Node {
    public:
        ParticleSystemUpdater();
        ParticleSystemUpdater(const ParticleSystemUpdater &copy, const osg::CopyOp &copyop = osg::CopyOp::SHALLOW_COPY);
        
        META_Node(osgParticleHPS,ParticleSystemUpdater);
        
        /// Add a particle system to the list.
        inline bool addParticleSystem(ParticleSystem *ps);

        /// Remove a particle system from the list (by pointer).
        inline bool removeParticleSystem(ParticleSystem *ps);

        /// Remove a particle system(s) from the list (by index).
        inline bool removeParticleSystem(unsigned int i, unsigned int numParticleSystemsToRemove=1);
        
        /// Replace ParticleSystem with another ParticleSystem.
        inline bool replaceParticleSystem(ParticleSystem *origPS, ParticleSystem *newPS);

        /// set a particle system by index.
        inline bool setParticleSystem( unsigned int i, ParticleSystem* ps );

        /// Return the number of particle systems on the list.
        inline unsigned int getNumParticleSystems() const;

        /// Get a particle system from the list.
        inline ParticleSystem* getParticleSystem(unsigned int i);
        
        /// Get a particle system from the list.
        inline const ParticleSystem* getParticleSystem(unsigned int i) const;

        /// return true if ParticleSystem is contained within ParticlsSystemUpdater.
        inline bool containsParticleSystem( const ParticleSystem* ps ) const;

        /// get index number of ParticleSystem.
        inline unsigned int getParticleSystemIndex( const ParticleSystem* ps ) const;
        
        virtual void traverse(osg::NodeVisitor &nv);
        
    protected:
        virtual ~ParticleSystemUpdater() {}
        ParticleSystemUpdater &operator=(const ParticleSystemUpdater &) { return *this; }
        
        inline virtual osg::BoundingSphere computeBound() const;        
        
    private:
        typedef std::vector<osg::ref_ptr<ParticleSystem> > ParticleSystem_Vector;
        
        ParticleSystem_Vector psv_;
        double t0_;
    };
    
    // INLINE FUNCTIONS
    
    inline osg::BoundingSphere ParticleSystemUpdater::computeBound() const
    {
        return osg::BoundingSphere();
    }
    
    inline bool ParticleSystemUpdater::addParticleSystem(ParticleSystem *ps)
    {
        psv_.push_back(ps);
        return true;
    }
    
    inline bool ParticleSystemUpdater::removeParticleSystem(ParticleSystem *ps)
    {
       unsigned int i = getParticleSystemIndex( ps );
       if( i >= psv_.size() ) return false;
       removeParticleSystem( i );
        return true;
    }

    inline bool ParticleSystemUpdater::removeParticleSystem(unsigned int pos, unsigned int numParticleSystemsToRemove)
    {
       if( (pos < psv_.size()) && (numParticleSystemsToRemove > 0) )
       {
          unsigned int endOfRemoveRange = pos + numParticleSystemsToRemove;
          if( endOfRemoveRange > psv_.size() )
          {
             osg::notify(osg::DEBUG_INFO)<<"Warning: ParticleSystem::removeParticleSystem(i,numParticleSystemsToRemove) has been passed an excessive number"<<std::endl;
             osg::notify(osg::DEBUG_INFO)<<"         of ParticleSystems to remove, trimming just to end of ParticleSystem list."<<std::endl;
             endOfRemoveRange = psv_.size();
          }
          psv_.erase(psv_.begin()+pos, psv_.begin()+endOfRemoveRange);
          return true;
       }
       return false;
    }

    inline bool ParticleSystemUpdater::replaceParticleSystem( ParticleSystem *origPS, ParticleSystem* newPS )
    {
       if( (newPS == NULL) || (origPS == newPS) ) return false;

       unsigned int pos = getParticleSystemIndex( origPS );
       if( pos < psv_.size() )
       {
          return setParticleSystem( pos, newPS );
       }
       return false;
    }

    inline bool ParticleSystemUpdater::setParticleSystem( unsigned int i, ParticleSystem* ps )
    {
       if( (i < psv_.size()) && ps )
       {
          psv_[i] = ps;
          return true;
       }
       return false;
    }

    inline unsigned int ParticleSystemUpdater::getNumParticleSystems() const
    {
        return static_cast<int>(psv_.size());
    }
    
    inline ParticleSystem* ParticleSystemUpdater::getParticleSystem(unsigned int i)
    {
        return psv_[i].get();
    }

    inline const ParticleSystem* ParticleSystemUpdater::getParticleSystem(unsigned int i) const
    {
        return psv_[i].get();
    }

    inline bool ParticleSystemUpdater::containsParticleSystem( const ParticleSystem* ps ) const
    {
       for( ParticleSystem_Vector::const_iterator itr=psv_.begin();
            itr!=psv_.end();
            ++itr )
       {
          if( itr->get() == ps ) return true;
       }
       return false;
    }

    inline unsigned int ParticleSystemUpdater::getParticleSystemIndex( const ParticleSystem* ps ) const
    {
       for( unsigned int particleSystemNum=0; particleSystemNum<psv_.size(); ++particleSystemNum )
       {
          if( psv_[particleSystemNum] == ps ) return particleSystemNum;
       }
       return psv_.size(); // node not found.
    }

}

#endif
