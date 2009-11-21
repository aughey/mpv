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

#include "BindSlot.h"

#include "HeadingCluster.h"
#include "Misc.h"
#include "SerializableSymbolLine.h"
#include "SerializableSymbolText.h"


// ================================================
// HeadingCluster
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
HeadingCluster::HeadingCluster(CigiOutgoingMsg &message, HUDState *h ) : 
	SymbolSet( message ),
	hudState( h ),
	ticksRange( 40. ),
	degreesPerTick( 2. ),
	degreesPerTextTick( 10. ),
	barWidth( 12. )
{
	hudState->headingChanged.connect( BIND_SLOT1( HeadingCluster::headingChanged, this ) );
	hudState->waypointHeadingChanged.connect( BIND_SLOT1( HeadingCluster::waypointHeadingChanged, this ) );
}


// ================================================
// ~HeadingCluster
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
HeadingCluster::~HeadingCluster() 
{
	
}


int HeadingCluster::getSymbolID( InstanceIDPool &symbolIdPool ) const
{
	int symbolID = symbolIdPool.getAvailableID();
	if( symbolID > 0xffff )
	{
		symbolIdPool.relenquishID( symbolID );
		return -1;
	}
	return symbolID;
}
	
	
void HeadingCluster::initialize( InstanceIDPool &symbolIdPool )
{
	int topBoxID = getSymbolID( symbolIdPool );
	if( topBoxID < 0 )
	{
		std::cerr << "In HeadingCluster::initialize - no more available symbol IDs\n";
		return;
	}
	
	int topTextID = getSymbolID( symbolIdPool );
	if( topTextID < 0 )
	{
		std::cerr << "In HeadingCluster::initialize - no more available symbol IDs\n";
		return;
	}
	
	int debugOutlineID = getSymbolID( symbolIdPool );
	if( debugOutlineID < 0 )
	{
		std::cerr << "In HeadingCluster::initialize - no more available symbol IDs\n";
		return;
	}
	
	topBox = new SymbolLine();
	topBox->setID( topBoxID );
	addSymbol( topBox.get() );
	topBox->addImplementation( new SerializableSymbolLine( topBox.get(), outgoingMessage ) );
	topBox->setState( Symbol::Visible );
	topBox->setSurfaceID( SURFACE_ID );
	topBox->setColor( Vect4( 0.0, 1.0, 0.0, 1.0 ) );
	topBox->setFlashDutyCyclePercentage( 100 );
	topBox->setPosition( 0.0, 9.0 );
	topBox->setRotation( 0.0 );

	topBox->setPrimitiveType( SymbolLine::LineLoop );
	topBox->addVertex( -0.85, -0.45 );
	topBox->addVertex(  0.85, -0.45 );
	topBox->addVertex(  0.85,  0.45 );
	topBox->addVertex( -0.85,  0.45 );

	
	topText = new SymbolText();
	topText->setID( topTextID );
	topText->setParent( true, topBox->getID(), topBox.get() );
	addSymbol( topText.get() );
	topText->addImplementation( new SerializableSymbolText( topText.get(), outgoingMessage ) );
	topText->setState( Symbol::Visible );
	topText->setSurfaceID( SURFACE_ID );
	topText->setColor( Vect4( 0.0, 1.0, 0.0, 1.0 ) );
	topText->setFlashDutyCyclePercentage( 100 );
	topText->setPosition( 0.0, 0.0 );
	topText->setRotation( 0.0 );
	
	topText->setTextAlignment( SymbolText::Center );
	topText->setTextOrientation( SymbolText::LeftToRight );
	topText->setFontID( 0 ); // fixme - fonts
	topText->setFontSize( 1.5 );
	topText->setText( "XXX" );
	
	
	barLine = new SymbolLine();
	barLine->setID( symbolIdPool.getAvailableID() ); // fixme - error checking
	addSymbol( barLine.get() );
	barLine->addImplementation( new SerializableSymbolLine( barLine.get(), outgoingMessage ) );
	barLine->setState( Symbol::Visible );
	barLine->setSurfaceID( SURFACE_ID );
	barLine->setColor( Vect4( 0.0, 1.0, 0.0, 1.0 ) );
	barLine->setFlashDutyCyclePercentage( 100 );
	barLine->setPosition( 0.0, 7.0 );
	barLine->setRotation( 0.0 );
    
	barLine->setPrimitiveType( SymbolLine::Lines );
	// the bar itself
	barLine->addVertex( -barWidth / 2., 0. );
	barLine->addVertex(  barWidth / 2., 0. );
	// the caret below the bar
	barLine->addVertex( 0., 0. );
	barLine->addVertex( -0.3, -0.5 );
	barLine->addVertex( 0., 0. );
	barLine->addVertex( 0.3, -0.5 );

	
	barTicks = new SymbolLine();
	barTicks->setID( symbolIdPool.getAvailableID() ); // fixme - error checking
	barTicks->setParent( true, barLine->getID(), barLine.get() );
	barLine->addSymbol( barTicks.get() );
	barTicks->addImplementation( new SerializableSymbolLine( barTicks.get(), outgoingMessage ) );
	barTicks->setState( Symbol::Visible );
	barTicks->setSurfaceID( SURFACE_ID );
	barTicks->setColor( Vect4( 0.0, 1.0, 0.0, 1.0 ) );
	barTicks->setFlashDutyCyclePercentage( 100 );
	barTicks->setPosition( 0.0, 0.0 );
	barTicks->setRotation( 0.0 );
	barTicks->setPrimitiveType( SymbolLine::LineLoop );
//barTicks->addVertex( 0., 0.0 );
//barTicks->addVertex( -0.18, 0.18 );
//barTicks->addVertex( 0.18, 0.18 );
	
	float tickInterval = barWidth / ( ticksRange / degreesPerTick );
	int numVertices = 0;
	SymbolLine *tickChild = NULL;
	// limited to 30 vertices per line symbol; need to split up ticks into 
	// multiple symbols
	for( float x = -barWidth / 2.; x < barWidth / 2.; x += tickInterval )
	{
		// note - actually limited to 28 vertices, due to CCL bug
		if( tickChild == NULL || numVertices >= 26 )
		{
			tickChild = new SymbolLine();
			tickChild->setID( symbolIdPool.getAvailableID() ); // fixme - error checking
			tickChild->setParent( true, barTicks->getID(), barTicks.get() );
			barTicks->addSymbol( tickChild );
			tickChild->addImplementation( new SerializableSymbolLine( tickChild, outgoingMessage ) );
			tickChild->setState( Symbol::Visible );
			tickChild->setSurfaceID( SURFACE_ID );
			tickChild->setColor( Vect4( 0.0, 1.0, 0.0, 1.0 ) );
			tickChild->setFlashDutyCyclePercentage( 100 );
			tickChild->setPosition( 0.0, 0.0 );
			tickChild->setRotation( 0.0 );
			tickChild->setPrimitiveType( SymbolLine::Lines );
			
			numVertices = 0;
		}

		tickChild->addVertex( x, 0.0 );
		numVertices++;
		tickChild->addVertex( x, 0.18 );
		numVertices++;
	}

	
	textTicks = new SymbolText();
	textTicks->setID( symbolIdPool.getAvailableID() ); // fixme - error checking
	textTicks->setParent( true, barLine->getID(), barLine.get() );
	barLine->addSymbol( textTicks.get() );
	textTicks->addImplementation( new SerializableSymbolText( textTicks.get(), outgoingMessage ) );
	textTicks->setState( Symbol::Visible );
	textTicks->setSurfaceID( SURFACE_ID );
	textTicks->setColor( Vect4( 0.0, 1.0, 0.0, 1.0 ) );
	textTicks->setFlashDutyCyclePercentage( 100 );
	textTicks->setPosition( 0.0, 0.0 );
	textTicks->setRotation( 0.0 );
	
	// list of text symbols for numbers above bar ticks
	tickInterval = barWidth / ( ticksRange / degreesPerTextTick );
	for( float x = -barWidth / 2.; x < barWidth / 2.; x += tickInterval )
	{
		SymbolText *text = new SymbolText();
		text->setID( symbolIdPool.getAvailableID() ); // fixme - error checking
		text->setParent( true, textTicks->getID(), textTicks.get() );
		textTicks->addSymbol( text );
		text->addImplementation( new SerializableSymbolText( text, outgoingMessage ) );
		text->setState( Symbol::Visible );
		text->setSurfaceID( SURFACE_ID );
		text->setColor( Vect4( 0.0, 1.0, 0.0, 1.0 ) );
		text->setFlashDutyCyclePercentage( 100 );
		text->setPosition( x, 0.25 );
		text->setRotation( 0.0 );
		
		text->setTextAlignment( SymbolText::BottomCenter );
		text->setTextOrientation( SymbolText::LeftToRight );
		text->setFontID( 0 ); // fixme - fonts
		text->setFontSize( 1.2 );
		text->setText( "XXX" );
		
		textTicksList.push_back( text );
	}

	
	waypointTick = new SymbolLine();
	waypointTick->setID( symbolIdPool.getAvailableID() ); // fixme - error checking
	waypointTick->setParent( true, barLine->getID(), barLine.get() );
	barLine->addSymbol( waypointTick.get() );
	waypointTick->addImplementation( new SerializableSymbolLine( waypointTick.get(), outgoingMessage ) );
	waypointTick->setState( Symbol::Visible );
	waypointTick->setSurfaceID( SURFACE_ID );
	waypointTick->setColor( Vect4( 0.0, 1.0, 0.0, 1.0 ) );
	waypointTick->setFlashDutyCyclePercentage( 100 );
	waypointTick->setPosition( 0.0, 0.0 );
	waypointTick->setRotation( 0.0 );
    
	waypointTick->setPrimitiveType( SymbolLine::TriangleFan );
	waypointTick->addVertex( -0.1,  0.0 );
	waypointTick->addVertex( -0.1, -0.6 );
	waypointTick->addVertex(  0.1, -0.6 );
	waypointTick->addVertex(  0.1,  0.0 );

	

	debugOutline = new SymbolLine();
	debugOutline->setID( debugOutlineID );
	addSymbol( debugOutline.get() );
	debugOutline->addImplementation( new SerializableSymbolLine( debugOutline.get(), outgoingMessage ) );
	debugOutline->setState( Symbol::Visible );
	debugOutline->setSurfaceID( SURFACE_ID );
	debugOutline->setColor( Vect4( 0.0, 1.0, 0.0, 1.0 ) );
	debugOutline->setFlashDutyCyclePercentage( 100 );
	debugOutline->setPosition( 0.0, 0.0 );
	debugOutline->setRotation( 0.0 );

	debugOutline->setPrimitiveType( SymbolLine::LineLoop );
	debugOutline->addVertex( SURFACE_MIN_U, SURFACE_MIN_V );
	debugOutline->addVertex( SURFACE_MAX_U, SURFACE_MIN_V );
	debugOutline->addVertex( SURFACE_MAX_U, SURFACE_MAX_V );
	debugOutline->addVertex( SURFACE_MIN_U, SURFACE_MAX_V );
}


void HeadingCluster::updateMotion( double deltaT )
{
//	barTicks->setRotation( barTicks->getRotation() + deltaT * 30. );
	
}


void HeadingCluster::headingChanged( HUDState * )
{
	std::ostringstream oss;
	
	oss.precision( 0 );
	oss << std::setfill( ' ' ) << setw( 3 ) << std::fixed 
		<< hudState->getHeading();
	
	topText->setText( oss.str() );
	// flush/clear oss
	oss.str( "" );
	
	barTicks->setPosition( 
		(barWidth / ticksRange) * -1. * fmodf( hudState->getHeading(), degreesPerTick ), 0.0 );
	
	textTicks->setPosition( 
		(barWidth / ticksRange) * -1. * fmodf( hudState->getHeading(), degreesPerTextTick ), 0.0 );
	
	// when the heading changes, the waypoint indicator is affected
	waypointHeadingChanged( hudState.get() );
}


void HeadingCluster::waypointHeadingChanged( HUDState * )
{
	float delta = hudState->getWaypointHeading() - hudState->getHeading();
	if( delta > ticksRange / 2. )
		delta = ticksRange / 2.;
	if( delta < -ticksRange / 2. )
		delta = -ticksRange / 2.;
	
	waypointTick->setPosition( delta * barWidth / ticksRange, 0.0 );
}

