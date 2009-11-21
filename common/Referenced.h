/** <pre>
 *  The MPV Common Library
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
 *  2008-07-15 Andrew Sampson
 *      Initial release
 *  
 *  
 *  </pre>
 */


#ifndef _REFERENCED_H_
#define _REFERENCED_H_

#include "MPVCommonTypes.h"

#include <boost/signal.hpp>
#include <boost/signals/trackable.hpp>

#include "RefPtr.h"


namespace mpv
{

//=========================================================
//! Base class for reference-counted objects
//! 
class MPVCMN_SPEC Referenced : public boost::signals::trackable
{
public:
	//=========================================================
	//! General Constructor
	//! 
	Referenced();
	
	//=========================================================
	//! Copy constructor, defined to ensure that refCount is 0 for copies
	//! 
	Referenced( const Referenced & );
	
	//=========================================================
	//! Copy operator, defined to ensure that refCount is 0 for copies
	//! 
	Referenced & operator=( const Referenced & )
	{
		return *this;
	}
	
	//! Increases reference count.  Should only be called by RefPtr.
	//! Warning - not thread-safe.
	void ref() const;
	
	//! Decreases reference count.  Should only be called by RefPtr.
	//! Warning - not thread-safe.
	void unref() const;

protected:
	//=========================================================
	//! General Destructor
	//! The destructor needs to be private, because this class is
	//! reference-counted
	//! 
	virtual ~Referenced();
	
	//! The reference count for this object.  Declared "mutable" because 
	//! it must be modified by const methods ref() and unref().
	mutable int refCount;
};

}

#endif
