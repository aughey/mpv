/** <pre>
 *  The MPV Common Library
 *  Copyright (c) 2008
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
 *  2008-07-05 Andrew Sampson
 *      Initial release
 *
 * </pre>
 */

#include "ComponentContainer.h"

using namespace mpv;


ComponentContainer::ComponentContainer() : Referenced()
{
	
}


ComponentContainer::~ComponentContainer()
{
	// when a ComponentContainer is deleted, all Components attached to it are deleted
//	flagAllComponentsAsDestroyed();
}


void ComponentContainer::updateComponents( double timeElapsed )
{
	ComponentMap::iterator iter = components.begin();
	for( ; iter != components.end(); iter++ )
	{
		iter->second->update( timeElapsed );
	}
}


/*void ComponentContainer::flagAllComponentsAsDestroyed()
{
	while( !components.empty() )
	{
		// hold on to a reference until signal emission is complete
		RefPtr<Component> component( components.begin()->second );

		component->setState( Component::Destroyed );
		// note - callback componentChangedState will handle removal of component 
		// from the map
	}
}*/


void ComponentContainer::addComponent( Component *component )
{
	if( component != NULL )
	{
//		component->stateChanged.connect( SLOT( this, ComponentContainer::componentChangedState ) );
		components[component->getID()] = component;

		addedComponent( this, component );
	}
}


void ComponentContainer::removeComponent( Component *component )
{
	ComponentMap::iterator iter = components.find( component->getID() );
	if( iter != components.end() )
	{
		// hold on to a reference until signal emission is complete
		RefPtr<Component> componentReference( component );
		
//		component->stateChanged.disconnect( SLOT( this, ComponentContainer::componentChangedState ) );
		components.erase( iter );
		
		removedComponent( this, component );
	}
}


Component *ComponentContainer::findOrCreateComponent( int componentID )
{
	Component *result = findComponent( componentID );
	if( result == NULL )
	{
		result = new Component;
		result->setID( componentID );
		addComponent( result );
	}
	return result;
}


/*void ComponentContainer::componentChangedState( Component *component )
{
	if( component->getState() == Component::Destroyed )
		removeComponent( component );
}*/

