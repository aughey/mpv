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
 *  FILENAME:   SimpleTimerBase.h
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

#ifndef _SIMPLE_TIMER_BASE_H_
#define _SIMPLE_TIMER_BASE_H_

#include <string>

#include "MPVCommonTypes.h"

//=========================================================
//! A class for measuring elapsed time.  This base implementation does nothing.
//! Class usage is as follows: 
//!     - call start()
//!     - do some stuff
//!     - call stop()
//!     - call getElapsedTime() to find out how long it took.
//! 
class MPVCMN_SPEC SimpleTimerBase
{
public:
	
	SimpleTimerBase() :
	  name( "None" ),
	  id( 0 ),
	  elapsedTime( 0.0 )
	{}
	
	virtual ~SimpleTimerBase() {}
	
	//=========================================================
	//! Starts the timer.  Child classes will need to implement this method.
	//! 
	virtual void start() = 0;
	
	//=========================================================
	//! Stops the timer.  Child classes will need to implement this method.
	//! 
	virtual void stop() = 0;
	
	//=========================================================
	//! Gets the timer's name
	//! \return the timer's name
	//!
	std::string getName() const { return name; }

	//=========================================================
	//! Sets the timer's name
	//! \param newName - the timer's new name
	//!
	void setName( const std::string &newName ) { name = newName; }

	//=========================================================
	//! Gets the timer's ID
	//! \return the timer's ID
	//!
	int getID() const { return id; }
	
	//=========================================================
	//! Sets the timer's ID
	//! \param newID - the timer's new ID
	//!
	void setID( int newID ) { id = newID; }

	//=========================================================
	//! Returns the time elapsed between the calls to start() and stop()
	//! \return the elapsed time, in seconds
	//!
	double getElapsedTime() { return elapsedTime; }

	
protected:

	//=========================================================
	//! The name of this timer.
	//!
	std::string name;
	
	//=========================================================
	//! The ID number for this timer.
	//!
	int id;

	//=========================================================
	//! The elapsed time between the calls to start() and stop(), in seconds.
	//!
	double elapsedTime;


};

#endif

