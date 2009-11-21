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
 *  FILENAME:   SimpleTimerGTOD.h
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

#ifndef _SIMPLE_TIMER_GTOD_H_
#define _SIMPLE_TIMER_GTOD_H_

#include <string>

#include "MPVCommonTypes.h"

#include "SimpleTimerBase.h"

//=========================================================
//! A class for measuring elapsed time.  This child class performs 
//! timing using gettimeofday().  It has much lower accuracy than the 
//! timer classes that use platform-specific high-performance timer APIs.
//! 
class MPVCMN_SPEC SimpleTimerGTOD : public SimpleTimerBase
{
public:
	
	SimpleTimerGTOD();
	
	virtual ~SimpleTimerGTOD();
	
	//=========================================================
	//! Starts the timer.
	//! 
	virtual void start();
	
	//=========================================================
	//! Stops the timer.
	//! 
	virtual void stop();
	
	
protected:

	//=========================================================
	//! The time recorded by start()
	//!
	double startEpoch;

	//=========================================================
	//! Calls gettimeofday(), and converts the unix epoch time to a double.
	//! \return the epoch time, in seconds
	//!
	double getEpochTime();
};

#endif

