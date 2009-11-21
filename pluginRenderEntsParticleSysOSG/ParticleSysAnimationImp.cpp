/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2007 The Boeing Company
 *  
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *  
 *  
 *  FILENAME:   ParticleSysAnimationImp.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  update the animation state of a particle system, based on the animation 
 *  state of the entity.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  2007-05-24 Andrew Sampson
 *      Ported to new entity-construction system; code largely derived from 
 *      EffectConstructor.cpp
 *  
 *  2008-08-23 Andrew Sampson
 *      Ported to new animation interface, renamed class to 
 *      ParticleSysAnimationImp.  Split class into a .h and a .cpp.
 *  
 * </pre>
 */

#include "BindSlot.h"

#include "ParticleSysAnimationImp.h"

using namespace mpv;

ParticleSysAnimationImp::ParticleSysAnimationImp( mpv::Animation *anim ) : 
	AnimationImp( anim )
{
	forceAnimationStateToContinue = false;
	numActiveParticlesLastFrame = 0;
	
	stateChanged( animation );
	
	animation->stateChanged.connect( BIND_SLOT1( ParticleSysAnimationImp::stateChanged, this ) );
}

ParticleSysAnimationImp::~ParticleSysAnimationImp()
{
}


void ParticleSysAnimationImp::stateChanged( Animation * )
{
	// This is the animation callback for special effect entity subgraphs.

	Animation::AnimationState animState = animation->getState();

	if( forceAnimationStateToContinue )
	{
		// the "forceAnimationStateToContinue" flag indicates that 
		// the animation states, as passed to the IG by the Host, 
		// should be ignored
		animState = Animation::Continue;
	}

	// update the particle processors and their associated particle systems 
	// according to animState
	std::list< osg::ref_ptr<osgParticleHPS::ParticleProcessor> >::iterator ppIter;
	for( ppIter = particleProcessors.begin(); ppIter != particleProcessors.end(); ppIter++ )
	{
		osgParticleHPS::ParticleProcessor* pp = ppIter->get();
		osgParticleHPS::ParticleSystem* ps = pp->getParticleSystem();
		bool resetPS = false;
		bool freezePS = false;

		// Note that this code might be executed several times for the same 
		// particle system.  For that reason, be careful to avoid 
		// "state oscillation"/flip-flopping.

		if( ps->isFrozen() )
		{
			freezePS = true;
			switch( animState )
			{
			case Animation::Stop :
				resetPS = true;
				break;
			case Animation::Pause :
				break;
			case Animation::Play :
				resetPS = true;
				// fall through to Continue
			case Animation::Continue :
				freezePS = false;
				break;
			default:
				break;
			}

		}
		else
		{
			freezePS = false;
			switch( animState )
			{
			case Animation::Stop :
				resetPS = true;
				// fall through to Pause
			case Animation::Pause :
				freezePS = true;
				break;
			case Animation::Play :
//fixme - after greg gets the ccl straightened out on this issue, uncomment me
//					resetPS = true;
				break;
			case Animation::Continue :
				break;
			default:
				break;
			}
		}

		ps->setFrozen( freezePS );
		if( resetPS ) 
		{
			int numLiveParticles = 
				ps->numParticles() - ps->numDeadParticles();
			if( numLiveParticles > 0 )
			{
				ps->destroyAllParticles();
			}

			pp->setCurrentTime( 0.0 );
		}
	}

}


void ParticleSysAnimationImp::update( double deltaT )
{
	// Need to check how many particles are alive.  If there are no active 
	// particles, and there *were* active particles last frame, then set 
	// isFinished, so that the system sends an 
	// animation-stop-notification.

	int numActiveParticles = 0;
	std::list< osg::ref_ptr<osgParticleHPS::ParticleProcessor> >::iterator ppIter;
	for( ppIter = particleProcessors.begin(); ppIter != particleProcessors.end(); ppIter++ )
	{
		osgParticleHPS::ParticleProcessor* pp = ppIter->get();
		osgParticleHPS::ParticleSystem* ps = pp->getParticleSystem();

		// A given particle system might be encountered multiple times 
		// in this loop, since multiple ParticleProcessors can be 
		// associated with a given ParticleSystem.  While this does 
		// inflate the value of numActiveParticles, it ultimately doesn't 
		// matter, since we just need to know if numActiveParticles is 
		// zero or non-zero.
		numActiveParticles += ps->numParticles() - ps->numDeadParticles();
	}

	if( numActiveParticles == 0 && numActiveParticlesLastFrame > 0 )
	{
		setIsFinished( true );
	}
	else if( numActiveParticles > 0 )
	{
		// hmm... if all the particles go away, and then more are created, 
		// and they *they* go away, then multiple anim-stop-notifications 
		// will have been sent.  I suppose that that is acceptable.
		setIsFinished( false );
	}

	numActiveParticlesLastFrame = numActiveParticles;
}

