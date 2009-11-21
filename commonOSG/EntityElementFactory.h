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
 *  The children of this base class construct EntityElements
 *  
 *  Initial Release: 2007-03-24 Andrew Sampson
 * </pre>
 */


#ifndef _ENTITY_ELEMENT_FACTORY_H_
#define _ENTITY_ELEMENT_FACTORY_H_

#include <string>

#include "Entity.h"
#include "DefFileGroup.h"
#include "EntityElement.h"

#include "MPVCommonOSGTypes.h"


namespace mpvosg
{

class MPVCMNOSG_SPEC EntityElementFactory
{
public:
	EntityElementFactory();
	virtual ~EntityElementFactory();
	
	const std::string &getKeyword() const { return keyword; }
	
	virtual EntityElement *createElement( 
		DefFileGroup *elementDefinition, mpv::Entity *ent ) = 0;
	
protected:
	
	//=========================================================
	//! The keyword that this factory recognizes/handles.  createElement() will 
	//! be called when an element is encountered in the config file that 
	//! matches this string.
	//!
	std::string keyword;
	
};

}
#endif
