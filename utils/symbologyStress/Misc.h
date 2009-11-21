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

#ifndef MISC_H
#define MISC_H

#define SURFACE_ID 0

/*
surface coordinates for surface 0 are as follows:

       *------------* 1,1
       |            |
       |            |
       |            |
       |            |
       |            |
-1,-1  *------------*

*/
#define SURFACE_MIN_U -1.0
#define SURFACE_MIN_V -1.0
#define SURFACE_MAX_U 1.0
#define SURFACE_MAX_V 1.0

bool randBool();

// note - this is inclusive; min and max are valid return values
int randInt( int min, int max );

float randFloat( float min, float max );

float interpolate( float min, float max, float percentage );

#endif
