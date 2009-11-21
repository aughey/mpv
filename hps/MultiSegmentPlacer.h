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

#ifndef OSGPARTICLE_MULTISEGMENTPLACER_
#define OSGPARTICLE_MULTISEGMENTPLACER_ 1

#include "Export.h"
#include "Placer.h"
#include "Particle.h"

#include <vector>
#include <utility>

#include <osg/Notify>
#include <osg/CopyOp>
#include <osg/Object>
#include <osg/Vec3>

namespace osgParticleHPS {

    /** A polyline-shaped particle placer.
        This placer class sets the position of incoming particles by choosing a random point on the
        specified sequence of connected segments.
    */
    class OSGPARTICLE_EXPORT MultiSegmentPlacer: public Placer {
    public:
        MultiSegmentPlacer();
        MultiSegmentPlacer(const MultiSegmentPlacer &copy, const osg::CopyOp &copyop = osg::CopyOp::SHALLOW_COPY);

        META_Object(osgParticleHPS, MultiSegmentPlacer);

        /// Get the number of vertices which define the segments.
        inline int numVertices() const;
        
        /// Get a vertex.
        inline const osg::Vec3 &getVertex(int i) const;
        
        /// Set a vertex.
        inline void setVertex(int i, const osg::Vec3 &v);
        
        /// Set a vertex.
        inline void setVertex(int i, float x, float y, float z);
        
        /// Add a vertex.
        inline void addVertex(const osg::Vec3 &v);
        
        /// Add a vertex.
        inline void addVertex(float x, float y, float z);
        
        /// Remove a vertex.
        inline void removeVertex(int i);
        
        /// Place a particle. Called automatically by <CODE>ModularEmitter</CODE>, do not call this method manually.
        void setPosition(Particle *P) const;

    protected:
        virtual ~MultiSegmentPlacer() {}
        MultiSegmentPlacer &operator=(const MultiSegmentPlacer &) { return *this; }        

    private:
        typedef std::pair<osg::Vec3, float> Vertex_data;
        typedef std::vector<Vertex_data> Vertex_vector;
        
        Vertex_vector vx_;
        float total_length_;
        
        void recompute_length();
    };

    // INLINE FUNCTIONS
    

    inline int MultiSegmentPlacer::numVertices() const
    {
        return static_cast<int>(vx_.size());
    }

    inline const osg::Vec3 &MultiSegmentPlacer::getVertex(int i) const
    {
        return vx_[i].first;
    }

    inline void MultiSegmentPlacer::setVertex(int i, const osg::Vec3 &v)
    {
        vx_[i].first = v;
        recompute_length();
    }

    inline void MultiSegmentPlacer::setVertex(int i, float x, float y, float z)
    {
        vx_[i].first.set(x, y, z);
        recompute_length();
    }

    inline void MultiSegmentPlacer::addVertex(const osg::Vec3 &v)
    {
        float l = 0;        
        if (vx_.size() > 0) {
            l = (v - vx_.back().first).length();
        }                
        total_length_ += l;
        vx_.push_back(std::make_pair(v, total_length_));        
    }

    inline void MultiSegmentPlacer::addVertex(float x, float y, float z)
    {
        addVertex(osg::Vec3(x, y, z));
    }

    inline void MultiSegmentPlacer::removeVertex(int i)
    {
        vx_.erase(vx_.begin()+i);
        recompute_length();
    }

}
#endif
