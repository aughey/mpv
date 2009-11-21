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
 *  FILENAME:   ParticleSysElementFactory.cpp
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
 */

#include <iostream>

#include "ParticleSysElementFactory.h"
#include "ParticleSysElement.h"

using namespace mpv;
using namespace mpvosg;

// ================================================
// constructor
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ParticleSysElementFactory::ParticleSysElementFactory() : EntityElementFactory()
{
	keyword = "particle_system";
	
	worldSpaceBranch = NULL;
	partSysUpdater = new osgParticleHPS::ParticleSystemUpdater;
}

// ================================================
// destructor
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ParticleSysElementFactory::~ParticleSysElementFactory()
{
}


// ================================================
// init
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ParticleSysElementFactory::init( DefFileGroup *root )
{
	partSysNameToDefinitionMap.clear();

	if( !root )
		return;
	
	DefFileGroup *partSystemsGroup = root->getGroupByURI( "/particle_systems/" );
	if( !partSystemsGroup )
	{
		return;
	}
	
	// For each particle system definition, populate partSysNameToDefinitionMap 
	// with a pointer to the def group.  This will speed up the lookup time 
	// when a new particle system is requested.
	std::list< DefFileGroup * >::iterator gIter;
	for( gIter = partSystemsGroup->children.begin(); 
		gIter != partSystemsGroup->children.end(); gIter++ )
	{
		DefFileGroup *group = (*gIter);
	
		if( group->getName() != "particle_system" )
			continue;

		DefFileAttrib *attr;
		attr = group->getAttribute( "name" );
		if( !attr ) 
		{
			continue;
		}
		
		partSysNameToDefinitionMap[attr->asString()] = group;
	}
	
}


// ================================================
// createElement
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
EntityElement *ParticleSysElementFactory::createElement( 
	DefFileGroup *elementDefinition, Entity *entity )
{
	if( elementDefinition == NULL || entity == NULL ) return NULL;
	
	if( worldSpaceBranch == NULL ) return NULL;
	
	// sanity check
	DefFileAttrib *attr = elementDefinition->getAttribute( "element_type" );
	if( attr == NULL || attr->asString() != keyword ) return NULL;
	
	attr = elementDefinition->getAttribute( "name" );
	if( !attr ) 
	{
		std::cout << "Warning - in definition for entity type " 
				<< entity->getType()
				<< " - particle system element is missing its \"name\" attribute\n";
		return NULL;
	}

	// look for a particle system definition with a matching name
	DefFileGroup *partSysDefinition = partSysNameToDefinitionMap[attr->asString()];

	if( !partSysDefinition )
	{
		std::cout << "Warning - in definition for entity type " 
				<< entity->getType()
				<< " - could not find a definition for particle system \"" 
				<< attr->asString() << "\"\n";
		return NULL;
	}

	ParticleSysElement *result = new ParticleSysElement();
	
	if( !result->construct( partSysDefinition, entity, worldSpaceBranch ) )
	{
		delete result;
		result = NULL;
	}
	
	return result;
}


