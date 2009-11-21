/** <pre>
 *  The MPV Symbology Plugin Collection
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
 *  2008-02-13 Andrew Sampson, Boeing
 *      Initial release
 *
 * </pre>
 */

#include <iostream>
#include <math.h>

#include "SymbolCircleImpOSG.h"

using namespace mpv;
using namespace mpvosg;

SymbolCircleImpOSG::SymbolCircleImpOSG( SymbolCircle *symbol ) : 
	SymbolImpOSG( symbol )
{
	circleTesselationThreshold = osg::DegreesToRadians( 5.0 );
	
	// retrieve the symbol state and apply it
	lineStyleChanged( symbol );
	circlesChanged( symbol );
	
	// set up slots for change notification
	symbol->drawingStyleChanged.connect( BIND_SLOT1( SymbolCircleImpOSG::circlesChanged, this ) );
	symbol->lineStyleChanged.connect( BIND_SLOT1( SymbolCircleImpOSG::lineStyleChanged, this ) );
	symbol->circlesChanged.connect( BIND_SLOT1( SymbolCircleImpOSG::circlesChanged, this ) );
}

SymbolCircleImpOSG::~SymbolCircleImpOSG()
{
	
}


void SymbolCircleImpOSG::update( double timeElapsed )
{
	
}


void SymbolCircleImpOSG::lineStyleChanged( SymbolCircle *symbol )
{

//FIXME - not finished
	symbol->getStipplePattern();
	symbol->getStipplePatternLength();
	symbol->getLineWidth();
	
}


void SymbolCircleImpOSG::circlesChanged( SymbolCircle *symbol )
{
	// clear existing geometry
	if( geometry->getNumPrimitiveSets() > 0 )
		geometry->removePrimitiveSet( 0, geometry->getNumPrimitiveSets() );
	geometry->setVertexArray( NULL );
	
	int firstVertexOfCurrentPrimitive = 0;
	osg::Vec3Array *vertices = new osg::Vec3Array();
	geometry->setVertexArray( vertices );
	
	for( unsigned int i = 0; i < symbol->getNumCircles(); i++ )
	{
		const SymbolCircle::Circle &circle = symbol->getCircle( i );
		
		firstVertexOfCurrentPrimitive = vertices->size();
		
		osg::PrimitiveSet::Mode drawingStyle = osg::PrimitiveSet::LINE_LOOP;

		if( symbol->getDrawingStyle() == SymbolCircle::Line )
		{
			if( circle.isCompleteCircle() )
				drawingStyle = osg::PrimitiveSet::LINE_LOOP;
			else
				drawingStyle = osg::PrimitiveSet::LINE_STRIP;
			// vertices for all variants of line circles are created in 
			// the same way
			createCircleVerts( circle, vertices );
		}
		else if( symbol->getDrawingStyle() == SymbolCircle::Fill )
		{
			if( circle.hasHole() )
			{
				// filled circles with holes (donuts, rainbows) need to be 
				// drawn using a triangle strip
				drawingStyle = osg::PrimitiveSet::TRIANGLE_STRIP;
				createDonutVerts( circle, vertices );
			}
			else
			{
				if( circle.isCompleteCircle() )
				{
					drawingStyle = osg::PrimitiveSet::TRIANGLE_FAN;
					createCircleVerts( circle, vertices );
				}
				else
				{
					drawingStyle = osg::PrimitiveSet::TRIANGLE_FAN;
					// incomplete, non-hole-y (ie pie-wedge-shaped) circles can be 
					// created using the same vertices as complete circles, if the 
					// vertex list is prefixed with the circle center
					vertices->push_back( osg::Vec3( 
						circle.centerPosition.getx(), 
						circle.centerPosition.gety(), 
						0.0 ) );
					createCircleVerts( circle, vertices );
				}
			}
		}

		geometry->addPrimitiveSet( new osg::DrawArrays( 
			drawingStyle, firstVertexOfCurrentPrimitive, vertices->size() ) );
	}
}


void SymbolCircleImpOSG::createCircleVerts( 
	const SymbolCircle::Circle &circle, osg::Vec3Array *vertices )
{
	osg::Vec3 circleCenter( 
			circle.centerPosition.getx(), 
			circle.centerPosition.gety(), 
			0.0 );
	
	float currentAngle = osg::DegreesToRadians( circle.startAngle );
	float endAngle = osg::DegreesToRadians( circle.endAngle );
	if( circle.startAngle >= circle.endAngle )
	{
		// the arc sweeps through the origin
		// also catches the situation where circle.isCompleteCircle()...
		endAngle += osg::DegreesToRadians( 360.0 );
	}
	
	int numSegments = (int)((endAngle - currentAngle) / circleTesselationThreshold);
	numSegments++; // round up, unconditionally
	float interval = (endAngle - currentAngle) / (float)numSegments;
	/*
	Here's why incomplete circles need an extra vertex:
	- filled incomplete circles (pacman, pie wedge) are drawn with triangle 
	  fans, and those fans need a "closing" vertex to complete the final 
	  triangle
	- unfilled incomplete circles (line-art arcs) are drawn with linestrips, 
	  and those strips need a "closing" vertex to complete the final line
	
	There are two circle types that do not need this extra vertex: 
	- filled complete circles use triangle fans, but the first vertex is 
	  placed on an edge instead of the center, and hence don't need the 
	  extra vertex
	- unfilled complete circles use lineloops instead of linestrips, and 
	  hence don't need the extra vertex
	*/
	if( !circle.isCompleteCircle() )
		numSegments++;
	for( int i = 0; i < numSegments; i++ )
	{
		osg::Vec3 point;
		
		// simple trig to calculate point on a circle
		point[0] = circle.radius * cosf( currentAngle );
		point[1] = circle.radius * sinf( currentAngle );
		
		vertices->push_back( circleCenter + point );
		
		currentAngle += interval;
	}
}


void SymbolCircleImpOSG::createDonutVerts( 
	const SymbolCircle::Circle &circle, osg::Vec3Array *vertices )
{
	osg::Vec3 circleCenter( 
			circle.centerPosition.getx(), 
			circle.centerPosition.gety(), 
			0.0 );
	
	float currentAngle = osg::DegreesToRadians( circle.startAngle );
	float endAngle = osg::DegreesToRadians( circle.endAngle );
	if( circle.startAngle >= circle.endAngle )
	{
		// the arc sweeps through the origin
		// also catches the situation where circle.isCompleteCircle()...
		endAngle += osg::DegreesToRadians( 360.0 );
	}
	
	int numSegments = (int)((endAngle - currentAngle) / circleTesselationThreshold);
	numSegments++; // round up, unconditionally
	float interval = (endAngle - currentAngle) / (float)numSegments;
	// note that the loop iterates numSegments+1; the triangle strip needs a 
	// "closing" endcap
	for( int i = 0; i <= numSegments; i++ )
	{
		osg::Vec3 point;
		
		float cosCurrentAngle = cosf( currentAngle );
		float sinCurrentAngle = sinf( currentAngle );
		
		// simple trig to calculate point on a circle
		point[0] = circle.innerRadius * cosCurrentAngle;
		point[1] = circle.innerRadius * sinCurrentAngle;
		
		vertices->push_back( circleCenter + point );
		
		point[0] = circle.radius * cosCurrentAngle;
		point[1] = circle.radius * sinCurrentAngle;
		
		vertices->push_back( circleCenter + point );
		
		currentAngle += interval;
	}
}
