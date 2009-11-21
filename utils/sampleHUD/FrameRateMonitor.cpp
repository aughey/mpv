/*
 *  acm : an aerial combat simulator for X
 *  Copyright (C) 2007       Andrew Sampson
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 dated June, 1991.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program;  if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave., Cambridge, MA 02139, USA.
 */

#include <math.h>
#include "FrameRateMonitor.h"


FrameRateMonitor::FrameRateMonitor()
{
	standardDeviation = 0.0;
	average = 0.0;
	maxHistoryLength = 60;
}

FrameRateMonitor::~FrameRateMonitor()
{

}


void FrameRateMonitor::update( double deltaT )
{
	history.push_back( deltaT );
	if( history.size() > maxHistoryLength )
		history.pop_front();
	
	updateAverage();
	updateStandardDeviation();
}


void FrameRateMonitor::updateAverage()
{
	average = 0.0;
	if( history.size() > 0 )
	{
		std::deque<double>::iterator iter = history.begin();
		for( ; iter != history.end(); iter++ )
		{
			average += *iter;
		}
		average = average / (double)history.size();
	}
}


void FrameRateMonitor::updateStandardDeviation()
{
	standardDeviation = 0.0;
	if( history.size() > 0 )
	{
		double deviation;
		std::deque<double>::iterator iter = history.begin();
		for( ; iter != history.end(); iter++ )
		{
			deviation = *iter - average;
			standardDeviation += deviation * deviation;
		}
		standardDeviation = sqrt( standardDeviation / (double)history.size() );
	}
}


