#include "range.h"

template<> int (*osgParticleHPS::rangef::randFunc)(void) = rand;
template<> int (*osgParticleHPS::rangev2::randFunc)(void) = rand;
template<> int (*osgParticleHPS::rangev3::randFunc)(void) = rand;
template<> int (*osgParticleHPS::rangev4::randFunc)(void) = rand;
template<> int osgParticleHPS::rangef::randMax = RAND_MAX;
template<> int osgParticleHPS::rangev2::randMax = RAND_MAX;
template<> int osgParticleHPS::rangev3::randMax = RAND_MAX;
template<> int osgParticleHPS::rangev4::randMax = RAND_MAX;

namespace osgParticleHPS
{

void rangeSetRandFunc( int (*newRandFunc)(void), int newRandMax )
{
    osgParticleHPS::rangef ::setRandFunc( newRandFunc, newRandMax );
    osgParticleHPS::rangev2::setRandFunc( newRandFunc, newRandMax );
    osgParticleHPS::rangev3::setRandFunc( newRandFunc, newRandMax );
    osgParticleHPS::rangev4::setRandFunc( newRandFunc, newRandMax );
}

}
