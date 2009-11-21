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
 *  FILENAME:   NearFarManipulatorElementFactory.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  DESCRIPTION: 
 *  This class constructs EntityElements containing nodes which will 
 *  manipulate the near and far clipping plane values.
 *  
 *  2007-05-22 Andrew Sampson
 *  
 */


#ifndef NEARFARMANIPULATORELEMENTFACTORY_H
#define NEARFARMANIPULATORELEMENTFACTORY_H

#include "EntityElementFactory.h"


class NearFarManipulatorElementFactory : public mpvosg::EntityElementFactory
{
public:
	NearFarManipulatorElementFactory();
	virtual ~NearFarManipulatorElementFactory();
	
	virtual mpvosg::EntityElement *createElement( 
		DefFileGroup *elementDefinition, mpv::Entity *ent );
	
};

#endif
