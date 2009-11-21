/** <pre>
 *  The MPV Symbology Plugin Collection
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
 *  
 *  2008-02-08 Andrew Sampson, Boeing
 *      Initial release
 *
 * </pre>
 */

#include "BindSlot.h"
#include "SymbolContainer.h"

using namespace mpv;


SymbolContainer::SymbolContainer() : Referenced()
{
	
}


SymbolContainer::~SymbolContainer()
{
	// The CIGI standard states that:
	// - when a Symbol is deleted, all Symbols attached to it are deleted
	// - when a SymbolSurface is deleted, all Symbols attached to it are deleted
	// 
	// This SymbolContainer class is used in Symbol, SymbolSurface, and 
	// elsewhere.  In all these situations, this is the desired behavior.
	
	flagAllSymbolsAsDestroyed();
}


void SymbolContainer::flagAllSymbolsAsDestroyed()
{
	while( !symbols.empty() )
	{
		// hold on to a reference until signal emission is complete
		RefPtr<Symbol> symbol( symbols.begin()->second );

		symbol->setState( Symbol::Destroyed );
		// note - callback symbolChangedState will handle removal of symbol 
		// from the map
	}
}


void SymbolContainer::addSymbol( Symbol *symbol )
{
	if( symbol != NULL )
	{
		symbol->stateChanged.connect( BIND_SLOT1( SymbolContainer::symbolChangedState, this ) );
		symbols[symbol->getID()] = symbol;

		addedSymbol( this, symbol );
	}
}


void SymbolContainer::removeSymbol( Symbol *symbol )
{
	SymbolMap::iterator iter = symbols.find( symbol->getID() );
	if( iter != symbols.end() )
	{
		// hold on to a reference until signal emission is complete
		RefPtr<Symbol> symbolReference( symbol );
		
		symbol->stateChanged.disconnect( BIND_SLOT1( SymbolContainer::symbolChangedState, this ) );
		symbols.erase( iter );
		
		removedSymbol( this, symbol );
	}
}


void SymbolContainer::symbolChangedState( Symbol *symbol )
{
	if( symbol->getState() == Symbol::Destroyed )
		removeSymbol( symbol );
}

