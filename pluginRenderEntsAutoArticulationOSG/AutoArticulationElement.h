/** <pre>
 *  MPV plugin to automatically manipulate entity articulations
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
 *  2008-11-02 Andrew Sampson
 *      Initial release
 *  
 * </pre>
 */


#ifndef _AUTO_ARTICULATION_ELEMENT_H_
#define _AUTO_ARTICULATION_ELEMENT_H_

#include <osg/Group>

#include "Articulation.h"

#include "EntityElement.h"

//! An entity element that manipulates Articulations.  
//! This class didn't need to be implemented as an EntityElement; it could 
//! have been implemented as an ArticulationImp, or as an EntityImp.  I chose 
//! to make it an EntityElement in order to leverage the existing config-data-
//! handling stuff which comes with PluginRenderEntsOSG & EntityElement.
class AutoArticulationElement : public mpvosg::EntityElement
{
public:
	
	AutoArticulationElement();
	
	virtual ~AutoArticulationElement();
	
	virtual bool construct( DefFileGroup *config, mpv::Entity *entity );
	
	virtual osg::Node *getTopNode() { return groupNode.get(); }
	
	virtual bool addChildElement( mpvosg::EntityElement *childElement );
	
protected:
	
	osg::ref_ptr< osg::Group > groupNode;
	
	mpv::RefPtr< mpv::Articulation > articulation;
	
	void entityPositionChanged( mpv::GeodeticObject* );
	
	mpv::Entity *entity;
	
	//! Only yaw and roll have 360 degrees of rotation.  Pitch has only 180, 
	//! so it can not be used for this kind of articulation.
	enum AxisOfRotation
	{
		AxisYaw,
		AxisPitch,
		AxisRoll
	};
	
	AxisOfRotation axisOfRotation;
	
	//! The wheel radius.  Used to calculate the amount of rotation based on 
	//! the distance traveled by the entity.
	float radius;
	
	//! The position of the wheel relative to the entity origin.  In entity 
	//! scene graph coordinates, ie ENU.
	mpv::Vect3 referencePosition;
	
	//! The position of the referencePosition, in absolute coordinates, the 
	//! last time the entity moved.
	mpv::Vect3 previousPosition;
};


#endif
