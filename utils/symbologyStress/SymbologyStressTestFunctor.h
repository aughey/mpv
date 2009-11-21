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

#ifndef SYMBOLOGYSTRESSTESTFUNCTOR_H
#define SYMBOLOGYSTRESSTESTFUNCTOR_H

#include <list>

#include "Functor.h"
#include "FrameRateMonitor.h"
#include "InstanceIDPool.h"
#include "SymbolSet.h"

class SymbologyStressTestFunctor : public Functor
{
public:
	SymbologyStressTestFunctor( CigiIncomingMsg &i, CigiOutgoingMsg &o );
	virtual ~SymbologyStressTestFunctor();
	
	virtual bool operator()( double deltaT );


protected:
	
	SymbolSet* createNewSymbol();
	void deleteSymbol( SymbolSet *symbol );
	

	FrameRateMonitor frameRateMonitor;
	InstanceIDPool symbolIdPool;
	
	typedef std::list< RefPtr<SymbolSet> > SymbolList;
	SymbolList symbols;
};

#endif
