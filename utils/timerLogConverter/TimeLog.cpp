// TimeLog.cpp : Defines the entry point for the console application.
//

#ifdef WIN32
#include "stdafx.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

#include <list>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;


int main(int argc, char **argv)
{
   ifstream fin;
   ifstream oin;
   ofstream fout;
   int id = 0;
   int cnt;
   char buf[1024];
   double CrntTime;
   vector<string> Tmr;  // Timer name
   vector<double> CTm;  // current time
   int MaxTmrID = -1;
   bool endfl = false;
   char *fname;

   if( argc == 2 )
      fname = argv[1];
   else
      fname = "tmr_bin.log";

   fin.open(fname,ios_base::in|ios_base::binary);
   
   if(fin.is_open())
   {
      int pos = fin.tellg();
      // Get all the timer names
      while(!endfl)
      {
         if(!(fin.read((char *)&id,4)))
         {
            endfl = true;
            continue;
         }

         if(id == 0)
         {
            if(!(fin.read((char *)&id,4)))
            {
               endfl = true;
               continue;
            }

            while(id > MaxTmrID)
            {
               Tmr.push_back("");
               CTm.push_back(0.0);
               MaxTmrID++;
            }

            if(!(fin.read((char *)&cnt,4)))
            {
               endfl = true;
               continue;
            }

            if(cnt > 1024)
            {
               // skip bad name
               fin.seekg((cnt - 12),ios_base::cur);
               continue;
            }
            else
            {
               memset(buf,0,(cnt));
               if(!(fin.get(buf,(cnt - 11))))
               {
                  endfl = true;
                  continue;
               }

               Tmr[id] = buf;
            }
         }
         else
         {
            // skip data
            fin.seekg(8,ios_base::cur);
         }
      }

      fin.close();
   }
   else
   {
      cout << "Couldn't open binary log file\n";
      exit( 1 );
   }


   oin.open(fname,ios_base::in|ios_base::binary);
   if(oin.is_open())
   {
      fout.open("tmr_txt.csv",ios_base::out);
      if(fout.is_open())
      {

         fout.setf( ios_base::scientific, ios_base::floatfield );
         fout.precision(10);

         // Print the timer names

         // printout data
         fout << "Frame,";
         for(vector<string>::iterator tter=(Tmr.begin() + 1);
            tter != Tmr.end(); tter++)
         {
            fout << *tter << ",";
         }
         fout << endl;


         // Get the times and print
         int pos = oin.tellg();
         int frame = 0;

         endfl = false;
         while(!endfl)
         {
            if(!(oin.read((char *)&id,4)))
            {
               endfl = true;
               continue;
            }


            if(id == 0)
            {
               // skip name
               if(!(oin.read((char *)&id,4)))
               {
                  endfl = true;
                  continue;
               }
               if(!(oin.read((char *)&cnt,4)))
               {
                  endfl = true;
                  continue;
               }
               oin.seekg((cnt-12),ios_base::cur);

            }
            else if((id > 0)&&(id <= MaxTmrID))
            {
               if(CTm[id] != 0.0)
               {
                  // printout data
                  fout << frame << ",";
                  for(vector<double>::iterator iter=(CTm.begin() + 1);
                        iter != CTm.end(); iter++)
                  {
                     fout << *iter << ",";
                     *iter = 0.0;
                  }
                  fout << endl;
                  frame++;
               }

               if(!(oin.read((char *)&CrntTime,8)))
               {
                  endfl = true;
                  continue;
               }

               CTm[id] = CrntTime;

            }
            else
               oin.seekg(8,ios_base::cur);
         }

         fout.close();
      }
      oin.close();
   }

   Tmr.clear();

	return 0;
}

