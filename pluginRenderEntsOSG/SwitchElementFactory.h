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
 *      Initial Release.  File copied from TransformElementFactory.h
 *  
 * </pre>
 */


#ifndef _SWITCH_ELEMENT_FACTORY_H_
#define _SWITCH_ELEMENT_FACTORY_H_

#include "EntityElementFactory.h"


//! This class constructs EntityElements containing a switch node
class SwitchElementFactory : public mpvosg::EntityElementFactory
{
public:
	SwitchElementFactory();
	virtual ~SwitchElementFactory();
	
	virtual mpvosg::EntityElement *createElement( 
		DefFileGroup *elementDefinition, mpv::Entity *ent );
	
	
protected:
	
};

#endif
