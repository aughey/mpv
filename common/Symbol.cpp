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
 *  2008-01-10 Andrew Sampson
 *      Initial Release.
 * </pre>
 */

#include <string>
#include <algorithm>

#include "BindSlot.h"
#include "Symbol.h"

using namespace mpv;


Symbol::FlashState::FlashState()
{
	dutyCyclePercentage = 50;
	period = 1.0;
	timeNow = 0.0;
	on = false;
}


Symbol::FlashState::~FlashState()
{
}


void Symbol::FlashState::update( double timeElapsed )
{
	bool prevState = on;
	
	timeNow += timeElapsed;
	
	if( dutyCyclePercentage == 0 )
		on = false;
	else if( dutyCyclePercentage >= 100 )
		on = true;
	else if( period <= 0.0 )
		// avoid divide-by-zero
		on = false;
	else
	{
		float cycleNum = timeNow / period;
		float remainder = cycleNum - ((int)cycleNum);
		on = (remainder < (float)dutyCyclePercentage / 100.0 );
	}
	
	if( on != prevState )
		stateChanged();
}



void Symbol::FlashState::resetSequence()
{
	timeNow = 0.0;
	update( 0.0 );
}



void Symbol::FlashState::setDutyCyclePercentage( Cigi_uint8 newPercentage )
{
	dutyCyclePercentage = newPercentage;
	update( 0.0 );
}



void Symbol::FlashState::setPeriod( float newPeriod )
{
	// clamp to positive values
	period = std::max( newPeriod, 0.0f );
	update( 0.0 );
}



bool Symbol::FlashState::getState() const
{
	return on;
}



bool Symbol::FlashState::isFlashing() const
{
	return ( dutyCyclePercentage < 100 );
}




Symbol::Symbol() : 
	ComponentContainer(),
	SymbolContainer(),
	id( 0xffff ),
	state( Hidden ),
	fullControlPacketRecvd( false ),
	isChild( false ),
	inheritColor( false ),
	parentID( 0xffff ),
	parent( NULL ),
	surfaceID( 0xffff ),
	layer( 0 ),
	flashState(),
	position( 0.0, 0.0 ),
	rotation( 0.0 ),
	color( 1.0, 1.0, 1.0, 1.0 ),
	scale( 1.0, 1.0 )
{
	flashState.stateChanged.connect( BIND_SLOT0( Symbol::forwardFlashStateChanged, this ) );
}


Symbol::~Symbol()
{
	// reference counting takes care of deleting the imp objects
}



void Symbol::update( double timeElapsed )
{
	flashState.update( timeElapsed );
	
	SymbolImpList::iterator iter;
	for( iter = imps.begin(); iter != imps.end(); iter++ )
	{
		if( (*iter).valid() )
			(*iter)->update( timeElapsed );
	}

	updateComponents( timeElapsed );
}



void Symbol::setID( Cigi_uint16 newID )
{
	id = newID;
}



Cigi_uint16 Symbol::getID() const
{
	return id;
}



void Symbol::setInheritColor( bool newInheritColor )
{
	if( inheritColor != newInheritColor )
	{
		inheritColor = newInheritColor;
		colorChanged( this );
	}
}



bool Symbol::getInheritColor() const
{
	return inheritColor;
}



void Symbol::setState( Symbol::SymbolState newState )
{
	if( state != newState )
	{
		state = newState;

		if( state == Destroyed )
		{
			// the CIGI standard states that when a symbol is destroyed, all the 
			// symbol's children are also destroyed
			flagAllSymbolsAsDestroyed();
		}
	
		stateChanged( this );
	}
}



Symbol::SymbolState Symbol::getState() const
{
	return state;
}



void Symbol::setFullControlPacketRecvd( bool truth )
{
	fullControlPacketRecvd = truth;
}



bool Symbol::getFullControlPacketRecvd() const
{
	return fullControlPacketRecvd;
}



bool Symbol::getIsChild() const
{
	return isChild;
}



Cigi_uint16 Symbol::getParentID() const
{
	return parentID;
}



void Symbol::setParent( 
	bool newIsChild, Cigi_uint16 newParentID, Symbol *newParent )
{
	if( isChild != newIsChild || 
		parentID != newParentID || 
		parent != newParent )
	{
		// disconnect signals for old parent
		if( parent != NULL )
		{
			parent->colorChanged.disconnect( BIND_SLOT1( Symbol::parentColorChanged, this ) );
			parent->flashStateChanged.disconnect( BIND_SLOT1( Symbol::parentFlashStateChanged, this ) );
		}

		if( newIsChild )
		{
			// sanity checks
			if( newParent == NULL )
				throw std::string( "attempted to set parent symbol to NULL when isChild set to true" );
			if( newParent->getID() != newParentID )
				throw std::string( "parentID and parent->getID() do not match" );
			Symbol *testParent = newParent;
			while( testParent != NULL )
			{
				if( testParent == this || testParent->getID() == getID() )
					throw std::string( "attempted to attach a symbol to itself" );
				testParent = testParent->getParent();
			}
			
			isChild = true;
			parentID = newParentID;
			parent = newParent;
			
			// connect signals for new parent
			parent->colorChanged.connect( BIND_SLOT1( Symbol::parentColorChanged, this ) );
			parent->flashStateChanged.connect( BIND_SLOT1( Symbol::parentFlashStateChanged, this ) );
		}
		else
		{
			isChild = false;
			parentID = 0xffff;
			parent = NULL;
		}
		parentChanged( this );
		
		// update flash state (also emits flashStateChanged if needed)
		flashState.update( 0.0 );

		// It isn't enough to rely on update() to emit flashStateChanged.  
		// Would need to check getFlashState() at the beginning of the method, 
		// and compare that value to the value obtained after changing the 
		// parent, and emit flashStateChanged if different.  It's easier to 
		// just emit flashStateChanged without checking.  
		flashStateChanged( this );
		
		// a new parent means a potential change in color
		if( inheritColor )
		{
			colorChanged( this );
		}
	}
}



Symbol * Symbol::getParent() const
{
	return parent;
}



void Symbol::setSurfaceID( Cigi_uint16 newSurfaceID )
{
	if( surfaceID != newSurfaceID )
	{
		surfaceID = newSurfaceID;
		surfaceChanged( this );
	}
}



Cigi_uint16 Symbol::getSurfaceID() const
{
	return surfaceID;
}



void Symbol::setLayer( Cigi_uint8 newLayer )
{
	if( layer != newLayer )
	{
		layer = newLayer;
		layerChanged( this );
	}
}



Cigi_uint8 Symbol::getLayer() const
{
	return layer;
}



void Symbol::setPosition( float u, float v )
{
	setPosition( Vect2( u, v ) );
}



void Symbol::setPosition( const Vect2 &pos )
{
	{
		position = pos;
		positionChanged( this );
	}
}



Vect2 Symbol::getPosition() const
{
	return position;
}



void Symbol::setRotation( float newRotation )
{
	if( rotation != newRotation )
	{
		rotation = newRotation;
		rotationChanged( this );
	}
}



float Symbol::getRotation() const
{
	return rotation;
}



void Symbol::setColor( Cigi_uint8 red, Cigi_uint8 green, Cigi_uint8 blue, Cigi_uint8 alpha )
{
	setColor( Vect4( 
		((float)red)   / 255.0, 
		((float)green) / 255.0, 
		((float)blue)  / 255.0, 
		((float)alpha) / 255.0 ) );
}



void Symbol::setColor( const Vect4 &newColor )
{
	{
		color = newColor;
		colorChanged( this );
	}
}



Vect4 Symbol::getColor() const
{
	if( parent != NULL && inheritColor )
		return parent->getColor();
	return color;
}



void Symbol::setScale( float u, float v )
{
	setScale( Vect2( u, v ) );
}



void Symbol::setScale( const Vect2 &newScale )
{
	{
		scale = newScale;
		scaleChanged( this );
	}
}



Vect2 Symbol::getScale() const
{
	return scale;
}



void Symbol::resetFlashSequence()
{
	flashState.resetSequence();
}


void Symbol::setFlashDutyCyclePercentage( Cigi_uint8 newPercentage )
{
	flashState.setDutyCyclePercentage( newPercentage );
}


void Symbol::setFlashPeriod( float newPeriod )
{
	flashState.setPeriod( newPeriod );
}


bool Symbol::getFlashState() const
{
	// if any of this symbol's parents is flashing, then inherit its flash state
	if( parent != NULL && parent->isFlashing() )
		return parent->getFlashState();
	return flashState.getState();
}



void Symbol::addImplementation( SymbolImp *newImp )
{
	if( newImp != NULL )
		imps.push_back( newImp );
}



bool Symbol::isFlashing() const
{
	// if any of this symbol's parents is flashing, then inherit its flash state
	bool isParentFlashing = false;
	if( parent != NULL )
		isParentFlashing = parent->isFlashing();
	return ( isParentFlashing || flashState.isFlashing() );
}



void Symbol::parentColorChanged( Symbol * )
{
	if( inheritColor )
		colorChanged( this );
}



void Symbol::parentFlashStateChanged( Symbol * )
{
	// if any of this symbol's parents is flashing, then inherit its flash 
	// state (and consequently emit flashStateChanged whenever the parent 
	// emits the same)
	if( isFlashing() )
		flashStateChanged( this );
}


void Symbol::forwardFlashStateChanged()
{
	flashStateChanged( this );
}


