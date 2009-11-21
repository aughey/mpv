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

#include "ArticulationContainer.h"

using namespace mpv;


ArticulationContainer::ArticulationContainer() : Referenced()
{
	
}


ArticulationContainer::~ArticulationContainer()
{
	// when an ArticulationContainer is deleted, all Articulations attached to it are deleted
//	flagAllArticulationsAsDestroyed();
}


void ArticulationContainer::updateArticulations( double timeElapsed )
{
	ArticulationMap::iterator iter = articulations.begin();
	for( ; iter != articulations.end(); iter++ )
	{
		iter->second->update( timeElapsed );
	}
}


/*void ArticulationContainer::flagAllArticulationsAsDestroyed()
{
	while( !articulations.empty() )
	{
		// hold on to a reference until signal emission is complete
		RefPtr<Articulation> articulation( articulations.begin()->second );

		articulation->setState( Articulation::Destroyed );
		// note - callback articulationChangedState will handle removal of articulation 
		// from the map
	}
}*/


void ArticulationContainer::addArticulation( Articulation *articulation )
{
	if( articulation != NULL )
	{
//		articulation->stateChanged.connect( SLOT( this, ArticulationContainer::articulationChangedState ) );
		articulations[articulation->getID()] = articulation;

		addedArticulation( this, articulation );
	}
}


void ArticulationContainer::removeArticulation( Articulation *articulation )
{
	ArticulationMap::iterator iter = articulations.find( articulation->getID() );
	if( iter != articulations.end() )
	{
		// hold on to a reference until signal emission is complete
		RefPtr<Articulation> articulationReference( articulation );
		
//		articulation->stateChanged.disconnect( SLOT( this, ArticulationContainer::articulationChangedState ) );
		articulations.erase( iter );
		
		removedArticulation( this, articulation );
	}
}


Articulation *ArticulationContainer::findOrCreateArticulation( int articulationID )
{
	Articulation *result = findArticulation( articulationID );
	if( result == NULL )
	{
		result = new Articulation;
		result->setID( articulationID );
		addArticulation( result );
	}
	return result;
}


/*void ArticulationContainer::articulationChangedState( Articulation *articulation )
{
	if( articulation->getState() == Articulation::Destroyed )
		removeArticulation( articulation );
}*/

