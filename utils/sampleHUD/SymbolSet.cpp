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

#include "SymbolSet.h"


SymbolSet::SymbolSet( CigiOutgoingMsg &message ) : SymbolContainer(),
	outgoingMessage( message )
{
	
}


SymbolSet::~SymbolSet()
{
	
}


void SymbolSet::update( double deltaT )
{
	updateMotion( deltaT );
	recursiveUpdate( deltaT, this );
}


void SymbolSet::recursiveUpdate( double deltaT, SymbolContainer *symbolContainer )
{
	// iterate over contents of symbol container
	SymbolContainer::SymbolIteratorPair iteratorPair = symbolContainer->getSymbols();
	SymbolContainer::SymbolMap::iterator iter = iteratorPair.first;
	for( ; iter != iteratorPair.second; iter++ )
	{
		// Symbol::update() will call SymbolImp::update() which will, in the 
		// case of our SerializableSymbols, serialize the symbol to CIGI 
		// packets.
		// It is important that update() is called on parent symbols before 
		// the children of those symbols; this is a tree of serializable 
		// symbols, and the parent symbols must be serialized to the IG 
		// before the child symbols, else the child symbols will be rejected 
		// by the IG
		iter->second->update( deltaT );
		// recurse
		recursiveUpdate( deltaT, iter->second.get() );
	}
}

