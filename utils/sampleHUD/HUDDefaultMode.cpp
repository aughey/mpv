/** <pre>
 * Sample HUD, using the CIGI symbology interface
 * Copyright (c) 2008 Andrew Sampson
 * Copyright (c) 2009 The Boeing Company
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 * 
 * Revision history:
 * 
 * 2008-04-12  Andrew Sampson
 *     Initial version.  
 * 
 * 2009-08-31  Andrew Sampson
 *     Started work on sample HUD.  Based on symbologyStress utility.
 * 
 */

#include <math.h>

#include "HUDDefaultMode.h"
#include "Misc.h"
#include "AirspeedBox.h"
#include "AltitudeBox.h"
#include "HeadingCluster.h"
#include "LowerLeftCluster.h"
#include "PitchLadder.h"
#include "VelocityVector.h"

HUDDefaultMode::HUDDefaultMode( 
	CigiIncomingMsg &i, CigiOutgoingMsg &o, HUDState *h ) : Functor( i, o ), 
	hudState( h ), 
	initializedSymbols( false ), 
	absoluteTime( 0. )
{
	hudState->setAngleOfIncidence( 1.0 ); // seems reasonable for a fighter aircraft
}


HUDDefaultMode::~HUDDefaultMode()
{
	
}


bool HUDDefaultMode::operator()( double deltaT )
{
	absoluteTime += deltaT;
	
	frameRateMonitor.update( deltaT );
	
	if( !initializedSymbols )
	{
		initializeSymbols();
	}
	
	// update the hud state... just need to create some motion
	hudState->setAirspeedKnots( fabs( sin( absoluteTime ) ) * 300. );
	hudState->setMach( hudState->getAirspeedKnots() / 660. ); // bogus conversion
	hudState->setHeading( fmod( absoluteTime * 2., 360. ) );
//	hudState->setWaypointHeading( 360. - fmod( absoluteTime * 10., 360. ) );
	hudState->setAltitudeFeet( fabs( sin( absoluteTime * 0.1 ) ) * 30000. );
	hudState->setRoll( sin( absoluteTime * 0.2 ) * 20. );
	hudState->setPitch( sin( absoluteTime * 0.1 ) * 20. );
	hudState->setAngleOfAttack( (sin( absoluteTime * 0.3 ) * 5.) + 4. ); // varies -1 to +9
	hudState->setAngleOfSideslip( sin( absoluteTime * 0.05 ) * 2. );
	
	SymbolList::iterator iter = symbols.begin();
	for( ; iter != symbols.end(); iter++ )
	{
		(*iter)->update( deltaT );
	}
	
	// return true to continue calling this functor
	return true;
}


void HUDDefaultMode::initializeSymbols()
{
	HeadingCluster *headingCluster = new HeadingCluster( outgoing, hudState.get() );
	headingCluster->initialize( symbolIdPool );
	symbols.push_back( headingCluster );
	
	AirspeedBox *airspeedBox = new AirspeedBox( outgoing, hudState.get() );
	airspeedBox->initialize( symbolIdPool );
	symbols.push_back( airspeedBox );
	
	AltitudeBox *altitudeBox = new AltitudeBox( outgoing, hudState.get() );
	altitudeBox->initialize( symbolIdPool );
	symbols.push_back( altitudeBox );
	
	PitchLadder *pitchLadder = new PitchLadder( outgoing, hudState.get() );
	pitchLadder->initialize( symbolIdPool );
	symbols.push_back( pitchLadder );
	
	LowerLeftCluster *lowerLeftCluster = new LowerLeftCluster( outgoing, hudState.get() );
	lowerLeftCluster->initialize( symbolIdPool );
	symbols.push_back( lowerLeftCluster );
	
	VelocityVector *velocityVector = new VelocityVector( outgoing, hudState.get() );
	velocityVector->initialize( symbolIdPool );
	symbols.push_back( velocityVector );
	
	initializedSymbols = true;
}


void HUDDefaultMode::deleteSymbol( SymbolSet *symbol )
{
	
	symbols.remove( symbol );
}


