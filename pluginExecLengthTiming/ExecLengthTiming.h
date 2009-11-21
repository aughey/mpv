/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2004 The Boeing Company
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
 *  FILENAME:   ExecLengthTiming.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This plugin writes the timer values to disk, so that they can be 
 *  recorded and analysed.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  04/04/2004 Greg Basler                       MPV_CR_DR_1
 *  Initial Release.
 *  
 *  2007-07-14 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#ifndef _PLUGINEXECLENGTHTIMING_H_
#define _PLUGINEXECLENGTHTIMING_H_

#include "Plugin.h"

#include <stdio.h>

#include <list>
#include <string>
#include <map>

#include <DefFileGroup.h>
#include <MPVTimer.h>



//=========================================================
//! This plugin writes the timer values to disk, so that they can be 
//! recorded and analysed.  The timing data is written out in a 
//! binary format.  The binary format can be converted to more useful 
//! formats by the timerLogConverter utility, which is included with 
//! the MPV.
//!
class ExecLengthTiming : public Plugin 
{
public:

   //=========================================================
   //! General Constructor
   //!
	ExecLengthTiming();

   //=========================================================
   //! General Destructor
   //!
	virtual ~ExecLengthTiming() throw();

	//=========================================================
	//! The per-frame processing that this plugin performs
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );

	
protected:


   //==> Member variables

   //=========================================================
   //! A pointer to a pointer to the Definition File data
   //!
   DefFileGroup **DefFileData;

   //=========================================================
   //! A pointer to a list of execution timers
   //!
   std::list<MPVTimer *> *ExeTmr;

   //=========================================================
   //! The Maximum Timer ID encountered
   //!
   int MxTmr;

   //=========================================================
   //! The number of Timer log identification pending
   //!
   int PendTmrCnt;

   //=========================================================
   //! A list of Timer log identification pending
   //!
   std::list<int> PendTmr;

   //=========================================================
   //! A list of Timer log identification pending
   //!
   FILE *Log;

   //=========================================================
   //! A list of Timer log identification pending
   //!
   char *BaseProgDir;


   //==> protected functions

   //=========================================================
   //! The per-frame processing that this plugin performs
   //!
	void ProcTimer( std::list<MPVTimer *> *TmrLst );

   //=========================================================
   //! Process the config file data
   //!
   void processConfigData( void );



};



#endif

