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
 *  FILENAME:   SimpleTimerLinuxX86.cpp
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

#include <string.h>
#include <sys/timex.h>
#include <errno.h>

#include "SimpleTimerLinuxX86.h"

// ================================================
// SimpleTimerLinuxX86
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
SimpleTimerLinuxX86::SimpleTimerLinuxX86() : SimpleTimerBase()
{
	double dFreq = GetMHz()*1e6;

	if( dFreq < 0. )
	{
		period = 0.0;
		periodValid = false;
	}
	else
	{
		period = 1.0/dFreq;
		periodValid = true;
	}
	startTSC = 0;
	
	havePrintedBuggyTSCWarning = false;
}


// ================================================
// ~SimpleTimerLinuxX86
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
SimpleTimerLinuxX86::~SimpleTimerLinuxX86()
{
}


// ================================================
// start
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void SimpleTimerLinuxX86::start()
{
	startTSC = getTSC();
}


// ================================================
// stop
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void SimpleTimerLinuxX86::stop()
{
	elapsedTime = -1.0;
	if( periodValid )
	{
		unsigned long long endTSC = getTSC();

		if( endTSC > startTSC )
		{
			elapsedTime = (double)(endTSC - startTSC) * period;
		}
		else if( !havePrintedBuggyTSCWarning )
		{
			std::cout << "Warning in SimpleTimerLinuxX86: Timer \""
				<< name << "\" has encountered nonsensical \n\ttimer values "
				<< "(startTSC = " << startTSC 
				<< ", endTSC = " << endTSC << ").\n";
			std::cout << "\tThis usually occurs only on older AMD systems with "
				<< "multiple processors.  \n\tIt is the result of a hardware bug "
				<< "related to the RDTSC instruction.  \n\tMore info can be found "
				<< "here: \n\t\thttp://en.wikipedia.org/wiki/Time_Stamp_Counter\n";
			std::cout << "\tBecause of this bug, timing values produced by this "
				<< "timer will be \n\tunreliable, and should not be trusted.\n";
			std::cout << "\tThis warning will not be repeated for this timer.\n";
			havePrintedBuggyTSCWarning = true;
		}

		startTSC = endTSC;
	}
}


// ================================================
// getTSC
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
unsigned long long SimpleTimerLinuxX86::getTSC()
{
	unsigned long long result;
	
#ifdef __i386
	// Treat eax and edx as one 64 bit register
	__asm__ __volatile__("rdtsc" : "=A" (result));
#elif defined(__x86_64)
	unsigned long high;
	/* Registers must be read individually then combined as there just
	* isn't an instruction to read the TSC into one 64 bit register.
	* The trick for 32 bit systems doesn't work because the registers
	* are already 64 bits.
	* =a is EAX, the low 32 bits, -d is EDX the high 32 bits
	*/
	__asm__ __volatile__("rdtsc" : "=a" (result), "=d" (high));
	result|=high<<32;
#endif

	return result;
}


// ================================================
// GetMHz
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
double SimpleTimerLinuxX86::GetMHz()
{
#ifdef __linux__
	// Should modify this value from the drift from adjtimex
	const char *cpuinfo ="/proc/cpuinfo";
	std::ifstream input(cpuinfo);
	if(!input)
	{
		fprintf(stderr, "Error opening %s: %s\n", cpuinfo,
			strerror(errno));
		return 0;
	}
	std::string data;
	double MHz=-1;
	while(!input.eof())
	{
		input >> data;
		if( !input.eof() && data == "cpu")
		{
			input >> data;
			if(!input.eof() && data == "MHz")
			{
				char colon;
				input >> colon;
				if(!input.eof() && colon == ':')
				{
					input >> MHz;
					break;
				}
			}
		}
	}
	if(MHz == -1)
	{
		fprintf(stderr, "Error MHz not found in %s\n", cpuinfo);
		return -1;
	}
	if(MHz < 1)
	{
		fprintf(stderr, "Error invalid MHz %f MHz found, "
			"even an Apple ][c would beat this one\n",
			MHz);
		return -1;
	}
	printf("MHz %.3f as read from %s\n", MHz, cpuinfo);
	struct timex adj;
	adj.modes=0;
	if(adjtimex(&adj) == -1)
	{
		perror("Error reading kernel clock information, timing will "
		"not be adjusted");
		return MHz;
	}
	// frequency/65536 is the parts per million that the system is off
	// negative numbers adjust it slower positive makes it faster
	// tick is in parts per 10,000
	double factor = 1 - adj.freq/65536.0/1000000.0 -
		(adj.tick-10000)/10000.0;
	MHz *= factor;
	printf("MHz %.3f as adjusted\n", MHz);

	return MHz;
#else
	// not on Linux
	return -1;
#endif
}
