/** <pre>
 *  Plugin to add text labels to entities, for the MPV
 *  Copyright (c) 2006 Andrew Sampson
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
 *  Initial Release: 2006-03-19 Andrew Sampson
 *  
 *  2007-07-10  Andrew Sampson
 *      Ported to the new entity construction system.  Based on the 
 *      LabelConstructor class.
 * </pre>
 */


#ifndef _LABEL_ELEMENT_H_
#define _LABEL_ELEMENT_H_

#include <osg/Group>

#include "EntityElement.h"


class LabelElement : public mpvosg::EntityElement
{
public:

	LabelElement();

	virtual ~LabelElement();
	
	virtual bool construct( DefFileGroup *config, mpv::Entity *entity );
	
	virtual osg::Node *getTopNode() { return groupNode.get(); }
	
	virtual bool addChildElement( mpvosg::EntityElement *childElement );
	
protected:

	osg::ref_ptr< osg::Group > groupNode;
};


#endif
