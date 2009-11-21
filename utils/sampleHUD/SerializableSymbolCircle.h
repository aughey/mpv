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

#ifndef SERIALIZABLESYMBOLCIRCLE_H
#define SERIALIZABLESYMBOLCIRCLE_H

#include <CigiOutgoingMsg.h>
#include <CigiSymbolCircleDefV3_3.h>

#include "SerializableSymbol.h"
#include "SymbolCircle.h"

// Contains a single symbol...
// Uses MPV's symbol classes, adding functionality to make them suitable for 
// host-side stuff.
class SerializableSymbolCircle : public SerializableSymbol
{
public:
	SerializableSymbolCircle( SymbolCircle *symbol, CigiOutgoingMsg &message );
	
protected:

	virtual ~SerializableSymbolCircle();

	virtual void serializeDefinitionPacket();
	
	void definitionChanged( SymbolCircle * );

	CigiSymbolCircleDefV3_3 definitionPacket;
	
	SymbolCircle *circle;
};

#endif
