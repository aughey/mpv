/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2004 The Boeing Company
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
 *  FILENAME:   Blackboard.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data sharing mechanism.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  03/29/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#ifndef _BLACKBOARD_INCLUDED_
#define _BLACKBOARD_INCLUDED_


#include <string>
#include <map>

#include <boost/any.hpp>

#include "MPVCommonTypes.h"
#include "MPVExceptions.h"

#if defined(_MSC_VER)
   #pragma warning(push)
   #pragma warning(disable : 4251)
   #pragma warning(disable: 4290)
#endif

class MPVCMN_SPEC Blackboard {

public:


	//==> Management

	//=========================================================
	//! General Constructor
	//!
	Blackboard();


	//=========================================================
	//! General Destructor
	//!
	~Blackboard();


	//==> Blackboard data handling


	//=========================================================
	//! Places a data object onto the blackboard
	//! \param key - name by which the data object will be indexed, retrieved
	//! \param obj - the data object to post; this is usually a pointer
	//!
	template <class T> void put( const std::string &key, T obj )
		throw ( MPVBlackboardException& )
	{
		if( locked )
			throw MPVBlackboardException( "Blackboard is locked" );
		map_[key] = obj;
	}

	//=========================================================
	//! Query the blackboard for a data object by name
	//! \param key - the name to search for
	//! \return true if a matching entry was found on the blackboard, false otherwise
	//!
	bool contains( const std::string &key ) const;

	//=========================================================
	//! Retrieves a data object from the blackboard
	//! \param key - the name of the object to retrieve
	//! \param obj - if the key is valid, this reference will be set to the object
	//! \param mandatory - if set to true, and if no matching entry was found, 
	//!    then an exception will be thrown
	//! \return true if a matching entry was found on the blackboard, false otherwise
	//!
	template <class T> bool get( const std::string &key, T &obj, bool mandatory = true ) 
		throw ( MPVBlackboardException& )
	{
		std::map<std::string, boost::any>::iterator iter = map_.find( key );
		if( iter == map_.end() )
		{
			if( mandatory )
				throw MPVBlackboardException( key + " not on blackboard" );
			return false;
		}
		obj = boost::any_cast<T>( (*iter).second );
		return true;
	}

private:

	//=========================================================
	//! Stores the contents of the blackboard.  The boost::any class contains 
	//! dark magic, which allows us to store a heterogeneous collection of 
	//! objects in this map, while still preserving type information.
	//! 
	std::map<std::string, boost::any> map_;

	bool locked;
};

#if defined(_MSC_VER)
   #pragma warning(pop)
#endif

#endif  // _BLACKBOARD_INCLUDED_

