/** <pre>
 *  Plugin to add model files to entities, for the MPV
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
 *  2007-03-28 Andrew Sampson
 *      Completely reworked the mechanism by which entity subgraphs are 
 *      constructed.
 *
 *  2007-07-15 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 *  2007-11-03  Andrew Sampson
 *      Moved code out of pluginRenderEntsOSG and into its own plugin
 *
 * </pre>
 */

#ifndef PLUGIN_RENDER_ENTS_MODEL_FILE_OSG_H
#define PLUGIN_RENDER_ENTS_MODEL_FILE_OSG_H

#include <list>

#include "Plugin.h"

//=========================================================
//! This plugin is responsible for loading model files from disk and 
//! adding them to entities.  This is a rather important and basic 
//! feature, so you'll probably want to load this plugin.
//! 
class PluginRenderEntsModelFileOSG : public Plugin 
{
public:

	//=========================================================
	//! General Constructor
	//! 
	PluginRenderEntsModelFileOSG();

	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginRenderEntsModelFileOSG() throw();
	
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
