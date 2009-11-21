/** <pre>
 *  MPV plugin to automatically manipulate entity articulations
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
 *  2008-11-02  Andrew Sampson
 *      Initial release
 *
 * </pre>
 */

#ifndef _PLUGIN_RENDER_ENTS_AUTO_ARTICULATION_OSG_H_
#define _PLUGIN_RENDER_ENTS_AUTO_ARTICULATION_OSG_H_

#include <list>

#include "Plugin.h"

//=========================================================
//! This plugin can automatically manipulate entity articulations.  Currently, 
//! the articulations are manipulated based on the entity's motion, with the 
//! intent of causing wheels to rotate without being explicitly driven by the 
//! Host.
//! Note that this plugin doesn't really add anything to the entity scene 
//! graph, as would be the case with 
//! 
class PluginRenderEntsAutoArticulationOSG : public Plugin 
{
public:

	//=========================================================
	//! General Constructor
	//! 
	PluginRenderEntsAutoArticulationOSG();

	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginRenderEntsAutoArticulationOSG() throw();
	
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
