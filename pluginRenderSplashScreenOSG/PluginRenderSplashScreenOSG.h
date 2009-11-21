/** <pre>
 *  Plugin for the MPV to create a splash screen
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
 *  2007-08-16 Andrew Sampson
 *      Initial implementation
 *
 * </pre>
 */


#ifndef PLUGIN_RENDER_SPLASH_SCREEN_OSG_H
#define PLUGIN_RENDER_SPLASH_SCREEN_OSG_H

#include <osg/Group>
#include <osg/Geode>
#include <osg/Vec3>
#include <osg/Vec4>

#include "Plugin.h"

//=========================================================
//! This plugin is responsible for creating an OSG scene graph (separate from 
//! the one created by pluginRenderOSG) that contains a splash screen.  The 
//! splash screen will be displayed when the MPV is in the Standby state.
//! Future plans include the ability to have several splash screens, one of 
//! which is specified through a config file or chosen at random.  
//! Additionally, it would be nice to have the ability to load an external 
//! model file and use it as a splash screen.
//! 
class PluginRenderSplashScreenOSG : public Plugin 
{
public:

	//=========================================================
	//! General Constructor
	//! 
	PluginRenderSplashScreenOSG();

	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginRenderSplashScreenOSG() throw();
	
	//=========================================================
	//! The per-frame processing that this plugin performs.
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );

private:

	//=========================================================
	//! The root node of the splash screen.  Posted to the blackboard.
	//! 
	osg::ref_ptr<osg::Group> splashScreenRoot;
	
	//=========================================================
	//! Populates splashScreenRoot 
	//! 
	void createSplashScreen();
	
	//=========================================================
	//! Creates a line-art 2D square, 1.0 units on a side, centered at 0,0
	//! \param color - the color to apply to the square
	//! \return a newly-allocated geode containing some square geometry
	//! 
	osg::Geode *createSquare( const osg::Vec4 &color );
	
	//=========================================================
	//! Creates a line-art 2D equilateral triangle, 1.1547 units on a side 
	//! (making the radius 2/3), centered at 0,0
	//! \param color - the color to apply to the square
	//! \return a newly-allocated geode containing some square geometry
	//! 
	osg::Geode *createTriangle( const osg::Vec4 &color );
};



#endif

