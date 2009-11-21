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
 *  The children of this base class are responsible for creating scene graph 
 *  nodes for "entity elements", where entity elements can include nodes from 
 *  model files, particle systems, etc.
 *  
 *  Initial Release: 2007-03-24 Andrew Sampson
 * </pre>
 */


#ifndef _ENTITY_ELEMENT_H_
#define _ENTITY_ELEMENT_H_

#include <list>

#include <osg/Group>

#include "Entity.h"
#include "Animation.h"
#include "DefFileGroup.h"

#include "MPVCommonOSGTypes.h"

namespace mpvosg
{

class MPVCMNOSG_SPEC EntityElement
{
public:
	
	EntityElement();
	
	virtual ~EntityElement();
	
	//! \return true on success
	virtual bool construct( DefFileGroup *config, mpv::Entity *entity ) = 0;
	
	virtual osg::Node *getTopNode() = 0;
	
	//! \return true on success
	virtual bool addChildElement( EntityElement *childElement );
	
	
protected:
	
	std::list< EntityElement* > childElements;
	
};

}

#endif
