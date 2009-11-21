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
 *  FILENAME:   PluginRenderEntsHighVisibilityOSG.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  adds high-visibility spheres to entities
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  04/15/2005 Andrew Sampson
 *  Initial Release.
 *  
 *  2007-05-21 Andrew Sampson
 *  Plugin reworked to use new entity-construction system.
 *  
 *  2007-07-15 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 * </pre>
 */


#ifndef PLUGIN_RENDER_ENTS_HIGH_VISIBILITY_OSG_H
#define PLUGIN_RENDER_ENTS_HIGH_VISIBILITY_OSG_H

#include <list>
#include <osg/Group>

#include "Plugin.h"
#include "Entity.h"
#include "DefFileGroup.h"

//=========================================================
//! This plugin adds high-visibility spheres to entities, which can help 
//! users locate entities more quickly.  This is especially useful for 
//! testing and debugging, and for "newbie" users who may have difficulty 
//! finding other entities.  Let's be honest: it's difficult to spot a 
//! two-pixel blob moving at 500+ knots.
//! 
class PluginRenderEntsHighVisibilityOSG : public Plugin 
{
public:

	//=========================================================
	//! General Constructor
	//! 
	PluginRenderEntsHighVisibilityOSG();

	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginRenderEntsHighVisibilityOSG() throw();
	
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

