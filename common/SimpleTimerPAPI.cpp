/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2007 The Boeing Company
 *  
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *  
 *  
 *  FILENAME:   SimpleTimerPAPI.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  accurately measure elapsed time.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  03/29/2004 Greg Basler                       MPV_CR_DR_1
 *  Initial Release.
 *  
 *  2007-11-23 Andrew Sampson
 *      Split up the MPVTimer class into several parts.  Those parts include 
 *      a simple timer base class, several platform-specific child classes, 
 *      and a more complex timer class that combines several simple timers.
 *  
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#define _EXPORT_MPVCMN_

#include <iostream>
#include <fstream>

#include <papi.h>

#include "SimpleTimerPAPI.h"

// ================================================
// 
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
SimpleTimerPAPI::SimpleTimerPAPI() : SimpleTimerBase()
{
	// FIXME - need  some way to ensure that this only gets called once ever for the lifetime of the program
	PapiError = false;
	if (PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT)
	{
		printf( "There was an error while initializing the PAPI library.\n" );
		printf( "The PAPI library is used for performance timing; timers will not work.\n" );
		PapiError = true;
	}
	startUSecs = 0;
}


// ================================================
// 
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
SimpleTimerPAPI::~SimpleTimerPAPI()
{
}


// ================================================
// start
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void SimpleTimerPAPI::start()
{
	if( PapiError )
	{
		startUSecs = 0;
	}
	else
	{
		startUSecs = PAPI_get_real_usec();
	}
}


// ================================================
// stop
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void SimpleTimerPAPI::stop()
{
	if( PapiError )
	{
		elapsedTime = -1;
	}
	else
	{
		long_long endUSecs = PAPI_get_real_usec();
		elapsedTime = (double)(endUSecs - startUSecs) / 1000000.0;
		startUSecs = endUSecs;
	}
}
