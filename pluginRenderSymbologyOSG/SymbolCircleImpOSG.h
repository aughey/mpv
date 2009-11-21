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


#ifndef SYMBOLCIRCLEIMPOSG_H
#define SYMBOLCIRCLEIMPOSG_H

#include <string>
#include <osg/PositionAttitudeTransform>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>

#include "SymbolImpOSG.h"
#include "SymbolCircle.h"

//=========================================================
//! 
//! 
class SymbolCircleImpOSG : public mpvosg::SymbolImpOSG
{
public:

	SymbolCircleImpOSG( mpv::SymbolCircle *symbol );

	virtual void update( double timeElapsed );

	void lineStyleChanged( mpv::SymbolCircle *symbol );

	void circlesChanged( mpv::SymbolCircle *symbol );

protected:

	virtual ~SymbolCircleImpOSG();
	
	//! Creates vertices for a circle which has trivial geometry.  The vertices 
	//! around the perimeter will be created and added to the "vertices" array.  
	//! The vertex order will be counter-clockwise, starting at the circle's 
	//! startAngle.
	void createCircleVerts( 
		const mpv::SymbolCircle::Circle &circle, osg::Vec3Array *vertices );
	
	//! Creates vertices for a circle which has a hole in the middle.  The 
	//! vertices around the inner and outer edges will be added, in an order 
	//! appropriate for use as a triangle strip, to the "vertices" array.  
	void createDonutVerts( 
		const mpv::SymbolCircle::Circle &circle, osg::Vec3Array *vertices );
	
	//! A vertex will be placed along the circle's circumference with 
	//! (approximately) this interval between points.  This value directly 
	//! influences the number of vertices in a circle.  Units are radians.
	float circleTesselationThreshold;
};

#endif
