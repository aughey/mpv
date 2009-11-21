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

#ifndef DIAMOND_H
#define DIAMOND_H

#include "SymbolLine.h"

#include "SymbolSet.h"

class Diamond : public SymbolSet
{
public:
	Diamond( CigiOutgoingMsg &message );
	
	virtual void initialize( InstanceIDPool &symbolIdPool );
	
	virtual void updateMotion( double deltaT );
	
protected:
	virtual ~Diamond();
	
	RefPtr< SymbolLine > line;
	
	Vect2 velocity;
	float rotationalVelocity;
	
	// pulses per second
	float pulseRate;
	// min and max scaling factors for pulses
	float pulseScaleMin;
	float pulseScaleMax;
	
	// in seconds
	double age;
};

#endif
