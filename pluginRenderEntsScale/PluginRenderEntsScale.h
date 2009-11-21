/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2007 The Boeing Company
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *
 *  FILENAME:   PluginRenderEntsScale.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *
 *  PROGRAM DESCRIPTION:
 *  This class contains the data and methods necessary to 
 *  fixme
 *
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *
 *  2006-10-29    Andrew Sampson
 *      Initial Release.
 *  
 *  2007-07-10    Andrew Sampson
 *      Plugin reworked to use new entity-construction system.
 *  
 *  2007-07-15 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#ifndef PLUGIN_RENDER_ENT_SCALE_H
#define PLUGIN_RENDER_ENT_SCALE_H 1

#include "Plugin.h"

//=========================================================
//! This plugin is ... fixme
//! 
class PluginRenderEntsScale : public Plugin
{
public:

	//=========================================================
	//! General Constructor
	//!
	PluginRenderEntsScale();

	//=========================================================
	//! General Destructor
	//!
	virtual ~PluginRenderEntsScale() throw();

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
