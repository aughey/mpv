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
 *  This class constructs EntityElements containing nodes from model files
 *  
 *  Initial Release: 2007-03-24 Andrew Sampson
 * </pre>
 */


#ifndef _MODEL_ELEMENT_FACTORY_H_
#define _MODEL_ELEMENT_FACTORY_H_

#include "EntityElementFactory.h"
#include "ModelCache.h"


class ModelElementFactory : public mpvosg::EntityElementFactory
{
public:
	ModelElementFactory();
	virtual ~ModelElementFactory();
	
	virtual mpvosg::EntityElement *createElement( 
		DefFileGroup *elementDefinition, mpv::Entity *ent );
	
	
protected:
	osg::ref_ptr< ModelCache > modelCache;
	
};

#endif
