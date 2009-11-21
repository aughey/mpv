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


#include <sstream>
#include <iomanip>
#include <math.h>

#include "BindSlot.h"

#include "AltitudeBox.h"
#include "Misc.h"
#include "SerializableSymbolLine.h"
#include "SerializableSymbolText.h"

// ================================================
// AltitudeBox
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
AltitudeBox::AltitudeBox(CigiOutgoingMsg &message, HUDState *h ) : 
	SymbolSet( message ),
	hudState( h )
{
	hudState->altitudeFeetChanged.connect( BIND_SLOT1( AltitudeBox::altitudeChanged, this ) );
}


// ================================================
// ~AltitudeBox
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
AltitudeBox::~AltitudeBox() 
{
	
}


void AltitudeBox::initialize( InstanceIDPool &symbolIdPool )
{
	box = new SymbolLine();
	box->setID( symbolIdPool.getAvailableID() ); // fixme - error checking
	addSymbol( box.get() );
	box->addImplementation( new SerializableSymbolLine( box.get(), outgoingMessage ) );
	box->setState( Symbol::Visible );
	box->setSurfaceID( SURFACE_ID );
	box->setColor( Vect4( 0.0, 1.0, 0.0, 1.0 ) );
	box->setFlashDutyCyclePercentage( 100 );
	box->setPosition( 7.5, 4.0 );
	box->setRotation( 0.0 );
    
	box->setPrimitiveType( SymbolLine::LineLoop );
	box->addVertex( -2.3, -0.4 );
	box->addVertex(  0.0, -0.4 );
	box->addVertex(  0.0,  0.4 );
	box->addVertex( -2.3,  0.4 );

	
	textThousands = new SymbolText();
	textThousands->setID( symbolIdPool.getAvailableID() ); // fixme - error checking
	textThousands->setParent( true, box->getID(), box.get() );
	box->addSymbol( textThousands.get() );
	textThousands->addImplementation( new SerializableSymbolText( textThousands.get(), outgoingMessage ) );
	textThousands->setState( Symbol::Visible );
	textThousands->setSurfaceID( SURFACE_ID );
	textThousands->setColor( Vect4( 0.0, 1.0, 0.0, 1.0 ) );
	textThousands->setFlashDutyCyclePercentage( 100 );
	textThousands->setPosition( -1.2, 0.0 );
	textThousands->setRotation( 0.0 );
	textThousands->setTextAlignment( SymbolText::CenterRight );
	textThousands->setTextOrientation( SymbolText::LeftToRight );
	textThousands->setFontID( 0 ); // fixme - fonts
	textThousands->setFontSize( 1.5 );
	textThousands->setText( "XX" );
	
	textRemainder = new SymbolText();
	textRemainder->setID( symbolIdPool.getAvailableID() ); // fixme - error checking
	textRemainder->setParent( true, box->getID(), box.get() );
	box->addSymbol( textRemainder.get() );
	textRemainder->addImplementation( new SerializableSymbolText( textRemainder.get(), outgoingMessage ) );
	textRemainder->setState( Symbol::Visible );
	textRemainder->setSurfaceID( SURFACE_ID );
	textRemainder->setColor( Vect4( 0.0, 1.0, 0.0, 1.0 ) );
	textRemainder->setFlashDutyCyclePercentage( 100 );
	textRemainder->setPosition( -0.1, 0.06 );
	textRemainder->setRotation( 0.0 );
	textRemainder->setTextAlignment( SymbolText::CenterRight );
	textRemainder->setTextOrientation( SymbolText::LeftToRight );
	textRemainder->setFontID( 0 ); // fixme - fonts
	textRemainder->setFontSize( 1.35 );
	textRemainder->setText( "XXX" );
	
	textDebug = new SymbolText();
	textDebug->setID( symbolIdPool.getAvailableID() ); // fixme - error checking
	addSymbol( textDebug.get() );
	textDebug->addImplementation( new SerializableSymbolText( textDebug.get(), outgoingMessage ) );
	textDebug->setState( Symbol::Visible );
	textDebug->setSurfaceID( SURFACE_ID );
	textDebug->setColor( Vect4( 0.0, 1.0, 0.0, 1.0 ) );
	textDebug->setFlashDutyCyclePercentage( 100 );
	textDebug->setPosition( 6.6, 5.0 );
	textDebug->setRotation( 0.0 );
	textDebug->setTextAlignment( SymbolText::Center );
	textDebug->setTextOrientation( SymbolText::LeftToRight );
	textDebug->setFontID( 0 ); // fixme - fonts
	textDebug->setFontSize( 1.5 );
	textDebug->setText( "XXXXX" );
	
}


void AltitudeBox::updateMotion( double deltaT )
{
	
}


void AltitudeBox::altitudeChanged( HUDState * )
{
	float altitude = hudState->getAltitudeFeet();
	std::ostringstream oss;
	
	oss.precision( 0 );
	
//	oss << std::fixed << floor( altitude / 1000.0f );
	// fixme - this isn't quite right... rollovers aren't instantaneous/correct
	oss << (int)altitude / 1000;
	textThousands->setText( oss.str() );
	
	// flush/clear oss
	oss.str( "" );
	
	oss << std::setfill( '0' ) << setw( 3 ) << std::fixed 
		<< fmodf( altitude, 1000.0f );
	textRemainder->setText( oss.str() );
	
	// flush/clear oss
	oss.str( "" );
	
	oss << std::fixed << altitude;
	textDebug->setText( oss.str() );
}


