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
 *  FILENAME:   SphereElement.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  create high-visibility visual aids, which can help 
 *  users locate entities more quickly.  
 *  
 *  2007-05-21 Andrew Sampson
 *  
 */

#include "SphereElement.h"

using namespace mpv;
using namespace mpvosg;

SphereElement::SphereElement() : EntityElement()
{
	sphere = new SphereIndicator;
}


SphereElement::~SphereElement()
{
	
}


bool SphereElement::construct( DefFileGroup *config, Entity *entity )
{
	// check to see if there is a "color" attribute
	DefFileAttrib *attr = config->getAttribute( "color" );
	if( attr )
	{
		std::vector<float> v = attr->asFloats();
		if( v.size() >= 3 )
			sphere->setColor( osg::Vec3( v[0], v[1], v[2] ) );
	}

	return true;
}


bool SphereElement::addChildElement( EntityElement *childElement )
{
	if( childElement == NULL ) return false;
	
	osg::Node *node = childElement->getTopNode();
	if( node == NULL ) return false;
	
	sphere->addChild( node );
	
	// call parent
	return EntityElement::addChildElement( childElement );
}


