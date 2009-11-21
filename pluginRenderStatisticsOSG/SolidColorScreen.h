/** <pre>
 *  Plugin for the MPV to add statistics displays, such as frame rate
 *  Copyright (c) 2007 Andrew Sampson
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
 *  2007-10-07 Andrew Sampson
 *      Initial release
 *
 * </pre>
 */

/*
Note - this class was developed on my personal time, and I'd like to license it 
under the LGPL.  Other portions of this plugin were released under the GPL, 
and I haven't yet been given permission to LGPL those portions.
*/


#ifndef SOLID_COLOR_TEST_PATTERN_SCREEN_H
#define SOLID_COLOR_TEST_PATTERN_SCREEN_H

#include <osg/Geode>

#include "OverlayScreen.h"

class SolidColorScreen : public mpvosg::OverlayScreen
{
public:

	SolidColorScreen( const osg::Vec4 &screenColor );
	virtual ~SolidColorScreen();
	
	virtual void resetView( const mpv::View *viewParams );
	virtual void act( double deltaT ) {}
	
	virtual osg::Node *getNode();
	
	
protected:

	//=========================================================
	//! 
	//! 
	osg::ref_ptr<osg::Geode> geode;
	
	osg::Vec4 color;

	void createGeode();
};


#endif

