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

#ifndef COMPONENT_CONTAINER_H
#define COMPONENT_CONTAINER_H

#include <utility>
#include <map>

#include "Referenced.h"
#include "MPVCommonTypes.h"
#include "Component.h"


namespace mpv
{

//=========================================================
//! Implements a container for components.  The lifetime of the components in the 
//! container will be managed, and some of the component-related signals will 
//! be handled here.
//! 
class MPVCMN_SPEC ComponentContainer : public virtual Referenced
{
public:

	boost::signal<void (ComponentContainer*, Component*)> addedComponent;
	boost::signal<void (ComponentContainer*, Component*)> removedComponent;
	
	typedef std::map< int, RefPtr<Component> > ComponentMap;
	typedef std::pair< ComponentMap::iterator, ComponentMap::iterator > ComponentIteratorPair;

	//=========================================================
	//! Constructor
	//! 
	ComponentContainer();

	//=========================================================
	//! Calls update() on the components in this container
	//! 
	void updateComponents( double timeElapsed );

	//=========================================================
	//! Flags each component in this container as 
	//! destroyed.
	//! 
//	void flagAllComponentsAsDestroyed();
	
	//=========================================================
	//! Adds the specified component to this container.
	//! \param component - the component to add
	//! 
	void addComponent( Component *component );
	
	//=========================================================
	//! Removes the specified component from this container.  
	//! \param component - the component to remove
	//! 
	void removeComponent( Component *component );
	
	//=========================================================
	//! Returns the begin and end iterators for the component container.
	//! \return the begin and end iterators for the component container.
	//! 
	ComponentIteratorPair getComponents()
	{
		return ComponentIteratorPair( components.begin(), components.end() );
	}
	
	//=========================================================
	//! Searches the container for the given component.
	//! \param componentID the id of the component to search for
	//! \return a pointer to the requested Component, or NULL if not found
	//! 
	Component *findComponent( int componentID )
	{
		ComponentMap::iterator mapIter = 
			components.find( componentID );
		if( mapIter != components.end() )
		{
			return mapIter->second.get();
		}
		return NULL;
	}
	
	//=========================================================
	//! Searches the container for the given component.  If a matching 
	//! component is not found, a new component is created and added.
	//! \param componentID the id of the component to search for
	//! \return a pointer to the requested Component
	//! 
	Component *findOrCreateComponent( int componentID );

#if 0
	//=========================================================
	//! Returns the underlying map data structure.  This should only be 
	//! called in very rare situations... I didn't plan on allowing 
	//! direct access to this variable.
	//! \return a pointer to the underlying map data structure
	//! 
	ComponentMap *getComponentMap()
	{
		return &components;
	}
#endif
	
protected:
	
	//=========================================================
	//! Destructor
	//! 
	virtual ~ComponentContainer();
	
	//=========================================================
	//! The components attached to this object.
	//! 
	ComponentMap components;

	//=========================================================
	//! Callback; notification of state change for a component
	//! 
//	void componentChangedState( Component *component );
};

}
#endif
