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
 *  FILENAME:   ParticleIntersectingQuads.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  represent a particle, formed by 2 or 3 intersecting quads.
 *  
 *  MODIFICATION NOTES:
 *  
 *  2007-11-17 Andrew Sampson
 *      Initial Release.
 *  
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#ifndef PARTICLE_INTERSECTING_QUADS_H
#define PARTICLE_INTERSECTING_QUADS_H

#include <deque>

#include "Export.h"

#include "Particle.h"

namespace osgParticleHPS
{

//=========================================================
//! This class implements a Particle.  It renders a simple shape; 2 or 3 
//! quads, intersecting at right angles.  This type of particle is useful if 
//! you need to fill a volume with lots of polygons.
//! It makes no sense to use type of particle in billboard mode.  
//! 
class OSGPARTICLE_EXPORT ParticleIntersectingQuads : public Particle {
public:

	//=========================================================
	//! The available rendering methods
	//! 
	enum ParticleIntersectingQuadsType
	{
		//=========================================================
		//! Draws two quads, intersecting at right angles; the line 
		//! formed by the two intersecting faces is coincident 
		//! with the y axis
		//! 
		TWO_QUADS,

		//=========================================================
		//! Draws three quads intersecting at right angles
		//! 
		THREE_QUADS
	};
	
	
	//=========================================================
	//! General Constructor
	//! 
	ParticleIntersectingQuads();

	//=========================================================
	//! Copy Constructor
	//! 
	ParticleIntersectingQuads( const ParticleIntersectingQuads &other );

	//=========================================================
	//! General Destructor
	//! 
	virtual ~ParticleIntersectingQuads() {}

	//=========================================================
	//! Called by the ps before calling render().
	//! 
	virtual void beginRender() const;

	//=========================================================
	//! Renders the particle
	//! \param xpos - translated position
	//! \param px - unused; a hold-over from the Particle interface
	//! \param py - unused; a hold-over from the Particle interface
	//! 
	virtual void render(const osg::Vec3 &xpos, const osg::Vec3 &px, const osg::Vec3 &py ) const;

	//=========================================================
	//! Called by the ps after calling render().
	//! 
	virtual void endRender() const;

	//=========================================================
	//! This particle will allocate a new copy of itself
	//! \return the newly created particle (actually a ParticleIntersectingQuads cast to 
	//! a Particle)
	//! 
	virtual Particle *copy() const ;
	

	//=========================================================
	//! Sets the geometry type.
	//! \param type - the new geometry type
	//! 
	void setGeometryType( ParticleIntersectingQuadsType type )
	{
		geometryType_ = type;
	}
	
protected:
	
	//=========================================================
	//! The type of rendering method to use.  See ParticleIntersectingQuadsType.
	//! 
	ParticleIntersectingQuadsType geometryType_;
	
	//=========================================================
	//! the Particle::Shape index for this child class
	//! 
	static int shapeType;
};

}

#endif

