/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2004 The Boeing Company
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
 *  FILENAME:   HPSFactory.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This file defines the the HPSFactory class, a factory for 
 *  creating particle systems according to the contents of a def file.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  05/28/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 *  
 *  2007-11-xx Andrew Sampson
 *      Removed HPSParticle and HPSParticleSystem.  All the important features 
 *      that existed in those classes was merged into Particle and 
 *      ParticleSystem.  Also cleaned up the def file syntax significantly.
 *  
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#ifndef HPS_FACTORY_H
#define HPS_FACTORY_H

#include <list>
#include <string>

#include "Export.h"

#include "ParticleSystemUpdater.h"
#include "ModularEmitter.h"
#include "ModularProgram.h"
#include "ParticleSystem.h"
#include "MultiEmitter.h"


#include "DefFileGroup.h"
#include "DefFileAttrib.h"


//=========================================================
//! This class is a factory for creating particle systems according to 
//! the contents of a def file.
//! 
class OSGPARTICLE_EXPORT HPSFactory {
public:
	
	//=========================================================
	//! Singleton constructor
	//! 
	static HPSFactory *Instance();

	//=========================================================
	//! General Destructor
	//! 
	~HPSFactory();

	//=========================================================
	//! Creates a particle system according to the info in the 
	//! def file group.
	//! \param group - the def file group to guide the construction of the ps
	//! \param ps - Stores the newly created ps
	//! \param emitters - Stores the list of emitters associated with the new ps.
	//! \param programs - Stores the list of programs associated with the new ps.
	//! 
	void createParticleSys( 
		DefFileGroup *partSysDefinition, 
		osgParticleHPS::ParticleSystem *&ps, 
		std::list<osgParticleHPS::MultiEmitter *> &emitters,
		std::list<osgParticleHPS::ModularProgram *> &programs );
	

protected:

	//=========================================================
	//! General Constructor
	//! 
	HPSFactory();

	
private:
	
	//=========================================================
	//! The singleton instance of this class.
	//! 
	static HPSFactory *instance;
	
	
	//=========================================================
	//! Creates an Emitter according to the contents of a def file group
	//! \param group - The def file group to retrieve parameters from
	//! \return The new Emitter, or NULL if there was a problem
	//! 
	osgParticleHPS::MultiEmitter *handleEmitterGroup( DefFileGroup *group );
	
	//=========================================================
	//! Creates a Program according to the contents of a def file group
	//! \param group - The def file group to retrieve parameters from
	//! \return The new Program, or NULL if there was a problem
	//! 
	osgParticleHPS::ModularProgram *handleBehaviorGroup( DefFileGroup *group );
	
	//=========================================================
	//! Creates a Operator according to the contents of a def file group
	//! \param group - The def file group to retrieve parameters from
	//! \return The new Operator, or NULL if there was a problem
	//! 
	osgParticleHPS::Operator *handleAffectorGroup( DefFileGroup *group );
	
	//=========================================================
	//! Sets parameters common to all ParticleProcessors (Programs and Emitters)
	//! according to the contents of a def file group.
	//! \param proc - the ParticleProcessor to modify
	//! \param group - The def file group to retrieve parameters from
	//! 
	void handleProcessorGroup( 
		osgParticleHPS::ParticleProcessor *proc, DefFileGroup *group );

	
};


#endif
