/** <pre>
 *  Plugin for the MPV to add positional audio to the scene
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
 *  Initial Release: 2007-04-07 Andrew Sampson
 *
 *  2007-07-21 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 * </pre>
 */


#ifndef PLUGIN_RENDER_SOUND_ROOT_OSGAL_H
#define PLUGIN_RENDER_SOUND_ROOT_OSGAL_H

#include <map>
#include <osgAL/SoundRoot>
#include <osg/MatrixTransform>

#include "Plugin.h"

//=========================================================
//! This plugin is responsible for setting up the osgAL::SoundRoot node, 
//! attaching it to the OSG scene root node, and updating its location to 
//! match that of the (primary) view.
//! 
class PluginRenderSoundRootOSGAL : public Plugin 
{
public:

	//=========================================================
	//! General Constructor
	//! 
	PluginRenderSoundRootOSGAL();

	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginRenderSoundRootOSGAL() throw();
	
	//=========================================================
	//! The per-frame processing that this plugin performs.
	//! This plugin's act() will update the soundRoot object's location
	//! to match that of the (primary) view.
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );

private:

	//=========================================================
	//! The camera matrices, one for each view.  Each one of these is a 
	//! complete modelview matrix.  Retrieved from the blackboard.
	//! 
	std::map< int, osg::Matrix > *cameraMatrixMap;

	//=========================================================
	//! The SoundRoot node; this node's position in the scene is used 
	//! for the OpenAL listener position
	//! 
//	osg::ref_ptr<osgAL::SoundRoot> soundRoot;

};



#endif

