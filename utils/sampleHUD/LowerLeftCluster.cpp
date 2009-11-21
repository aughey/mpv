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

#include "LowerLeftCluster.h"
#include "Misc.h"
#include "SerializableSymbolLine.h"
#include "SerializableSymbolText.h"

// ================================================
// LowerLeftCluster
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
LowerLeftCluster::LowerLeftCluster(CigiOutgoingMsg &message, HUDState *h ) : 
	SymbolSet( message ),
	hudState( h )
{
	hudState->angleOfAttackChanged.connect( BIND_SLOT1( LowerLeftCluster::textChanged, this ) );
	hudState->machChanged.connect( BIND_SLOT1( LowerLeftCluster::textChanged, this ) );
	hudState->geesChanged.connect( BIND_SLOT1( LowerLeftCluster::textChanged, this ) );
	hudState->maxGeesChanged.connect( BIND_SLOT1( LowerLeftCluster::textChanged, this ) );
}


// ================================================
// ~LowerLeftCluster
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
LowerLeftCluster::~LowerLeftCluster() 
{
	
}


void LowerLeftCluster::initialize( InstanceIDPool &symbolIdPool )
{
	text = new SymbolText();
	text->setID( symbolIdPool.getAvailableID() ); // fixme - error checking
	addSymbol( text.get() );
	text->addImplementation( new SerializableSymbolText( text.get(), outgoingMessage ) );
	text->setState( Symbol::Visible );
	text->setSurfaceID( SURFACE_ID );
	text->setColor( Vect4( 0.0, 1.0, 0.0, 1.0 ) );
	text->setFlashDutyCyclePercentage( 100 );
	text->setPosition( -7.5, 0.5 );
	text->setRotation( 0.0 );
	
	text->setTextAlignment( SymbolText::TopLeft );
	text->setTextOrientation( SymbolText::LeftToRight );
	text->setFontID( 0 ); // fixme - fonts
	text->setFontSize( 1.35 );
	text->setText( "a XX.X\nM XX.XX\nG XX.X\n  XX.X\n\nXX:XX:XXZ" );
	
	textChanged( hudState.get() );
}


void LowerLeftCluster::updateMotion( double deltaT )
{
	
}


void LowerLeftCluster::textChanged( HUDState * )
{
	std::ostringstream oss;
	
	oss.precision( 1 );
	oss << "a " << std::setfill( ' ' ) << setw( 4 ) << std::fixed << hudState->getAngleOfAttack();
	oss.precision( 2 );
	oss << "\nM " << std::setfill( ' ' ) << setw( 5 ) << std::fixed << hudState->getMach();
	oss.precision( 1 );
	oss << "\nG " << std::setfill( ' ' ) << setw( 4 ) << std::fixed << hudState->getGees();
	oss << "\n  " << std::setfill( ' ' ) << setw( 4 ) << std::fixed << hudState->getMaxGees();
	oss << "\n\nXX:XX:XXZ";
	
	text->setText( oss.str() );
}

