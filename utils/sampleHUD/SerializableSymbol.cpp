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
 * 2008-04-12  Andrew Sampson
 *     Initial version.  
 * 
 */

#include <iostream>

#include "BindSlot.h"
#include "SerializableSymbol.h"

SerializableSymbol::SerializableSymbol( Symbol *symbol, CigiOutgoingMsg &message ) : 
	SymbolImp( symbol ), 
	outgoingMessage( message ), 
	shouldSendDefinitionPacket( true ), 
	haveSentFullControlPacket( false )
{
	
	// retrieve the symbol state and apply it
	parentChanged( symbol );
	surfaceChanged( symbol );
	stateChanged( symbol );
	layerChanged( symbol );
	colorChanged( symbol );
	flashStateChanged( symbol );
	positionChanged( symbol );
	rotationChanged( symbol );
	scaleChanged( symbol );

	// set up slots for change notification
	symbol->colorChanged.connect( BIND_SLOT1( SerializableSymbol::colorChanged, this ) );
	symbol->parentChanged.connect( BIND_SLOT1( SerializableSymbol::parentChanged, this ) );
	symbol->flashStateChanged.connect( BIND_SLOT1( SerializableSymbol::flashStateChanged, this ) );
	symbol->surfaceChanged.connect( BIND_SLOT1( SerializableSymbol::surfaceChanged, this ) );
	symbol->stateChanged.connect( BIND_SLOT1( SerializableSymbol::stateChanged, this ) );
	symbol->layerChanged.connect( BIND_SLOT1( SerializableSymbol::layerChanged, this ) );
	symbol->positionChanged.connect( BIND_SLOT1( SerializableSymbol::positionChanged, this ) );
	symbol->rotationChanged.connect( BIND_SLOT1( SerializableSymbol::rotationChanged, this ) );
	symbol->scaleChanged.connect( BIND_SLOT1( SerializableSymbol::scaleChanged, this ) );
}


SerializableSymbol::~SerializableSymbol()
{
	
}


void SerializableSymbol::update( double timeElapsed )
{
	if( shouldSendDefinitionPacket )
	{
		serializeDefinitionPacket();
		shouldSendDefinitionPacket = false;
	}
	
	// we can use short symbol control packets to save bandwidth
	if( haveSentFullControlPacket )
	{
		// use short symbol control packet(s) to transmit symbol state
		
		/* heuristic for determining whether to send one or more short 
		   symbol ctrl packets (sscp):
		   - each fscp is 40 bytes.
		   - each sscp can carry a payload of 2 attributes, in addition to 
		   various flags in its header.  each sscp is 16 bytes.
		   - 40 / 16 = 2.something
		   
		   Two sscp can be sent in less space than a fscp.  Each sscp 
		   carries two attributes.  Thus, if 2*2=4 or fewer attributes have 
		   changed in a symbol, sscp's are used.  If more than 4 attributes 
		   need to be sent, then a fscp is used.
		*/
		static int numChangedAttributesThreshold = 
			( CIGI_SYMBOL_CONTROL_PACKET_SIZE_V3_3 / CIGI_SHORT_SYMBOL_CONTROL_PACKET_SIZE_V3_3 )
			* 2; // 2 = num attributes per sscp
		
		int numLegitAttributesChanged = changedAttributes.size();
		// "None" (used to indicate that the symbol state has changed, which is 
		// in the sscp header rather than one of the sscp datum) shouldn't 
		// influence the heuristic, unless None is the only attribute that's 
		// changed.  Note that this isn't terribly important, as None will 
		// only spoil the heuristic in rare cases.
		if( numLegitAttributesChanged > 1 )
		{
			std::map<CigiBaseShortSymbolCtrl::DatumTypeGrp, bool>::iterator iter = 
				changedAttributes.find( CigiBaseShortSymbolCtrl::None );
			if( iter != changedAttributes.end() && 
				changedAttributes[CigiBaseShortSymbolCtrl::None] )
			{
				numLegitAttributesChanged--;
			}
		}
		
		if( numLegitAttributesChanged <= numChangedAttributesThreshold )
			serializeShortControlPacket();
		else
			serializeControlPacket();
	}
	else
	{
		serializeControlPacket();
		haveSentFullControlPacket = true;
	}
	changedAttributes.clear();
}


void SerializableSymbol::parentChanged( Symbol *symbol )
{
	attributeChanged( CigiBaseShortSymbolCtrl::ParentIdDatumType );
}


void SerializableSymbol::surfaceChanged( Symbol *symbol )
{
	attributeChanged( CigiBaseShortSymbolCtrl::SurfaceIdDatumType );
}


void SerializableSymbol::stateChanged( Symbol *symbol )
{
	// note - symbol state is included in every short symbol ctrl, hence None is used here
	attributeChanged( CigiBaseShortSymbolCtrl::None );
}


void SerializableSymbol::layerChanged( Symbol *symbol )
{
	attributeChanged( CigiBaseShortSymbolCtrl::LayerDatumType );
}


void SerializableSymbol::colorChanged( Symbol *symbol )
{
	attributeChanged( CigiBaseShortSymbolCtrl::ColorDatumType );
}


void SerializableSymbol::flashStateChanged( Symbol *symbol )
{
	// FIXME - don't know how to handle this; the flash state stuff in Symbol is 
	// very much tailored for IG use... there's really nothing to do here...
	//attributeChanged( CigiBaseShortSymbolCtrl:: );
}


void SerializableSymbol::positionChanged( Symbol *symbol )
{
	attributeChanged( CigiBaseShortSymbolCtrl::UPositionDatumType );
	attributeChanged( CigiBaseShortSymbolCtrl::VPositionDatumType );
}


void SerializableSymbol::rotationChanged( Symbol *symbol )
{
	attributeChanged( CigiBaseShortSymbolCtrl::RotationDatumType );
}


void SerializableSymbol::scaleChanged( Symbol *symbol )
{
	attributeChanged( CigiBaseShortSymbolCtrl::ScaleUDatumType );
	attributeChanged( CigiBaseShortSymbolCtrl::ScaleVDatumType );
}


void SerializableSymbol::serializeControlPacket()
{
	controlPacket.SetSymbolID( baseSymbol->getID() );
	controlPacket.SetSymbolState( (CigiBaseSymbolCtrl::SymbolStateGrp)baseSymbol->getState() );

	controlPacket.SetAttachState( baseSymbol->getIsChild() ? 
		CigiBaseSymbolCtrl::Attach : 
		CigiBaseSymbolCtrl::Detach );
	controlPacket.SetParentSymbolID( baseSymbol->getParentID() );
	controlPacket.SetSurfaceID( baseSymbol->getSurfaceID() );

	controlPacket.SetLayer( baseSymbol->getLayer() );

	controlPacket.SetInheritColor( baseSymbol->getInheritColor() ? 
		CigiBaseSymbolCtrl::Inherit : 
		CigiBaseSymbolCtrl::NotInherit );

	// fixme - flash state hard-coded - no accessors for individual flash state in Symbol
	controlPacket.SetFlashCtrl( true ? 
		CigiBaseSymbolCtrl::Continue : 
		CigiBaseSymbolCtrl::Reset );
	controlPacket.SetFlashDutyCycle( 100 );
	controlPacket.SetFlashPeriod( 1.0 );

	Vect2 position = baseSymbol->getPosition();
	controlPacket.SetUPosition( position.getx() );
	controlPacket.SetVPosition( position.gety() );
	controlPacket.SetRotation( baseSymbol->getRotation() );

	Vect4 color = baseSymbol->getColor();
	controlPacket.SetColor( 
		(Cigi_uint8)(color.getx() * 255.0),
		(Cigi_uint8)(color.gety() * 255.0),
		(Cigi_uint8)(color.getz() * 255.0),
		(Cigi_uint8)(color.getw() * 255.0) );

	Vect2 scale = baseSymbol->getScale();
	controlPacket.SetScaleU( scale.getx() );
	controlPacket.SetScaleV( scale.gety() );

	outgoingMessage << controlPacket;
	
}


void SerializableSymbol::serializeShortControlPacket()
{
	CigiShortSymbolCtrlV3_3 shortControlPacket;
	
	shortControlPacket.SetSymbolID( baseSymbol->getID() );
	shortControlPacket.SetSymbolState( (CigiBaseSymbolCtrl::SymbolStateGrp)baseSymbol->getState() );

	shortControlPacket.SetAttachState( baseSymbol->getIsChild() ? 
		CigiBaseSymbolCtrl::Attach : 
		CigiBaseSymbolCtrl::Detach );

	shortControlPacket.SetInheritColor( baseSymbol->getInheritColor() ? 
		CigiBaseSymbolCtrl::Inherit : 
		CigiBaseSymbolCtrl::NotInherit );

	// fixme - flash state hard-coded - no accessors for individual flash state in Symbol
	shortControlPacket.SetFlashCtrl( true ? 
		CigiBaseSymbolCtrl::Continue : 
		CigiBaseSymbolCtrl::Reset );

	shortControlPacket.SetDatumNone( 0 );
	shortControlPacket.SetDatumNone( 1 );

	Vect2 position = baseSymbol->getPosition();
	Vect4 color = baseSymbol->getColor();
	Vect2 scale = baseSymbol->getScale();

	while( changedAttributes.size() > 0 )
	{
		int currentDatum = 0;
		
		std::map<CigiBaseShortSymbolCtrl::DatumTypeGrp, bool>::iterator iter;
		iter = changedAttributes.begin();
		
		while( iter != changedAttributes.end() && currentDatum < 2 )
		{
			if( iter->first != CigiBaseShortSymbolCtrl::None && 
				iter->second == true )
			{
				switch( iter->first )
				{
				case CigiBaseShortSymbolCtrl::SurfaceIdDatumType:
					shortControlPacket.SetParentSymbolID( currentDatum, baseSymbol->getParentID() );
					break;
				case CigiBaseShortSymbolCtrl::ParentIdDatumType:
					shortControlPacket.SetSurfaceID( currentDatum, baseSymbol->getSurfaceID() );
					break;
				case CigiBaseShortSymbolCtrl::LayerDatumType:
					shortControlPacket.SetLayer( currentDatum, baseSymbol->getLayer() );
					break;
				case CigiBaseShortSymbolCtrl::FlashDutyCycleDatumType:
					shortControlPacket.SetFlashDutyCycle( currentDatum, 100 );
					break;
				case CigiBaseShortSymbolCtrl::FlashPeriodDatumType:
					shortControlPacket.SetFlashPeriod( currentDatum, 1.0 );
					break;
				case CigiBaseShortSymbolCtrl::UPositionDatumType:
					shortControlPacket.SetUPosition( currentDatum, position.getx() );
					break;
				case CigiBaseShortSymbolCtrl::VPositionDatumType:
					shortControlPacket.SetVPosition( currentDatum, position.gety() );
					break;
				case CigiBaseShortSymbolCtrl::RotationDatumType:
					shortControlPacket.SetRotation( currentDatum, baseSymbol->getRotation() );
					break;
				case CigiBaseShortSymbolCtrl::ColorDatumType:
					shortControlPacket.SetColor( currentDatum, 
						(Cigi_uint8)(color.getx() * 255.0),
						(Cigi_uint8)(color.gety() * 255.0),
						(Cigi_uint8)(color.getz() * 255.0),
						(Cigi_uint8)(color.getw() * 255.0) );
					break;
				case CigiBaseShortSymbolCtrl::ScaleUDatumType:
					shortControlPacket.SetScaleU( currentDatum, scale.getx() );
					break;
				case CigiBaseShortSymbolCtrl::ScaleVDatumType:
					shortControlPacket.SetScaleV( currentDatum, scale.gety() );
					break;
				default:
					break;
				}
				currentDatum++;
			}
			changedAttributes.erase( iter );
			iter = changedAttributes.begin();
		}
		
		outgoingMessage << shortControlPacket;
		shortControlPacket.SetDatumNone( 0 );
		shortControlPacket.SetDatumNone( 1 );
	}
}


void SerializableSymbol::attributeChanged( CigiBaseShortSymbolCtrl::DatumTypeGrp attribute )
{
	changedAttributes[attribute] = true;
}


