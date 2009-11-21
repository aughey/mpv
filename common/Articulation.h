/** <pre>
 *  The MPV Common Library
 *  Copyright (c) 2008 
 *  
 *  This component is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; specifically 
 *  version 2.1 of the License.
 *  
 *  This software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *  
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this software; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *  
 *  
 *  2008-07-06 Andrew Sampson
 *      Initial release
 *  
 *  
 *  </pre>
 */


#ifndef _ARTICULATION_H_
#define _ARTICULATION_H_

#include <string>
#include <list>

#include <CigiTypes.h>

#include "Referenced.h"
#include "MPVCommonTypes.h"
#include "Vect3.h"


namespace mpv
{

// forward declaration; defined at bottom of file
class ArticulationImp;


//=========================================================
//! This class encapsulates the CIGI Articulated Part concept.
//! 
class MPVCMN_SPEC Articulation : public Referenced
{
public:
	boost::signal<void (Articulation*)> nameChanged;
	boost::signal<void (Articulation*)> enabledChanged;
	boost::signal<void (Articulation*)> offsetChanged;
	boost::signal<void (Articulation*)> rotationChanged;
	boost::signal<void (Articulation*)> offsetVelocityChanged;
	boost::signal<void (Articulation*)> rotationVelocityChanged;
	
	//=========================================================
	//! General Constructor
	//! 
	Articulation();
	
	//=========================================================
	//! The container for this articulation will call this method once per frame
	//! \param timeElapsed the time elapsed since the last call, in seconds
	//! 
	virtual void update( double timeElapsed );

	void setID( Cigi_uint16 newID );
	void setEntityID( Cigi_uint16 newEntityID );
	void setName( const std::string &newName );
	void setEnabled( bool newEnabled );
	void setOffset( const Vect3 &newOffset );
	void setRotation( const Vect3 &newRotation );
	void setOffsetVelocity( const Vect3 &newOffsetVelocity );
	void setRotationVelocity( const Vect3 &newRotationVelocity );

	Cigi_uint16 getID() const { return id; }
	Cigi_uint16 getEntityID() const { return entityID; }
	const std::string &getName() const { return name; }
	bool getEnabled() const { return enabled; }
	Vect3 getOffset() const { return offset; }
	Vect3 getRotation() const { return rotation; }
	Vect3 getOffsetVelocity() const { return offsetVelocity; }
	Vect3 getRotationVelocity() const { return rotationVelocity; }
	
	void addImplementation( ArticulationImp *newImp );
	
protected:
	//=========================================================
	//! General Destructor
	//! 
	virtual ~Articulation();
	
	//=========================================================
	//! The ID of this articulation; must be unique among the articulations 
	//! in this articulation's container
	//!
	Cigi_uint16 id;

	//=========================================================
	//! The ID of whatever entity this articulation is associated with.
	//!
	Cigi_uint16 entityID;

	//=========================================================
	//! A human-readable name for the articulation; can be left blank
	//!
	std::string name;
	
	//=========================================================
	//! Enables/disables this articulated part.
	//!
	bool enabled;

	//=========================================================
	//! Articulation translation.  Order is north east down, units are meters.
	//! (Re. north-east-down - There's a lot of leeway given for 
	//! articulation reference frames.  The reference frame is largely 
	//! dictated by the artist who created the 3D entity model.)
	//! 
	Vect3 offset;
	
	//=========================================================
	//! Articulation orientation.  Order is yaw pitch roll, units are degrees.
	//! (Re. orientation - There's a lot of leeway given for 
	//! articulation reference frames.  The reference frame is largely 
	//! dictated by the artist who created the 3D entity model.)
	//! 
	Vect3 rotation;
	
	//=========================================================
	//! Articulation translation velocity.  Order is north east down, units 
	//! are meters-per-second.
	//! 
	Vect3 offsetVelocity;
	
	//=========================================================
	//! Articulation orientation.  Order is yaw pitch roll, units are 
	//! degrees-per-second.
	//! 
	Vect3 rotationVelocity;
	
	//=========================================================
	//! If clamping is enabled, then this variable stores the minimum 
	//! permitted value for each of x, y, and z offset.
	//! Order is north east down, units are meters.
	//! 
	Vect3 offsetMin;

	//=========================================================
	//! If clamping is enabled, then this variable stores the maximum 
	//! permitted value for each of x, y, and z offset.
	//! Order is north east down, units are meters.
	//! 
	Vect3 offsetMax;

	//=========================================================
	//! If clamping is enabled, then this variable stores the minimum 
	//! permitted value for each of yaw, pitch, and roll rotation.
	//! Order is yaw pitch roll, units are degrees.
	//! 
	Vect3 rotationMin;

	//=========================================================
	//! If clamping is enabled, then this variable stores the maximum 
	//! permitted value for each of yaw, pitch, and roll rotation.
	//! Order is yaw pitch roll, units are degrees.
	//! 
	Vect3 rotationMax;

	//=========================================================
	//! Implementation objects for this Articulation.  
	//!
	std::list< RefPtr<ArticulationImp> > imps;
	
};


//=========================================================
//! An implementation class, which complements Articulation.
//! Those seeking to do interesting things with Articulations should 
//! inherit from this class.
//! 
class MPVCMN_SPEC ArticulationImp : public Referenced
{
public:
	//=========================================================
	//! General Constructor
	//! 
	ArticulationImp( Articulation *_articulation ) : 
		Referenced(), 
		articulation( _articulation )
	{}
	

	virtual void update( double timeElapsed ) {}

protected:
	//=========================================================
	//! General Destructor
	//! 
	virtual ~ArticulationImp() {}
	
	//=========================================================
	//! Pointer to the owning Articulation.  
	//!
	Articulation *articulation;
};

}
#endif
