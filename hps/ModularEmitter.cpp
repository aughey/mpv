#include "ModularEmitter.h"
#include "Emitter.h"

osgParticleHPS::ModularEmitter::ModularEmitter()
:    Emitter(),
    counter_(new RandomRateCounter),
    placer_(new PointPlacer),
    shooter_(new RadialShooter)
{
}
    
osgParticleHPS::ModularEmitter::ModularEmitter(const ModularEmitter &copy, const osg::CopyOp &copyop)
:    Emitter(copy, copyop),
    counter_(static_cast<Counter *>(copyop(copy.counter_.get()))), 
    placer_(static_cast<Placer *>(copyop(copy.placer_.get()))), 
    shooter_(static_cast<Shooter *>(copyop(copy.shooter_.get())))
{
}

void osgParticleHPS::ModularEmitter::emit(double dt) 
{
    int n = counter_->numParticlesToCreate(dt);
    for (int i=0; i<n; ++i) {
        Particle *P = getParticleSystem()->createParticle();
        if (P) {
            placer_->place(P);
            shooter_->shoot(P);
            if (getReferenceFrame() == RELATIVE_TO_PARENTS) {
                P->transformPositionVelocity(getLocalToWorldMatrix());
            }
        }
    }
}
