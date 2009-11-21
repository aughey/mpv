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

#ifndef OSGPARTICLE_PARTICLESYSTEM_
#define OSGPARTICLE_PARTICLESYSTEM_ 1

#include "Export.h"
#include "Particle.h"

#include <vector>
#include <stack>
#include <algorithm>
#include <string>

#include <osg/Object>
#include <osg/Drawable>
#include <osg/CopyOp>
#include <osg/State>
#include <osg/Vec3>
#include <osg/BoundingBox>

namespace osgParticleHPS
{

    /**    The heart of this class library; its purpose is to hold a set of particles and manage particle creation, update, rendering and destruction.
        You can add this drawable to any <CODE>Geode</CODE> as you usually do with other
        <CODE>Drawable</CODE> classes. Each instance of <CODE>ParticleSystem</CODE> is a separate set of
        particles; it provides the interface for creating particles and iterating
        through them (see the <CODE>Emitter</CODE> and <CODE>Program</CODE> classes).
    */
    class OSGPARTICLE_EXPORT ParticleSystem: public osg::Drawable {
    public:
    
        enum Alignment {
            BILLBOARD,
            FIXED
        };

        ParticleSystem();
        ParticleSystem(const ParticleSystem &copy, const osg::CopyOp &copyop = osg::CopyOp::SHALLOW_COPY);

        META_Object(osgParticleHPS, ParticleSystem);
        
        /// Get the alignment type of particles.
        inline Alignment getParticleAlignment() const;
        
        /// Set the alignment type of particles.
        inline void setParticleAlignment(Alignment a);
        
        /// Get the X-axis alignment vector.
        inline const osg::Vec3 &getAlignVectorX() const;
        
        /// Set the X-axis alignment vector.
        inline void setAlignVectorX(const osg::Vec3 &v);

        /// Get the Y-axis alignment vector.
        inline const osg::Vec3 &getAlignVectorY() const;
        
        /// Set the Y-axis alignment vector.
        inline void setAlignVectorY(const osg::Vec3 &v);
        
        /// Set the alignment vectors.
        inline void setAlignVectors(const osg::Vec3 &X, const osg::Vec3 &Y);

        /// Get the default bounding box
        inline const osg::BoundingBox &getDefaultBoundingBox() const;        
        
        /**    Set the default bounding box.
            The default bounding box is used when a real bounding box cannot be computed, for example
            because no particles has been updated yet.
        */
        inline void setDefaultBoundingBox(const osg::BoundingBox &bbox);

        /// Get the double pass rendering flag.
        inline bool getDoublePassRendering() const;
        
        /** Set the double pass rendering flag.
            Double pass rendering avoids overdraw problems between particle systems
            and other opaque objects. If you can render all the particle systems <U>after</U>
            the opaque objects, then double pass is not necessary and can be turned off (best choice).
            If you set the default attributes with <CODE>setDefaultAttributes</CODE>, then the particle
            system will fall into a transparent bin.
        */
        inline void setDoublePassRendering(bool v);
        
        /// Return true if the particle system is frozen.
        inline bool isFrozen() const;
        
        /**    Set or reset the <I>frozen</I> state.
            When the particle system is frozen, emitters and programs won't do anything on it.
        */
        inline void setFrozen(bool v);

		//=========================================================
		//! Returns the total number of particles, including dead particles
		//! \return the number of particles
		//! 
        inline virtual int numParticles() const;
        
		//=========================================================
		//! Returns the number of dead particles
		//! \return the number of dead particles
		//! 
        inline virtual int numDeadParticles() const;
        
		//=========================================================
		//! Returns the particle in position i
		//! \param i - the index of the particle to get
		//! \return the particle, or NULL
		//! 
        inline virtual Particle *getParticle(int i);
        
		//=========================================================
		//! Returns the particle in position i
		//! \param i - the index of the particle to get
		//! \return the particle, or NULL
		//! 
        inline virtual const Particle *getParticle(int i) const;
        
		//=========================================================
		//! Creates a new particle.
		//! \return the new particle
		//! 
        virtual Particle *createParticle();
        
		//=========================================================
		//! Destroys the particle in position i (particle is tagged as dead)
		//! \param i - the index of the particle to destroy
		//! 
        inline virtual void destroyParticle(int i);
        
		//=========================================================
		//! Destroys all the particles in the ps (particles are tagged as dead)
		//! 
		inline virtual void destroyAllParticles();

        /// Get the last frame number.
        inline int getLastFrameNumber() const;

		//=========================================================
		//! Returns a pointer to the template particle
		//! \return pointer to the template particle
		//! 
        inline virtual const Particle *getParticleTemplate() const;
        
		//=========================================================
		//! Sets the template particle
		//! \param p - the new template particle
		//! 
        inline virtual void setParticleTemplate(const Particle *p);
        
        /// Get whether the particle system can freeze when culled
        inline bool getFreezeOnCull() const;
        
        /// Set whether the particle system can freeze when culled (default is true)
        inline void setFreezeOnCull(bool v);
        
		//=========================================================
		//! Sets up some defaults for the particle system's stateset
		//! \param texturefile - the name of the file to load and apply to the 
		//!      particles.  Leave blank to indicate that the ps should not set 
		//!      a ps-wide texture.
		//! \param emissive_particles - turns on/off additive blending 
		//! \param lighting - turns on/off lighting
		//! \param texture_unit - texture unit to use; leave it alone unless you're doing multitexturing
		//! 
        virtual void setDefaultAttributes(const std::string &texturefile = "", bool emissive_particles = true, bool lighting = false, int texture_unit = 0);
        
        /// (<B>EXPERIMENTAL</B>) Get the level of detail.
        inline int getLevelOfDetail() const;
        
        /** (<B>EXPERIMENTAL</B>) Set the level of detail. The total number of particles is divided by the detail value to
            get the actual number of particles to be drawn. This value must be greater than zero.
        */
        inline void setLevelOfDetail(int v);

		//=========================================================
		//! Calls update on all of the particles; puts dead particles on the 
		//! dead-particles-list.  Don't call this directly, use a 
		//! <CODE>ParticleSystemUpdater</CODE> instead.
		//! \param dt - the time in seconds elapsed since the last frame
		//! 
        virtual void update(double dt);

        virtual void drawImplementation(osg::RenderInfo &renderInfo) const;

		//=========================================================
		//! If false, culling of this particle system is discouraged.  This 
		//! function should be queried when the particle system is added to the 
		//! scene.
		//! 
		bool getPermitCulling() { return permitCulling_; }

		//=========================================================
		//! If permitCulling_ is set to false, culling of this particle system is 
		//! discouraged.  
		//! \param permitCulling - the new value for permitCulling_
		//! 
		void setPermitCulling( bool permitCulling ) { permitCulling_ = permitCulling; }


    protected:

        virtual ~ParticleSystem();

        ParticleSystem &operator=(const ParticleSystem &) { return *this; }

        inline virtual osg::BoundingBox computeBound() const;
        inline void update_bounds(const osg::Vec3 &p, const osg::Vec3 &r);
        virtual void single_pass_render(osg::State &state, const osg::Matrix &modelview) const;
		
		inline void deleteAllParticles();

    private:
        typedef std::vector<Particle*> Particle_vector;
        typedef std::stack<Particle*> Death_stack;

		//=========================================================
		//! the list of particles, including dead ones
		//! 
        Particle_vector particles_;

		//=========================================================
		//! the stack of dead particles
		//! 
        Death_stack deadparts_;
        
        osg::BoundingBox def_bbox_;
        
        Alignment alignment_;
        osg::Vec3 align_X_axis_;
        osg::Vec3 align_Y_axis_;

        bool doublepass_;
        bool frozen_;

        osg::Vec3 bmin_;
        osg::Vec3 bmax_;

        bool reset_bounds_flag_;
        bool bounds_computed_;

		//=========================================================
		//! the template particle; this is used when creating new particles
		//! 
		Particle *ptemplate_;

        mutable int last_frame_;
        bool freeze_on_cull_;
        
        mutable int draw_count_;
        
		//=========================================================
		//! If false, culling of this particle system is discouraged.
		//! 
		bool permitCulling_;

    };
    
    // INLINE FUNCTIONS
    
    inline ParticleSystem::Alignment ParticleSystem::getParticleAlignment() const
    {
        return alignment_;
    }
    
    inline void ParticleSystem::setParticleAlignment(Alignment a)
    {
        alignment_ = a;
    }
    
    inline const osg::Vec3 &ParticleSystem::getAlignVectorX() const
    {
        return align_X_axis_;
    }
    
    inline void ParticleSystem::setAlignVectorX(const osg::Vec3 &v)
    {
        align_X_axis_ = v;
    }

    inline const osg::Vec3 &ParticleSystem::getAlignVectorY() const
    {
        return align_Y_axis_;
    }
    
    inline void ParticleSystem::setAlignVectorY(const osg::Vec3 &v)
    {
        align_Y_axis_ = v;
    }
    
    inline void ParticleSystem::setAlignVectors(const osg::Vec3 &X, const osg::Vec3 &Y)
    {
        align_X_axis_ = X;
        align_Y_axis_ = Y;
    }

    inline bool ParticleSystem::isFrozen() const
    {
        return frozen_;
    }
    
    inline void ParticleSystem::setFrozen(bool v)
    {
        frozen_ = v;
    }
    
    inline const osg::BoundingBox &ParticleSystem::getDefaultBoundingBox() const
    {
        return def_bbox_;
    }
    
    inline void ParticleSystem::setDefaultBoundingBox(const osg::BoundingBox &bbox)
    {
        def_bbox_ = bbox;
    }

    inline bool ParticleSystem::getDoublePassRendering() const
    {
        return doublepass_;
    }

    inline void ParticleSystem::setDoublePassRendering(bool v)
    {
        doublepass_ = v;
    }

    inline int ParticleSystem::numParticles() const
    {
        return static_cast<int>(particles_.size());
    }

    inline int ParticleSystem::numDeadParticles() const
    {
        return static_cast<int>(deadparts_.size());
    }

    inline Particle *ParticleSystem::getParticle(int i)
    {
        return particles_[i];
    }

    inline const Particle *ParticleSystem::getParticle(int i) const
    {
        return particles_[i];
    }

    inline void ParticleSystem::destroyParticle(int i)
    {
        particles_[i]->kill();
    }
    
	// ================================================
	// destroyAllParticles
	// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	inline void ParticleSystem::destroyAllParticles()
	{
		int i, size = particles_.size();
		for( i = 0; i < size; i++ )
		{
			particles_[i]->kill();
		}
		/* We can not wait until the next time update is called by our 
		ParticleSystemUpdater, because we might get frozen before then 
		(ParticleSystemUpdater will not call our update if we are frozen).
		We need to call update on ourselves manually. */
		update( 1./60. );
	}

    inline int ParticleSystem::getLastFrameNumber() const
    {
        return last_frame_;
    }

    inline osg::BoundingBox ParticleSystem::computeBound() const
    {        
        if (!bounds_computed_) {
            return(def_bbox_);
        } else {
	    return(osg::BoundingBox(bmin_,bmax_));
        }
    }

    inline void ParticleSystem::update_bounds(const osg::Vec3 &p, const osg::Vec3 &r)
    {
        if (reset_bounds_flag_) {
            reset_bounds_flag_ = false;
            bmin_ = p;
            bmax_ = p;
        } else {
			float maxSizeDimension = std::max<float>( r[0], r[1] );
			maxSizeDimension = std::max<float>( maxSizeDimension, r[2] );
			
            if (p.x() - maxSizeDimension < bmin_.x()) bmin_.x() = p.x() - maxSizeDimension;
            if (p.y() - maxSizeDimension < bmin_.y()) bmin_.y() = p.y() - maxSizeDimension;
            if (p.z() - maxSizeDimension < bmin_.z()) bmin_.z() = p.z() - maxSizeDimension;
            if (p.x() + maxSizeDimension > bmax_.x()) bmax_.x() = p.x() + maxSizeDimension;
            if (p.y() + maxSizeDimension > bmax_.y()) bmax_.y() = p.y() + maxSizeDimension;
            if (p.z() + maxSizeDimension > bmax_.z()) bmax_.z() = p.z() + maxSizeDimension;
            
            if (!bounds_computed_) 
                bounds_computed_ = true;
        }
    }

    inline const Particle *ParticleSystem::getParticleTemplate() const
    {
        return ptemplate_;
    }

    inline void ParticleSystem::setParticleTemplate(const Particle *p)
    {
		// delete all existing particles, to enforce that all particles 
		// in this particle system are of homogenous type
		deleteAllParticles();
		
		delete ptemplate_;
        ptemplate_ = p->copy();
    }
    
    inline bool ParticleSystem::getFreezeOnCull() const
    {
        return freeze_on_cull_;
    }
    
    inline void ParticleSystem::setFreezeOnCull(bool v)
    {
        freeze_on_cull_ = v;
    }
    
	inline void ParticleSystem::deleteAllParticles()
	{
		Particle_vector::iterator i;
		Particle_vector::iterator end = particles_.end();
		// free particles
		for( i = particles_.begin(); i != end; ++i )
		{
			delete (*i);
		}
		
		particles_.clear();
		
		// std::stack has no clear().  The STL is asinine.  C++ sucks.  -ADS
		//deadparts_.clear();
		while( !deadparts_.empty() )
			deadparts_.pop();
	}

}

#endif
