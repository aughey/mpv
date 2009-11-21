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
 *  FILENAME:   ParticleSysElement.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  
 *  
 *  2007-05-24 Andrew Sampson
 *  
 */

#ifndef PARTICLESYSELEMENT_H
#define PARTICLESYSELEMENT_H

#include "EntityElement.h"

//=========================================================
//! 
//! 
class ParticleSysElement : public mpvosg::EntityElement
{
public:
	ParticleSysElement();
	virtual ~ParticleSysElement();
	
	virtual bool construct( DefFileGroup *config, mpv::Entity *entity );
	bool construct( DefFileGroup *partSysDefinition, mpv::Entity *entity, 
		osg::Group *worldSpaceBranch );
	
	virtual osg::Node *getTopNode() { return groupNode.get(); }
	
	virtual bool addChildElement( mpvosg::EntityElement *childElement );
	
protected:
	
	//! Contains entity-space particle system components, like Emitters and 
	//! Programs.  
	//! Attached to the entity subgraph (see getTopNode()...)
	osg::ref_ptr< osg::Group > groupNode;
	
	//! Contains world-space particle system components, like ParticleSystem 
	//! and ParticleSystemUpdater.
	//! Attached to the world-space branch.
	osg::ref_ptr< osg::Group > worldSpaceGroup;
};

#endif
