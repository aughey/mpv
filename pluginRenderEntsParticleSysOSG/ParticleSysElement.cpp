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
 *  FILENAME:   ParticleSysElement.cpp
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

#include <iostream>
#include "HPSFactory.h"

#include "ParticleSysElement.h"

#include "ParticleSysAnimationImp.h"

using namespace mpv;
using namespace mpvosg;

ParticleSysElement::ParticleSysElement() : EntityElement()
{
	groupNode = new osg::Group;
}


ParticleSysElement::~ParticleSysElement()
{
	// remove worldSpaceGroup from its parent(s)
	if( worldSpaceGroup.valid() )
	{
		while( worldSpaceGroup->getNumParents() > 0 )
		{
			osg::Group *parent = worldSpaceGroup->getParent( 0 );
			parent->removeChild( worldSpaceGroup.get() );
		}
	}
}


bool ParticleSysElement::construct( DefFileGroup *config, Entity *entity )
{
	std::cerr << "Error - you're not supposed to call this version of construct()\n";

	return false;
}


bool ParticleSysElement::construct( 
	DefFileGroup *partSysDefinition, Entity *entity, 
	osg::Group *worldSpaceBranch )
{
	osgParticleHPS::ParticleSystem *hps = NULL;
	std::list<osgParticleHPS::MultiEmitter *> emitters;
	std::list<osgParticleHPS::ModularProgram *> programs;
	HPSFactory::Instance()->createParticleSys( 
		partSysDefinition, hps, emitters, programs );

	if( hps == NULL )
		return false;
	
	// retrieve (or create) the entity's default animation
	Animation *animation = entity->findOrCreateAnimation( 0 );

	ParticleSysAnimationImp *animImp = new ParticleSysAnimationImp( animation );
	animation->addImplementation( animImp );
	
	worldSpaceGroup = new osg::Group;
	worldSpaceBranch->addChild( worldSpaceGroup.get() );

	osgParticleHPS::ParticleSystemUpdater *partSysUpdater = new osgParticleHPS::ParticleSystemUpdater;
	worldSpaceGroup->addChild( partSysUpdater );

	osg::Geode *geode = new osg::Geode;	
	geode->addDrawable( hps );
	worldSpaceGroup->addChild( geode );

	std::list<osgParticleHPS::MultiEmitter *> ::iterator eiter;
	for( eiter = emitters.begin(); eiter != emitters.end(); eiter++ )
	{
		if( !hps->getPermitCulling() )
			(*eiter)->setCullingActive( false );
		groupNode->addChild( (*eiter) );
		animImp->addParticleProcessor( (*eiter) );
	}
	
	std::list<osgParticleHPS::ModularProgram *> ::iterator piter;
	for( piter = programs.begin(); piter != programs.end(); piter++ )
	{
		if( !hps->getPermitCulling() )
			(*piter)->setCullingActive( false );
		groupNode->addChild( (*piter) );
		animImp->addParticleProcessor( (*piter) );
	}
	
	partSysUpdater->addParticleSystem( hps );
	
	if( !hps->getPermitCulling() )
	{
		partSysUpdater->setCullingActive( false );
		geode->setCullingActive( false );
	}

	// set the ps to be frozen; we don't want the ps to "start" until the 
	// host sets the ps's entity's anim mode to "play"
// fixme - this likely isn't necessary, and will probably break things
	hps->setFrozen( true );
	
	return true;
}


bool ParticleSysElement::addChildElement( EntityElement *childElement )
{
	if( childElement == NULL ) return false;
	
	osg::Node *node = childElement->getTopNode();
	if( node == NULL ) return false;
	
	groupNode->addChild( node );
	
	// call parent
	return EntityElement::addChildElement( childElement );
}


