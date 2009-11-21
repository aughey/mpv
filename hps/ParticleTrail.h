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
 *  FILENAME:   ParticleTrail.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  represent a particle, and its history or "trail".
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  06/22/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 *  
 *  2007-11-11 Andrew Sampson
 *      Ported class from HPSParticle to Particle
 *  
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#ifndef PARTICLE_TRAIL_H
#define PARTICLE_TRAIL_H

#include <deque>

#include "Export.h"

#include "Particle.h"

namespace osgParticleHPS
{

//=========================================================
//! This struct holds a position and a timestamp.
//! 
struct HPSTrailPoint
{
	//=========================================================
	//! Initializes this data structure
	//! 
	void set( const osg::Vec3 &p, const osg::Vec3 &r, float t )
	{
		pos = p;
		rpy = r;
		timestamp = t;
	}
	
	//=========================================================
	//! The position being recorded
	//! 
	osg::Vec3 pos;

	//=========================================================
	//! The orientation being recorded
	//! 
	osg::Vec3 rpy;

	//=========================================================
	//! The time when this position was recorded
	//! 
	float timestamp;
};

//=========================================================
//! This class implements a Particle with a trail.
//! It has all the attributes of a Particle, with the following additions:
//!    - it will remember its previous locations and orientations, and will
//!      use one of several rendering methods to place geometry at each of 
//!      those locations
//! Objects instanced from this class can be treated as Particles, with the 
//! following exceptions:
//!    - these particles should not be used in conjunction with the texture 
//!      animation operator or the scale operator
//! 
//! If these particles are being used to create a vapor-trail-type effect or 
//! vehicle-track-type effect, the following considerations should be made:
//!    - the particles should probably be given immortality (ie lifetime = -1)
//!    - the user should probably give the emitter a FiniteCounter with a 
//!      range of 1 to 1
//!    - the user needs to include a FollowOperator in whatever ModularProgram 
//!      will be manipulating the particles
//!    - the particles should not be used in conjunction with any operator 
//!      that changes a particle's orientation or angular velocity
//!    - the particles should be emitted with 0 rotation and 0 angular 
//!      velocity
//! 
//! If these particles are being used to create something other than a 
//! vapor-trail-type effect (for example, to create "tendrils" of smoke and 
//! dust that are thrown outward from an explosion), then these particles do 
//! not require a FollowOperator and do not have any restrictions on their 
//! orientation or angular velocity.  They should be given a finite lifetime, 
//! and can be used with any counter.
//! 
class OSGPARTICLE_EXPORT ParticleTrail : public Particle {
public:

	//=========================================================
	//! The available rendering methods
	//! 
	enum ParticleTrailType
	{
		//=========================================================
		//! Uses Particle::render() to draw a particle at each point on the 
		//! trail.  See 
		//! 
		BUILT_IN_SHAPE,

		//=========================================================
		//! Draws a GL line connecting all of the trail points
		//! This is the default.
		//! 
		LINE,

		//=========================================================
		//! Draws a quad strip connecting all of the trail points
		//! The width of the strip is determined by current_size_ (inherited 
		//! from Particle).
		//! The texture applied to the particle (if any) will be stretched 
		//! along the length of the entire ribbon.
		//! 
		RIBBON,

		//=========================================================
		//! Draws two ribbons (see above) which intersect at right angles
		//! 
		CROSS_RIBBON,

		//=========================================================
		//! used for debugging
		//! 
		DEBUG
	};
	
	
	//=========================================================
	//! General Constructor
	//! 
	ParticleTrail();

	//=========================================================
	//! Copy Constructor
	//! 
	ParticleTrail( const ParticleTrail &other );

	//=========================================================
	//! General Destructor
	//! 
	virtual ~ParticleTrail() {}

	//=========================================================
	//! Overrides the osgParticleHPS::Particle::update.  
	//! \param dt - the deltaT, in seconds
	virtual bool update(double dt);

	//=========================================================
	//! Just a proxy for nonconstRender.  
	//! \param xpos - translated position
	//! \param px - unused; a hold-over from the Particle interface
	//! \param py - unused; a hold-over from the Particle interface
	//! \param scale - multiplier for current size
	//! 
	virtual void render(const osg::Vec3 &xpos, const osg::Vec3 &px, const osg::Vec3 &py ) const;

	//=========================================================
	//! Called by the ps before calling render().  Does nothing.
	//! 
	virtual void beginRender() const {}

	//=========================================================
	//! Called by the ps after calling render().  Does nothing.
	//! 
	virtual void endRender() const {}

	//=========================================================
	//! This particle will allocate a new copy of itself
	//! \return the newly created particle (actually a ParticleTrail cast to 
	//! a Particle)
	//! 
	virtual Particle *copy() const ;
	
	//=========================================================
	//! Called by ParticleSystem before returning this particle to 
	//! the list of free particles.  
	//! This function clears the position history list.
	//! 
	virtual void reset();


	//=========================================================
	//! Sets the trail type
	//! \param type - the new trail type
	//! 
	void setTrailType( ParticleTrailType type )
	{
		trailType_ = type;
	}
	
	//=========================================================
	//! Sets the trail shape; only used if trailType_ is set to BUILT_IN_SHAPE
	//! \param shape - the new trail shape
	//! 
	void setTrailShape( Particle::Shape shape )
	{
		trailShape_ = shape;
	}
	
	//=========================================================
	//! Sets the number of trail points to record
	//! \param n - the new maxNumPointsToRecord_
	//! 
	void setNumPoints( unsigned int n )
	{
		maxNumPointsToRecord_ = n;
	}
	
	//=========================================================
	//! Sets the amount of time, in seconds, that this particle should wait 
	//! before adding each new trail point.
	//! \param t - the new timeBetweenPoints_
	//! 
	void setInterval( float t )
	{
		timeBetweenPoints_ = t;
	}
	
	void setTextureStretching( bool enable )
	{
		stretchTexture = enable;
	}
	
protected:
	
	//=========================================================
	//! The type of rendering method to use.  See ParticleTrailType.
	//! 
	ParticleTrailType trailType_;
	
	//=========================================================
	//! If trailType_ is set to BUILT_IN_SHAPE, this variable will determine 
	//! which type of particle is drawn.
	//! 
	Particle::Shape trailShape_;
	
	//=========================================================
	//! The list of previous positions/orientations.  
	//! 
	std::deque< HPSTrailPoint > positions_;
	
	//=========================================================
	//! The timestamp (particle age, not wall time) when the most recently 
	//! added trail point was added.
	//! 
	float timestampForLastNewPoint_;

	//=========================================================
	//! The amount of time, in seconds, that this particle should wait before 
	//! adding the next new trail point.  The default is 1/10 second.
	//! 
	float timeBetweenPoints_;

	//=========================================================
	//! The maximum number of trail points to record.  The default is 40.
	//! 
	unsigned int maxNumPointsToRecord_;
	
	//=========================================================
	//! Determines whether the texture will be stretched over the length of 
	//! the ribbon, or if it is repeated every section.  The default is true.
	//! 
	bool stretchTexture;
	
	//=========================================================
	//! The total number of history points recorded by this particle.  Used by 
	//! the texture-mapping code to prevent pulsing/alternation.  
	//! 
	unsigned int totalHistoryPoints;
	
	//=========================================================
	//! the Particle::Shape index for this child class
	//! 
	static int shapeType;

	//=========================================================
	//! Draws BUILT_IN_SHAPE trails.  See ParticleTrailType for a description.
	//! 
	void renderBuiltInShape( const osg::Vec3 &px, const osg::Vec3 &py );

	//=========================================================
	//! Draws LINE trails.  See ParticleTrailType for a description.
	//! 
	void renderLine() const;

	//=========================================================
	//! Draws RIBBON trails.  See ParticleTrailType for a description.
	//! 
	void renderRibbon() const;

	//=========================================================
	//! Draws CROSS_RIBBON trails.  See ParticleTrailType for a description.
	//! 
	void renderCrossRibbon() const;

	//=========================================================
	//! Draws DEBUG trails.  See ParticleTrailType for a description.
	//! 
	void renderDebug( float scale ) const;
};


}

#endif

