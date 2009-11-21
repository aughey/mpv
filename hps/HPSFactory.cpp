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
 *  FILENAME:   HPSFactory.cpp
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



#include <iostream>
#include <vector>
#include <string>

#include <osg/Vec3>
#include <osg/Vec4>

#define _USE_MATH_DEFINES

#include "HPSFactory.h"

#include "Particle.h"
#include "ParticleIntersectingQuads.h"
#include "ParticleTrail.h"

#include "FiniteCounter.h"
#include "RandomRateCounter.h"

#include "PointPlacer.h"
#include "SectorPlacer.h"
#include "SegmentPlacer.h"
#include "MultiSegmentPlacer.h"
#include "SpherePlacer.h"

#include "RadialShooter.h"

#include "AccelOperator.h"
#include "AngularAccelOperator.h"
#include "FluidFrictionOperator.h"
#include "ForceOperator.h"
#include "ThermalUpdraftOperator.h"
#include "TextureAnimOperator.h"
#include "FollowOperator.h"

#include "range.h"


HPSFactory * HPSFactory::instance = NULL;

// ================================================
// Instance
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
HPSFactory * HPSFactory::Instance()
{
	if( instance == NULL )
		instance = new HPSFactory;
	return instance;
}

// ================================================
// HPSFactory
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
HPSFactory::HPSFactory() 
{
}

// ================================================
// ~HPSFactory
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
HPSFactory::~HPSFactory() 
{

}

// ================================================
// createParticleSys
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void HPSFactory::createParticleSys( 
	DefFileGroup *partSysDefinition, 
	osgParticleHPS::ParticleSystem *&ps, 
	std::list<osgParticleHPS::MultiEmitter *> &emitters,
	std::list<osgParticleHPS::ModularProgram *> &programs )
{
	
	ps = NULL;
	
	if( !partSysDefinition )
		return;
		
	
	DefFileAttrib *attr;
	std::list< DefFileGroup * >::iterator cIter;
	
	std::string textureFilename;
	bool emissive = true;
	bool lighting = false;
	
	ps = new osgParticleHPS::ParticleSystem;
	
	// We do not want the particles to stop moving/updating when they 
	// move off-screen.  Setting this option will prevent that from happening.
	ps->setFreezeOnCull( false );
	
	// This will be our template particle.  The particle system will use it 
	// when creating new particles.
	osgParticleHPS::Particle * particle = NULL;

	std::string shape;
	attr = partSysDefinition->getAttribute( "shape" );
	if( attr )
		shape = attr->asString();

	if( shape == "point" )
	{
		particle = new osgParticleHPS::Particle;
		particle->setShape( osgParticleHPS::Particle::POINT );
	}
	else if( shape == "quad" )
	{
		particle = new osgParticleHPS::Particle;
		particle->setShape( osgParticleHPS::Particle::QUAD );
	}
	else if( shape == "quad_offset" )
	{
		particle = new osgParticleHPS::Particle;
		particle->setShape( osgParticleHPS::Particle::QUAD_OFFSET );
	}
	else if( shape == "quad_trianglestrip" )
	{
		particle = new osgParticleHPS::Particle;
		particle->setShape( osgParticleHPS::Particle::QUAD_TRIANGLESTRIP );
	}
	else if( shape == "hexagon" )
	{
		particle = new osgParticleHPS::Particle;
		particle->setShape( osgParticleHPS::Particle::HEXAGON );
	}
	else if( shape == "intersecting_quads_2" )
	{
		osgParticleHPS::ParticleIntersectingQuads *iquadParticle = 
			new osgParticleHPS::ParticleIntersectingQuads;
		particle = iquadParticle;
		iquadParticle->setGeometryType( osgParticleHPS::ParticleIntersectingQuads::TWO_QUADS );
	}
	else if( shape == "intersecting_quads_3" )
	{
		osgParticleHPS::ParticleIntersectingQuads *iquadParticle = 
			new osgParticleHPS::ParticleIntersectingQuads;
		particle = iquadParticle;
		iquadParticle->setGeometryType( osgParticleHPS::ParticleIntersectingQuads::THREE_QUADS );
	}
	else if( shape == "trail" )
	{
		osgParticleHPS::ParticleTrail *trailParticle = new osgParticleHPS::ParticleTrail;
		particle = trailParticle;

		// We don't want to cull trail particles, because the draw function in 
		// the trail particles is the same function that records the trail's 
		// position.  If the trail gets culled, it doesn't record where it has 
		// been.  If it doesn't record where it has been, there will be gaps 
		// in the trail.  Gaps look ugly.
		ps->setPermitCulling( false );

		attr = partSysDefinition->getAttribute( "trail_type" );
		if( attr )
		{
			std::string type = attr->asString();

			if( type == "line" )
				trailParticle->setTrailType( osgParticleHPS::ParticleTrail::LINE );
			else if( type == "ribbon" )
				trailParticle->setTrailType( osgParticleHPS::ParticleTrail::RIBBON );
			else if( type == "cross_ribbon" )
				trailParticle->setTrailType( osgParticleHPS::ParticleTrail::CROSS_RIBBON );
			else if( type == "point" )
			{
				trailParticle->setTrailType( osgParticleHPS::ParticleTrail::BUILT_IN_SHAPE );
				trailParticle->setTrailShape( osgParticleHPS::Particle::POINT );
			}
			else if( type == "quad" )
			{
				trailParticle->setTrailType( osgParticleHPS::ParticleTrail::BUILT_IN_SHAPE );
				trailParticle->setTrailShape( osgParticleHPS::Particle::QUAD );
			}
			else if( type == "quad_offset" )
			{
				trailParticle->setTrailType( osgParticleHPS::ParticleTrail::BUILT_IN_SHAPE );
				trailParticle->setTrailShape( osgParticleHPS::Particle::QUAD_OFFSET );
			}
			else if( type == "quad_trianglestrip" )
			{
				trailParticle->setTrailType( osgParticleHPS::ParticleTrail::BUILT_IN_SHAPE );
				trailParticle->setTrailShape( osgParticleHPS::Particle::QUAD_TRIANGLESTRIP );
			}
			else if( type == "hexagon" )
			{
				trailParticle->setTrailType( osgParticleHPS::ParticleTrail::BUILT_IN_SHAPE );
				trailParticle->setTrailShape( osgParticleHPS::Particle::HEXAGON );
			}
		}

		attr = partSysDefinition->getAttribute( "trail_num_points" );
		if( attr )
			trailParticle->setNumPoints( attr->asInt() );

		attr = partSysDefinition->getAttribute( "trail_interval" );
		if( attr )
			trailParticle->setInterval( attr->asFloat() );

		attr = partSysDefinition->getAttribute( "trail_texture_stretching" );
		if( attr )
			trailParticle->setTextureStretching( attr->asInt() );
		
	}
	else if( shape == "" )
	{
		// User didn't specify a "shape".  Create a generic particle, with 
		// the default settings.
		std::cout << "Warning - in particle system \"(fixme)\", user "
			<< "did not specify a shape.  Using default shape.\n";
		particle = new osgParticleHPS::Particle;
	}
	else
	{
		std::cout << "Warning - in particle system \"(fixme)\", user "
			<< "specified an unrecognized shape \"" << shape 
			<< "\".  Using default shape.\n";
		particle = new osgParticleHPS::Particle;
	}
	
	attr = partSysDefinition->getAttribute( "texture" );
	if( attr ) {
		textureFilename = attr->asString();
	}

	attr = partSysDefinition->getAttribute( "emissive" );
	if( attr ) {
		emissive = attr->asInt();
	}
	
	attr = partSysDefinition->getAttribute( "lighting" );
	if( attr ) {
		lighting = attr->asInt();
	}
	
	attr = partSysDefinition->getAttribute( "alignment" );
	if( attr ) {
		if( attr->asString() == "fixed" )
			ps->setParticleAlignment( osgParticleHPS::ParticleSystem::FIXED );
		if( attr->asString() == "billboard" )
			ps->setParticleAlignment( osgParticleHPS::ParticleSystem::BILLBOARD );
	}
	
	attr = partSysDefinition->getAttribute( "time_to_live" );
	if( attr ) {
		particle->setLifeTime( attr->asFloat() );
	}
	
	attr = partSysDefinition->getAttribute( "size_range" );
	if( attr ) {
		std::vector<float> sizeRange = attr->asFloats();
		if( sizeRange.size() >= 6 )
			particle->setSizeRange(
				osgParticleHPS::rangev3( 
					osg::Vec3( sizeRange[0], sizeRange[1], sizeRange[2] ), 
					osg::Vec3( sizeRange[3], sizeRange[4], sizeRange[5] ) ) );
		else if( sizeRange.size() == 2 )
			// allow a pair of floats to be specified
			particle->setSizeRange(
				osgParticleHPS::rangev3( 
					osg::Vec3( sizeRange[0], sizeRange[0], sizeRange[0] ), 
					osg::Vec3( sizeRange[1], sizeRange[1], sizeRange[1] ) ) );
		else if( sizeRange.size() == 1 )
			// also allow a single float to be specified
			particle->setSizeRange(
				osgParticleHPS::rangev3( 
					osg::Vec3( sizeRange[0], sizeRange[0], sizeRange[0] ), 
					osg::Vec3( sizeRange[0], sizeRange[0], sizeRange[0] ) ) );
	}
	
	attr = partSysDefinition->getAttribute( "color_range" );
	if( attr ) {
		std::vector<float> cR = attr->asFloats();
		if( cR.size() >= 8 )
			particle->setColorRange(
				osgParticleHPS::rangev4(
					osg::Vec4(cR[0], cR[1], cR[2], cR[3]), 
					osg::Vec4(cR[4], cR[5], cR[6], cR[7])));
	}
	
	attr = partSysDefinition->getAttribute( "mass" );
	if( attr ) {
		particle->setMass( attr->asFloat() );
	}
	
	attr = partSysDefinition->getAttribute( "radius" );
	if( attr ) {
		particle->setRadius( attr->asFloat() );
	}
	
	// We're finished setting up the template particle.  Hand it to the 
	// particle system.
	ps->setParticleTemplate( particle );
	delete particle;
	particle = NULL;
	
	for( cIter = partSysDefinition->children.begin(); cIter != partSysDefinition->children.end(); cIter++ )
	{
		DefFileGroup *child = (*cIter);
		if( child->getName() == "emitter" ) {
			osgParticleHPS::MultiEmitter *memitter = handleEmitterGroup( child );
			if( memitter ) {
				memitter->setParticleSystem(ps);
				emitters.push_back( memitter );
			}
		}
		else if( child->getName() == "behavior" ) {
			osgParticleHPS::ModularProgram *program = handleBehaviorGroup( child );
			if( program ) {
				program->setParticleSystem(ps);
				programs.push_back( program );
			}
		}
	}
	
	ps->setDefaultAttributes( textureFilename, emissive, lighting );

	//
	// If the transparency_workaround option is enabled,
	// override the defaults and place all particle effects
	// in render bin 11 so that they are placed in front of
	// elements in the database.
	//
	DefFileGroup *parent = partSysDefinition->parent;
	if(parent)
	{
		attr = parent->getAttribute( "transparency_workaround" );
		if(attr && attr->asInt())
		{
			osg::StateSet *ss = ps->getOrCreateStateSet();
			ss->setBinNumber(11);
		}
	}
}


// ================================================
// handleEmitterGroup
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
osgParticleHPS::MultiEmitter *HPSFactory::handleEmitterGroup( DefFileGroup *group )
{
	if( group->getName() != "emitter" ) return NULL;

	osgParticleHPS::MultiEmitter *memitter = new osgParticleHPS::MultiEmitter;
	handleProcessorGroup( memitter, group );

	DefFileAttrib *attr;
	
	// there is an additional "reference_frame" choice for MultiEmitters...
	attr = group->getAttribute( "reference_frame" );
	if( attr ) {
		if( attr->asString() == "time_zero" ) {
			memitter->setReferenceFrame( 
				(osgParticleHPS::ParticleProcessor::ReferenceFrame)RELATIVE_TO_TZERO );
			double startTime = memitter->getStartTime();
			double lifeTime = memitter->getLifeTime();
			memitter->setAdditionalDelay( startTime );
			memitter->setStartTime( 0.0 );
			memitter->setLifeTime( startTime + lifeTime );
		}
	}

	attr = group->getAttribute( "additive_velocity" );
	if( attr ) {
		int i = attr->asInt();
		memitter->setAdditiveVelocity( i );
	}
	
	std::list< DefFileGroup * >::iterator cIter;
	for( cIter = group->children.begin(); cIter != group->children.end(); cIter++ )
	{
		DefFileGroup *child = (*cIter);
		
		if( child->getName() == "counter" ) {
			std::string type;
			attr = child->getAttribute( "type" );
			if( attr ) {
				type = attr->asString();
			}
			if( type == "random" ) {
				osgParticleHPS::RandomRateCounter *rrc = new osgParticleHPS::RandomRateCounter;
				attr = child->getAttribute( "rate_range" );
				if( attr ) {
					std::vector<float> rr = attr->asFloats();
					if( rr.size() >= 2 )
						rrc->setRateRange( rr[0], rr[1] );
				}
				memitter->setCounter( rrc );
			}
			else if( type == "finite" ) {
				FiniteCounter *fc = new FiniteCounter;
				fc->setEmitter( memitter );
				attr = child->getAttribute( "num_particles_range" );
				if( attr ) {
					std::vector<float> rr = attr->asFloats();
					if( rr.size() >= 2 )
						fc->setNumParticlesRange( rr[0], rr[1] );
				}
				memitter->setCounter( fc );
			}
			
		}
		else if( child->getName() == "placement" ) {
			std::string type;
			attr = child->getAttribute( "type" );
			if( attr ) {
				type = attr->asString();
			}
			if( type == "point" )
			{
				osgParticleHPS::PointPlacer *placer = new osgParticleHPS::PointPlacer;
				attr = child->getAttribute( "center" );
				if( attr ) {
					std::vector<float> c = attr->asFloats();
					if( c.size() >= 3 )
						placer->setCenter( c[0], c[1], c[2] );
				}
				attr = child->getAttribute( "orientation_range" );
				if( attr ) {
					std::vector<float> r = attr->asFloats();
					// convert from degrees to radians
					for( unsigned int i = 0; i < r.size(); i++ )
						r[i] = osg::DegreesToRadians( r[i] );
					if( r.size() >= 6 )
						placer->setOrientationRanges( 
							osg::Vec3( r[0], r[1], r[2] ),
							osg::Vec3( r[3], r[4], r[5] ) );
				}
				memitter->addPlacer( placer );
			}
			else if( type == "circle" )
			{
				osgParticleHPS::SectorPlacer *placer = new osgParticleHPS::SectorPlacer;
				attr = child->getAttribute( "center" );
				if( attr ) {
					std::vector<float> c = attr->asFloats();
					if( c.size() >= 3 )
						placer->setCenter( c[0], c[1], c[2] );
				}
				attr = child->getAttribute( "radius_range" );
				if( attr ) {
					std::vector<float> r = attr->asFloats();
					if( r.size() >= 2 )
						placer->setRadiusRange( r[0], r[1] );
				}
				attr = child->getAttribute( "phi_range" );
				if( attr ) {
					std::vector<float> r = attr->asFloats();
					// convert from degrees to radians
					for( unsigned int i = 0; i < r.size(); i++ )
						r[i] = osg::DegreesToRadians( r[i] );
					if( r.size() >= 2 )
						placer->setPhiRange( r[0], r[1] );
				}
				attr = child->getAttribute( "orientation_range" );
				if( attr ) {
					std::vector<float> r = attr->asFloats();
					// convert from degrees to radians
					for( unsigned int i = 0; i < r.size(); i++ )
						r[i] = osg::DegreesToRadians( r[i] );
					if( r.size() >= 6 )
						placer->setOrientationRanges( 
							osg::Vec3( r[0], r[1], r[2] ),
							osg::Vec3( r[3], r[4], r[5] ) );
				}
				memitter->addPlacer( placer );
			}
			else if( type == "sphere" )
			{
				osgParticleHPS::SpherePlacer *placer = new osgParticleHPS::SpherePlacer;
				attr = child->getAttribute( "center" );
				if( attr ) {
					std::vector<float> c = attr->asFloats();
					if( c.size() >= 3 )
						placer->setCenter( c[0], c[1], c[2] );
				}
				attr = child->getAttribute( "radius_range" );
				if( attr ) {
					std::vector<float> r = attr->asFloats();
					if( r.size() >= 2 )
						placer->setRadiusRange( r[0], r[1] );
				}
				attr = child->getAttribute( "phi_range" );
				if( attr ) {
					std::vector<float> r = attr->asFloats();
					// convert from degrees to radians
					for( unsigned int i = 0; i < r.size(); i++ )
						r[i] = osg::DegreesToRadians( r[i] );
					if( r.size() >= 2 )
						placer->setPhiRange( r[0], r[1] );
				}
				attr = child->getAttribute( "theta_range" );
				if( attr ) {
					std::vector<float> r = attr->asFloats();
					// convert from degrees to radians
					for( unsigned int i = 0; i < r.size(); i++ )
						r[i] = osg::DegreesToRadians( r[i] );
					if( r.size() >= 2 )
						placer->setThetaRange( r[0], r[1] );
				}
				attr = child->getAttribute( "orientation_range" );
				if( attr ) {
					std::vector<float> r = attr->asFloats();
					// convert from degrees to radians
					for( unsigned int i = 0; i < r.size(); i++ )
						r[i] = osg::DegreesToRadians( r[i] );
					if( r.size() >= 6 )
						placer->setOrientationRanges( 
							osg::Vec3( r[0], r[1], r[2] ),
							osg::Vec3( r[3], r[4], r[5] ) );
				}
				memitter->addPlacer( placer );
			}
			else if( type == "line" )
			{
				osgParticleHPS::SegmentPlacer *placer = new osgParticleHPS::SegmentPlacer;
				attr = child->getAttribute( "point_a" );
				if( attr ) {
					std::vector<float> c = attr->asFloats();
					if( c.size() >= 3 )
						placer->setVertexA( c[0], c[1], c[2] );
				}
				attr = child->getAttribute( "point_b" );
				if( attr ) {
					std::vector<float> c = attr->asFloats();
					if( c.size() >= 3 )
						placer->setVertexB( c[0], c[1], c[2] );
				}
				attr = child->getAttribute( "orientation_range" );
				if( attr ) {
					std::vector<float> r = attr->asFloats();
					// convert from degrees to radians
					for( unsigned int i = 0; i < r.size(); i++ )
						r[i] = osg::DegreesToRadians( r[i] );
					if( r.size() >= 6 )
						placer->setOrientationRanges( 
							osg::Vec3( r[0], r[1], r[2] ),
							osg::Vec3( r[3], r[4], r[5] ) );
				}
				memitter->addPlacer( placer );
			}
			else if( type == "multiline" )
			{
				osgParticleHPS::MultiSegmentPlacer *placer = new osgParticleHPS::MultiSegmentPlacer;
				std::list< DefFileAttrib * >::iterator aIter;
				for( aIter = child->attributes.begin(); 
					aIter != child->attributes.end(); aIter++ )
				{
					attr = (*aIter);
					if( attr->getName() == "point" ) {
						std::vector<float> c = attr->asFloats();
						if( c.size() >= 3 )
							placer->addVertex( c[0], c[1], c[2] );
					}
				}
				attr = child->getAttribute( "orientation_range" );
				if( attr ) {
					std::vector<float> r = attr->asFloats();
					// convert from degrees to radians
					for( unsigned int i = 0; i < r.size(); i++ )
						r[i] = osg::DegreesToRadians( r[i] );
					if( r.size() >= 6 )
						placer->setOrientationRanges( 
							osg::Vec3( r[0], r[1], r[2] ),
							osg::Vec3( r[3], r[4], r[5] ) );
				}
				memitter->addPlacer( placer );
			}
		}
		else if( child->getName() == "velocity" ) {
			// fixme - other types of shooters
			osgParticleHPS::RadialShooter *rs = new osgParticleHPS::RadialShooter;

			attr = child->getAttribute( "speed_range" );
			if( attr ) {
				std::vector<float> sr = attr->asFloats();
				if( sr.size() >= 2 )
					rs->setInitialSpeedRange( sr[0], sr[1] );
			}

			attr = child->getAttribute( "rot_vel_range" );
			if( attr ) {
				std::vector<float> rvr = attr->asFloats();
				// convert from degrees to radians
				for( unsigned int i = 0; i < rvr.size(); i++ )
					rvr[i] = osg::DegreesToRadians( rvr[i] );
				if( rvr.size() >= 2 )
					rs->setInitialRotationalSpeedRange( 
						osg::Vec3( rvr[0], rvr[1], rvr[2] ),
						osg::Vec3( rvr[3], rvr[4], rvr[5] ) );
			}
			
			attr = child->getAttribute( "phi_range" );
			if( attr ) {
				std::vector<float> pr = attr->asFloats();
				// convert from degrees to radians
				for( unsigned int i = 0; i < pr.size(); i++ )
					pr[i] = osg::DegreesToRadians( pr[i] );
				if( pr.size() >= 2 )
					rs->setPhiRange( pr[0], pr[1] );
			}

			attr = child->getAttribute( "theta_range" );
			if( attr ) {
				std::vector<float> tr = attr->asFloats();
				// convert from degrees to radians
				for( unsigned int i = 0; i < tr.size(); i++ )
					tr[i] = osg::DegreesToRadians( tr[i] );
				if( tr.size() >= 2 )
					rs->setThetaRange( tr[0], tr[1] );
			}

			memitter->addShooter( rs );
		}
		
	}
	
	return memitter;
}


// ================================================
// handleBehaviorGroup
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
osgParticleHPS::ModularProgram *HPSFactory::handleBehaviorGroup( DefFileGroup *group )
{
	if( group->getName() != "behavior" ) return NULL;
	
	osgParticleHPS::ModularProgram * program = new osgParticleHPS::ModularProgram;
	handleProcessorGroup( program, group );

	std::list< DefFileGroup * >::iterator cIter;
	for( cIter = group->children.begin(); cIter != group->children.end(); cIter++ )
	{
		DefFileGroup *child = (*cIter);
		osgParticleHPS::Operator *op = handleAffectorGroup( child );
		if( op )
			program->addOperator( op );
	}
	
	return program;
}


// ================================================
// handleAffectorGroup
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
osgParticleHPS::Operator *HPSFactory::handleAffectorGroup( DefFileGroup *group )
{
	if( group->getName() != "affector" ) return NULL;
	
	osgParticleHPS::Operator *result = NULL;
	DefFileAttrib *attr;
	
	attr = group->getAttribute( "type" );
	if( attr ) {
		std::string type = attr->asString();
		
		if( type == "linear_accel" ) 
		{
			osgParticleHPS::AccelOperator *op = 
				new osgParticleHPS::AccelOperator;
			result = op;

			attr = group->getAttribute( "vector" );
			if( attr ) {
				std::vector<float> v = attr->asFloats();
				if( v.size() >= 3 )
					op->setAcceleration( osg::Vec3( v[0], v[1], v[2] ) );
			}
		}
		else if( type == "angular_accel" ) 
		{
			osgParticleHPS::AngularAccelOperator *op = 
				new osgParticleHPS::AngularAccelOperator;
			result = op;
			
			attr = group->getAttribute( "vector" );
			if( attr ) {
				std::vector<float> v = attr->asFloats();
				if( v.size() >= 3 )
					op->setAngularAcceleration( osg::Vec3( v[0], v[1], v[2] ) );
			}
		}
		else if( type == "linear_force" ) 
		{
			osgParticleHPS::ForceOperator *op = 
				new osgParticleHPS::ForceOperator;
			result = op;
			
			attr = group->getAttribute( "vector" );
			if( attr ) {
				std::vector<float> v = attr->asFloats();
				if( v.size() >= 3 )
					op->setForce( osg::Vec3( v[0], v[1], v[2] ) );
			}
		}
		else if( type == "fluid_friction" ) 
		{
			osgParticleHPS::FluidFrictionOperator *op = 
				new osgParticleHPS::FluidFrictionOperator;
			result = op;
			
			attr = group->getAttribute( "density" );
			if( attr ) {
				float f = attr->asFloat();
				op->setFluidDensity( f );
			}
			attr = group->getAttribute( "viscosity" );
			if( attr ) {
				float f = attr->asFloat();
				op->setFluidViscosity( f );
			}
		}
		else if( type == "texture_animation" ) 
		{
			TextureAnimOperator *op = new TextureAnimOperator;
			result = op;
			
			std::list< DefFileAttrib * >::iterator aIter;
			for( aIter = group->attributes.begin(); 
				aIter != group->attributes.end(); aIter++ )
			{
				attr = (*aIter);
				if( attr->getName() == "image" ) {
					std::string filename = attr->asString();
					op->addFrame( filename );
				}
				else if( attr->getName() == "period" ) {
					op->setFramePeriod( attr->asFloat() );
				}
				else if( attr->getName() == "loop" ) {
					op->setLoop( attr->asInt() );
				}
				else if( attr->getName() == "hold_last" ) {
					op->setHoldLastFrame( attr->asInt() );
				}
			}
		}
/*		else if( type == "scale" )
		{
			ScaleOperator *op = new ScaleOperator;
			result = op;
			
			attr = group->getAttribute( "range" );
			if( attr ) {
				std::vector<float> v = attr->asFloats();
				if( v.size() >= 6 )
					op->setRange( 
						osg::Vec3( v[0], v[1], v[2] ), 
						osg::Vec3( v[3], v[4], v[5] ) );
			}
			
		}*/
		else if( type == "thermal_updraft" )
		{
			ThermalUpdraftOperator *op = new ThermalUpdraftOperator;
			result = op;
			
			attr = group->getAttribute( "radius" );
			if( attr )
			{
				op->setRadius( attr->asFloat() );
			}
			
			attr = group->getAttribute( "height" );
			if( attr )
			{
				op->setHeight( attr->asFloat() );
			}
			
			attr = group->getAttribute( "force" );
			if( attr )
			{
				op->setForce( attr->asFloat() );
			}
			
		}
		else if( type == "follow" )
		{
			FollowOperator *op = new FollowOperator;
			result = op;
			
			attr = group->getAttribute( "exclude_roll" );
			if( attr ) {
				op->setExcludeRoll( attr->asInt() );
			}
			attr = group->getAttribute( "exclude_pitch" );
			if( attr ) {
				op->setExcludePitch( attr->asInt() );
			}
			attr = group->getAttribute( "exclude_yaw" );
			if( attr ) {
				op->setExcludeYaw( attr->asInt() );
			}
		}
	}
	
	return result;
}


// ================================================
// handleProcessorGroup
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void HPSFactory::handleProcessorGroup( 
	osgParticleHPS::ParticleProcessor *proc, DefFileGroup *group )
{
	DefFileAttrib *attr;
	
	attr = group->getAttribute( "duration" );
	if( attr ) {
		float f = attr->asFloat();
		proc->setLifeTime( f );
	}
	
	attr = group->getAttribute( "start_time" );
	if( attr ) {
		float f = attr->asFloat();
		proc->setStartTime( f );
	}
	
	attr = group->getAttribute( "endless" );
	if( attr ) {
		int i = attr->asInt();
		proc->setEndless( i );
	}
	
	/* 
	RELATIVE_TO_PARENTS is the default.
	RELATIVE_TO_ABSOLUTE is almost never what you want, at least for special effects.
	(It is certainly never used for emitters.)  However, there is at least one situation 
	where RELATIVE_TO_ABSOLUTE is used: the fluid friction operator. 
	*/
	attr = group->getAttribute( "reference_frame" );
	if( attr ) {
		std::string refFrame = attr->asString();
		if( refFrame == "parent" )
			proc->setReferenceFrame( osgParticleHPS::ParticleProcessor::RELATIVE_TO_PARENTS );
		else if( refFrame == "absolute" )
			proc->setReferenceFrame( osgParticleHPS::ParticleProcessor::RELATIVE_TO_ABSOLUTE );
	}
	
}

