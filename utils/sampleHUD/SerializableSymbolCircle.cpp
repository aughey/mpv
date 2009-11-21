/** <pre>
 * Sample HUD, using the CIGI symbology interface
 * Copyright (c) 2008 Andrew Sampson
 * Copyright (c) 2009 The Boeing Company
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
 * 2008-04-13  Andrew Sampson
 *     Initial version.  
 * 
 * 2009-08-31 Andrew Sampson
 *     Created SerializableSymbolCircle, based on SerializableSymbolLine
 * 
 */

#include "BindSlot.h"
#include "SerializableSymbolCircle.h"

SerializableSymbolCircle::SerializableSymbolCircle( SymbolCircle *symbol, CigiOutgoingMsg &message ) : 
	SerializableSymbol( symbol, message ),
	circle( symbol )
{
	definitionChanged( circle );
	
	circle->drawingStyleChanged.connect( BIND_SLOT1( SerializableSymbolCircle::definitionChanged, this ) );
	circle->lineStyleChanged.connect( BIND_SLOT1( SerializableSymbolCircle::definitionChanged, this ) );
	circle->circlesChanged.connect( BIND_SLOT1( SerializableSymbolCircle::definitionChanged, this ) );
}


SerializableSymbolCircle::~SerializableSymbolCircle()
{
	
}


void SerializableSymbolCircle::serializeDefinitionPacket()
{
	definitionPacket.SetSymbolID( circle->getID() );
	definitionPacket.SetDrawingStyle( (CigiBaseSymbolCircleDef::DrawingStyleGrp)circle->getDrawingStyle() );

	definitionPacket.SetStipplePattern( circle->getStipplePattern() );
	definitionPacket.SetLineWidth( circle->getLineWidth() );
	definitionPacket.SetStipplePatternLen( circle->getStipplePatternLength() );

	definitionPacket.ClearCircles();

	for( unsigned int i = 0; i < circle->getNumCircles(); i++ )
	{
		CigiBaseCircleSymbolData *cclCircle = definitionPacket.AddCircle();
		const SymbolCircle::Circle &mpvCircle = circle->getCircle( i );
		
		cclCircle->SetCenterUPosition( mpvCircle.centerPosition.getx() );
		cclCircle->SetCenterVPosition( mpvCircle.centerPosition.gety() );
		cclCircle->SetRadius( mpvCircle.radius );
		cclCircle->SetInnerRadius( mpvCircle.innerRadius );
		cclCircle->SetStartAngle( mpvCircle.startAngle );
		cclCircle->SetEndAngle( mpvCircle.endAngle );
	}
	
	outgoingMessage << definitionPacket;
}


void SerializableSymbolCircle::definitionChanged( SymbolCircle * )
{
	shouldSendDefinitionPacket = true;
}

