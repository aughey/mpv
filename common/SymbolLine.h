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

#ifndef SymbolLine_h
#define SymbolLine_h

#if defined(_MSC_VER)
    #pragma warning(disable: 4251)
#endif

#include <vector>

#include <CigiBaseSymbolLineDef.h>

#include "Vect2.h"
#include "Symbol.h"
#include "MPVCommonTypes.h"


namespace mpv
{

//=========================================================
//! Line symbol class.  Inherits from Symbol.  Line symbols are very flexible; 
//! the line definition packet can be used to create points, lines, lineloops, 
//! triangle fans, etc.
//! 
class MPVCMN_SPEC SymbolLine : public Symbol
{

public:

	enum SymbolLinePrimitiveType
	{
		Points = CigiBaseSymbolLineDef::Points,
		Lines = CigiBaseSymbolLineDef::Lines,
		LineStrip = CigiBaseSymbolLineDef::LineStrip,
		LineLoop = CigiBaseSymbolLineDef::LineLoop,
		Triangles = CigiBaseSymbolLineDef::Triangles,
		TriangleStrip = CigiBaseSymbolLineDef::TriangleStrip,
		TriangleFan = CigiBaseSymbolLineDef::TriangleFan
	};

public:

	boost::signal<void (SymbolLine*)> lineStyleChanged;
	boost::signal<void (SymbolLine*)> verticesChanged;

	SymbolLine();
	
	virtual SymbolType getType() const;
	
	void setPrimitiveType( SymbolLinePrimitiveType newPrimitiveType );

	SymbolLinePrimitiveType getPrimitiveType() const;

	void setLineWidth( float newLineWidth );

	float getLineWidth() const;

	void setStipplePattern( Cigi_uint16 newStipplePattern );

	Cigi_uint16 getStipplePattern() const;

	void setStipplePatternLength( float newStipplePatternLength );

	float getStipplePatternLength() const;

	void addVertex( const Vect2 &newVertex );

	void addVertex( float u, float v );

	unsigned int getNumVertices() const;

	const Vect2 &getVertex( unsigned int vertexNum );

	void removeAllVertices();

protected:

	virtual ~SymbolLine();
	

protected:
	SymbolLinePrimitiveType primitiveType;
	float lineWidth;
	Cigi_uint16 stipplePattern;
	float stipplePatternLength;
	std::vector<Vect2> vertices;
};

}
#endif
