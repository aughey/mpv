#include "Emitter.h"
#include "ParticleProcessor.h"

#include <osg/CopyOp>

osgParticleHPS::Emitter::Emitter()
:    ParticleProcessor()
{
}

osgParticleHPS::Emitter::Emitter(const Emitter &copy, const osg::CopyOp &copyop)
:     ParticleProcessor(copy, copyop)
{
}
