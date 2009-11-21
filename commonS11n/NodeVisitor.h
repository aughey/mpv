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


#ifndef _NODEVISITOR_H_
#define _NODEVISITOR_H_

#include <ostream>

namespace mpvs11n
{
// forward declarations
class GroupNode;
class StringNode;
class IntNode;
class FloatNode;
class DoubleNode;
class DoubleVectorNode;


//=========================================================
//! 
//! 
class NodeVisitor
{
public:
	//=========================================================
	//! General Constructor
	//! 
	NodeVisitor( std::ostream &os );
	
	//=========================================================
	//! General Destructor
	//! 
	virtual ~NodeVisitor();
	
	virtual void visit( GroupNode		 &n ) const = 0;
	virtual void visit( StringNode		 &n ) const = 0;
	virtual void visit( IntNode 		 &n ) const = 0;
	virtual void visit( FloatNode		 &n ) const = 0;
	virtual void visit( DoubleNode		 &n ) const = 0;
	virtual void visit( DoubleVectorNode &n ) const = 0;
	
protected:
	
	//! The output stream.  Could be an ofstream, an ostringstream, cout, etc.
	//! Child classes will write to this.
	std::ostream &output;
	
};

}

#endif
