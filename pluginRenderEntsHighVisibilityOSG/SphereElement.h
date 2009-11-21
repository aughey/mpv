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
 *  FILENAME:   SphereElement.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  create high-visibility visual aids, which can help 
 *  users locate entities more quickly.  
 *  
 *  2007-05-21 Andrew Sampson
 *  
 */

#ifndef SPHERE_ELEMENT_H
#define SPHERE_ELEMENT_H

#include "EntityElement.h"
#include "SphereIndicator.h"

//=========================================================
//! This class can create high-visibility visual aids, which can help 
//! users locate entities more quickly.  This is especially useful for 
//! testing and debugging, and for "newbie" users who may have difficulty 
//! finding other entities.  Let's be honest: it's difficult to spot a 
//! two-pixel blob moving at 500+ knots.
//! 
class SphereElement : public mpvosg::EntityElement
{
public:
	SphereElement();
	virtual ~SphereElement();
	
	virtual bool construct( DefFileGroup *config, mpv::Entity *entity );
	
	virtual osg::Node *getTopNode() { return sphere.get(); }
	
	virtual bool addChildElement( mpvosg::EntityElement *childElement );
	
protected:
	
	osg::ref_ptr< SphereIndicator > sphere;
};

#endif
