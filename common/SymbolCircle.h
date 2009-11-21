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

#ifndef SymbolCircle_h
#define SymbolCircle_h

#include "MPVCommonTypes.h"

#include <vector>

#include <CigiBaseSymbolCircleDef.h>

#include "Vect2.h"
#include "Symbol.h"


namespace mpv
{

//=========================================================
//! Circle symbol class.  Inherits from Symbol.
//! 
class MPVCMN_SPEC SymbolCircle : public Symbol
{
public:


	class Circle
	{

	public:
		Circle() : 
			centerPosition( 0, 0 ),
			radius( 1.0 ),
			innerRadius( 0.0 ),
			startAngle( 0.0 ),
			endAngle( 0.0 )
		{
		}
		
		bool isCompleteCircle() const
		{
			if( startAngle == endAngle )
				return true;
			return false;
		}
		
		bool hasHole() const
		{
			if( innerRadius > 0.0 )
				return true;
			return false;
		}
		
		Vect2 centerPosition;
		float radius;
		float innerRadius;
		float startAngle;
		float endAngle;
	};
	
	enum SymbolCircleDrawStyle
	{
		Line = CigiBaseSymbolCircleDef::Line,
		Fill = CigiBaseSymbolCircleDef::Fill
	};


public:

	boost::signal<void (SymbolCircle*)> drawingStyleChanged;
	boost::signal<void (SymbolCircle*)> lineStyleChanged;
	boost::signal<void (SymbolCircle*)> circlesChanged;

	SymbolCircle();
	
	virtual SymbolType getType() const;
	
	void setDrawingStyle( SymbolCircleDrawStyle newStyle );

	SymbolCircleDrawStyle getDrawingStyle() const;

	void setStipplePattern( Cigi_uint16 newStipplePattern );

	Cigi_uint16 getStipplePattern() const;

	void setLineWidth( float newLineWidth );

	float getLineWidth() const;

	void setStipplePatternLength( float newStipplePatternLength );

	float getStipplePatternLength() const;

	void addCircle( const Circle &newCircle );

	unsigned int getNumCircles() const;

	const Circle &getCircle( unsigned int circleNum );

	void removeAllCircles();

protected:

	virtual ~SymbolCircle();
	

protected:
	SymbolCircleDrawStyle drawingStyle;
	Cigi_uint16 stipplePattern;
	float lineWidth;
	float stipplePatternLength;
	std::vector<Circle> circles;
};

}
#endif
