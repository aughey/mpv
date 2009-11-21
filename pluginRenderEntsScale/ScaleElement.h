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
 *  FILENAME:   ScaleElement.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  fixme
 *  
 *  2007-07-10 Andrew Sampson
 *      Initial release.
 *  
 */

#ifndef _SCALE_ELEMENT_H_
#define _SCALE_ELEMENT_H_

#include "EntityElement.h"
#include <osg/MatrixTransform>

//=========================================================
//! This class fixme
//! 
class ScaleElement : public mpvosg::EntityElement
{
public:
	ScaleElement();
	virtual ~ScaleElement();
	
	virtual bool construct( DefFileGroup *config, mpv::Entity *entity );
	
	virtual osg::Node *getTopNode() { return transform.get(); }
	
	virtual bool addChildElement( mpvosg::EntityElement *childElement );
	
protected:
	
	osg::ref_ptr< osg::MatrixTransform > transform;
};

#endif
