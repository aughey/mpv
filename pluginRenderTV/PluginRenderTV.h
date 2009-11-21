/** <pre>
 *  Plugin to incorporate Video4Linux input (tv capture cards and the like)
 *  into the MPV scene.
 *  Copyright (c) 2006 David Fries
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
 *  Initial Release: 2006-XX-XX David Fries
 *
 *  2007-07-21 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 * </pre>
 */

#ifndef PLUGIN_RENDER_TV_H
#define PLUGIN_RENDER_TV_H

#include <list>
#include <osg/Group>

#include "Plugin.h"
#include "Entity.h"
#include "DefFileGroup.h"

//=========================================================
//! This plugin is used to display the input of a V4L compatible device to
//! a textured quad drawn in the background.
//! 
class PluginRenderTV : public Plugin 
{
public:

	//=========================================================
	//! General Constructor
	//! 
	PluginRenderTV();

	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginRenderTV() throw();
	
	//=========================================================
	//! The per-frame processing that this plugin performs
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );
	
private:

	//=========================================================
	//! Creates a TV node with an ortographic project etc.
	//! 
	virtual osg::Node* CreateTVNode();

	//=========================================================
	//! Configuration data.  Retrieved from the blackboard.
	//! 
	DefFileGroup **DefFileData;

	//=========================================================
	//! Root node.  Retrieved from the blackboard.
	//! 
	osg::Group *RootNode;

	//=========================================================
	//! Width and Height of the captured data.
	//! TextureHeight is the height given to the texture.
	//! Width and TextureHeight must be a power of two.  Height does
	//! not have to be.  If you specify 480 for Height specify 512
	//! for TextureHeight to be able to have more vertical resolution
	//! than say 256 would allow.  The texture coordinates are modified
	//! such that Height lines are streatched to the full size, the
	//! additional lines are not displayed, but just required to make
	//! it a power of two.
	int Width, Height, TextureHeight;

	//=========================================================
	//! Options to display only a portion of the texture.
	float StartLine, StopLine, StartColumn, StopColumn;

	//=========================================================
	//! The device file to open.
	std::string DeviceFile;
	
	//=========================================================
	//! A flag indicating whether act has been called with mode Operate.
	//! Ensures that the tv node is added to the scene *after* the config 
	//! parameters have been retrieved.
	bool firstTimeThrough;
};

#endif
