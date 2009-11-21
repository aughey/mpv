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

#ifndef OSGPARTICLE_LINEARINTERPOLATOR_
#define OSGPARTICLE_LINEARINTERPOLATOR_

#include "Export.h"

#include "Interpolator.h"

#include <osg/CopyOp>
#include <osg/Object>
#include <osg/Vec3>
#include <osg/Vec4>

namespace osgParticleHPS
{

    /// A linear interpolator.
    class OSGPARTICLE_EXPORT LinearInterpolator: public Interpolator {
    public:
        LinearInterpolator()
        : Interpolator() {}
        
        LinearInterpolator(const LinearInterpolator &copy, const osg::CopyOp &copyop = osg::CopyOp::SHALLOW_COPY)
        : Interpolator(copy, copyop) {}
        
        META_Object(osgParticleHPS, LinearInterpolator);
        
        virtual float interpolate(float t, float y1, float y2) const
        {
            return y1 + (y2 - y1) * t;
        }
        
    protected:
        virtual ~LinearInterpolator() {}
    };

}

#endif
