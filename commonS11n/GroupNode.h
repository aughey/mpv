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


#ifndef _GROUPNODE_H_
#define _GROUPNODE_H_

#include <list>

#include "Node.h"

namespace mpvs11n
{

//=========================================================
//! 
//! 
class GroupNode : public Node
{
public:

	typedef std::list< mpv::RefPtr< Node > > NodeList;
	typedef std::pair< NodeList::iterator, NodeList::iterator > NodeIteratorPair;

	//=========================================================
	//! General Constructor
	//! \param n node's name
	//! 
	explicit GroupNode( const std::string &n );
	
	//! Visitor pattern: called by visitor, turns around and calls 
	//! visitor.visit( something )
	virtual void accept( const NodeVisitor &visitor );
	
	//! 
	void addChild( Node *node );
	
	//! 
	void removeChild( Node *node );
	
	//! 
	void removeAllChildren();
	
	//=========================================================
	//! Returns the begin and end iterators for the child node list.
	//! \return the begin and end iterators for the child node list.
	//! 
	NodeIteratorPair getChildren()
	{
		return NodeIteratorPair( children.begin(), children.end() );
	}
	
	
protected:

	//=========================================================
	//! General Destructor
	//! 
	virtual ~GroupNode();
	
	NodeList children;
};

}

#endif
