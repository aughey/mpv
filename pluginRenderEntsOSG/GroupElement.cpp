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
 *  This class is responsible for creating a subgraph containing a single 
 *  group node.  It is the simplest EntityElement, and is used as a fall-back 
 *  when things go awry.
 *  
 *  Initial Release: 2007-03-27 Andrew Sampson
 * </pre>
 */


#include "GroupElement.h"


using namespace mpv;
using namespace mpvosg;

GroupElement::GroupElement() : EntityElement()
{
	groupNode = new osg::Group;
}

GroupElement::~GroupElement()
{
	
}


bool GroupElement::construct( DefFileGroup *config, Entity *entity )
{
	return true;
}


bool GroupElement::addChildElement( EntityElement *childElement )
{
	if( childElement == NULL ) return false;
	
	osg::Node *node = childElement->getTopNode();
	if( node == NULL ) return false;
	
	groupNode->addChild( node );
	
	// call parent
	return EntityElement::addChildElement( childElement );
}

