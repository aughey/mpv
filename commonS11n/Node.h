/** <pre>
 *  MPV Serialization Framework
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
 *  2008-12-29 Andrew Sampson
 *      Initial release
 *  
 *  
 *  </pre>
 */


#ifndef _NODE_H_
#define _NODE_H_

#include <string>

#include "Referenced.h"

#include "NodeVisitor.h"

namespace mpvs11n
{

//=========================================================
//! 
//! 
class Node : public mpv::Referenced
{
public:
	//=========================================================
	//! General Constructor
	//! \param n node's name
	//! 
	explicit Node( const std::string &n );
	
	//! Returns the Node's name
	const std::string &getName() const { return name; }
	
	//! Visitor pattern: called by visitor, turns around and calls 
	//! visitor.visit( something )
	virtual void accept( const NodeVisitor &visitor ) = 0;
	
protected:

	//=========================================================
	//! General Destructor
	//! Because this class is reference-counted, instances must be created 
	//! on the heap, not the stack.  Declaring the destructor protected 
	//! ensures that users of this class will not be able to create 
	//! stack-allocated instances.  
	//! 
	virtual ~Node();
	
	//! Must not contain whitespace, quotes, punctuation.  Must start with a 
	//! letter.  Should be lower-case.
	std::string name;
};

}

#endif
