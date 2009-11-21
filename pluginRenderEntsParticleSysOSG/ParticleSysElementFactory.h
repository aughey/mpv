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
 *  FILENAME:   ParticleSysElementFactory.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  DESCRIPTION: 
 *  This class constructs EntityElements containing particle system nodes
 *  
 *  2007-05-24 Andrew Sampson
 *      Ported to new entity-construction system.  Code is mostly from the old 
 *      EffectConstructor class.
 *  
 */


#ifndef PARTICLESYSELEMENTFACTORY_H
#define PARTICLESYSELEMENTFACTORY_H

#include <string>
#include <map>
#include <list>
#include <osg/Group>
#include "ParticleSystemUpdater.h"

#include "DefFileGroup.h"
#include "DefFileAttrib.h"

#include "EntityElementFactory.h"


class ParticleSysElementFactory : public mpvosg::EntityElementFactory
{
public:
	ParticleSysElementFactory();
	virtual ~ParticleSysElementFactory();
	
	virtual mpvosg::EntityElement *createElement( 
		DefFileGroup *elementDefinition, mpv::Entity *ent );
	
	//=========================================================
	//! Retrieves configuration info from the def file group.  This info 
	//! is used to map typeIDs to groups within the def file tree.  Doing so 
	//! will speed up the lookup time when a new particle system is requested.
	//! \param root - The root of the def file tree
	//! 
	void init( DefFileGroup *root );
	
	//=========================================================
	//! Is used to pass in the "world space" branch.  The "world space" branch 
	//! is a Group which is attached directly to the scene root.  Some of the 
	//! particle system nodes need to be in world space, rather than 
	//! entity-relative-space.
	//! \param wsb - the "world space" branch
	//! 
	void setWorldSpaceBranch( osg::Group *wsb ) { worldSpaceBranch = wsb; }

protected:

	//=========================================================
	//! the "world space" branch; see description for setWorldSpaceBranch
	//! 
	osg::Group *worldSpaceBranch;
	
	//=========================================================
	//! Maps particle system names to the def group which defines the 
	//! corresponding particle system.  Is used to speed up particle system 
	//! creation.
	//! 
	std::map< std::string, DefFileGroup * > partSysNameToDefinitionMap;
	
	//=========================================================
	//! ParticleSystemUpdater - calls update() on all the particle systems.
	//! For this application, one ParticleSystemUpdater services all the 
	//! particle systems in the scene.
	//! 
	osg::ref_ptr<osgParticleHPS::ParticleSystemUpdater> partSysUpdater;
};

#endif
