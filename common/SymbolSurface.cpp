/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2008 The Boeing Company
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
 *  2008-01-17 Andrew Sampson
 *      Initial Release.
 * </pre>
 */

#include "SymbolSurface.h"

using namespace mpv;


SymbolSurface::SymbolSurface() : 
	ComponentContainer(),
	SymbolContainer(),
	id( -1 ),
	state( Active ),
	attachment( View ),
	
	// entity-attached
	isBillboard( false ),
	isPerspectiveEnabled( true ),
	entityID( -1 ),
	position( 0., 0., 0. ),
	yaw( 0.0 ),
	pitch( 0.0 ),
	roll( 0.0 ),
	width( 1.0 ),
	height( 1.0 ),
	
	// view-attached
	viewID( -1 ),
	viewPositionLeft( 0.0 ),
	viewPositionRight( 1.0 ),
	viewPositionTop( 1.0 ),
	viewPositionBottom( 0.0 ),
	
	minimumCoordinates( -1., -1. ),
	maximumCoordinates( 1., 1. )
{
}


SymbolSurface::~SymbolSurface()
{
	// reference counting takes care of deleting the imp objects
}


void SymbolSurface::setID( int newID )
{
	id = newID;
}

void SymbolSurface::setState( SurfaceState newState )
{
	state = newState;
	
	if( newState == Destroyed )
	{
		// the CIGI standard states that when a surface is destroyed, all the 
		// symbols in that surface are also destroyed
		flagAllSymbolsAsDestroyed();
	}
	
	stateChanged( this );
}

void SymbolSurface::setAttachState( SurfaceAttachState newAttachment )
{
	if( attachment != newAttachment )
	{
		attachment = newAttachment;
		attachmentChanged( this );
	}
}

void SymbolSurface::setIsBillboard( bool newIsBillboard )
{
	if( isBillboard != newIsBillboard )
	{
		isBillboard = newIsBillboard;
		billboardingChanged( this );
	}
}

void SymbolSurface::setIsPerspectiveEnabled( bool newIsPerspectiveEnabled )
{
	if( isPerspectiveEnabled != newIsPerspectiveEnabled )
	{
		isPerspectiveEnabled = newIsPerspectiveEnabled;
		billboardingChanged( this );
	}
}

void SymbolSurface::setMinimumCoordinates( const Vect2 &newMinimumCoordinates )
{
//	if( minimumCoordinates != newMinimumCoordinates )
	{
		minimumCoordinates = newMinimumCoordinates;
		surfaceCoordinatesChanged( this );
	}
}

void SymbolSurface::setMaximumCoordinates( const Vect2 &newMaximumCoordinates )
{
//	if( maximumCoordinates != newMaximumCoordinates )
	{
		maximumCoordinates = newMaximumCoordinates;
		surfaceCoordinatesChanged( this );
	}
}


// entity-attached
void SymbolSurface::setEntityID( int newEntityID )
{
	entityID = newEntityID;
}

void SymbolSurface::setPosition( const Vect3 &newPosition )
{
//	if( position != newPosition )
	{
		position = newPosition;
		entityPositionChanged( this );
	}
}

void SymbolSurface::setYaw( float newYaw )
{
	if( yaw != newYaw )
	{
		yaw = newYaw;
		entityOrientationChanged( this );
	}
}

void SymbolSurface::setPitch( float newPitch )
{
	if( pitch != newPitch )
	{
		pitch = newPitch;
		entityOrientationChanged( this );
	}
}

void SymbolSurface::setRoll( float newRoll )
{
	if( roll != newRoll )
	{
		roll = newRoll;
		entityOrientationChanged( this );
	}
}

void SymbolSurface::setWidth( float newWidth )
{
	if( width != newWidth )
	{
		width = newWidth;
		entityDimensionsChanged( this );
	}
}

void SymbolSurface::setHeight( float newHeight )
{
	if( height != newHeight )
	{
		height = newHeight;
		entityDimensionsChanged( this );
	}
}


// view-attached
void SymbolSurface::setViewID( int newViewID )
{
	viewID = newViewID;
}

void SymbolSurface::setViewPositionLeft( float newViewPositionLeft )
{
	if( viewPositionLeft != newViewPositionLeft )
	{
		viewPositionLeft = newViewPositionLeft;
		viewPositionChanged( this );
	}
}

void SymbolSurface::setViewPositionRight( float newViewPositionRight )
{
	if( viewPositionRight != newViewPositionRight )
	{
		viewPositionRight = newViewPositionRight;
		viewPositionChanged( this );
	}
}

void SymbolSurface::setViewPositionTop( float newViewPositionTop )
{
	if( viewPositionTop != newViewPositionTop )
	{
		viewPositionTop = newViewPositionTop;
		viewPositionChanged( this );
	}
}

void SymbolSurface::setViewPositionBottom( float newViewPositionBottom )
{
	if( viewPositionBottom != newViewPositionBottom )
	{
		viewPositionBottom = newViewPositionBottom;
		viewPositionChanged( this );
	}
}


void SymbolSurface::addImplementation( SymbolSurfaceImp *newImplementation )
{
	if( newImplementation != NULL )
		imps.push_back( newImplementation );
}




SymbolSurfaceImp::SymbolSurfaceImp( SymbolSurface *newSurface ) : 
	Referenced(),
	surface( newSurface )
{
}


SymbolSurfaceImp::~SymbolSurfaceImp()
{
}

