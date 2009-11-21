/** <pre>
 *  Sample HUD, using the CIGI symbology interface
 *  Copyright (c) 2009 The Boeing Company
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
 *  2009-08-31 Andrew Sampson
 *      Initial release
 *  
 *  
 *  </pre>
 */


#ifndef _HUDSTATE_H_
#define _HUDSTATE_H_

#include "Referenced.h"

//=========================================================
//! 
//! 
class HUDState : public mpv::Referenced
{
public:
	boost::signal<void (HUDState*)> headingChanged;
	boost::signal<void (HUDState*)> pitchChanged;
	boost::signal<void (HUDState*)> rollChanged;
	boost::signal<void (HUDState*)> airspeedKnotsChanged;
	boost::signal<void (HUDState*)> altitudeFeetChanged;
	boost::signal<void (HUDState*)> waypointHeadingChanged;
	boost::signal<void (HUDState*)> angleOfAttackChanged;
	boost::signal<void (HUDState*)> angleOfSideslipChanged;
	boost::signal<void (HUDState*)> angleOfIncidenceChanged;
	boost::signal<void (HUDState*)> machChanged;
	boost::signal<void (HUDState*)> geesChanged;
	boost::signal<void (HUDState*)> maxGeesChanged;

	//=========================================================
	//! General Constructor
	//! 
	HUDState();
	
	float getHeading() const { return heading; }
	float getPitch() const { return pitch; }
	float getRoll() const { return roll; }
	float getAirspeedKnots() const { return airspeedKnots; }
	float getAltitudeFeet() const { return altitudeFeet; }
	float getWaypointHeading() const { return waypointHeading; }
	float getAngleOfAttack() const { return angleOfAttack; }
	float getAngleOfSideslip() const { return angleOfSideslip; }
	float getAngleOfIncidence() const { return angleOfIncidence; }
	float getMach() const { return mach; }
	float getGees() const { return gees; }
	float getMaxGees() const { return maxGees; }
	
	void setHeading( float newHeading );
	void setPitch( float newPitch );
	void setRoll( float newRoll );
	void setAirspeedKnots( float newAirspeedKnots );
	void setAltitudeFeet( float newAltitudeFeet );
	void setWaypointHeading( float newWaypointHeading );
	void setAngleOfAttack( float newAngleOfAttack );
	void setAngleOfSideslip( float newAngleOfSideslip );
	void setAngleOfIncidence( float newAngleOfIncidence );
	void setMach( float newMach );
	void setGees( float newGees );
	void setMaxGees( float newMaxGees );
	
protected:
	
	//=========================================================
	//! General Destructor
	//! 
	virtual ~HUDState();
	
private:
	
	float heading;
	
	float pitch;
	float roll;
	
	float airspeedKnots;
	float altitudeFeet;
	
	float waypointHeading;
	
	float angleOfAttack;
	float angleOfSideslip;
	float angleOfIncidence;
	float mach;
	float gees;
	float maxGees;
	
	
	
};

#endif
