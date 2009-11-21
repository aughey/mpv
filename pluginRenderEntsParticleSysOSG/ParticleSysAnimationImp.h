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
 *  FILENAME:   ParticleSysAnimationImp.h
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

#ifndef _PARTICLE_SYS_ANIMATION_IMP_H_
#define _PARTICLE_SYS_ANIMATION_IMP_H_

#include <stdio.h>
#include <iostream>

#include <osg/Group>
#include <osg/Geode>

#include "Animation.h"

#include "ParticleProcessor.h"
#include "ParticleSystem.h"

#ifdef WIN32
#define snprintf _snprintf
#endif


//=========================================================
//! This class will control the animation state of a particle system, according 
//! to the state changes of an Animation.
//! 
class ParticleSysAnimationImp : public mpv::AnimationImp
{
public:
	
	//=========================================================
	//! constructor 
	//! 
	ParticleSysAnimationImp( mpv::Animation *anim );

	//=========================================================
	//! Adds a particle processor to this animation class.  Without 
	//! pointers to the particle system's processors, this class will 
	//! not be able to reset them when an animation state change occurs 
	//! (specifically, when transitioning to Stop).
	//! \param proc - the particle processor (emitter, program) to add
	//! 
	void addParticleProcessor( osgParticleHPS::ParticleProcessor *proc )
	{
		particleProcessors.push_back( proc );

		// the animation is "dirty" now, and needs to update its state
		stateChanged( animation );
	}
	
	//=========================================================
	//! Sets forceAnimationStateToContinue
	//! \param force - forceAnimationStateToContinue will be set to this
	//! 
	void setForceAnimationState( bool force )
	{
		forceAnimationStateToContinue = force;
	}
	
	//=========================================================
	//! Called by the parent entity once per frame.
	//! \param deltaT - the time elapsed in seconds since the last call to act()
	//!
	virtual void update( double deltaT );
	
protected:
	
	//=========================================================
	//! destructor
	//! 
	virtual ~ParticleSysAnimationImp();
	
	//=========================================================
	//! Called when the animation's state has changed
	//! 
	void stateChanged( mpv::Animation * );
	
	//=========================================================
	//! A list of the particle processors in the entity.
		// Each particle system has components in two different places in the 
		// scene graph.  The particle-system-updater and the geode are stored in 
		// a world-space group, and the rest of the components are stored in the 
		// model subgraph.  In this callback, we only have access to the components 
		// in the model subgraph.  Thankfully, it seems that these components are 
		// all that is required in order to play/pause/stop the animations...
	//! 
	std::list< osg::ref_ptr<osgParticleHPS::ParticleProcessor> > particleProcessors;
	
	bool forceAnimationStateToContinue;

	int numActiveParticlesLastFrame;
};


#endif
