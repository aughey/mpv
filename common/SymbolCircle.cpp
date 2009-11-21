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

#include "SymbolCircle.h"

using namespace mpv;


SymbolCircle::SymbolCircle() : Symbol(),
	drawingStyle( Line ),
	stipplePattern( 0xffff ),
	lineWidth( 1.0 ),
	stipplePatternLength( 10.0 )
{
}


SymbolCircle::~SymbolCircle()
{
}



Symbol::SymbolType SymbolCircle::getType() const
{
	return Symbol::Circle;
}


	
void SymbolCircle::setDrawingStyle( SymbolCircle::SymbolCircleDrawStyle newStyle )
{
	if( drawingStyle != newStyle )
	{
		drawingStyle = newStyle;
		drawingStyleChanged( this );
	}
}



SymbolCircle::SymbolCircleDrawStyle SymbolCircle::getDrawingStyle() const
{
	return drawingStyle;
}



void SymbolCircle::setStipplePattern( Cigi_uint16 newStipplePattern )
{
	if( stipplePattern != newStipplePattern )
	{
		stipplePattern = newStipplePattern;
		lineStyleChanged( this );
	}
}



Cigi_uint16 SymbolCircle::getStipplePattern() const
{
	return stipplePattern;
}



void SymbolCircle::setLineWidth( float newLineWidth )
{
	if( lineWidth != newLineWidth )
	{
		lineWidth = newLineWidth;
		lineStyleChanged( this );
	}
}



float SymbolCircle::getLineWidth() const
{
	return lineWidth;
}



void SymbolCircle::setStipplePatternLength( float newStipplePatternLength )
{
	if( stipplePatternLength != newStipplePatternLength )
	{
		stipplePatternLength = newStipplePatternLength;
		lineStyleChanged( this );
	}
}



float SymbolCircle::getStipplePatternLength() const
{
	return stipplePatternLength;
}



void SymbolCircle::addCircle( const Circle &newCircle )
{
	circles.push_back( newCircle );
	circlesChanged( this );
}



unsigned int SymbolCircle::getNumCircles() const
{
	return circles.size();
}



const SymbolCircle::Circle &SymbolCircle::getCircle( unsigned int circleNum )
{
	if( circleNum >= circles.size() )
		throw std::string( "Invalid circle index" );
	return circles[circleNum];
}



void SymbolCircle::removeAllCircles()
{
	circles.clear();
	circlesChanged( this );
}
