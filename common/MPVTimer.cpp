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
 *  FILENAME:   MPVTimer.cpp
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

#include <iostream>
#include <fstream>
#include <string>

#ifdef __linux__
#include <errno.h>
#include <string.h>
#endif

#include "MPVTimer.h"



// ================================================
// MPVTimerInit
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void MPVTimer::MPVTimerInit(std::string NameIn, bool MainTimerIn, GenerateID *genIDIn)
{
   MainTimer = MainTimerIn;
   name = NameIn;

   if(genIDIn)
   {
      GenID = genIDIn;
      TmrID = GenID->GetNextID("Timer");
   }
   else
   {
      GenID = NULL;
      TmrID = 1;
   }

#if TIMER_TYPE == TIMER_TYPE_WIN32

   if(MainTimer)
   {
      LARGE_INTEGER tFreq;
      double dFreq;

      if(QueryPerformanceFrequency(&tFreq))
      {
         dFreq = (double)tFreq.QuadPart;
         TimerPeriod = 1.0/dFreq;
         TmrPVld = true;
      }
      else
      {
         TimerPeriod = 0.0;
         TmrPVld = false;
      }
   }
   else
   {
      TmrPVld = false;
   }
   RecordedClockTicks = 0;

#elif TIMER_TYPE == TIMER_TYPE_PAPI

   // FIXME - need  some way to ensure that this only gets called once ever for the lifetime of the program
   PapiError = false;
   if (PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT)
   {
      printf( "There was an error while initializing the PAPI library.\n" );
      printf( "The PAPI library is used for performance timing; timers will not work.\n" );
      PapiError = true;
//      exit(1);
   }
   RecordedUSecs = 0;

#elif TIMER_TYPE == TIMER_TYPE_x86_GCC_ASM

   if(MainTimer)
   {
      double dFreq = GetMHz()*1e6;
   
      if( dFreq < 0. )
      {
         TimerPeriod = 0.0;
         TmrPVld = false;
      }
      else
      {
         TimerPeriod = 1.0/dFreq;
         TmrPVld = true;
      }
   }
   else
   {
      TmrPVld = false;
   }
   RecordedClockTicks = 0;
   
#endif

   CurrentTime = 0.0;
   PrevTime = 0.0;
   ParentTimer = NULL;

}


// ================================================
// ~MPVTimer
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
MPVTimer::~MPVTimer()
{

   ClearSubTimers( );
}


// ================================================
// Begin
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void MPVTimer::Begin(void)
{
   Time();
}


// ================================================
// Time
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void MPVTimer::Time(void)
{

#if TIMER_TYPE == TIMER_TYPE_WIN32
   LARGE_INTEGER tcnt;

   if(TmrPVld)
   {
      if(TimeVld = (QueryPerformanceCounter(&tcnt) != 0))
         RecordedClockTicks = tcnt.QuadPart;
      else
         RecordedClockTicks = 0;
   }
   else
   {
      RecordedClockTicks = 0;
      TimeVld = false;
   }
#elif TIMER_TYPE == TIMER_TYPE_PAPI
   
   if( PapiError )
   {
      RecordedUSecs = 0;
      TimeVld = false;
   }
   else
   {
      RecordedUSecs = PAPI_get_real_usec();
      TimeVld = true;
   }
#elif TIMER_TYPE == TIMER_TYPE_x86_GCC_ASM
   if(TmrPVld)
   {
#ifdef __i386
      // Treat eax and edx as one 64 bit register
      __asm__ __volatile__("rdtsc" : "=A" (RecordedClockTicks));
#elif defined(__x86_64)
      unsigned long high;
      /* Registers must be read individually then combined as there just
       * isn't an instruction to read the TSC into one 64 bit register.
       * The trick for 32 bit systems doesn't work because the registers
       * are already 64 bits.
       * =a is EAX, the low 32 bits, -d is EDX the high 32 bits
       */
      __asm__ __volatile__("rdtsc" : "=a" (RecordedClockTicks), "=d" (high));
      RecordedClockTicks|=high<<32;
#endif
      TimeVld = true;
   }
   else
   {
      RecordedClockTicks = 0;
      TimeVld = false;
   }
#else

   TimeVld = false;

#endif

}


// ================================================
// End
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void MPVTimer::End(void)
{
   PrevTime = CurrentTime;

   if(MainTimer)
   {
      MPVTimer *PrevTmr = this;

      for(std::list<MPVTimer *>::iterator Iter=SubTimers.begin();
          Iter != SubTimers.end(); Iter++)
      {
         (*Iter)->End(PrevTmr);

         PrevTmr = *Iter;
      }

      // PrevTmr now points to the *last* subtimer
      
      if(TimeVld && PrevTmr->TimeVld)
      {
#if TIMER_TYPE == TIMER_TYPE_WIN32
         __int64 tTime = 0;

         if(RecordedClockTicks < PrevTmr->RecordedClockTicks)
            tTime = PrevTmr->RecordedClockTicks - RecordedClockTicks;
//         else
//            tTime = RecordedClockTicks + (??? - PrevTimer->RecordedClockTicks);
         else
            printf("MainTimer Time Problem: a=%d : b=%d\n",RecordedClockTicks,PrevTmr->RecordedClockTicks);

         CurrentTime = (double)tTime * TimerPeriod;
#elif TIMER_TYPE == TIMER_TYPE_PAPI
         long_long deltaT;
         deltaT = RecordedUSecs - PrevTmr->RecordedUSecs;
         
         CurrentTime = (double)deltaT / 1000000.0;
#elif TIMER_TYPE == TIMER_TYPE_x86_GCC_ASM
         unsigned long long tTime = 0;

         if(RecordedClockTicks < PrevTmr->RecordedClockTicks)
            tTime = PrevTmr->RecordedClockTicks - RecordedClockTicks;
         else
            printf("MainTimer Time Problem: %s=%lli : %s=%lli\n", name.c_str(), RecordedClockTicks, PrevTmr->name.c_str(), PrevTmr->RecordedClockTicks);

         CurrentTime = (double)tTime * TimerPeriod;
#else
         CurrentTime = 0.0;
#endif
      }
      else
         CurrentTime = 0.0;

   }

}


void MPVTimer::AddSubTimer( std::string &NameIn ) 
{
   AttachSubTimer( new MPVTimer(NameIn,false,GenID) );
}


void MPVTimer::ClearSubTimers( ) 
{
   // Delete and clear the subtimers
   for(std::list<MPVTimer *>::iterator Iter=SubTimers.begin();
        Iter != SubTimers.end(); Iter++)
   {
      delete (*Iter);
   }

   SubTimers.clear();
}


// ================================================
// End  - protected
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void MPVTimer::End(MPVTimer *PrevTimer)
{
   PrevTime = CurrentTime;

   if(!MainTimer)
   {
      if(TimeVld && PrevTimer->TimeVld)
      {
#if TIMER_TYPE == TIMER_TYPE_WIN32
         __int64 tTime = 0;

         if(RecordedClockTicks > PrevTimer->RecordedClockTicks)
            tTime = RecordedClockTicks - PrevTimer->RecordedClockTicks;
//         else
//            tTime = PrevTimer->RecordedClockTicks + (??? - RecordedClockTicks);
         else
            printf("Sub-Timer Time Problem: a=%d : b=%d\n",RecordedClockTicks,PrevTimer->RecordedClockTicks);

         CurrentTime = (double)tTime * TimerPeriod;
#elif TIMER_TYPE == TIMER_TYPE_PAPI
         long_long deltaT;
         deltaT = RecordedUSecs - PrevTimer->RecordedUSecs;
         
         CurrentTime = (double)deltaT / 1000000.0;
#elif TIMER_TYPE == TIMER_TYPE_x86_GCC_ASM
         unsigned long long tTime = 0;

         if(RecordedClockTicks > PrevTimer->RecordedClockTicks)
            tTime = PrevTimer->RecordedClockTicks - RecordedClockTicks;
         else
            printf("Sub-Timer Time Problem: %s=%lli : %s=%lli\n", name.c_str(), RecordedClockTicks, PrevTimer->name.c_str(), PrevTimer->RecordedClockTicks);

         CurrentTime = (double)tTime * TimerPeriod;
#else
         CurrentTime = 0.0;
#endif
      }
      else
         CurrentTime = 0.0;

   }


}


// ================================================
// AttachSubTimer - protected
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void MPVTimer::AttachSubTimer(MPVTimer *Timer)
{

   SubTimers.push_back(Timer);
   Timer->ParentTimer = this;
#if TIMER_TYPE == TIMER_TYPE_WIN32 || TIMER_TYPE == TIMER_TYPE_x86_GCC_ASM
   Timer->TimerPeriod = TimerPeriod;
   Timer->TmrPVld = TmrPVld;
#endif

}

double MPVTimer::GetMHz()
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
