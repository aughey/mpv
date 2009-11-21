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
 *  FILENAME:   SimpleTimerGTOD.cpp
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

#include <sys/time.h>

#include "SimpleTimerGTOD.h"

// ================================================
// SimpleTimerGTOD
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
SimpleTimerGTOD::SimpleTimerGTOD() : SimpleTimerBase()
{
}


// ================================================
// ~SimpleTimerGTOD
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
SimpleTimerGTOD::~SimpleTimerGTOD()
{
}


// ================================================
// start
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void SimpleTimerGTOD::start()
{
	startEpoch = getEpochTime();
}


// ================================================
// stop
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void SimpleTimerGTOD::stop()
{
	double endEpoch = getEpochTime();
	elapsedTime = endEpoch - startEpoch;
	startEpoch = endEpoch;
}


// ================================================
// getEpochTime
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
double SimpleTimerGTOD::getEpochTime()
{
	struct timeval tv;
	gettimeofday( &tv, NULL );
	return (double)tv.tv_sec + ((double)tv.tv_usec / 1000000.0f);
}

