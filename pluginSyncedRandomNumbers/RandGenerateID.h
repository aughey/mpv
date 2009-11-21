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
 *  FILENAME:	RandGenerateID.h
 *  LANGUAGE:	C++
 *  CLASS:		UNCLASSIFIED
 *  PROJECT:	 Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  generate pseudo-random numbers, using the GenerateID interface.
 *  
 *  MODIFICATION NOTES:
 *  DATE	  NAME										  SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  05/10/2004 Andrew Sampson							  MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#ifndef RAND_GENERATE_ID_H
#define RAND_GENERATE_ID_H

#include "GenerateID.h"

#define MPV_RAND_MAX 2147483647

class RandGenerateID : public GenerateID
{
public:
	RandGenerateID() : GenerateID() 
	{
		seed = NULL;
		prevSeed = 0;
		lastRandNum = 0;
	}
	
	virtual int GetNextID(std::string Type)
	{
//std::cout << "hello, this is randgenid" << std::endl;
		if( !seed ) return 0;
//std::cout << "hello, this is randgenid " << __LINE__ << " seed " << *seed << " lastrandnum " << lastRandNum <<  std::endl;
		if( *seed != prevSeed )
		{
//std::cout << "hello, this is randgenid " << __LINE__ << " seed " << *seed << " lastrandnum " << lastRandNum <<  std::endl;
			lastRandNum = randFirst( *seed );
			prevSeed = *seed;
//std::cout << "hello, this is randgenid " << __LINE__ << " seed " << *seed << " lastrandnum " << lastRandNum <<  std::endl;
		}
		else 
		{
//std::cout << "hello, this is randgenid " << __LINE__ << " seed " << *seed << " lastrandnum " << lastRandNum <<  std::endl;
			lastRandNum = randSubsequent( lastRandNum );
//std::cout << "hello, this is randgenid " << __LINE__ << " seed " << *seed << " lastrandnum " << lastRandNum <<  std::endl;
		}
//std::cout << "hello, this is randgenid " << __LINE__ << " seed " << *seed << " lastrandnum " << lastRandNum <<  std::endl;
		return lastRandNum;
	}
	
	void setSeed( unsigned int *s ) { seed = s; }
	
private:
	unsigned int *seed;
	unsigned int prevSeed;
	int lastRandNum;
	
	int randFirst( int input )
	{

		int r = (( (16807 * input) + 0 ) % MPV_RAND_MAX);

		((unsigned char*)(&r))[3] = ((unsigned char*)(&r))[0];

		return r;
	}

	int randSubsequent( int input )
	{
		int r = (( (16807 * input) + 0 ) % MPV_RAND_MAX);
		return r;
	}

	
};

#endif

