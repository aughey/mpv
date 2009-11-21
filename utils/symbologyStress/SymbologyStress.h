/** <pre>
 * MPV symbology stress test utility
 * Copyright (c) 2007 General Dynamics Land Systems
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
 *     Initial version.  Based in part on symbologyTest.cpp, which was in turn 
 *     based on the GDLS symbology test.
 * 
 */


#ifndef SYMBOLOGYSTRESS_H
#define SYMBOLOGYSTRESS_H

#include <CigiHostSession.h>
#include <CigiIGCtrlV3_2.h>
#include <CigiSOFV3_2.h>
#include <CigiIO.h>
#include <CigiExceptions.h>
#include <CigiSymbolSurfaceDefV3_3.h>
#include <CigiSymbolCtrlV3_3.h>
#include <CigiSymbolTextDefV3_3.h>
#include <CigiSymbolCircleDefV3_3.h>
#include <CigiSymbolLineDefV3_3.h>

#include "Network.h"
#include "SimpleTimer.h"

#include "Functor.h"


class SymbologyStress
{

public:

	SymbologyStress();
	~SymbologyStress();

	void init();
	int run();

private:

	void runFunctor( Functor &functor );


	SimpleTimer timer;
	
	unsigned int frame;

	CigiHostSession session;
	CigiIncomingMsg &incoming;
	CigiOutgoingMsg &outgoing;

	CigiIGCtrlV3_2 IG;

	Network network;
	unsigned char incomingBuffer[ 4096 ];
	int incomingBufferSize;
	
	
};

#endif
