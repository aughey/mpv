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

#include <iostream>
#include <math.h>

#include "SymbolCircleImpS11n.h"

using namespace mpv;
using namespace mpvs11n;

SymbolCircleImpS11n::SymbolCircleImpS11n( SymbolCircle *symbol ) : 
	SymbolImpS11n( symbol )
{
	type->set( "Circle" ); // "type" exists in parent class
	
	drawingStyle = new mpvs11n::StringNode( "drawing_style" );
	stipplePattern = new mpvs11n::IntNode( "stipple_pattern" );
	lineWidth = new mpvs11n::FloatNode( "line_width" );
	stipplePatternLength = new mpvs11n::FloatNode( "stipple_pattern_length" );
	circles = new mpvs11n::GroupNode( "circles" );
	
	topGroup->addChild( drawingStyle.get() );
	topGroup->addChild( stipplePattern.get() );
	topGroup->addChild( lineWidth.get() );
	topGroup->addChild( stipplePatternLength.get() );
	topGroup->addChild( circles.get() );
	
	// retrieve the symbol state and apply it
	lineStyleChanged( symbol );
	circlesChanged( symbol );
	
	// set up slots for change notification
	symbol->drawingStyleChanged.connect( BIND_SLOT1( SymbolCircleImpS11n::circlesChanged, this ) );
	symbol->lineStyleChanged.connect( BIND_SLOT1( SymbolCircleImpS11n::lineStyleChanged, this ) );
	symbol->circlesChanged.connect( BIND_SLOT1( SymbolCircleImpS11n::circlesChanged, this ) );
}

SymbolCircleImpS11n::~SymbolCircleImpS11n()
{
	
}


void SymbolCircleImpS11n::update( double timeElapsed )
{
	
}


void SymbolCircleImpS11n::lineStyleChanged( SymbolCircle *symbol )
{
	lineWidth->set( symbol->getLineWidth() );
	// fixme - stipple pattern might be better represented as a 
	// string ("0011...")
	stipplePattern->set( symbol->getStipplePattern() );
	stipplePatternLength->set( symbol->getStipplePatternLength() );
}


void SymbolCircleImpS11n::circlesChanged( SymbolCircle *symbol )
{
	switch( symbol->getDrawingStyle() )
	{
	case SymbolCircle::Line:
		drawingStyle->set( "Line" );
		break;
	case SymbolCircle::Fill:
		drawingStyle->set( "Fill" );
		break;
	default:
		drawingStyle->set( "Unknown" );
		break;
	}

	// remove circle nodes
	circles->removeAllChildren();
	
	// add new circle nodes
	for( unsigned int i = 0; i < symbol->getNumCircles(); i++ )
	{
		const SymbolCircle::Circle &circle = symbol->getCircle( i );
		
		GroupNode *circleNode = new GroupNode( "circle" );
		circles->addChild( circleNode );
		
		DoubleVectorNode *centerPosition = new DoubleVectorNode( "center_position" );
		centerPosition->set( circle.centerPosition );
		circleNode->addChild( centerPosition );
		
		FloatNode *radius = new FloatNode( "radius" );
		radius->set( circle.radius );
		circleNode->addChild( radius );
		
		FloatNode *innerRadius = new FloatNode( "inner_radius" );
		innerRadius->set( circle.innerRadius );
		circleNode->addChild( innerRadius );
		
		FloatNode *startAngle = new FloatNode( "start_angle" );
		startAngle->set( circle.startAngle );
		circleNode->addChild( startAngle );
		
		FloatNode *endAngle = new FloatNode( "end_angle" );
		endAngle->set( circle.endAngle );
		circleNode->addChild( endAngle );
	}
}


