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
 *  This class constructs EntityElements containing just a group node
 *  
 *  Initial Release: 2007-03-27 Andrew Sampson
 * </pre>
 */


#ifndef _GROUP_ELEMENT_FACTORY_H_
#define _GROUP_ELEMENT_FACTORY_H_

#include "EntityElementFactory.h"


class GroupElementFactory : public mpvosg::EntityElementFactory
{
public:
	GroupElementFactory();
	virtual ~GroupElementFactory();
	
	virtual mpvosg::EntityElement *createElement( 
		DefFileGroup *elementDefinition, mpv::Entity *ent );
	
	
protected:
	
};

#endif
