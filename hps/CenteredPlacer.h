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

#ifndef OSGPARTICLE_CENTEREDPLACER_
#define OSGPARTICLE_CENTEREDPLACER_ 1

#include "Export.h"

#include "Placer.h"

#include <osg/CopyOp>
#include <osg/Object>
#include <osg/Vec3>

namespace osgParticleHPS
{

    /**    An abstract placer base class for placers which need a <I>center point</I>.
    */
    class OSGPARTICLE_EXPORT CenteredPlacer: public Placer {
    public:
        inline CenteredPlacer();
        inline CenteredPlacer(const CenteredPlacer &copy, const osg::CopyOp &copyop = osg::CopyOp::SHALLOW_COPY);
        
        virtual const char *libraryName() const { return "osgParticleHPS"; }
        virtual const char *className() const { return "CenteredPlacer"; }
        virtual bool isSameKindAs(const osg::Object *obj) const { return dynamic_cast<const Placer *>(obj) != 0; }        
    
        /// Get the center point.
        inline const osg::Vec3 &getCenter() const;
        
        /// Set the center point.
        inline void setCenter(const osg::Vec3 &v);
        
        /// Set the center point.
        inline void setCenter(float x, float y, float z);
        
    protected:
        virtual ~CenteredPlacer() {}
        
    private:
        osg::Vec3 center_;
    };
    
    // INLINE FUNCTIONS
    
    inline CenteredPlacer::CenteredPlacer()
    : Placer(), center_(0, 0, 0)
    {
    }
    
    inline CenteredPlacer::CenteredPlacer(const CenteredPlacer &copy, const osg::CopyOp &copyop)
    : Placer(copy, copyop), center_(copy.center_)
    {
    }
    
    inline const osg::Vec3 &CenteredPlacer::getCenter() const
    {
        return center_;
    }
    
    inline void CenteredPlacer::setCenter(const osg::Vec3 &v)
    {
        center_ = v;
    }
    
    inline void CenteredPlacer::setCenter(float x, float y, float z)
    {
        center_.set(x, y, z);
    }

}


#endif
