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

#ifndef SYMBOL_CONTAINER_H
#define SYMBOL_CONTAINER_H

#include <utility>
#include <map>

#include "Referenced.h"
#include "MPVCommonTypes.h"

namespace mpv
{

// forward declaration; defined via an include at the bottom of the file
class Symbol;

//=========================================================
//! Implements a container for symbols.  The lifetime of the symbols in the 
//! container will be managed, and some of the symbol-related signals will 
//! be handled here.
//! 
class MPVCMN_SPEC SymbolContainer : public virtual Referenced
{
public:

	boost::signal<void (SymbolContainer*, Symbol*)> addedSymbol;
	boost::signal<void (SymbolContainer*, Symbol*)> removedSymbol;
	
	typedef std::map< int, RefPtr<Symbol> > SymbolMap;
	typedef std::pair< SymbolMap::iterator, SymbolMap::iterator > SymbolIteratorPair;

	//=========================================================
	//! Constructor
	//! 
	SymbolContainer();

	//=========================================================
	//! Flags each symbol in this container as 
	//! destroyed.
	//! 
	void flagAllSymbolsAsDestroyed();
	
	//=========================================================
	//! Adds the specified symbol to this container.
	//! \param symbol - the symbol to add
	//! 
	void addSymbol( Symbol *symbol );
	
	//=========================================================
	//! Removes the specified symbol from this container.  
	//! \param symbol - the symbol to remove
	//! 
	void removeSymbol( Symbol *symbol );
	
	//=========================================================
	//! Returns the begin and end iterators for the symbol container.
	//! \return the begin and end iterators for the symbol container.
	//! 
	SymbolIteratorPair getSymbols()
	{
		return SymbolIteratorPair( symbols.begin(), symbols.end() );
	}
	
	//=========================================================
	//! Searches the container for the given symbol.
	//! \return a pointer to the requested Symbol, or NULL if not found
	//! 
	Symbol *findSymbol( int symbolID )
	{
		SymbolMap::iterator mapIter = 
			symbols.find( symbolID );
		if( mapIter != symbols.end() )
		{
			return mapIter->second.get();
		}
		return NULL;
	}
	
#if 0
	//=========================================================
	//! Returns the underlying map data structure.  This should only be 
	//! called in very rare situations... I didn't plan on allowing 
	//! direct access to this variable.
	//! \return a pointer to the underlying map data structure
	//! 
	SymbolMap *getSymbolMap()
	{
		return &symbols;
	}
#endif
	
protected:
	
	//=========================================================
	//! Destructor
	//! 
	virtual ~SymbolContainer();
	
	//=========================================================
	//! The symbols attached to this object.
	//! 
	SymbolMap symbols;

	//=========================================================
	//! Callback; notification of state change for a symbol
	//! 
	void symbolChangedState( Symbol *symbol );
};

}

#include "Symbol.h"

#endif
