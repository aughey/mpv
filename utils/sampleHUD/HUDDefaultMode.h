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
 * 2008-04-12  Andrew Sampson
 *     Initial version.  
 * 
 * 2009-08-31  Andrew Sampson
 *     Started work on sample HUD.  Based on symbologyStress utility.
 * 
 */

#ifndef HUDDEFAULTMODE_H
#define HUDDEFAULTMODE_H

#include <list>

#include "Functor.h"
#include "FrameRateMonitor.h"
#include "HUDState.h"
#include "InstanceIDPool.h"
#include "SymbolSet.h"

class HUDDefaultMode : public Functor
{
public:
	HUDDefaultMode( CigiIncomingMsg &i, CigiOutgoingMsg &o, HUDState *h );
	virtual ~HUDDefaultMode();
	
	virtual bool operator()( double deltaT );


protected:
	
	void initializeSymbols();
	
	void deleteSymbol( SymbolSet *symbol );
	

	FrameRateMonitor frameRateMonitor;
	InstanceIDPool symbolIdPool;
	
	typedef std::list< RefPtr<SymbolSet> > SymbolList;
	SymbolList symbols;
	
	mpv::RefPtr<HUDState> hudState;
	
	bool initializedSymbols;
	
	double absoluteTime;
};

#endif
