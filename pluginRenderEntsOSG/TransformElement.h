/** <pre>
 *  Entity scene graph plugin
 *  Copyright (c) 2007 Andrew Sampson
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
 *  Initial Release: 2007-11-03 Andrew Sampson
 * </pre>
 */


#ifndef _TRANSFORM_ELEMENT_H_
#define _TRANSFORM_ELEMENT_H_

#include <osg/Vec3>
#include <osg/MatrixTransform>

#include "EntityElement.h"


/**
 *  This class is responsible for creating a subgraph containing a 
 *  transform node.  It can be used to move and rotate other entity elements,
 *  which can be attached underneath.
 */
class TransformElement : public mpvosg::EntityElement
{
public:
	
	TransformElement();
	
	virtual ~TransformElement();
	
	virtual bool construct( DefFileGroup *config, mpv::Entity *entity );
	
	virtual osg::Node *getTopNode() { return transformNode.get(); }
	
	virtual bool addChildElement( mpvosg::EntityElement *childElement );
	
protected:
	
	osg::ref_ptr< osg::MatrixTransform > transformNode;
	
	//=========================================================
	//! Convenience variable
	//! 
	static osg::Vec3 xAxis;
	//=========================================================
	//! Convenience variable
	//! 
	static osg::Vec3 yAxis;
	//=========================================================
	//! Convenience variable
	//! 
	static osg::Vec3 zAxis;

};


#endif
