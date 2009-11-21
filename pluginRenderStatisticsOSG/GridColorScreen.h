/** <pre>
 *  Plugin for the MPV to add statistics displays, such as frame rate
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
 *  2008-02-24 Andrew Sampson
 *      Initial release
 *
 * </pre>
 */

/*
Note - this class was developed on my personal time, and I'd like to license it 
under the LGPL.  Other portions of this plugin were released under the GPL, 
and I haven't yet been given permission to LGPL those portions.
*/


#ifndef GRID_COLOR_TEST_PATTERN_SCREEN_H
#define GRID_COLOR_TEST_PATTERN_SCREEN_H

#include <vector>

#include <osg/Vec2>
#include <osg/Vec4>
#include <osg/Geode>

#include "OverlayScreen.h"

class GridColorScreen : public mpvosg::OverlayScreen
{
public:

	GridColorScreen();
	virtual ~GridColorScreen();
	
	virtual void resetView( const mpv::View *view );
	virtual void act( double deltaT ) {}
	
	virtual osg::Node *getNode();
	
	//=========================================================
	//! Sets the packing dimensions for the grid.  If this method is called, 
	//! then the class will try to subdivide the screen into numCells columns, 
	//! and add new rows as needed (depending on the number of colors added).
	//! Calling this will disable automatic grid layout.
	//! 
	void setNumGridCellsWide( unsigned int numCells );
	
	//=========================================================
	//! Sets the packing dimensions for the grid.  If this method is called, 
	//! then the class will try to subdivide the screen such that the cells 
	//! formed are roughly square.
	//! Calling this will cause numHorizontalGridCells to be ignored.
	//! 
	void applyAutomaticGridLayout();
	
	void addColor( const osg::Vec4 &color );
	
	
protected:

	//=========================================================
	//! 
	//! 
	osg::ref_ptr<osg::Geode> geode;
	
	std::vector< osg::Vec4 > colors;

	void createGeode();

	void addQuad( const osg::Vec2 &minCorner, const osg::Vec2 &maxCorner, 
		const osg::Vec4 &color );
	
	//=========================================================
	//! The class will try to subdivide the screen into numHorizontalGridCells 
	//! columns, and add new rows as needed (depending on the number of 
	//! colors added).
	//! 
	unsigned int numHorizontalGridCells;
	
	//=========================================================
	//! If this is true, then the class will try to subdivide the screen such 
	//! that the cells formed are roughly square.  Also, if this is true, 
	//! then numHorizontalGridCells will be ignored.
	//! 
	bool autoGridLayout;
};


#endif

