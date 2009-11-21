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
 *  FILENAME:   MPVTimer.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *   manage the timing measurements.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  03/29/2004 Greg Basler                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#if !defined(_MPV_TIMER_INCLUDED_)
#define _MPV_TIMER_INCLUDED_

#include <list>
#include <string>

// The TIMER_TYPE define has the following possible values:
#define TIMER_TYPE_NONE            0
#define TIMER_TYPE_WIN32           1
#define TIMER_TYPE_PAPI            2
#define TIMER_TYPE_x86_GCC_ASM    3

#if defined(WIN32) && !(defined(__CYGWIN__) || defined(__MINGW32__))
#  include <windows.h>
#  define TIMER_TYPE TIMER_TYPE_WIN32
#elif defined(HAVE_PAPI)
#  include <papi.h>
#  define TIMER_TYPE TIMER_TYPE_PAPI
#elif defined(__GNUC__)
#  if defined(__i386) || defined(__x86_64)
#    define TIMER_TYPE TIMER_TYPE_x86_GCC_ASM
#  elif defined(__CYGWIN__) || defined(__MINGW32__)
#    define TIMER_TYPE TIMER_TYPE_NONE
#  else
#    define TIMER_TYPE TIMER_TYPE_NONE
#  endif
#else
#  define TIMER_TYPE TIMER_TYPE_NONE
#endif

#if TIMER_TYPE == TIMER_TYPE_x86_GCC_ASM
#include <sys/timex.h>
#endif

#include "GenerateID.h"

#if defined(_MSC_VER)
   #pragma warning(push)
   #pragma warning(disable : 4251)
#endif

//=========================================================
//! A class for measuring elapsed time.  It has support for so-called 
//! "sub timers" (analogous to the lap time feature of stopwatches).  
//! On systems that support it, this class uses the CPU's instruction 
//! counter to provide highly-accurate timing values.
//!
class MPVCMN_SPEC MPVTimer  
{
public:


   //==> Management

   //=========================================================
   //! General Constructor
   //!
	MPVTimer(GenerateID *genIDIn)
   {
      MPVTimerInit("None", false, genIDIn);
   }

   //=========================================================
   //! General Constructor
   //!
	MPVTimer(std::string NameIn, GenerateID *genIDIn)
   {
      MPVTimerInit(NameIn, false, genIDIn);
   }

   //=========================================================
   //! General Constructor
   //!
	MPVTimer(bool MainTimerIn, GenerateID *genIDIn)
   {
      MPVTimerInit("None", MainTimerIn, genIDIn);
   }


   //=========================================================
   //! General Constructor
   //!
   MPVTimer(std::string NameIn, bool MainTimerIn, GenerateID *genIDIn)
   {
      MPVTimerInit(NameIn, MainTimerIn, genIDIn);
   }

   //=========================================================
   //! General Initializer
   //!
   void MPVTimerInit(std::string NameIn, bool MainTimerIn, GenerateID *genIDIn);

   //=========================================================
   //! General Destructor
   //!
	virtual ~MPVTimer();




   //==> Accessing Timing Functions


   //=========================================================
   //! Start the timing process<br>
   //! This is used mostly for main timers
   //!
   void Begin(void);

   //=========================================================
   //! Record the current time<br>
   //! This is used mostly for sub-timers
   //!
   void Time(void);

   //=========================================================
   //! End the timing process.<br>
   //! This determines all the times in seconds
   //!
   void End(void);


   //==> Accessing Modifiable Member functions


/* win32-specific, and not used anywhere -ads
   //=========================================================
   //! Gets the timer period
   //! \return the timer period
   //!
   double GetTimerPeriod(void) { return(TimerPeriod); }
*/

   //=========================================================
   //! Gets the timer's name
   //! \return the timer's name
   //!
   std::string GetName(void) { return(name); }

   //=========================================================
   //! Gets the timer's ID
   //! \return the timer's ID
   //!
   int GetID(void) { return(TmrID); }

   //=========================================================
   //! Gets the current time
   //! \return the current time
   //!
   double GetCrntTime(void) { return(CurrentTime); }

   //=========================================================
   //! Gets the previous time
   //! \return the previous time
   //!
   double GetPrevTime(void) { return(PrevTime); }

   //=========================================================
   //! Gets a list of the subtimers
   //! \return a pointer to the list of subtimers
   //!
   std::list<MPVTimer *> *GetSubTimers(void)
   {
      return(&SubTimers);
   }

   //=========================================================
   //! Create a new subtimer with the given name
   //! \param NameIn - the name for the new subtimer
   //!
   void AddSubTimer( std::string &NameIn ) ;

   //=========================================================
   //! Delete and clear the subtimers
   //!
   void ClearSubTimers( ) ;

   //=========================================================
   //! Get the MHz of the cpu clock if on Linux returning
   //! -1 on any errors.
   //! 
   double GetMHz();

protected:


   //==> Member variables

   //=========================================================
   //! name<br>
   //! The name of this timer.
   //!
   std::string name;

   //=========================================================
   //! TmrID<br>
   //! The ID number for this timer.
   //!
   int TmrID;

#if TIMER_TYPE == TIMER_TYPE_WIN32
   //=========================================================
   //! TimerPeriod<br>
   //! The time period between timer clock ticks.<br>
   //!
   double TimerPeriod;

   //=========================================================
   //! RecordedClockTicks<br>
   //! The clock-tick counter value recorded by the Begin, End, or Time calls.
   //!
   __int64 RecordedClockTicks;
#elif TIMER_TYPE == TIMER_TYPE_PAPI
   //=========================================================
   //! RecordedUSecs<br>
   //! The time recorded by the Begin, End, or Time calls.
   //!
   long_long RecordedUSecs;
   
   //=========================================================
   //! PapiError<br>
   //! Set to true if there was an error while setting up the PAPI library.
   //!
   bool PapiError;
#elif TIMER_TYPE == TIMER_TYPE_x86_GCC_ASM
   // If we have the header file or we have the gnu compiler that can
   // take the assembly for the rdtscll routine we do timing.
   
   //=========================================================
   //! TimerPeriod<br>
   //! The time period between timer clock ticks.<br>
   //!
   double TimerPeriod;
   
   //=========================================================
   //! RecordedClockTicks<br>
   //! The clock-tick counter value recorded by the Begin, End, or Time calls.
   //!
   unsigned long long RecordedClockTicks;
#endif

   //=========================================================
   //! CurrentTime<br>
   //! The time recorded for this frame in seconds.
   //!
   double CurrentTime;

   //=========================================================
   //! PrevTime<br>
   //! The time recorded for the previous frame in seconds.
   //!
   double PrevTime;

   //=========================================================
   //! SubTimers<br>
   //! A list of sub-timers.
   //!
	std::list<MPVTimer *> SubTimers;

   //=========================================================
   //! ParentTimer<br>
   //! The parent timer.
   //!
	MPVTimer *ParentTimer;

   //=========================================================
   //! MainTimer<br>
   //! A flag which, when true, indicates that this is a main timer...???
   //!
	bool MainTimer;


   //==> Valid Flags

#if TIMER_TYPE == TIMER_TYPE_WIN32 || TIMER_TYPE == TIMER_TYPE_x86_GCC_ASM
   //=========================================================
   //! TmrPVld<br>
   //! TimerPeriod is valid.
   //!
   bool TmrPVld;
#endif

   //=========================================================
   //! TimeVld<br>
   //! Recorded time is valid.
   //!
   bool TimeVld;


   //=========================================================
   //! GenID<br>
   //! Used for getting unique timer IDs
   //!
   GenerateID *GenID;

   //==> protected functions

   //=========================================================
   //! End the timing process.<br>
   //! This determines all the times in seconds
   //!
   void End(MPVTimer *PrevTimer);


   //=========================================================
   //! Adds a sub-timer
   //! \param Timer - The sub-timer
   //!
   void AttachSubTimer(MPVTimer *Timer);


};

#if defined(_MSC_VER)
   #pragma warning(pop)
#endif

#endif // !defined(_MPV_TIMER_INCLUDED_)
