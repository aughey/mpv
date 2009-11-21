/** <pre>
 * MPV symbology stress test utility
 * Copyright (c) 2008 Andrew Sampson
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
 * 2008-04-20  Andrew Sampson
 *     Initial version.  
 * 
 */

#include <iostream>

#include "Pinwheel.h"
#include "Misc.h"


Pinwheel::Pinwheel( CigiOutgoingMsg &message ) : SymbolSet( message ),
	velocity( 0, 0 ), 
	rotationalVelocity( 400.0f )
{

}


Pinwheel::~Pinwheel()
{

}


void Pinwheel::initialize( InstanceIDPool &symbolIdPool )
{
	if( line.valid() )
	{
		removeSymbol( line.get() );
		line = NULL;
	}
	
	int symbolID = symbolIdPool.getAvailableID();
	if( symbolID > 0xffff )
	{
		std::cerr << "In Pinwheel::initialize - no more available symbol IDs\n";
		symbolIdPool.relenquishID( symbolID );
		return;
	}
	
	int childSymbolID = symbolIdPool.getAvailableID();
	if( childSymbolID > 0xffff )
	{
		std::cerr << "In Pinwheel::initialize - no more available symbol IDs\n";
		symbolIdPool.relenquishID( childSymbolID );
		return;
	}
	
	line = new SymbolLine;
	line->addImplementation( new SerializableSymbolLine( line.get(), outgoingMessage ) );
	
	line->setID( symbolID );
	line->setState( Symbol::Visible );
	line->setSurfaceID( SURFACE_ID );
	line->setColor( Vect4( 0.8, 0.0, 1.0, 1.0 ) );
	line->setFlashDutyCyclePercentage( 100 );
	
	line->setPosition( 
		randFloat( SURFACE_MIN_U, SURFACE_MAX_U ),
		randFloat( SURFACE_MIN_V, SURFACE_MAX_V ) );
	line->setRotation( randFloat( 0.0, 360.0 ) );

	line->setPrimitiveType( SymbolLine::LineLoop );
	line->addVertex( 0.05, -0.05 );
	line->addVertex( 0.0, -0.05 );
	line->addVertex( 0.0, 0.05 );
	line->addVertex( -0.05, 0.05 );
	

	SymbolLine *childLine = new SymbolLine;
	childLine->addImplementation( new SerializableSymbolLine( childLine, outgoingMessage ) );
	
	childLine->setID( childSymbolID );
	childLine->setState( Symbol::Visible );
	childLine->setSurfaceID( SURFACE_ID );
	childLine->setInheritColor( true );
	childLine->setFlashDutyCyclePercentage( 100 );
	
	childLine->setPosition( 0, 0 );
	childLine->setRotation( 90 );

	childLine->setPrimitiveType( SymbolLine::LineLoop );
	childLine->addVertex( 0.05, -0.05 );
	childLine->addVertex( 0.0, -0.05 );
	childLine->addVertex( 0.0, 0.05 );
	childLine->addVertex( -0.05, 0.05 );
	
	childLine->setParent( true, symbolID, line.get() );
	line->addSymbol( childLine );

	
	addSymbol( line.get() );
	
	velocity.Set( 
		randFloat( -1.0, 1.0 ),
		randFloat( -1.0, 1.0 ) );
	
	// clockwise versus counter-clockwise
	if( randBool() )
	{
		rotationalVelocity *= -1.0f;
		line->setScale( -1.0f, 1.0f );
	}
}


void Pinwheel::updateMotion( double deltaT )
{
	// extremely realistic physics!
	
	Vect2 position = line->getPosition() + ( velocity * deltaT );
	
	if( position.getx() < SURFACE_MIN_U )
	{
		// bounce
		if( velocity.getx() < 0.0 )
		{
			velocity.Set( velocity.getx() * -1.0, velocity.gety() );
		}
		// clamp
		position.Set( SURFACE_MIN_U, position.gety() );
	}
	else if( position.getx() > SURFACE_MAX_U )
	{
		// bounce
		if( velocity.getx() > 0.0 )
		{
			velocity.Set( velocity.getx() * -1.0, velocity.gety() );
		}
		// clamp
		position.Set( SURFACE_MAX_U, position.gety() );
	}
	
	if( position.gety() < SURFACE_MIN_V )
	{
		// bounce
		if( velocity.gety() < 0.0 )
		{
			velocity.Set( velocity.getx(), velocity.gety() * -1.0 );
		}
		// clamp
		position.Set( position.getx(), SURFACE_MIN_V );
	}
	else if( position.gety() > SURFACE_MAX_V )
	{
		// bounce
		if( velocity.gety() > 0.0 )
		{
			velocity.Set( velocity.getx(), velocity.gety() * -1.0 );
		}
		// clamp
		position.Set( position.getx(), SURFACE_MAX_V );
	}
	
	line->setPosition( position );

	
	float rotation = line->getRotation() + ( rotationalVelocity * deltaT );
	
	// clamp
	// prevent near-infinite looping, just in case
	if( rotation < -720.0 || rotation > 720.0 )
		rotation = 0.0;
	while( rotation < 0.0 )
		rotation += 360.0;
	while( rotation > 360.0 )
		rotation -= 360.0;

	line->setRotation( rotation );
}
	
