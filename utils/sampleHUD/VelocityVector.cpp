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


#include "BindSlot.h"

#include "VelocityVector.h"

#include "Misc.h"
#include "SerializableSymbolCircle.h"
#include "SerializableSymbolLine.h"

// ================================================
// VelocityVector
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
VelocityVector::VelocityVector(CigiOutgoingMsg &message, HUDState *h ) : 
	SymbolSet( message ),
	hudState( h ),
	pipRadius( 0.25 ),
	waterlinePosition( 4.4 ) // fixme - sync this value with the one in PitchLadder (or change the coordinate system, such that the waterline is at 0,0)
{
	hudState->angleOfAttackChanged.connect( BIND_SLOT1( VelocityVector::velocityVectorPositionChanged, this ) );
	hudState->angleOfSideslipChanged.connect( BIND_SLOT1( VelocityVector::velocityVectorPositionChanged, this ) );
	hudState->angleOfIncidenceChanged.connect( BIND_SLOT1( VelocityVector::velocityVectorPositionChanged, this ) );
}


// ================================================
// ~VelocityVector
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
VelocityVector::~VelocityVector() 
{
	
}


void VelocityVector::initialize( InstanceIDPool &symbolIdPool )
{
	waterlineReference = new SymbolLine();
	waterlineReference->setID( symbolIdPool.getAvailableID() ); // fixme - error checking
	addSymbol( waterlineReference.get() );
	waterlineReference->addImplementation( new SerializableSymbolLine( waterlineReference.get(), outgoingMessage ) );
	waterlineReference->setState( Symbol::Visible );
	waterlineReference->setSurfaceID( SURFACE_ID );
	waterlineReference->setColor( Vect4( 0.0, 1.0, 0.0, 1.0 ) );
	waterlineReference->setFlashDutyCyclePercentage( 100 );
	waterlineReference->setPosition( 0.0, waterlinePosition );
	waterlineReference->setRotation( 0.0 );
	// waterlineReference has no geometry


	pip = new SymbolCircle();
	pip->setID( symbolIdPool.getAvailableID() ); // fixme - error checking
	pip->setParent( true, waterlineReference->getID(), waterlineReference.get() );
	waterlineReference->addSymbol( pip.get() );
	pip->addImplementation( new SerializableSymbolCircle( pip.get(), outgoingMessage ) );
	pip->setState( Symbol::Visible );
	pip->setSurfaceID( SURFACE_ID );
	pip->setColor( Vect4( 0.0, 1.0, 0.0, 1.0 ) );
	pip->setFlashDutyCyclePercentage( 100 );
	pip->setPosition( 0.0, 0.0 );
	pip->setRotation( 0.0 );

	pip->setDrawingStyle( SymbolCircle::Line );
	SymbolCircle::Circle circle;
	circle.centerPosition.Set( 0., 0. );
	circle.radius = pipRadius;
	circle.innerRadius = 0.; // unused
	circle.startAngle = 0.;
	circle.endAngle = 0.;
	pip->addCircle( circle );
	
	
	SymbolLine *line = new SymbolLine();
	line->setID( symbolIdPool.getAvailableID() ); // fixme - error checking
	line->setParent( true, pip->getID(), pip.get() );
	pip->addSymbol( line );
	line->addImplementation( new SerializableSymbolLine( line, outgoingMessage ) );
	line->setState( Symbol::Visible );
	line->setSurfaceID( SURFACE_ID );
	line->setColor( Vect4( 0.0, 1.0, 0.0, 1.0 ) );
	line->setFlashDutyCyclePercentage( 100 );
	line->setPosition( 0., 0. );
	line->setRotation( 0.0 );
	line->setPrimitiveType( SymbolLine::Lines );
	
	line->addVertex( 0.0, pipRadius );
	line->addVertex( 0.0, pipRadius + 0.6 );
	
	line->addVertex( -pipRadius, 0. );
	line->addVertex( -pipRadius - 0.7, 0. );
	
	line->addVertex( pipRadius, 0. );
	line->addVertex( pipRadius + 0.7, 0. );

	velocityVectorPositionChanged( hudState.get() );
}


void VelocityVector::updateMotion( double deltaT )
{
	
}


void VelocityVector::velocityVectorPositionChanged( HUDState * )
{
	pip->setPosition( 
		-1. * hudState->getAngleOfSideslip(), 
		hudState->getAngleOfIncidence() - hudState->getAngleOfAttack() );
}
