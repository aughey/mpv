/** <pre>
 *  Sample HUD, using the CIGI symbology interface
 *  Copyright (c) 2009 The Boeing Company
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
 *  2009-08-31 Andrew Sampson
 *      Initial release
 *  
 *  
 *  </pre>
 */


#ifndef _PITCHLADDER_H_
#define _PITCHLADDER_H_

#include <map>

#include "HUDState.h"
#include "SymbolLine.h"
#include "SymbolText.h"
#include "SymbolSet.h"

//=========================================================
//! 
//! 
class PitchLadder : public SymbolSet
{
public:
	//=========================================================
	//! General Constructor
	//! 
	PitchLadder( CigiOutgoingMsg &message, HUDState *h );
	
	virtual void initialize( InstanceIDPool &symbolIdPool );
	
	virtual void updateMotion( double deltaT );
	
	
protected:
	//=========================================================
	//! General Destructor
	//! 
	virtual ~PitchLadder();
	
	Symbol * createLadderRung( InstanceIDPool &symbolIdPool, Symbol *parent, int pitch );
	
	void pitchChanged( HUDState * );
	void rollChanged( HUDState * );
	
	mpv::RefPtr<HUDState> hudState;
	
	float waterlinePosition;
	
	mpv::RefPtr<SymbolLine> waterline;
	mpv::RefPtr<SymbolLine> ladderRoll;
	mpv::RefPtr<SymbolLine> ladderPitch;
	
	std::map< int, mpv::RefPtr<Symbol> > ladderRungs;
};

#endif
