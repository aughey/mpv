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


#include "HUDState.h"

using namespace mpv;

// ================================================
// HUDState
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
HUDState::HUDState() : Referenced(),
	heading( 0. ),
	pitch( 0. ),
	roll( 0. ),
	airspeedKnots( 0. ),
	altitudeFeet( 0. ),
	waypointHeading( 0. ),
	angleOfAttack( 0. ),
	angleOfSideslip( 0. ),
	angleOfIncidence( 0. ),
	mach( 0. ),
	gees( 1. ),
	maxGees( 7.5 )
{
	
}


// ================================================
// ~HUDState
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
HUDState::~HUDState() 
{
	
}


void HUDState::setHeading( float newHeading )
{
	if( newHeading != heading )
	{
		heading = newHeading;
		headingChanged( this );
	}
}


void HUDState::setPitch( float newPitch )
{
	if( newPitch != pitch )
	{
		pitch = newPitch;
		pitchChanged( this );
	}
}


void HUDState::setRoll( float newRoll )
{
	if( newRoll != roll )
	{
		roll = newRoll;
		rollChanged( this );
	}
}


void HUDState::setAirspeedKnots( float newAirspeedKnots )
{
	if( newAirspeedKnots != airspeedKnots )
	{
		airspeedKnots = newAirspeedKnots;
		airspeedKnotsChanged( this );
	}
}


void HUDState::setAltitudeFeet( float newAltitudeFeet )
{
	if( newAltitudeFeet != altitudeFeet )
	{
		altitudeFeet = newAltitudeFeet;
		altitudeFeetChanged( this );
	}
}


void HUDState::setWaypointHeading( float newWaypointHeading )
{
	if( newWaypointHeading != waypointHeading )
	{
		waypointHeading = newWaypointHeading;
		waypointHeadingChanged( this );
	}
}


void HUDState::setAngleOfAttack( float newAngleOfAttack )
{
	if( newAngleOfAttack != angleOfAttack )
	{
		angleOfAttack = newAngleOfAttack;
		angleOfAttackChanged( this );
	}
}


void HUDState::setAngleOfSideslip( float newAngleOfSideslip )
{
	if( newAngleOfSideslip != angleOfSideslip )
	{
		angleOfSideslip = newAngleOfSideslip;
		angleOfSideslipChanged( this );
	}
}


void HUDState::setAngleOfIncidence( float newAngleOfIncidence )
{
	if( newAngleOfIncidence != angleOfIncidence )
	{
		angleOfIncidence = newAngleOfIncidence;
		angleOfIncidenceChanged( this );
	}
}


void HUDState::setMach( float newMach )
{
	if( newMach != mach )
	{
		mach = newMach;
		machChanged( this );
	}
}


void HUDState::setGees( float newGees )
{
	if( newGees != gees )
	{
		gees = newGees;
		geesChanged( this );
	}
}


void HUDState::setMaxGees( float newMaxGees )
{
	if( newMaxGees != maxGees )
	{
		maxGees = newMaxGees;
		maxGeesChanged( this );
	}
}

