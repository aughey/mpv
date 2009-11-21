/** <pre>
 *  The MPV Symbology Plugin Collection
 *  Copyright (c) 2007 General Dynamics Land Systems
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
 *  
 *  2007-03-30 Philip Lowman, GDLS
 *      Initial release
 *  
 *  2008-02-13 Andrew Sampson, Boeing
 *      Merged some code from GDLS symbology rendering into new plugin; part 
 *      of this class is from OSGPolyLineSymbol, etc.  
 *
 * </pre>
 */

#include "SymbolLineImpOSG.h"

using namespace mpv;
using namespace mpvosg;

SymbolLineImpOSG::SymbolLineImpOSG( SymbolLine *symbol ) : 
	SymbolImpOSG( symbol )
{
	// retrieve the symbol state and apply it
	verticesChanged( symbol );
	lineStyleChanged( symbol );
	
	// set up slots for change notification
	symbol->verticesChanged.connect( BIND_SLOT1( SymbolLineImpOSG::verticesChanged, this ) );
	symbol->lineStyleChanged.connect( BIND_SLOT1( SymbolLineImpOSG::lineStyleChanged, this ) );
}

SymbolLineImpOSG::~SymbolLineImpOSG()
{

}


void SymbolLineImpOSG::update( double timeElapsed )
{
	
}


void SymbolLineImpOSG::lineStyleChanged( SymbolLine *symbol )
{

//FIXME - not finished
	symbol->getStipplePattern();
	symbol->getStipplePatternLength();
	symbol->getLineWidth();
	
}


void SymbolLineImpOSG::verticesChanged( SymbolLine *symbol )
{
	// clear existing geometry
	if( geometry->getNumPrimitiveSets() > 0 )
		geometry->removePrimitiveSet( 0, geometry->getNumPrimitiveSets() );
	geometry->setVertexArray( NULL );
	
	// add new geometry
	osg::Vec3Array *vertices = new osg::Vec3Array();
	for( unsigned int i = 0; i < symbol->getNumVertices(); i++ )
	{
		const Vect2 &vert2 = symbol->getVertex( i );
		vertices->push_back( osg::Vec3d( vert2.getx(), vert2.gety(), 0. ) );
	}
	geometry->setVertexArray( vertices );
	
	osg::PrimitiveSet::Mode drawingStyle = osg::PrimitiveSet::POINTS;
	switch( symbol->getPrimitiveType() )
	{
	case SymbolLine::Points:
		drawingStyle = osg::PrimitiveSet::POINTS;
		break;
	case SymbolLine::Lines:
		drawingStyle = osg::PrimitiveSet::LINES;
		break;
	case SymbolLine::LineStrip:
		drawingStyle = osg::PrimitiveSet::LINE_STRIP;
		break;
	case SymbolLine::LineLoop:
		drawingStyle = osg::PrimitiveSet::LINE_LOOP;
		break;
	case SymbolLine::Triangles:
		drawingStyle = osg::PrimitiveSet::TRIANGLES;
		break;
	case SymbolLine::TriangleStrip:
		drawingStyle = osg::PrimitiveSet::TRIANGLE_STRIP;
		break;
	case SymbolLine::TriangleFan:
		drawingStyle = osg::PrimitiveSet::TRIANGLE_FAN;
		break;
	default:
		break;
	}

	geometry->addPrimitiveSet( 
		new osg::DrawArrays( drawingStyle, 0, vertices->size() ) );
}


