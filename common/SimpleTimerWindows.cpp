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
 *  FILENAME:   SimpleTimerWindows.cpp
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

#include <iostream>
#include <fstream>

#include <windows.h>

#include "SimpleTimerWindows.h"

// ================================================
// SimpleTimerWindows
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
SimpleTimerWindows::SimpleTimerWindows() : SimpleTimerBase()
{
	LARGE_INTEGER tFreq;
	double dFreq;

	if(QueryPerformanceFrequency(&tFreq))
	{
		dFreq = (double)tFreq.QuadPart;
		period = 1.0/dFreq;
		periodValid = true;
	}
	else
	{
		period = 0.0;
		periodValid = false;
	}
	startClockTicks = 0;
}


// ================================================
// ~SimpleTimerWindows
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
SimpleTimerWindows::~SimpleTimerWindows()
{
}


// ================================================
// start
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void SimpleTimerWindows::start()
{
	startClockTicks = getClockTicks();
}


// ================================================
// stop
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void SimpleTimerWindows::stop()
{
	elapsedTime = -1.0;
	if( periodValid )
	{
		__int64 endClockTicks = getClockTicks();

		if( endClockTicks > startClockTicks )
		{
			elapsedTime = (double)(endClockTicks - startClockTicks) * period;
		}
		else
			printf( "Warning in SimpleTimerWindows: Timer %s has startClockTicks = %d, endClockTicks = %d\n", name.c_str(), startClockTicks, endClockTicks );

		startClockTicks = endClockTicks;
	}
}


// ================================================
// getClockTicks
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
__int64 SimpleTimerWindows::getClockTicks()
{
	__int64 result = 0;

	LARGE_INTEGER tcnt;
	if( QueryPerformanceCounter(&tcnt) != 0 )
		result = tcnt.QuadPart;
	
	return result;
}
