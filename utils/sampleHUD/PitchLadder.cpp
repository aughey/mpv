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


#include <iostream>
#include <sstream>
#include <iomanip>
#include <math.h>
#include <stdlib.h>

#include "BindSlot.h"

#include "PitchLadder.h"
#include "Misc.h"
#include "SerializableSymbolLine.h"
#include "SerializableSymbolText.h"


// ================================================
// PitchLadder
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PitchLadder::PitchLadder(CigiOutgoingMsg &message, HUDState *h ) : 
	SymbolSet( message ),
	hudState( h ),
	waterlinePosition( 4.4 )
{
	hudState->pitchChanged.connect( BIND_SLOT1( PitchLadder::pitchChanged, this ) );
	hudState->rollChanged.connect( BIND_SLOT1( PitchLadder::rollChanged, this ) );
}


// ================================================
// ~PitchLadder
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PitchLadder::~PitchLadder() 
{
	
}


void PitchLadder::initialize( InstanceIDPool &symbolIdPool )
{
	waterline = new SymbolLine();
	waterline->setID( symbolIdPool.getAvailableID() ); // fixme - error checking
	addSymbol( waterline.get() );
	waterline->addImplementation( new SerializableSymbolLine( waterline.get(), outgoingMessage ) );
	waterline->setState( Symbol::Visible );
	waterline->setSurfaceID( SURFACE_ID );
	waterline->setColor( Vect4( 0.0, 1.0, 0.0, 1.0 ) );
	waterline->setFlashDutyCyclePercentage( 100 );
	waterline->setPosition( 0.0, waterlinePosition );
	waterline->setRotation( 0.0 );

	// waterline is about 1.7 wide, 0.5 tall
	waterline->setPrimitiveType( SymbolLine::LineStrip );
	waterline->addVertex( -0.85,  0.0 );
	waterline->addVertex( -0.50,  0.0 );
	waterline->addVertex( -0.25, -0.5 );
	waterline->addVertex(  0.00,  0.0 );
	waterline->addVertex(  0.25, -0.5 );
	waterline->addVertex(  0.50,  0.0 );
	waterline->addVertex(  0.85,  0.0 );
	
	
	ladderRoll = new SymbolLine();
	ladderRoll->setID( symbolIdPool.getAvailableID() ); // fixme - error checking
	ladderRoll->setParent( true, waterline->getID(), waterline.get() );
	waterline->addSymbol( ladderRoll.get() );
	ladderRoll->addImplementation( new SerializableSymbolLine( ladderRoll.get(), outgoingMessage ) );
	ladderRoll->setState( Symbol::Visible );
	ladderRoll->setSurfaceID( SURFACE_ID );
	ladderRoll->setColor( Vect4( 0.0, 1.0, 0.0, 1.0 ) );
	ladderRoll->setFlashDutyCyclePercentage( 100 );
	ladderRoll->setPosition( 0.0, 0.0 );
	ladderRoll->setRotation( 0.0 );
	ladderRoll->setPrimitiveType( SymbolLine::Lines );
	// ladderRoll has no geometry
	
	
	ladderPitch = new SymbolLine();
	ladderPitch->setID( symbolIdPool.getAvailableID() ); // fixme - error checking
	ladderPitch->setParent( true, ladderRoll->getID(), ladderRoll.get() );
	ladderRoll->addSymbol( ladderPitch.get() );
	ladderPitch->addImplementation( new SerializableSymbolLine( ladderPitch.get(), outgoingMessage ) );
	ladderPitch->setState( Symbol::Visible );
	ladderPitch->setSurfaceID( SURFACE_ID );
	ladderPitch->setColor( Vect4( 0.0, 1.0, 0.0, 1.0 ) );
	ladderPitch->setFlashDutyCyclePercentage( 100 );
	ladderPitch->setPosition( 0.0, 0.0 );
	ladderPitch->setRotation( 0.0 );
	ladderPitch->setPrimitiveType( SymbolLine::Lines );
	// ladderPitch has no geometry
	
	
	int pitchLadderInterval = 5;
	for( int pitch = -90; pitch <= 90; pitch += pitchLadderInterval )
	{
		if( pitch == 90 || pitch == -90 )
		{
			// top and bottom are circles
			
			//ladderRungs[pitch] = 
		}
		else
		{
			ladderRungs[pitch] = 
				createLadderRung( symbolIdPool, ladderPitch.get(), pitch );
		}
	}
	
	
	pitchChanged( hudState.get() );
	rollChanged( hudState.get() );
}


Symbol * PitchLadder::createLadderRung( InstanceIDPool &symbolIdPool, Symbol *parent, int pitch )
{
	SymbolLine *rung = new SymbolLine();
	rung->setID( symbolIdPool.getAvailableID() ); // fixme - error checking
	rung->setParent( true, parent->getID(), parent );
	parent->addSymbol( rung );
	rung->addImplementation( new SerializableSymbolLine( rung, outgoingMessage ) );
	rung->setState( Symbol::Visible );
	rung->setSurfaceID( SURFACE_ID );
	rung->setColor( Vect4( 0.0, 1.0, 0.0, 1.0 ) );
	rung->setFlashDutyCyclePercentage( 100 );
	rung->setPosition( 0.0, 0.0 );
	rung->setRotation( 0.0 );
	rung->setPrimitiveType( SymbolLine::Lines );
	
	if( pitch == 0 )
	{
		// center rung is wider
		rung->addVertex( -4.9, pitch - 0.75 );
		rung->addVertex( -4.9, pitch );
		rung->addVertex( -4.9, pitch );
		rung->addVertex( -1.0, pitch );
		rung->addVertex(  1.0, pitch );
		rung->addVertex(  4.9, pitch );
		rung->addVertex(  4.9, pitch );
		rung->addVertex(  4.9, pitch - 0.75 );
	}
	else
	{
		if( pitch > 0 )
		{
			rung->addVertex( -3.6, pitch - 0.6 );
			rung->addVertex( -3.6, pitch );
			rung->addVertex( -3.6, pitch );
			rung->addVertex( -1.0, pitch );
			rung->addVertex(  1.0, pitch );
			rung->addVertex(  3.6, pitch );
			rung->addVertex(  3.6, pitch );
			rung->addVertex(  3.6, pitch - 0.6 );
		}
		else
		{
			rung->addVertex( -3.6, pitch + 0.6 );
			rung->addVertex( -3.6, pitch );
			rung->addVertex( -3.6, pitch );
			rung->addVertex( -1.0, pitch );
			rung->addVertex(  1.0, pitch );
			rung->addVertex(  3.6, pitch );
			rung->addVertex(  3.6, pitch );
			rung->addVertex(  3.6, pitch + 0.6 );
		}
		
		float textV = pitch;
		if( pitch > 0 )
			textV -= 0.3;
		else
			textV += 0.3;
		
		std::ostringstream oss;
		oss << abs( pitch );
		
		SymbolText *text = new SymbolText();
		text->setID( symbolIdPool.getAvailableID() ); // fixme - error checking
		text->setParent( true, rung->getID(), rung );
		rung->addSymbol( text );
		text->addImplementation( new SerializableSymbolText( text, outgoingMessage ) );
		text->setState( Symbol::Visible );
		text->setSurfaceID( SURFACE_ID );
		text->setColor( Vect4( 0.0, 1.0, 0.0, 1.0 ) );
		text->setFlashDutyCyclePercentage( 100 );
		text->setPosition( -3.7, textV );
		text->setRotation( 0.0 );
		text->setTextAlignment( SymbolText::CenterRight );
		text->setTextOrientation( SymbolText::LeftToRight );
		text->setFontID( 0 ); // fixme - fonts
		text->setFontSize( 1.35 );
		text->setText( oss.str() );
		
		text = new SymbolText();
		text->setID( symbolIdPool.getAvailableID() ); // fixme - error checking
		text->setParent( true, rung->getID(), rung );
		rung->addSymbol( text );
		text->addImplementation( new SerializableSymbolText( text, outgoingMessage ) );
		text->setState( Symbol::Visible );
		text->setSurfaceID( SURFACE_ID );
		text->setColor( Vect4( 0.0, 1.0, 0.0, 1.0 ) );
		text->setFlashDutyCyclePercentage( 100 );
		text->setPosition( 3.7, textV );
		text->setRotation( 0.0 );
		text->setTextAlignment( SymbolText::CenterLeft );
		text->setTextOrientation( SymbolText::LeftToRight );
		text->setFontID( 0 ); // fixme - fonts
		text->setFontSize( 1.35 );
		text->setText( oss.str() );

	}
	
	return rung;
}


void PitchLadder::updateMotion( double deltaT )
{
	
}


void PitchLadder::pitchChanged( HUDState * )
{
	// the coordinate system for this symbol surface is deliberately set up 
	// such that 1 unit equals approximately 1 degree of FOV
	ladderPitch->setPosition( 0.0, -hudState->getPitch() );
	
	// hide the rungs that are off-screen, or nearly so
	std::map< int, mpv::RefPtr<Symbol> >::iterator iter;
	for( iter = ladderRungs.begin(); iter != ladderRungs.end(); iter++ )
	{
		float rungDelta = iter->first + waterlinePosition - hudState->getPitch();
		if( rungDelta > 7. || rungDelta < -10. )
			iter->second->setState( Symbol::Hidden );
		else
			iter->second->setState( Symbol::Visible );
	}
}


void PitchLadder::rollChanged( HUDState * )
{
	ladderRoll->setRotation( hudState->getRoll() );
}

