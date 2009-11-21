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


#ifndef _AIRSPEEDBOX_H_
#define _AIRSPEEDBOX_H_

#include "HUDState.h"
#include "SymbolLine.h"
#include "SymbolText.h"
#include "SymbolSet.h"

//=========================================================
//! 
//! 
class AirspeedBox : public SymbolSet
{
public:
	//=========================================================
	//! General Constructor
	//! 
	AirspeedBox( CigiOutgoingMsg &message, HUDState *h );
	
	virtual void initialize( InstanceIDPool &symbolIdPool );
	
	virtual void updateMotion( double deltaT );
	
	
protected:
	//=========================================================
	//! General Destructor
	//! 
	virtual ~AirspeedBox();
	
	void airspeedChanged( HUDState * );
	
	mpv::RefPtr<HUDState> hudState;
	
	mpv::RefPtr<SymbolLine> box;
	mpv::RefPtr<SymbolText> text;
};

#endif
