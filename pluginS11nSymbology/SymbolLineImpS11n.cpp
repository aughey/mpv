/** <pre>
 *  MPV Serialization Framework
 *  Copyright (c) 2009 Andrew Sampson
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
 *  2009-09-06 Andrew Sampson
 *      Initial release
 *
 * </pre>
 */

#include "SymbolLineImpS11n.h"

using namespace mpv;
using namespace mpvs11n;

SymbolLineImpS11n::SymbolLineImpS11n( SymbolLine *symbol ) : 
	SymbolImpS11n( symbol )
{
	type->set( "Line" ); // "type" exists in parent class
	
	primitiveType = new mpvs11n::StringNode( "primitive_type" );
	lineWidth = new mpvs11n::FloatNode( "line_width" );
	stipplePattern = new mpvs11n::IntNode( "stipple_pattern" );
	stipplePatternLength = new mpvs11n::FloatNode( "stipple_pattern_length" );
	vertices = new mpvs11n::GroupNode( "vertices" );
	
	topGroup->addChild( primitiveType.get() );
	topGroup->addChild( lineWidth.get() );
	topGroup->addChild( stipplePattern.get() );
	topGroup->addChild( stipplePatternLength.get() );
	topGroup->addChild( vertices.get() );
	
	// retrieve the symbol state and apply it
	verticesChanged( symbol );
	lineStyleChanged( symbol );
	
	// set up slots for change notification
	symbol->verticesChanged.connect( BIND_SLOT1( SymbolLineImpS11n::verticesChanged, this ) );
	symbol->lineStyleChanged.connect( BIND_SLOT1( SymbolLineImpS11n::lineStyleChanged, this ) );
}

SymbolLineImpS11n::~SymbolLineImpS11n()
{

}


void SymbolLineImpS11n::update( double timeElapsed )
{
	
}


void SymbolLineImpS11n::lineStyleChanged( SymbolLine *symbol )
{
	lineWidth->set( symbol->getLineWidth() );
	// fixme - stipple pattern might be better represented as a 
	// string ("0011...")
	stipplePattern->set( symbol->getStipplePattern() );
	stipplePatternLength->set( symbol->getStipplePatternLength() );
}


void SymbolLineImpS11n::verticesChanged( SymbolLine *symbol )
{
	switch( symbol->getPrimitiveType() )
	{
	case SymbolLine::Points:
		primitiveType->set( "Points" );
		break;
	case SymbolLine::Lines:
		primitiveType->set( "Lines" );
		break;
	case SymbolLine::LineStrip:
		primitiveType->set( "LineStrip" );
		break;
	case SymbolLine::LineLoop:
		primitiveType->set( "LineLoop" );
		break;
	case SymbolLine::Triangles:
		primitiveType->set( "Triangles" );
		break;
	case SymbolLine::TriangleStrip:
		primitiveType->set( "TriangleStrip" );
		break;
	case SymbolLine::TriangleFan:
		primitiveType->set( "TriangleFan" );
		break;
	default:
		primitiveType->set( "Unknown" );
		break;
	}

	// remove vertex nodes
	vertices->removeAllChildren();
	
	// add new vertex nodes
	for( unsigned int i = 0; i < symbol->getNumVertices(); i++ )
	{
		GroupNode *vertexNode = new GroupNode( "vertex" );
		vertices->addChild( vertexNode );
		
		DoubleVectorNode *vertexPosition = new DoubleVectorNode( "position" );
		vertexPosition->set( symbol->getVertex( i ) );
		vertexNode->addChild( vertexPosition );
	}
}


