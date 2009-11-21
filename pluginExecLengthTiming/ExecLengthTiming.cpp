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
 *  FILENAME:   ExecLengthTiming.cpp
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


#include <iostream>
#include <memory>
#include <string.h>

#include "ExecLengthTiming.h"

#include <Blackboard.h>
#include <DefFileParser.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

EXPORT_DYNAMIC_CLASS( ExecLengthTiming )

// ================================================
// ExecLengthTiming
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ExecLengthTiming::ExecLengthTiming() : Plugin() 
{
	name_ = "ExecLengthTiming";
	licenseInfo_.setLicense( LicenseInfo::LicenseGPL );
	licenseInfo_.setOrigin( "Boeing" );

	dependencies_.push_back( "PluginDefFileReader" );

   DefFileData = NULL;
   BaseProgDir = NULL;
   ExeTmr = NULL;
   MxTmr = 0;
   PendTmrCnt = 0;
   Log = NULL;

}


// ================================================
// ~ExecLengthTiming
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ExecLengthTiming::~ExecLengthTiming() throw()
{
   if(Log != NULL)
      fclose(Log);

}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ExecLengthTiming::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{

	case SystemState::BlackboardRetrieve:
      bb_->get( "DefinitionData", DefFileData );

      bb_->get( "TimerList", ExeTmr );

      bb_->get( "BaseProgramDirectory", BaseProgDir );

      break;

	case SystemState::ConfigurationProcess:
		processConfigData();
		break;

	case SystemState::Operate:
		if((ExeTmr != NULL)&&(Log != NULL))
			ProcTimer(ExeTmr);
		break;

	default:
		break;
	}
	
}


// ================================================
// ProcTimer
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ExecLengthTiming::ProcTimer( std::list<MPVTimer *> *TmrLst )
{
   char LogBuf[1024];
   int cnt;

   union bufload {
      double *pdbl;
      long *plng;
      char *pchr;
   } bufld;

   for(std::list<MPVTimer *>::iterator tmr = TmrLst->begin();
       tmr != TmrLst->end(); tmr++)
   {
      int id = (*tmr)->GetID();

      if(id > MxTmr)
      {
         PendTmrCnt = id - MxTmr;
         for(int idlp=(MxTmr+1);idlp<=id;idlp++)
         {
            PendTmr.push_back(idlp);
         }
         MxTmr = id;
      }

      if(PendTmrCnt > 0)
      {
         std::list<int>::iterator iPend = PendTmr.begin();
         bool found = false;
         while(!found && (iPend != PendTmr.end()))
         {
            if(id == *iPend)
            {
               found = true;
               if(Log != NULL)
               {
                  std::string name = (*tmr)->GetName();
                  cnt = (int)name.length() + 12;
                  bufld.pchr = LogBuf;
                  *bufld.plng++ = 0;
                  *bufld.plng++ = (*tmr)->GetID();

                  if(cnt < 1000)
                  {
                     *bufld.plng++ = cnt;
                     strcpy(bufld.pchr,name.c_str());
                  }
                  else
                  {
                     cnt = 1000;
                     *bufld.plng++ = cnt;

                     memcpy(bufld.pchr,name.c_str(),987);
                     memset((bufld.pchr+987),0,1);
                  }

                  cnt=(int)fwrite(LogBuf,1,cnt,Log);
               }

               PendTmr.erase(iPend);
            }
         }
      }


      // Write out the log information
      bufld.pchr = LogBuf;
      *bufld.plng++ = id;
      *bufld.pdbl = (*tmr)->GetCrntTime();
      cnt = 12;
      cnt = (int)fwrite(LogBuf,1,12,Log);

      std::list<MPVTimer *> *tTmrLst = (*tmr)->GetSubTimers();
      if(tTmrLst->begin() != tTmrLst->end())
         ProcTimer(tTmrLst);

   }

}


// ================================================
// processConfigData
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ExecLengthTiming::processConfigData( void )
{
   char flnm[1024];

   if(*DefFileData != NULL)
   {

      std::list< DefFileGroup * >::iterator iter = (*DefFileData)->children.begin();

      while(iter != (*DefFileData)->children.end())
	   {
   		if( (*iter)->getName() == "execlengthtiming" ) // ignore everything else
	   	{
            DefFileAttrib *attr = (*iter)->getAttribute("directory");
            strcpy(flnm,(attr->asString()).c_str());

            int len = strlen(flnm);

            if(len > 0)
            {
               if((flnm[len-1] != '\\')&&(flnm[len-1] != '/'))
                  strcat(flnm,"/");

               strcat(flnm,"ExecLenTmr.log");

               // Open the file
               Log = fopen(flnm,"wb");

               break;
            }
         }
         iter++;
      }
   }

   if(Log == NULL)
   {
      if(BaseProgDir != NULL)
      {
         // Set the log directory to the default directory
         strcpy(flnm,BaseProgDir);

         int len = strlen(flnm);

         if(len > 0)
         {
            if((flnm[len-1] != '\\')&&(flnm[len-1] != '/'))
               strcat(flnm,"/");

            strcat(flnm,"ExecLenTmr.log");

            // Open the file
            Log = fopen(flnm,"wb");
         }
      }
   }

}


