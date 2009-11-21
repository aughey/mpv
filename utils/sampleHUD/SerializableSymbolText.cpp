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
 *     Created SerializableSymbolText, based on SerializableSymbolLine
 * 
 */

#include "BindSlot.h"
#include "SerializableSymbolText.h"

SerializableSymbolText::SerializableSymbolText( SymbolText *symbol, CigiOutgoingMsg &message ) : 
	SerializableSymbol( symbol, message ),
	text( symbol )
{
	definitionChanged( text );
	
	text->alignmentChanged.connect( BIND_SLOT1( SerializableSymbolText::definitionChanged, this ) );
	text->orientationChanged.connect( BIND_SLOT1( SerializableSymbolText::definitionChanged, this ) );
	text->fontChanged.connect( BIND_SLOT1( SerializableSymbolText::definitionChanged, this ) );
	text->fontSizeChanged.connect( BIND_SLOT1( SerializableSymbolText::definitionChanged, this ) );
	text->textChanged.connect( BIND_SLOT1( SerializableSymbolText::definitionChanged, this ) );
}


SerializableSymbolText::~SerializableSymbolText()
{
	
}


void SerializableSymbolText::serializeDefinitionPacket()
{
	definitionPacket.SetSymbolID( text->getID() );
	
	definitionPacket.SetAlignment( (CigiBaseSymbolTextDef::AlignmentGrp)text->getTextAlignment() );
	definitionPacket.SetOrientation( (CigiBaseSymbolTextDef::OrientationGrp)text->getTextOrientation() );
	definitionPacket.SetFontID( text->getFontID() );
	definitionPacket.SetFontSize( text->getFontSize() );
	definitionPacket.SetText( text->getText() );
	
	outgoingMessage << definitionPacket;
}


void SerializableSymbolText::definitionChanged( SymbolText * )
{
	shouldSendDefinitionPacket = true;
}

