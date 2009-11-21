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


#include "DoubleVectorNode.h"


using namespace mpv;
using namespace mpvs11n;

// ================================================
// DoubleVectorNode
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
DoubleVectorNode::DoubleVectorNode( const std::string &n ) : Node( n )
{
	
}


// ================================================
// ~DoubleVectorNode
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
DoubleVectorNode::~DoubleVectorNode() 
{
	
}


// ================================================
// accept
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void DoubleVectorNode::accept( const NodeVisitor &visitor )
{
	visitor.visit( *this );
}


std::vector<double> DoubleVectorNode::get() const
{
	return value;
}


void DoubleVectorNode::set( const Vect2 &v )
{
	value.clear();
	
	value.resize( 2 );
	for( int i = 0; i < 2; i++ )
	{
		value[i] = v[i];
	}
}


void DoubleVectorNode::set( const Vect3 &v )
{
	value.clear();
	
	value.resize( 3 );
	for( int i = 0; i < 3; i++ )
	{
		value[i] = v[i];
	}
}


void DoubleVectorNode::set( const Vect4 &v )
{
	value.clear();
	
	value.resize( 4 );
	for( int i = 0; i < 4; i++ )
	{
		value[i] = v[i];
	}
}


