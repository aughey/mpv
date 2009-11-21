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

#ifndef OSGPARTICLE_PARTICLEPROCESSOR_
#define OSGPARTICLE_PARTICLEPROCESSOR_ 1

#include "Export.h"
#include "ParticleSystem.h"

#include <osg/ref_ptr>
#include <osg/Object>
#include <osg/Transform>
#include <osg/NodeVisitor>
#include <osg/CopyOp>
#include <osg/Vec3>
#include <osg/Matrix>

namespace osgParticleHPS
{

    /**    A common base interface for those classes which need to do something on particles. Such classes
        are, for example, <CODE>Emitter</CODE> (particle generation) and <CODE>Program</CODE> (particle animation).
        This class holds some properties, like a <I>reference frame</I> and a reference to a <CODE>ParticleSystem<CODE>;
        descendant classes should process the particles taking into account the reference frame, computing the right
        transformations when needed.
    */
    class OSGPARTICLE_EXPORT ParticleProcessor: public osg::Node {
    public:
    
        enum ReferenceFrame {
            RELATIVE_TO_PARENTS,
            RELATIVE_TO_ABSOLUTE
			,
			ABSOLUTE_RF=RELATIVE_TO_ABSOLUTE,
			RELATIVE_RF=RELATIVE_TO_PARENTS
        };
        
        ParticleProcessor();
        ParticleProcessor(const ParticleProcessor &copy, const osg::CopyOp &copyop = osg::CopyOp::SHALLOW_COPY);

        virtual const char *libraryName() const { return "osgParticleHPS"; }
        virtual const char *className() const { return "ParticleProcessor"; }
        virtual bool isSameKindAs(const osg::Object *obj) const { return dynamic_cast<const ParticleProcessor*>(obj) != 0; }
        virtual void accept(osg::NodeVisitor& nv) { if (nv.validNodeMask(*this)) { nv.pushOntoNodePath(this); nv.apply(*this); nv.popFromNodePath(); } }        
        
        /// Get the reference frame.
        inline ReferenceFrame getReferenceFrame() const;
        
        /// Set the reference frame.
        inline void setReferenceFrame(ReferenceFrame rf);
        
        /// Get whether this processor is enabled or not.
        inline bool isEnabled() const;
        
        /// Set whether this processor is enabled or not.
        inline void setEnabled(bool v);
        
        /// Get a pointer to the destination particle system.
        inline ParticleSystem *getParticleSystem();
        
        /// Get a const pointer to the destination particle system.
        inline const ParticleSystem *getParticleSystem() const;
        
        /// Set the destination particle system.
        inline void setParticleSystem(ParticleSystem *ps);
        
        /// Set the endless flag of this processor.
        inline void setEndless(bool type);
        
        /// Check whether this processor is endless.
        inline bool isEndless() const;
        
        /// Set the lifetime of this processor.
        inline void setLifeTime(double t);
        
        /// Get the lifetime of this processor.
        inline double getLifeTime() const;
        
        /// Set the start time of this processor.
        inline void setStartTime(double t);
        
        /// Get the start time of this processor.
        inline double getStartTime() const;

        /// Set the current time of this processor.
        inline void setCurrentTime(double t);
        
        /// Get the current time of this processor.
        inline double getCurrentTime() const;
        
        /// Set the reset time of this processor. A value of 0 disables reset.
        inline void setResetTime(double t);
        
        /// Get the reset time of this processor.
        inline double getResetTime() const;
        
        /**
          Check whether the processor is alive with respect to start time and
          life duration. Note that this method may return true even if the 
          processor has been disabled by calling setEnabled(false). To test
          whether the processor is actually processing particles or not, you
          should evaluate (isEnabled() && isAlive()).
         */
        inline bool isAlive() const;
        
        void traverse(osg::NodeVisitor &nv);

        /// Get the current local-to-world transformation matrix (valid only during cull traversal).
        inline const osg::Matrix &getLocalToWorldMatrix();
        
        /// Get the current world-to-local transformation matrix (valid only during cull traversal).
        inline const osg::Matrix &getWorldToLocalMatrix();
        
        /// Transform a point from local to world coordinates (valid only during cull traversal).
        inline osg::Vec3 transformLocalToWorld(const osg::Vec3 &P);
        
        /// Transform a vector from local to world coordinates, discarding translation (valid only during cull traversal).
        inline osg::Vec3 rotateLocalToWorld(const osg::Vec3 &P);
        
        /// Transform a point from world to local coordinates (valid only during cull traversal).
        inline osg::Vec3 transformWorldToLocal(const osg::Vec3 &P);
        
        /// Transform a vector from world to local coordinates, discarding translation (valid only during cull traversal).
        inline osg::Vec3 rotateWorldToLocal(const osg::Vec3 &P);

    protected:
        virtual ~ParticleProcessor() {}
        ParticleProcessor &operator=(const ParticleProcessor &) { return *this; }
        
        inline osg::BoundingSphere computeBound() const;    
        
        virtual void process(double dt) = 0;
        
    private:
        ReferenceFrame rf_;
        bool enabled_;
        double t0_;
        osg::ref_ptr<ParticleSystem> ps_;
        bool need_ltw_matrix_;
        bool need_wtl_matrix_;
        osg::Matrix ltw_matrix_;
        osg::Matrix wtl_matrix_;
        osg::NodeVisitor *current_nodevisitor_;
        
        bool endless_;
        
        double lifeTime_;
        double startTime_;
        double currentTime_;
        double resetTime_;
    };
    
    // INLINE FUNCTIONS
    
    inline ParticleProcessor::ReferenceFrame ParticleProcessor::getReferenceFrame() const
    {
        return rf_;
    }
    
    inline void ParticleProcessor::setReferenceFrame(ReferenceFrame rf)
    {
        rf_ = rf;
    }
    
    inline bool ParticleProcessor::isEnabled() const
    {
        return enabled_;
    }
    
    inline void ParticleProcessor::setEnabled(bool v)
    {
        enabled_ = v;
        if (enabled_) {
			t0_ = -1;
			currentTime_ = 0;
		}
    }
    
    inline ParticleSystem *ParticleProcessor::getParticleSystem()
    {
        return ps_.get();
    }

    inline const ParticleSystem *ParticleProcessor::getParticleSystem() const
    {
        return ps_.get();
    }
    
    inline void ParticleProcessor::setParticleSystem(ParticleSystem *ps)
    {
        ps_ = ps;
    }
   
    inline void ParticleProcessor::setEndless(bool type)
    {
		endless_ = type;
    }
        
    inline bool ParticleProcessor::isEndless() const
    {
		return endless_;
    }

    inline void ParticleProcessor::setLifeTime(double t)
    {
		lifeTime_ = t;
	}
        
    inline double ParticleProcessor::getLifeTime() const
    {
		return lifeTime_;
	}
	   
    inline void ParticleProcessor::setStartTime(double t)
    {
		startTime_ = t;
	}
        
    inline double ParticleProcessor::getStartTime() const
    {
		return startTime_;
	}
    inline void ParticleProcessor::setCurrentTime(double t)
    {
		currentTime_ = t;
	}
        
    inline double ParticleProcessor::getCurrentTime() const
    {
		return currentTime_;
	}
	
	inline void ParticleProcessor::setResetTime(double t)
	{
		resetTime_ = t;
	}
	
	inline double ParticleProcessor::getResetTime() const
	{
		return resetTime_;
	}

    inline osg::BoundingSphere ParticleProcessor::computeBound() const
    {
        return osg::BoundingSphere();
    }
    
    inline const osg::Matrix &ParticleProcessor::getLocalToWorldMatrix()    
    {
        if (need_ltw_matrix_) {
            ltw_matrix_ = osg::Matrix::identity();
            //current_nodevisitor_->getLocalToWorldMatrix(ltw_matrix_, this);
            ltw_matrix_ = osg::computeLocalToWorld(current_nodevisitor_->getNodePath());
            need_ltw_matrix_ = false;
        }
        return ltw_matrix_;
    }

    inline const osg::Matrix &ParticleProcessor::getWorldToLocalMatrix()    
    {
        if (need_wtl_matrix_) {
            wtl_matrix_ = osg::Matrix::identity();
            //current_nodevisitor_->getWorldToLocalMatrix(wtl_matrix_, this);
            wtl_matrix_ = osg::computeWorldToLocal(current_nodevisitor_->getNodePath());
            need_wtl_matrix_ = false;
        }
        return wtl_matrix_;
    }
    
    inline osg::Vec3 ParticleProcessor::transformLocalToWorld(const osg::Vec3 &P)
    {
        return getLocalToWorldMatrix().preMult(P);
    }
    
    inline osg::Vec3 ParticleProcessor::transformWorldToLocal(const osg::Vec3 &P)
    {
        return getWorldToLocalMatrix().preMult(P);
    }

    inline osg::Vec3 ParticleProcessor::rotateLocalToWorld(const osg::Vec3 &P)
    {
        return getLocalToWorldMatrix().preMult(P) - 
            getLocalToWorldMatrix().preMult(osg::Vec3(0, 0, 0));
    }
    
    inline osg::Vec3 ParticleProcessor::rotateWorldToLocal(const osg::Vec3 &P)
    {
        return getWorldToLocalMatrix().preMult(P) -
            getWorldToLocalMatrix().preMult(osg::Vec3(0, 0, 0));
    }
    
    inline bool ParticleProcessor::isAlive() const
    {
		return currentTime_ < (lifeTime_ + startTime_);
    }

}


#endif
