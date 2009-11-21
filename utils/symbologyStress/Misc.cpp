/** <pre>
 * MPV symbology stress test utility
 * Copyright (c) 2008 Andrew Sampson
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 * 
 * Revision history:
 * 
 * 2008-04-13  Andrew Sampson
 *     Initial version.  
 * 
 */

#include <stdlib.h>

#include "Misc.h"

bool randBool()
{
	// Which is more random, the top bit or the bottom bit?  Using top bit...
	return ( rand() > RAND_MAX/2 );
}


int randInt( int min, int max )
{
	if( min == max )
		return min;
	if( min > max )
		return randInt( max, min );

	int delta = (max - min) + 1;
	return (rand() % delta) + min;
}


float randFloat( float min, float max )
{
	if( min > max )
		return randFloat( max, min );

	return interpolate( min, max, (float)rand() / (float)RAND_MAX );
}


float interpolate( float min, float max, float percentage )
{
	float delta = max - min;
	return delta * percentage + min;
}


