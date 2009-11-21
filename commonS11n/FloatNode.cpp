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


#include "FloatNode.h"


using namespace mpvs11n;

// ================================================
// FloatNode
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
FloatNode::FloatNode( const std::string &n ) : Node( n ),
	value( 0.0 )
{
	
}


// ================================================
// ~FloatNode
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
FloatNode::~FloatNode() 
{
	
}


// ================================================
// accept
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void FloatNode::accept( const NodeVisitor &visitor )
{
	visitor.visit( *this );
}


float FloatNode::get() const
{
	return value;
}


void FloatNode::set( float v )
{
	value = v;
}


