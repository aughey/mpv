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

#include "BindSlot.h"

#include "AirspeedBox.h"
#include "Misc.h"
#include "SerializableSymbolLine.h"
#include "SerializableSymbolText.h"

// ================================================
// AirspeedBox
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
AirspeedBox::AirspeedBox(CigiOutgoingMsg &message, HUDState *h ) : 
	SymbolSet( message ),
	hudState( h )
{
	hudState->airspeedKnotsChanged.connect( BIND_SLOT1( AirspeedBox::airspeedChanged, this ) );
}


// ================================================
// ~AirspeedBox
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
AirspeedBox::~AirspeedBox() 
{
	
}


void AirspeedBox::initialize( InstanceIDPool &symbolIdPool )
{
	box = new SymbolLine();
	box->setID( symbolIdPool.getAvailableID() ); // fixme - error checking
	addSymbol( box.get() );
	box->addImplementation( new SerializableSymbolLine( box.get(), outgoingMessage ) );
	box->setState( Symbol::Visible );
	box->setSurfaceID( SURFACE_ID );
	box->setColor( Vect4( 0.0, 1.0, 0.0, 1.0 ) );
	box->setFlashDutyCyclePercentage( 100 );
	box->setPosition( -6.0, 4.0 );
	box->setRotation( 0.0 );
    
	box->setPrimitiveType( SymbolLine::LineLoop );
	box->addVertex( -1.7, -0.4 );
	box->addVertex(  0.0, -0.4 );
	box->addVertex(  0.0,  0.4 );
	box->addVertex( -1.7,  0.4 );

	
	text = new SymbolText();
	text->setID( symbolIdPool.getAvailableID() ); // fixme - error checking
	text->setParent( true, box->getID(), box.get() );
	box->addSymbol( text.get() );
	text->addImplementation( new SerializableSymbolText( text.get(), outgoingMessage ) );
	text->setState( Symbol::Visible );
	text->setSurfaceID( SURFACE_ID );
	text->setColor( Vect4( 0.0, 1.0, 0.0, 1.0 ) );
	text->setFlashDutyCyclePercentage( 100 );
	text->setPosition( -0.1, 0.0 );
	text->setRotation( 0.0 );
	
	text->setTextAlignment( SymbolText::CenterRight );
	text->setTextOrientation( SymbolText::LeftToRight );
	text->setFontID( 0 ); // fixme - fonts
	text->setFontSize( 1.5 );
	text->setText( "1XXX" );
	
}


void AirspeedBox::updateMotion( double deltaT )
{
	
}


void AirspeedBox::airspeedChanged( HUDState * )
{
	std::ostringstream oss;
	
	oss.precision( 0 );
	oss << std::fixed << hudState->getAirspeedKnots();
	
	text->setText( oss.str() );
}

