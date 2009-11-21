/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2008 The Boeing Company
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
 *  2008-01-10 Andrew Sampson
 *      Initial Release.
 * </pre>
 */

#include <string>

#include "SymbolLine.h"

using namespace mpv;


SymbolLine::SymbolLine() : Symbol(),
	primitiveType( Points ),
	lineWidth( 1.0 ),
	stipplePattern( 0xffff ),
	stipplePatternLength( 10.0 )
{
}


SymbolLine::~SymbolLine()
{
}



Symbol::SymbolType SymbolLine::getType() const
{
	return Symbol::Line;
}


	
void SymbolLine::setPrimitiveType( SymbolLine::SymbolLinePrimitiveType newPrimitiveType )
{
	if( primitiveType != newPrimitiveType )
	{
		primitiveType = newPrimitiveType;
		verticesChanged( this );
	}
}



SymbolLine::SymbolLinePrimitiveType SymbolLine::getPrimitiveType() const
{
	return primitiveType;
}



void SymbolLine::setLineWidth( float newLineWidth )
{
	if( lineWidth != newLineWidth )
	{
		lineWidth = newLineWidth;
		lineStyleChanged( this );
	}
}



float SymbolLine::getLineWidth() const
{
	return lineWidth;
}



void SymbolLine::setStipplePattern( Cigi_uint16 newStipplePattern )
{
	if( stipplePattern != newStipplePattern )
	{
		stipplePattern = newStipplePattern;
		lineStyleChanged( this );
	}
}



Cigi_uint16 SymbolLine::getStipplePattern() const
{
	return stipplePattern;
}



void SymbolLine::setStipplePatternLength( float newStipplePatternLength )
{
	if( stipplePatternLength != newStipplePatternLength )
	{
		stipplePatternLength = newStipplePatternLength;
		lineStyleChanged( this );
	}
}



float SymbolLine::getStipplePatternLength() const
{
	return stipplePatternLength;
}



void SymbolLine::addVertex( const Vect2 &newVertex )
{
	vertices.push_back( newVertex );
	verticesChanged( this );
}



void SymbolLine::addVertex( float u, float v )
{
	addVertex( Vect2( u, v ) );
}



unsigned int SymbolLine::getNumVertices() const
{
	return vertices.size();
}



const Vect2 &SymbolLine::getVertex( unsigned int vertexNum )
{
	if( vertexNum >= vertices.size() )
		throw std::string( "Invalid vertex index" );
	return vertices[vertexNum];
}



void SymbolLine::removeAllVertices()
{
	vertices.clear();
	verticesChanged( this );
}

