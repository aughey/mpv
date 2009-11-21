/** <pre>
 *  Plugin to add text labels to entities, for the MPV
 *  Copyright (c) 2006 Andrew Sampson
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
 *  Initial Release: 2006-03-19 Andrew Sampson
 *  
 *  2007-07-10  Andrew Sampson
 *      Ported to the new entity construction system.
 *
 *  2007-07-15 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 * </pre>
 */

#ifndef PLUGIN_RENDER_ENTS_LABELS_OSG_H
#define PLUGIN_RENDER_ENTS_LABELS_OSG_H

#include <list>

#include "Plugin.h"

//=========================================================
//! This plugin is responsible for adding text labels to entities.  
//! Possible uses include callsigns, debugging output (position, DIS type 
//! fields, etc), and the like.  Note that the *Host* determines the content 
//! of the text fields.
//! 
class PluginRenderEntsLabelsOSG : public Plugin 
{
public:

	//=========================================================
	//! General Constructor
	//! 
	PluginRenderEntsLabelsOSG();

	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginRenderEntsLabelsOSG() throw();
	
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
