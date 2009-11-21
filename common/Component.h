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


#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include <string>
#include <list>

#include <CigiTypes.h>
#include <CigiCompCtrlV3_3.h>
#include <CigiShortCompCtrlV3_3.h>

#include "Referenced.h"
#include "MPVCommonTypes.h"


namespace mpv
{

// forward declaration; defined at bottom of file
class ComponentImp;


//=========================================================
//! This class encapsulates the CIGI Component Control concept.
//! 
class MPVCMN_SPEC Component : public Referenced
{
public:
	boost::signal<void (Component*)> nameChanged;
	boost::signal<void (Component*)> stateChanged;
	boost::signal<void (Component*)> dataChanged;
	//! emitted only when a packet is received; useful if you are interested 
	//! only in being notified only of complete changes, rather than piecemeal 
	//! changes
	boost::signal<void (Component*)> packetReceived;
	
	//=========================================================
	//! General Constructor
	//! 
	Component();
	
	//=========================================================
	//! The container for this component will call this method once per frame
	//! \param timeElapsed the time elapsed since the last call, in seconds
	//! 
	virtual void update( double timeElapsed );

	void processPacket( CigiCompCtrlV3_3 *packet );
	void processPacket( CigiShortCompCtrlV3_3 *packet );

	void setID( Cigi_uint16 newID );
	void setInstanceID( Cigi_uint16 newInstanceID );
	void setName( const std::string &newName );
	void setState( Cigi_uint8 newState );

	Cigi_uint16 getID() const { return id; }
	Cigi_uint16 getInstanceID() const { return instanceID; }
	const std::string &getName() const { return name; }
	Cigi_uint8 getState() const { return state; }
	
	void addImplementation( ComponentImp *newImp );
	
protected:
	//=========================================================
	//! General Destructor
	//! 
	virtual ~Component();
	
	//=========================================================
	//! The ID of this component; must be unique among the components 
	//! in this component's container
	//!
	Cigi_uint16 id;

	//=========================================================
	//! The ID of whatever this component is associated with.
	//! Could be an entity ID, could be a symbol ID, could be ...
	//!
	Cigi_uint16 instanceID;

	//=========================================================
	//! A human-readable name for the component; can be left blank
	//!
	std::string name;
	
	//=========================================================
	//! The current state value of this component.  The meaning of 
	//! the values this can be set to is component- and 
	//! application-specific.
	//!
	Cigi_uint8 state;

	//=========================================================
	//! The current data array for this component.  As with the "state" 
	//! field, the meaning of the values this can be set to is component- 
	//! and application-specific.  Furthermore, this data array can be 
	//! divided up in many different ways.  For example, it could be treated 
	//! as a sequence of bytes, or as a set of 6 32-bit integers, or as a 
	//! set of floats, or as a mixture of several different types.
	//!
	Cigi_uint8 data[24];
	
	//=========================================================
	//! Implementation objects for this Component.  
	//!
	std::list< RefPtr<ComponentImp> > imps;
	
};


//=========================================================
//! An implementation class, which complements Component.
//! Those seeking to do interesting things with Components should 
//! inherit from this class.
//! 
class MPVCMN_SPEC ComponentImp : public Referenced
{
public:
	//=========================================================
	//! General Constructor
	//! 
	ComponentImp( Component *_component ) : 
		Referenced(), 
		component( _component )
	{}
	

	virtual void update( double timeElapsed ) {}

protected:
	//=========================================================
	//! General Destructor
	//! 
	virtual ~ComponentImp() {}
	
	//=========================================================
	//! Pointer to the owning Component.  
	//!
	Component *component;
};

}
#endif
