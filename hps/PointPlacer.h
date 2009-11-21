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

#ifndef OSGPARTICLE_POINTPLACER_
#define OSGPARTICLE_POINTPLACER_ 1

#include "Export.h"

#include "CenteredPlacer.h"
#include "Particle.h"

#include <osg/CopyOp>
#include <osg/Object>

namespace osgParticleHPS
{

    /**    A point-shaped particle placer.
        This placer class uses the center point defined in its base class <CODE>CenteredPlacer</CODE>
        to place there all incoming particles.
    */
    class OSGPARTICLE_EXPORT PointPlacer: public CenteredPlacer {
    public:
        inline PointPlacer();
        inline PointPlacer(const PointPlacer &copy, const osg::CopyOp &copyop = osg::CopyOp::SHALLOW_COPY);

        META_Object(osgParticleHPS, PointPlacer);
        
        /** Place a particle. 
            This method is called automatically by <CODE>ModularEmitter</CODE> and should not be called
            manually.
        */
        inline void setPosition(Particle *P) const;

    protected:
        virtual ~PointPlacer() {}
        PointPlacer &operator=(const PointPlacer &) { return *this; }        
    };

    // INLINE FUNCTIONS
    
    inline PointPlacer::PointPlacer()
    : CenteredPlacer()
    {
    }
    
    inline PointPlacer::PointPlacer(const PointPlacer &copy, const osg::CopyOp &copyop)
    : CenteredPlacer(copy, copyop)
    {
    }

    inline void PointPlacer::setPosition(Particle *P) const
    {
        P->setPosition(getCenter());
    }


}


#endif
