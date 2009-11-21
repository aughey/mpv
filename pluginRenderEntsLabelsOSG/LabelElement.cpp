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

#include <stdio.h>
#include <iostream>

#include <osg/Group>
#include <osg/Geode>

#ifdef WIN32
#define snprintf _snprintf
#endif

#include "LabelElement.h"
#include "LabelNode.h"
#include "LabelNodeComponentImp.h"

using namespace mpv;
using namespace mpvosg;

// ================================================
// LabelElement
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
LabelElement::LabelElement() : EntityElement()
{
	groupNode = new osg::Group;
}


// ================================================
// ~LabelElement
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
LabelElement::~LabelElement()
{

}


// ================================================
// construct
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
bool LabelElement::construct( DefFileGroup *config, Entity *entity )
{
	
	osg::Vec3 color( 0, 1, 0 );
	std::string defaultText = entity->getName();
	float textSize = 0.2;
	osg::Vec3 offset( 0, 0, 0 );

	DefFileAttrib *attr;

	// check to see if there is a "default_text" attribute
	attr = config->getAttribute( "default_text" );
	if( attr )
	{
		defaultText = attr->asString();
	}

	// check to see if there is a "color" attribute
	attr = config->getAttribute( "color" );
	if( attr )
	{
		std::vector<float> v = attr->asFloats();
		if( v.size() >= 3 )
			color.set( v[0], v[1], v[2] );
	}

	// check to see if there is a "size" attribute
	attr = config->getAttribute( "size" );
	if( attr )
	{
		textSize = attr->asFloat();
	}

	// check to see if there is a "offset" attribute
	attr = config->getAttribute( "offset" );
	if( attr )
	{
		std::vector<float> v = attr->asFloats();
		if( v.size() >= 3 )
			offset.set( v[0], v[1], v[2] );
	}

	LabelNode *label = new LabelNode;
	label->setText( defaultText );
	label->setColor( color );
	label->setSize( textSize );
	label->setPosition( offset );
	groupNode->addChild( label );

	// check to see if there is a "component_id" attribute
	attr = config->getAttribute( "component_id" );
	if( attr )
	{
		int componentID = attr->asInt();

		Component *component = entity->findOrCreateComponent( componentID );
		// check to see if the component was just created
		if( component->getInstanceID() != entity->getID() )
		{
			component->setInstanceID( entity->getID() );
			component->setName( "Entity Label" );
		}

		LabelNodeComponentImp *componentImp = 
			new LabelNodeComponentImp( component, defaultText, label );
		component->addImplementation( componentImp );
	}

	return true;
}


// ================================================
// addChildElement
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
bool LabelElement::addChildElement( EntityElement *childElement )
{
	if( childElement == NULL ) return false;
	
	osg::Node *node = childElement->getTopNode();
	if( node == NULL ) return false;
	
	groupNode->addChild( node );
	
	// call parent
	return EntityElement::addChildElement( childElement );
}

