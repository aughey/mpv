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

#ifndef SERIALIZABLESYMBOL_H
#define SERIALIZABLESYMBOL_H

#include <CigiOutgoingMsg.h>
#include <CigiSymbolCtrlV3_3.h>
#include <CigiShortSymbolCtrlV3_3.h>

#include "Symbol.h"

using namespace mpv;

//! A SymbolImp that, instead of sending output to some display device, 
//! outputs to a CIGI stream.  This makes it usable for Host-side symbology 
//! work, while leveraging the functionality already implemented in the MPV.
class SerializableSymbol : public SymbolImp
{
public:
	SerializableSymbol( Symbol *symbol, CigiOutgoingMsg &message );
	
	//! performs serialization
	virtual void update( double timeElapsed );

	void parentChanged( Symbol *symbol );
	void surfaceChanged( Symbol *symbol );
	void stateChanged( Symbol *symbol );
	void layerChanged( Symbol *symbol );
	void colorChanged( Symbol *symbol );
	void flashStateChanged( Symbol *symbol );
	void positionChanged( Symbol *symbol );
	void rotationChanged( Symbol *symbol );
	void scaleChanged( Symbol *symbol );
	
protected:
	virtual ~SerializableSymbol();

	virtual void serializeDefinitionPacket() = 0;

	void serializeControlPacket();
	void serializeShortControlPacket();

	void attributeChanged( CigiBaseShortSymbolCtrl::DatumTypeGrp );
	
	CigiOutgoingMsg &outgoingMessage;
	
	//! True if the symbol's definition is "dirty", and needs to be (re)sent 
	//! to the IG.
	//! Symbol Control packets shouldn't be sent until the definition packet 
	//! has been sent.
	bool shouldSendDefinitionPacket;
	
	// True if we've sent a full Symbol Control packet to the IG.
	// Short Symbol Control packets shouldn't be sent until at least one 
	// full Symbol Control packet has been sent.
	bool haveSentFullControlPacket;
	
	std::map<CigiBaseShortSymbolCtrl::DatumTypeGrp, bool> changedAttributes;
	
	CigiSymbolCtrlV3_3 controlPacket;
};


#endif
