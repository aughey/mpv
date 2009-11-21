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

#ifndef OSGPARTICLE_RANGE_
#define OSGPARTICLE_RANGE_ 1

// include Export simply to disable Visual Studio silly warnings.
#include "Export.h"

#ifndef __sgi
#include <cstdlib>
#else
#include <stdlib.h>
#endif

#include <osg/Vec2>
#include <osg/Vec3>
#include <osg/Vec4>

//using std::rand;

namespace osgParticleHPS
{

    /**
        A simple struct template useful to store ranges of values as min/max pairs.
        This struct template helps storing min/max ranges for values of any kind; class <CODE>T_</CODE> is
        the type of values to be stored, and it must support operations <CODE>T_ + T_</CODE>, <CODE>T_ - T_</CODE>, 
        and <CODE>T_ * float</CODE>, otherwise the <CODE>get_random()</CODE> method will not compile.
        This struct has been extended to customize the random number generator (by default it uses 
        <CODE>std::rand()</CODE>).
    */
    template<class T_> struct range {
    
        /// Lower bound.
        T_ minimum;
        
        /// Higher bound.
        T_ maximum;

        /// Construct the object by calling default constructors for min and max.
        range() : minimum(T_()), maximum(T_()) {}
        
        /// Construct and initialize min and max directly.
        range(const T_ &mn, const T_ &mx) : minimum(mn), maximum(mx) {}

        /// Set min and max.
        void set(const T_ &mn, const T_ &mx) { minimum = mn; maximum = mx; }

        /// Get a random value between min and max.
        T_ get_random() const
        {
            return minimum + (maximum - minimum) * randFunc() / randMax;
        }

        static void setRandFunc( int (*newRandFunc)(void), int newRandMax ) 
        {
            randFunc = newRandFunc; 
            randMax = newRandMax;
        }

        /// A pointer to the function that will be used for generating random 
        /// numbers.  The default is to use the C library's rand().
        static int (*randFunc)(void);
        
        /// The largest value that could be returned by the randFunc.  The 
        // default is to use the C library's RAND_MAX.
        static int randMax;
    };

    /// Range of floats.
    typedef range<float> rangef;
    
    /// Range of osg::Vec2s.
    typedef range<osg::Vec2> rangev2;
    
    /// Range of osg::Vec3s.
    typedef range<osg::Vec3> rangev3;
    
    /// Range of osg::Vec4s.
    typedef range<osg::Vec4> rangev4;

    /// Sets randFunc and randMax for the 4 range types listed above.
    /// Just a convenience function.
#ifndef WIN32
    void rangeSetRandFunc( int (*newRandFunc)(void), int newRandMax );
#else
    OSGPARTICLE_EXPORT void rangeSetRandFunc( int (*newRandFunc)(void), int newRandMax );
#endif
}

#endif
