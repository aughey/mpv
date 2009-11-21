/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2003 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/
//osgParticle - Copyright (C) 2002 Marco Jez

#ifndef OSGPARTICLE_PARTICLE_
#define OSGPARTICLE_PARTICLE_ 1

#include "Export.h"
#include "Interpolator.h"
#include "range.h"

#include <osg/ref_ptr>
#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Matrix>
#include <osg/GL>

namespace osgParticleHPS
{

    /**    Implementation of a <B>particle</B>.
        Objects of this class are particles, they have some graphical properties
        and some physical properties. Particles are created by emitters and then placed
        into Particle Systems, where they live and get updated at each frame.
        Particles can either live forever (lifeTime < 0), or die after a specified
        time (lifeTime >= 0). For each property which is defined as a range of values, a
        "current" value will be evaluated at each frame by interpolating the <I>min</I>
        and <I>max</I> values so that <I>curr_value = min</I> when <I>t == 0</I>, and 
        <I>curr_value = max</I> when <I>t == lifeTime</I>.
        You may customize the interpolator objects to achieve any kind of transition.
        If you want the particle to live forever, set its lifetime to any value <= 0;
        in that case, no interpolation is done to compute real-time properties, and only
        minimum values are used.
    */
    class OSGPARTICLE_EXPORT Particle {
    public:

		/**
		 Shape of particles.
		 NOTE: the LINE shape should be used in conjunction with FIXED alignment mode (see ParticleSystem).
		*/
		enum Shape {
			POINT,                // uses GL_POINTS as primitive
			QUAD,                 // uses GL_QUADS as primitive
			QUAD_OFFSET,          // uses GL_QUADS as primitive, with the origin of the particle at the bottom edge instead of the center
			QUAD_TRIANGLESTRIP,   // uses GL_TRIANGLE_STRIP as primitive, but each particle needs a glBegin/glEnd pair
			HEXAGON,              // may save some filling time, but uses more triangles
			LINE,                 // uses GL_LINES to draw line segments that point to the direction of motion
			CUSTOM                // rendering is performed by a child class of Particle
		};
        
        Particle();
        Particle( const Particle &other );
        virtual ~Particle() {}

        /// Get the shape of the particle.
        inline Shape getShape() const;
        
        /// Set the shape of the particle.
        inline void setShape(Shape s);

        /// Get whether the particle is still alive.
        inline bool isAlive() const;
        
        /// Get the life time of the particle (in seconds).
        inline double getLifeTime() const;
        
        /// Get the age of the particle (in seconds).
        inline double getAge() const;
        
        /// Get the minimum and maximum values for polygon size.
        inline const rangev3 &getSizeRange() const;
        
        /// Get the minimum and maximum values for color.
        inline const rangev4 &getColorRange() const;
        
        /// Get the interpolator for computing the size of polygons.
        inline const Interpolator *getSizeInterpolator() const;
        
        /// Get the interpolator for computing color values.
        inline const Interpolator *getColorInterpolator() const;

        /** Get the physical radius of the particle.
            For built-in operators to work correctly, lengths must be expressed in meters.
        */
        inline float getRadius() const;
        
        /** Get the mass of the particle.
            For built-in operators to work correctly, remember that the mass is expressed in kg.
        */
        inline float getMass() const;
        
        /// Get <CODE>1 / getMass()</CODE>.
        inline float getMassInv() const;
        
        /// Get the position vector.
        inline const osg::Vec3 &getPosition() const;
        
        /**    Get the velocity vector.
            For built-in operators to work correctly, remember that velocity components are expressed
            in meters per second.
        */
        inline const osg::Vec3 &getVelocity() const;        
        
        /// Get the previous position (the position before last update).
        inline const osg::Vec3 &getPreviousPosition() const;

		/// Get the angle vector.
		inline const osg::Vec3 &getAngle() const;
		
		/// Get the rotational velocity vector.
		inline const osg::Vec3 &getAngularVelocity() const;
		
		/// Get the previous angle vector.
		inline const osg::Vec3 &getPreviousAngle() const;

        /** Kill the particle on next update
            NOTE: after calling this function, the <CODE>isAlive()</CODE> method will still 
            return true until the particle is updated again.
        */
        inline void kill();
        
		//=========================================================
		//! This particle will allocate a new copy of itself.  Child classes 
		//! must override this method.
		//! \return the newly created particle
		//! 
		virtual Particle *copy() const ;

		//=========================================================
		//! Called by ParticleSystem in order to bring this particle 
		//! back to life.  Resets all the lifetime-related member variables.
		//! If child classes override this method, they must call this method 
		//! from their version.
		//! 
		virtual void reset();

        /// Set the life time of the particle.
        inline void setLifeTime(double t);
        
        /// Set the minimum and maximum values for polygon size.
        inline void setSizeRange(const rangev3 &r);
        
        /// Set the minimum and maximum values for color.
        inline void setColorRange(const rangev4 &r);
        
        /// Set the interpolator for computing size values.
        inline void setSizeInterpolator(Interpolator *ri);
        
        /// Set the interpolator for computing color values.
        inline void setColorInterpolator(Interpolator *ci);

        /** Set the physical radius of the particle.
            For built-in operators to work correctly, lengths must be expressed in meters.
        */
        inline void setRadius(float r);
        
        /** Set the mass of the particle.
            For built-in operators to work correctly, remember that the mass is expressed in kg.
        */
        inline void setMass(float m);
        
        /// Set the position vector.        
        inline void setPosition(const osg::Vec3 &p);
        
        /**    Set the velocity vector.
            For built-in operators to work correctly, remember that velocity components are expressed
            in meters per second.
        */
        inline void setVelocity(const osg::Vec3 &v);
        
        /// Add a vector to the velocity vector.
        inline void addVelocity(const osg::Vec3 &dv);
        
        /// Transform position and velocity vectors by a matrix.
        inline void transformPositionVelocity(const osg::Matrix &xform);

		/// Set the angle vector.
		inline void setAngle(const osg::Vec3 &a);
		
		/**
		  Set the angular velocity vector. 
		  Components x, y and z are angles of rotation around the respective axis (in radians).
	    */
		inline void setAngularVelocity(const osg::Vec3 &v);
		
		/// Add a vector to the angular velocity vector.
		inline void addAngularVelocity(const osg::Vec3 &dv);
		
		/// Transform angle and angularVelocity vectors by a matrix.
		inline void transformAngleVelocity(const osg::Matrix &xform);
		
        /**    Update the particle (don't call this method manually).
            This method is called automatically by <CODE>ParticleSystem::update()</CODE>; it
            updates the graphical properties of the particle for the current time,
            checks whether the particle is still alive, and then updates its position
            by computing <I>P = P + V * dt</I> (where <I>P</I> is the position and <I>V</I> is the velocity).
        */
        virtual bool update(double dt);

        /// Perform some pre-rendering tasks. Called automatically by particle systems.
        inline virtual void beginRender() const;
        
        /// Render the particle. Called automatically by particle systems.
        virtual void render(const osg::Vec3 &xpos, const osg::Vec3 &px, const osg::Vec3 &py ) const;
        
        /// Perform some post-rendering tasks. Called automatically by particle systems.
        inline virtual void endRender() const;
        
        /// Get the current (interpolated) polygon size. Valid only after the first call to update().
        inline osg::Vec3 getCurrentSize() const;
        
        // Specify how the particle texture is tiled
        inline void setTextureTile(int sTile, int tTile, int numTiles = 0);
		
		//=========================================================
		//! Child classes should use this to reserve a Particle::Shape index.
		//! The return value should be saved off to a static variable. 
		//! 
		static int reserveNewShapeType();

    protected:
        Shape shape_;

        rangev3 sizeRange_;
        rangev4 cr_;

        osg::ref_ptr<Interpolator> si_;
        osg::ref_ptr<Interpolator> ci_;

        bool alive_;
        bool mustdie_;
        double lifetime_;

        float radius_;
        float mass_;
        float massinv_;
        osg::Vec3 prev_pos_;
        osg::Vec3 position_;
        osg::Vec3 velocity_;

        osg::Vec3 prev_angle_;
        osg::Vec3 angle_;
		osg::Vec3 angular_vel_;

        double t0_;        

        osg::Vec3 current_size_;
        osg::Vec4 current_color_;
        
        float s_tile_;
        float t_tile_;
        int num_tile_;
        int cur_tile_;
        float s_coord_;
        float t_coord_;
    };

    // INLINE FUNCTIONS

    inline Particle::Shape Particle::getShape() const
    {
        return shape_;
    }

    inline void Particle::setShape(Shape s)
    {
        shape_ = s;
    }

    inline bool Particle::isAlive() const
    {
        return alive_;
    }

    inline double Particle::getLifeTime() const
    {
        return lifetime_;
    }
    
    inline double Particle::getAge() const
    {
        return t0_;
    }
    
    inline float Particle::getRadius() const
    {
        return radius_;
    }
    
    inline void Particle::setRadius(float r)
    {
        radius_ = r;
    }

    inline const rangev3 &Particle::getSizeRange() const
    {
        return sizeRange_;
    }

    inline const rangev4 &Particle::getColorRange() const
    {
        return cr_;
    }

    inline const Interpolator *Particle::getSizeInterpolator() const
    {
        return si_.get();
    }

    inline const Interpolator *Particle::getColorInterpolator() const
    {
        return ci_.get();
    }

    inline const osg::Vec3 &Particle::getPosition() const
    {
        return position_;
    }

    inline const osg::Vec3 &Particle::getVelocity() const
    {
        return velocity_;
    }
    
    inline const osg::Vec3 &Particle::getPreviousPosition() const
    {
        return prev_pos_;
    }

	inline const osg::Vec3 &Particle::getAngle() const
	{
		return angle_;
	}
	
	inline const osg::Vec3 &Particle::getAngularVelocity() const
	{
		return angular_vel_;
	}
	
	inline const osg::Vec3 &Particle::getPreviousAngle() const
	{
		return prev_angle_;
	}
    
    inline void Particle::kill()
    {
        mustdie_ = true;
    }

    inline void Particle::setLifeTime(double t)
    {
        lifetime_ = t;
    }

    inline void Particle::setSizeRange(const rangev3 &r)
    {
        sizeRange_ = r;
    }

    inline void Particle::setColorRange(const rangev4 &r)
    {
        cr_ = r;
    }

    inline void Particle::setSizeInterpolator(Interpolator *ri)
    {
        si_ = ri;
    }

    inline void Particle::setColorInterpolator(Interpolator *ci)
    {
        ci_ = ci;
    }

    inline void Particle::setPosition(const osg::Vec3 &p)
    {
        position_ = p;
    }

    inline void Particle::setVelocity(const osg::Vec3 &v)
    {
        velocity_ = v;
    }

    inline void Particle::addVelocity(const osg::Vec3 &dv)
    {
        velocity_ += dv;
    }

    inline void Particle::transformPositionVelocity(const osg::Matrix &xform)
    {
        // this should be optimized!

        osg::Vec3 p1 = position_ + velocity_;
                
        position_ = xform.preMult(position_);
        p1 = xform.preMult(p1);
        
        velocity_ = p1 - position_;
    }
    
    inline void Particle::setAngle(const osg::Vec3 &a)
    {
		angle_ = a;
	}
	
	inline void Particle::setAngularVelocity(const osg::Vec3 &v)
	{
		angular_vel_ = v;
	}
	
	inline void Particle::addAngularVelocity(const osg::Vec3 &dv)
	{
		angular_vel_ += dv;
	}
	
	inline void Particle::transformAngleVelocity(const osg::Matrix &xform)
	{
		// this should be optimized!
		
		osg::Vec3 a1 = angle_ + angular_vel_;
		
		angle_ = xform.preMult(angle_);
		a1 = xform.preMult(a1);
		
		angular_vel_ = a1 - angle_;
	}
            
    inline float Particle::getMass() const
    {
        return mass_;
    }
    
    inline float Particle::getMassInv() const
    {
        return massinv_;
    }
    
    inline void Particle::setMass(float m)
    {
        mass_ = m;
        massinv_ = 1 / m;
    }
    
    inline void Particle::beginRender() const
    {
        switch (shape_)
        {
        case POINT:
            glBegin(GL_POINTS);
            break;
        case QUAD:
        case QUAD_OFFSET:
            glBegin(GL_QUADS);
            break;
		case LINE:
			glBegin(GL_LINES);
			break;
        default: ;
        }
    }
    
    inline void Particle::endRender() const
    {
        switch (shape_)
        {        
        case POINT:
        case QUAD:
        case QUAD_OFFSET:
		case LINE:
            glEnd();
            break;
        default: ;
        }
    }
    
    inline osg::Vec3 Particle::getCurrentSize() const
    {
        return current_size_;
    }
    
    inline void Particle::setTextureTile(int sTile, int tTile, int numTiles)
	{
		s_tile_ = 1.0f / static_cast<float>(sTile);
		t_tile_ = 1.0f / static_cast<float>(tTile);
		if (numTiles <= 0)
		{
			num_tile_ = sTile * tTile;
		}
		else
		{
			num_tile_ = numTiles;
		}
	}


}

#endif

