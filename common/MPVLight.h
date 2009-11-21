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
 *  FILENAME:   MPVLight.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This defines the structure used to store lighting parameters.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  12/20/2005 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#ifndef _MPVLIGHT_INCLUDED_
#define _MPVLIGHT_INCLUDED_

#include "Vect3.h"
#include "Vect4.h"

//=========================================================
//! Structure containing the lighting parameters
//!
struct MPVLight
{
	//=========================================================
	//! If this is true, this light source is directional, and the 
	//! position attribute should be ignored.  Examples of directional 
	//! lights include the sun and moon (the sun and moon are far enough away 
	//! that they are directional lights for all practical purposes).
	//! 
	bool directional;
	
	//=========================================================
	//! Light direction.  Not guaranteed to be a unit vector.
	//! 
	mpv::Vect3 direction;
	
	//=========================================================
	//! Light position, in database coordinates.  This is ignored if 
	//! directional is set to true.
	//! 
	mpv::Vect3 position;
	
	//=========================================================
	//! Ambient color component of light.
	//! 
	mpv::Vect4 ambient;
	
	//=========================================================
	//! Diffuse color component of light.
	//! 
	mpv::Vect4 diffuse;
	
	//=========================================================
	//! Specular color component of light.
	//! 
	mpv::Vect4 specular;
	
	//Future extension: spot light parameters, such as falloff, cone size, etc.
};


#endif
