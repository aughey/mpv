/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2006 The Boeing Company
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
 *  FILENAME:   PluginUserInputMgrOSGGA.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  receive user input events and distribute those events to other plugins.
 *  
 *  2006-11-18 Andrew Sampson
 *  
 *  2007-07-21 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 */

#ifndef PLUGIN_USER_INPUT_MGR_OSGGA_H
#define PLUGIN_USER_INPUT_MGR_OSGGA_H

#include <list>
#include <osgGA/GUIEventHandler>
#include <osgGA/GUIEventAdapter>

#include "Plugin.h"
#include "DefFileGroup.h"

//=========================================================
//! This plugin is responsible for distributing user input events
//! 
class PluginUserInputMgrOSGGA : public Plugin 
{
public:

	//=========================================================
	//! General Constructor
	//! 
	PluginUserInputMgrOSGGA();

	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginUserInputMgrOSGGA() throw();
	
	//=========================================================
	//! The per-frame processing that this plugin performs.
	//! This plugin's act() will check for input events and process them.
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );

	//=========================================================
	//! Sets a flag to indicate that the user has requested that the program 
	//! exit.  Called by the keyboard handler.
	//! 
	void setUserRequestedQuit() { userRequestedQuit = true; }
	
	//=========================================================
	//! Sets a flag to indicate that the user has requested that the program 
	//! exit.  Called by the keyboard handler.
	//! 
	void setUserRequestedDebug() { userRequestedDebug = true; }
	
private:
	//=========================================================
	//! A list of pointers to osgGA event handlers.  Other plugins can add 
	//! their own event handlers here, and the plugin will send events to 
	//! the handlers it finds in this list.  Posted to the blackboard.
	//! 
	std::list< osg::ref_ptr<osgGA::GUIEventHandler> > GUIEventHandlerList;
	
	//=========================================================
	//! A list of osgGA events.  Events should be added to this list by 
	//! the plugin responsible for event translation.  Posted to the blackboard.
	//! 
	std::list< osg::ref_ptr<osgGA::GUIEventAdapter> > GUIEventList;
	
	//=========================================================
	//! Boolean that indicates that the user has requested that the program 
	//! exit.  If this is set to true, then the plugin will set stateContext's 
	//! userRequestedQuit flag to true.
	//! 
	bool userRequestedQuit;
	
	//=========================================================
	//! Boolean that indicates that the user has requested that the program 
	//! transition from Standby to Debug.  If this is set to true, then the 
	//! plugin will set stateContext's userRequestedDebug flag to true.
	//! 
	bool userRequestedDebug;
	
};

#endif
