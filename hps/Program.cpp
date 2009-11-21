#include "Program.h"
#include "ParticleProcessor.h"

#include <osg/CopyOp>

osgParticleHPS::Program::Program()
:    ParticleProcessor()
{
}

osgParticleHPS::Program::Program(const Program &copy, const osg::CopyOp &copyop)
:    ParticleProcessor(copy, copyop)
{
}
