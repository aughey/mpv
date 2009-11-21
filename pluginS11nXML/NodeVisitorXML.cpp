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


#include "NodeVisitorXML.h"


using namespace mpvs11n;

// ================================================
// NodeVisitorXML
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
NodeVisitorXML::NodeVisitorXML( std::ostream &os ) : NodeVisitor( os ), 
	indentLevel( 0 )
{
	
}


// ================================================
// ~NodeVisitorXML
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
NodeVisitorXML::~NodeVisitorXML() 
{
	
}


void NodeVisitorXML::visit( GroupNode &n ) const
{
	startElement( n );
	output << std::endl;
	increaseIndent();
	
	GroupNode::NodeIteratorPair iterPair = n.getChildren();
	GroupNode::NodeList::iterator iter;
	for( iter = iterPair.first; iter != iterPair.second; iter++ )
	{
		(*iter)->accept( *this );
	}
	
	decreaseIndent();
	output << indentString;
	endElement( n );
}


void NodeVisitorXML::visit( StringNode &n ) const
{
	// FIXME FIXME FIXME turn certain characters into entities 
	// example: & -> &amp;
	startElement( n );
	output << n.get();
	endElement( n );
}


void NodeVisitorXML::visit( IntNode &n ) const
{
	startElement( n );
	output << n.get();
	endElement( n );
}


void NodeVisitorXML::visit( FloatNode &n ) const
{
	startElement( n );
	output << n.get();
	endElement( n );
}


void NodeVisitorXML::visit( DoubleNode &n ) const
{
	startElement( n );
	output << n.get();
	endElement( n );
}


void NodeVisitorXML::visit( DoubleVectorNode &n ) const
{
	startElement( n );
	std::vector<double> v = n.get();
	if( !v.empty() )
	{
		output << v[0];
		for( unsigned int i = 1; i < v.size(); i++ )
			output << ", " << v[i];
	}
	endElement( n );
}


void NodeVisitorXML::increaseIndent() const
{
	indentLevel++;
	indentString.push_back( '\t' );
}


void NodeVisitorXML::decreaseIndent() const
{
	indentLevel--;
	if( indentLevel < 0 )
		indentLevel = 0;
	
//	indentString.clear();
//	for( int i = 0; i < indentLevel; i++ )
//		indentString.push_back( '\t' );
	indentString.resize( indentLevel );
}


void NodeVisitorXML::startElement( mpvs11n::Node &n ) const
{
	// FIXME FIXME FIXME for getname - turn certain characters into entities 
	// example: & -> &amp;
	output << indentString << "<" << n.getName() << ">";
}


void NodeVisitorXML::endElement( mpvs11n::Node &n ) const
{
	// FIXME FIXME FIXME for getname - turn certain characters into entities 
	// example: & -> &amp;
	output << "</" << n.getName() << ">\n";
}


