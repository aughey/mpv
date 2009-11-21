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
 * 2008-04-13  Andrew Sampson
 *     Initial version.  
 * 
 */

#include "BindSlot.h"
#include "SerializableSymbolLine.h"

SerializableSymbolLine::SerializableSymbolLine( SymbolLine *symbol, CigiOutgoingMsg &message ) : 
	SerializableSymbol( symbol, message ),
	line( symbol )
{
	definitionChanged( line );
	
	line->lineStyleChanged.connect( BIND_SLOT1( SerializableSymbolLine::definitionChanged, this ) );
	line->verticesChanged.connect( BIND_SLOT1( SerializableSymbolLine::definitionChanged, this ) );
}


SerializableSymbolLine::~SerializableSymbolLine()
{
	
}


void SerializableSymbolLine::serializeDefinitionPacket()
{
	definitionPacket.SetSymbolID( line->getID() );
	definitionPacket.SetPrimitive( (CigiBaseSymbolLineDef::PrimitiveGrp)line->getPrimitiveType() );

	definitionPacket.SetStipplePattern( line->getStipplePattern() );
	definitionPacket.SetLineWidth( line->getLineWidth() );
	definitionPacket.SetStipplePatternLen( line->getStipplePatternLength() );

	definitionPacket.ClearVertices();

	for( unsigned int i = 0; i < line->getNumVertices(); i++ )
	{
		CigiBaseVertexSymbolData *cclVertex = definitionPacket.AddVertex();
		const Vect2 &mpvVertex = line->getVertex( i );
		
		cclVertex->SetVertex( mpvVertex.getx(), mpvVertex.gety() );
	}
	
	outgoingMessage << definitionPacket;
}


void SerializableSymbolLine::definitionChanged( SymbolLine * )
{
	shouldSendDefinitionPacket = true;
}

