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
 *  This class is responsible for loading model files from disk, and presenting 
 *  the resulting scene graph nodes in the form of an "entity element".
 *  
 *  Initial Release: 2007-03-24 Andrew Sampson
 * </pre>
 */


#ifndef _MODEL_ELEMENT_H_
#define _MODEL_ELEMENT_H_

#include <osg/Group>

#include "EntityElement.h"
#include "ModelCache.h"


//=========================================================
//! This class is responsible for loading model files from disk, and presenting 
//! the resulting scene graph nodes in the form of an "entity element".
//! 
class ModelElement : public mpvosg::EntityElement
{
public:
	
	ModelElement( ModelCache *cache );
	
	virtual ~ModelElement();
	
	virtual bool construct( DefFileGroup *config, mpv::Entity *entity );
	
	virtual osg::Node *getTopNode() { return groupNode.get(); }
	
	virtual bool addChildElement( mpvosg::EntityElement *childElement );
	
protected:
	
	osg::ref_ptr< osg::Group > groupNode;
	osg::ref_ptr< ModelCache > modelCache;
	
	void copyAndAttachModelNode( DefFileGroup *config, mpv::Entity *entity, osg::Node *cachedNode );

	void constructTransformArtPart( 
		DefFileGroup *config, mpv::Entity *entity, osg::Node *modelNode );
	void constructSwitchComponentCtrl( 
		DefFileGroup *config, mpv::Entity *entity, osg::Node *modelNode );

	//=========================================================
	//! Creates the default model (the model displayed when a specified model 
	//! file could not be loaded)
	//! 
	osg::Node *createDefaultModel();

};


#endif
