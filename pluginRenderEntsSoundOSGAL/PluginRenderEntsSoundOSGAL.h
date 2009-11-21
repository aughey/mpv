/** <pre>
 *  Plugin to add positional audio to entities, for the MPV
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
 *  2007-07-15 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 * </pre>
 */


#ifndef PLUGIN_RENDER_ENTS_SOUND_OSGAL_H
#define PLUGIN_RENDER_ENTS_SOUND_OSGAL_H 1

namespace osg
{
	class Group;
}

#include "Plugin.h"
#include "DefFileGroup.h"
#include "Entity.h"


//=========================================================
//! This plugin is for adding positional audio to entities
//! 
class PluginRenderEntsSoundOSGAL : public Plugin
{
public:

	//=========================================================
	//! General Constructor
	//!
	PluginRenderEntsSoundOSGAL();

	//=========================================================
	//! General Destructor
	//!
	virtual ~PluginRenderEntsSoundOSGAL() throw();

	//=========================================================
	//! The per-frame processing that this plugin performs
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );

private:

};

#endif
