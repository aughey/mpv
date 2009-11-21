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

#ifndef ARTICULATION_CONTAINER_H
#define ARTICULATION_CONTAINER_H

#include <utility>
#include <map>

#include "Referenced.h"
#include "MPVCommonTypes.h"
#include "Articulation.h"


namespace mpv
{

//=========================================================
//! Implements a container for articulations.  The lifetime of the articulations in the 
//! container will be managed, and some of the articulation-related signals will 
//! be handled here.
//! 
class MPVCMN_SPEC ArticulationContainer : public virtual Referenced
{
public:

	boost::signal<void (ArticulationContainer*, Articulation*)> addedArticulation;
	boost::signal<void (ArticulationContainer*, Articulation*)> removedArticulation;
	
	typedef std::map< int, RefPtr<Articulation> > ArticulationMap;
	typedef std::pair< ArticulationMap::iterator, ArticulationMap::iterator > ArticulationIteratorPair;

	//=========================================================
	//! Constructor
	//! 
	ArticulationContainer();

	//=========================================================
	//! Calls update() on the articulations in this container
	//! 
	void updateArticulations( double timeElapsed );

	//=========================================================
	//! Flags each articulation in this container as 
	//! destroyed.
	//! 
//	void flagAllArticulationsAsDestroyed();
	
	//=========================================================
	//! Adds the specified articulation to this container.
	//! \param articulation - the articulation to add
	//! 
	void addArticulation( Articulation *articulation );
	
	//=========================================================
	//! Removes the specified articulation from this container.  
	//! \param articulation - the articulation to remove
	//! 
	void removeArticulation( Articulation *articulation );
	
	//=========================================================
	//! Returns the begin and end iterators for the articulation container.
	//! \return the begin and end iterators for the articulation container.
	//! 
	ArticulationIteratorPair getArticulations()
	{
		return ArticulationIteratorPair( articulations.begin(), articulations.end() );
	}
	
	//=========================================================
	//! Searches the container for the given articulation.
	//! \return a pointer to the requested Articulation, or NULL if not found
	//! 
	Articulation *findArticulation( int articulationID )
	{
		ArticulationMap::iterator mapIter = 
			articulations.find( articulationID );
		if( mapIter != articulations.end() )
		{
			return mapIter->second.get();
		}
		return NULL;
	}
	
	//=========================================================
	//! Searches the container for the given articulation.  If a matching 
	//! articulation is not found, a new articulation is created and added.
	//! \param articulationID the id of the articulation to search for
	//! \return a pointer to the requested articulation
	//! 
	Articulation *findOrCreateArticulation( int articulationID );

#if 0
	//=========================================================
	//! Returns the underlying map data structure.  This should only be 
	//! called in very rare situations... I didn't plan on allowing 
	//! direct access to this variable.
	//! \return a pointer to the underlying map data structure
	//! 
	ArticulationMap *getArticulationMap()
	{
		return &articulations;
	}
#endif
	
protected:
	
	//=========================================================
	//! Destructor
	//! 
	virtual ~ArticulationContainer();
	
	//=========================================================
	//! The articulations attached to this object.
	//! 
	ArticulationMap articulations;

	//=========================================================
	//! Callback; notification of state change for a articulation
	//! 
//	void articulationChangedState( Articulation *articulation );
};

}
#endif
