/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2007 The Boeing Company
 *  
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *  
 *  
 *  FILENAME:   ScaleElement.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  fixme
 *  
 *  2007-07-10 Andrew Sampson
 *      Initial release.
 *  
 */

#include <iostream>

#include "ScaleElement.h"
#include "ScaleComponentImp.h"

#define DEFAULT_SCALE_COMPONENT_CTRL 301  /* matches value used by Quantum */

using namespace mpv;
using namespace mpvosg;

ScaleElement::ScaleElement() : EntityElement()
{
	transform = new osg::MatrixTransform;
}


ScaleElement::~ScaleElement()
{
	
}


bool ScaleElement::construct( DefFileGroup *config, Entity *entity )
{
	DefFileAttrib *attr = NULL;

	attr = config->getAttribute( "scale" );
	if( attr )
	{
		std::vector<float> scale = attr->asFloats();
		if( scale.size() >= 3 )
			transform->setMatrix( osg::Matrix::scale( scale[0], scale[1], scale[2] ) );
	}
	
	// check to see if there is a "component_id" attribute
	attr = config->getAttribute( "component_id" );
	if( attr )
	{
		int componentID = attr->asInt();

		// set up the component control
		Component *component = entity->findOrCreateComponent( componentID );
		// check to see if the component was just created
		if( component->getInstanceID() != entity->getID() )
		{
			component->setInstanceID( entity->getID() );
			component->setName( "Entity Scale" );
		}

		ScaleComponentImp *componentImp = 
			new ScaleComponentImp( component, transform.get() );
		component->addImplementation( componentImp );
	}
	else
	{
/*		std::cout << "Warning - in definition for entity type " 
			<< entity->getType()
			<< " - scale element is missing its \"component_id\" attribute\n";*/
	}

	return true;
}


bool ScaleElement::addChildElement( EntityElement *childElement )
{
	if( childElement == NULL ) return false;
	
	osg::Node *node = childElement->getTopNode();
	if( node == NULL ) return false;
	
	transform->addChild( node );
	
	// call parent
	return EntityElement::addChildElement( childElement );
}


