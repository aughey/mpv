/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2004 The Boeing Company
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
 *  FILENAME:   PluginRenderEntsNearModel.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *
 *  PROGRAM DESCRIPTION:
 *  This class contains the data and methods necessary to modify the near and
 *  far clipping planes for the view glFrustum.  This is required when objects
 *  are closer than the near clipping plane and yet need to be visible.
 *  Objects such as the cockpit are an example.
 *
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *
 *  03-29-2005 David Fries                       MPV_CR_DR_1
 *      Initial Release.
 *
 *  2007-05-22 Andrew Sampson
 *      Ported to new entity-construction system.
 *
 *  2007-07-15 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#ifndef PLUGIN_RENDER_ENT_NEAR_MODEL_H
#define PLUGIN_RENDER_ENT_NEAR_MODEL_H 1

#include "Plugin.h"

//=========================================================
//! This plugin can add nodes to entities which manipulate the 
//! near and far clipping plane values.  The intended application 
//! is to allow canopy/cockpit models to be drawn very close to the 
//! eyepoint, without having to use a small near-clipping-plane 
//! distance.  Small near-clipping-plane distances will compromise 
//! z-buffer accuracy, causing z-fighting.
class PluginRenderEntsNearModel : public Plugin
{
public:

	//=========================================================
	//! General Constructor
	//!
	PluginRenderEntsNearModel();

	//=========================================================
	//! General Destructor
	//!
	virtual ~PluginRenderEntsNearModel() throw();

	//=========================================================
	//! The per-frame processing that this plugin performs
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );

};

#endif
