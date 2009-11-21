/** <pre>
 *  Entity scene graph plugin
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
 *  2008-09-28 Andrew Sampson
 *      Initial Release.  File copied from TransformElement.cpp
 *  
 * </pre>
 */

#include <iostream>

#include "SwitchElement.h"

#include "SwitchNodeComponentImp.h"

using namespace mpv;
using namespace mpvosg;


SwitchElement::SwitchElement() : EntityElement()
{
	switchNode = new osg::Switch;
}

SwitchElement::~SwitchElement()
{
	
}


bool SwitchElement::construct( DefFileGroup *config, Entity *entity )
{
	int componentID = -1;
	DefFileAttrib *attr = NULL;

	
	attr = config->getAttribute( "component_id" );
	if( attr )
		componentID = attr->asInt();
	else
	{
		std::cout << "Warning - in definition for entity type " 
			<< entity->getType()
			<< " - switch component is missing its \"component_id\" attribute\n";
		return false;
	}
	

	Component *component = entity->findOrCreateComponent( componentID );
	// check to see if the component was just created
	if( component->getInstanceID() != entity->getID() )
	{
		component->setInstanceID( entity->getID() );
	}

	SwitchNodeComponentImp *componentImp = 
		new SwitchNodeComponentImp( component, switchNode->asGroup() );
	component->addImplementation( componentImp );

	// Find all the "state" sections.  These sections map a given CIGI value 
	// to a position/child-node of the switch.  I.E. if the Host specifies a 
	// value of N for the component control, the SwitchNodeCompCtrl will use 
	// these state mappings to look up the switch position associated with N, 
	// and will activate that switch position.
	
	std::list<DefFileGroup *>::iterator groupIter;
	for( groupIter = config->children.begin(); 
		groupIter != config->children.end(); groupIter++ )
	{
		DefFileGroup *stateGroup = *groupIter;
		if( stateGroup->getName() == "state" )
		{
			int cigiValue, switchValue;
			attr = stateGroup->getAttribute( "cigi_value" );
			if( attr )
			{
				cigiValue = attr->asInt();
				attr = stateGroup->getAttribute( "switch_value" );
				if( attr )
				{
					switchValue = attr->asInt();
					componentImp->addSwitchStatePair( cigiValue, switchValue );
				}
			}
		}
	}

	// If the user has specified a default value for the component's state, 
	// then honor that value.  The default value is set *after* the 
	// cigi->osgswitch state mappings are processed above.
	// Note that this has the potential for being a bit tricky:
	// - if the component already existed, then the value specified here
	//   will clobber the value already set for the component
	// - if the user has multiple component sections for the same 
	//   component id, and has provided conflicting default values for 
	//   those sections, then the results will be unpredictable
	attr = config->getAttribute( "default_cigi_value" );
	if( attr )
	{
		component->setState( attr->asInt() );
	}

	return true;
}


bool SwitchElement::addChildElement( EntityElement *childElement )
{
	if( childElement == NULL ) return false;
	
	osg::Node *node = childElement->getTopNode();
	if( node == NULL ) return false;
	
	switchNode->addChild( node );
	
	// call parent
	return EntityElement::addChildElement( childElement );
}

