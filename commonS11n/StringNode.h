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


#ifndef _STRINGNODE_H_
#define _STRINGNODE_H_

#include <string>

#include "Node.h"

namespace mpvs11n
{

//=========================================================
//! 
//! 
class StringNode : public Node
{
public:
	//=========================================================
	//! General Constructor
	//! \param n node's name
	//! 
	explicit StringNode( const std::string &n );
	
	//! Visitor pattern: called by visitor, turns around and calls 
	//! visitor.visit( something )
	virtual void accept( const NodeVisitor &visitor );
	
	const std::string &get() const;
	
	void set( const std::string &v );
	
protected:

	//=========================================================
	//! General Destructor
	//! 
	virtual ~StringNode();
	
	std::string value;
};

}

#endif
