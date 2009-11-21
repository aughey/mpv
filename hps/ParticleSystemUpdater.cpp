#include "ParticleSystemUpdater.h"

#include <osg/CopyOp>
#include <osg/Node>

using namespace osg;

osgParticleHPS::ParticleSystemUpdater::ParticleSystemUpdater()
: osg::Node(), t0_(-1)
{
    setCullingActive(false);
}

osgParticleHPS::ParticleSystemUpdater::ParticleSystemUpdater(const ParticleSystemUpdater &copy, const osg::CopyOp &copyop)
: osg::Node(copy, copyop), t0_(copy.t0_)
{
    ParticleSystem_Vector::const_iterator i;
    for (i=copy.psv_.begin(); i!=copy.psv_.end(); ++i) {
        psv_.push_back(static_cast<ParticleSystem *>(copyop(i->get())));
    }
}

void osgParticleHPS::ParticleSystemUpdater::traverse(osg::NodeVisitor &nv)
{
    osgUtil::CullVisitor *cv = dynamic_cast<osgUtil::CullVisitor *>(&nv);
    if (cv) {
        if (nv.getFrameStamp())
        {
            double t = nv.getFrameStamp()->getReferenceTime();
            if (t0_ != -1)
            {
                ParticleSystem_Vector::iterator i;
                for (i=psv_.begin(); i!=psv_.end(); ++i)
                {
                    if (!i->get()->isFrozen() && (i->get()->getLastFrameNumber() >= (nv.getFrameStamp()->getFrameNumber() - 1) || !i->get()->getFreezeOnCull()))
                    {
                        i->get()->update(t - t0_);
                    }
                }
            }
            t0_ = t;
        }
        else
        {
            osg::notify(osg::WARN) << "osgParticleHPS::ParticleSystemUpdater::traverse(NodeVisitor&) requires a valid FrameStamp to function, particles not updated.\n";
        }

    }
    Node::traverse(nv);
}
