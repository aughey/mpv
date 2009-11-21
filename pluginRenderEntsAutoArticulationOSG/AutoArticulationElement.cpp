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

#if defined(_MSC_VER)
   #define _USE_MATH_DEFINES
#endif

#include <cmath>
#include <iostream>

#include "BindSlot.h"

#include "AutoArticulationElement.h"

using namespace mpv;
using namespace mpvosg;

AutoArticulationElement::AutoArticulationElement() : EntityElement(),
	entity( NULL ),
	axisOfRotation( AxisRoll ),
	radius( 0.22 )
{
	groupNode = new osg::Group;
}

AutoArticulationElement::~AutoArticulationElement()
{
	
}


bool AutoArticulationElement::construct( DefFileGroup *config, Entity *_entity )
{
	this->entity = _entity;
	DefFileAttrib *attr = NULL;
	
	int articulationID = -1;

	attr = config->getAttribute( "art_part_id" );
	if( attr )
	{
		articulationID = attr->asInt();
	}
	else
	{
		std::cout << "Warning - in definition for entity type " 
			<< entity->getType()
			<< " - auto-articulation element is missing its \"art_part_id\" attribute\n";
		return false;
	}
	
	attr = config->getAttribute( "axis" );
	if( attr )
	{
		std::string axis = attr->asString();
		if( axis == "yaw" )
		{
			axisOfRotation = AxisYaw;
		}
		else if( axis == "pitch" )
		{
			axisOfRotation = AxisPitch;
		}
		else if( axis == "roll" )
		{
			axisOfRotation = AxisRoll;
		}
		else
		{
			std::cout << "Warning - in definition for entity type " 
				<< entity->getType()
				<< " - auto-articulation element's \"axis\" attribute \n"
				<< "\tis set to: \"" << axis
				<< "\", which is not a valid value\n"
				<< "\tvalid values include: yaw, roll\n";
			return false;
		}
	}
	else
	{
		std::cout << "Warning - in definition for entity type " 
			<< entity->getType()
			<< " - auto-articulation element is missing its \"axis\" attribute\n";
		return false;
	}
	
	attr = config->getAttribute( "radius" );
	if( attr )
	{
		radius = attr->asFloat();
	}
	else
	{
		std::cout << "Warning - in definition for entity type " 
			<< entity->getType()
			<< " - auto-articulation element is missing its \"radius\" attribute\n";
		return false;
	}
	
	attr = config->getAttribute( "reference_position" );
	if( attr )
	{
		std::vector<float> v = attr->asFloats();
		if( v.size() >= 3 )
			referencePosition.Set( v[0], v[1], v[2] );
	}
	else
	{
		std::cout << "Warning - in definition for entity type " 
			<< entity->getType()
			<< " - auto-articulation element is missing its \"position\" attribute\n";
		return false;
	}
	
	
	
	articulation = entity->findOrCreateArticulation( articulationID );
	// check to see if the articulation was just created
	if( articulation->getEntityID() != entity->getID() )
	{
		articulation->setEntityID( entity->getID() );
	}
	
	
	
	// set up a slot for entity position changes
	entityPositionChanged( entity );
	entity->positionDBChanged.connect( BIND_SLOT1( AutoArticulationElement::entityPositionChanged, this ) );
	
	return true;
}


bool AutoArticulationElement::addChildElement( EntityElement *childElement )
{
	if( childElement == NULL ) return false;
	
	osg::Node *node = childElement->getTopNode();
	if( node == NULL ) return false;
	
	groupNode->addChild( node );
	
	// call parent
	return EntityElement::addChildElement( childElement );
}


void AutoArticulationElement::entityPositionChanged( GeodeticObject* )
{
	int axisIndex = 0;
	if( axisOfRotation == AxisYaw )
		axisIndex = 0;
	else if( axisOfRotation == AxisPitch )
		axisIndex = 1;
	else if( axisOfRotation == AxisRoll )
		axisIndex = 2;
	
	Mtx4 transform = entity->getAbsoluteTransform();
	Vect3 absolutePosition = transform * referencePosition;
	Vect3 delta = absolutePosition - previousPosition;
	
	float distanceTraveled = delta.mag(); // sqrt
	float numRotations = distanceTraveled / (2.0 * M_PI * radius);
	
	Vect3 rotation = articulation->getRotation();
	
	rotation[axisIndex] = fmodf( rotation[axisIndex] + (numRotations * 360.0), 360.0 );
	
	articulation->setRotation( rotation );
	
	previousPosition = absolutePosition;
}
