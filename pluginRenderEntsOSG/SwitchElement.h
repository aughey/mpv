/** <pre>
 *  Entity scene graph plugin
 *  Copyright (c) 2008 Andrew Sampson
 *  
 *  This component is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; specifically 
 *  version 2.1 of the License.
 *  
 *  This software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *  
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this software; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *  
 *  
 *  2008-09-28 Andrew Sampson
 *      Initial Release.  File copied from TransformElement.h
 *  
 * </pre>
 */


#ifndef _SWITCH_ELEMENT_H_
#define _SWITCH_ELEMENT_H_

#include <osg/Vec3>
#include <osg/Switch>

#include "EntityElement.h"


/**
 *  This class is responsible for creating a subgraph containing a 
 *  switch node.  It can be used to enable/disable other entity elements,
 *  which are attached underneath.
 */ 
class SwitchElement : public mpvosg::EntityElement
{
public:
	
	SwitchElement();
	
	virtual ~SwitchElement();
	
	virtual bool construct( DefFileGroup *config, mpv::Entity *entity );
	
	virtual osg::Node *getTopNode() { return switchNode.get(); }
	
	virtual bool addChildElement( mpvosg::EntityElement *childElement );
	
protected:
	
	osg::ref_ptr< osg::Switch > switchNode;
	
};


#endif
