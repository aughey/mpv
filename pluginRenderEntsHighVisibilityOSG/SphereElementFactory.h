/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2007 The Boeing Company
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
 *  FILENAME:   SphereElementFactory.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  DESCRIPTION: 
 *  This class constructs EntityElements containing high-vis sphere nodes
 *  
 *  2007-05-21 Andrew Sampson
 *  
 */


#ifndef _SPHERE_ELEMENT_FACTORY_H_
#define _SPHERE_ELEMENT_FACTORY_H_

#include "EntityElementFactory.h"


class SphereElementFactory : public mpvosg::EntityElementFactory
{
public:
	SphereElementFactory();
	virtual ~SphereElementFactory();
	
	virtual mpvosg::EntityElement *createElement( 
		DefFileGroup *elementDefinition, mpv::Entity *ent );
	
};

#endif
