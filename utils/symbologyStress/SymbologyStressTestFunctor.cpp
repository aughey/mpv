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

#include "SymbologyStressTestFunctor.h"
#include "Misc.h"
#include "Diamond.h"
#include "Pinwheel.h"

SymbologyStressTestFunctor::SymbologyStressTestFunctor( 
	CigiIncomingMsg &i, CigiOutgoingMsg &o ) : Functor( i, o )
{
	
}


SymbologyStressTestFunctor::~SymbologyStressTestFunctor()
{
	
}


bool SymbologyStressTestFunctor::operator()( double deltaT )
{
	frameRateMonitor.update( deltaT );
	
	if( symbols.size() < 200 )
	{
		createNewSymbol();
	}
	
	SymbolList::iterator iter = symbols.begin();
	for( ; iter != symbols.end(); iter++ )
	{
		(*iter)->update( deltaT );
	}
	
	// return true to continue calling this functor
	return true;
}


SymbolSet* SymbologyStressTestFunctor::createNewSymbol()
{
	SymbolSet *result = NULL;
	
	switch( randInt( 0, 1 ) )
	{
	case 0:
		result = new Diamond( outgoing );
		break;
	case 1:
		result = new Pinwheel( outgoing );
		break;
	default:
		break;
	}
	
	result->initialize( symbolIdPool );
	symbols.push_back( result );
	return result;
}


void SymbologyStressTestFunctor::deleteSymbol( SymbolSet *symbol )
{
	
	symbols.remove( symbol );
}


