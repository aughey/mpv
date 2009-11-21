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
 *  FILENAME:   NearFarManipulatorElement.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class creates nodes which, when inserted into an entity's subgraph, 
 *  will manipulate the near and far clipping plane values.
 *  
 *  03-29-2005 David Fries
 *      Initial implementation.
 *  
 *  2007-05-22 Andrew Sampson
 *      Ported to new entity-construction system.
 *  
 */

#ifndef NEARFARMANIPULATORELEMENT_H
#define NEARFARMANIPULATORELEMENT_H

#include <osg/Group>

#include "EntityElement.h"

//=========================================================
//! This class creates nodes which, when inserted into an entity's subgraph, 
//! will manipulate the near and far clipping plane values.
//! 
class NearFarManipulatorElement : public mpvosg::EntityElement
{
public:
	NearFarManipulatorElement();
	virtual ~NearFarManipulatorElement();
	
	virtual bool construct( DefFileGroup *config, mpv::Entity *entity );
	
	virtual osg::Node *getTopNode() { return nearFarTree.get(); }
	
	virtual bool addChildElement( mpvosg::EntityElement *childElement );
	
protected:
	
	osg::ref_ptr< osg::Group > nearFarTree;
	osg::ref_ptr< osg::Group > subgraph;
};

#endif
