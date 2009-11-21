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
 *  FILENAME:   PluginViewMgr.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  listen for ViewControl and ViewDefinition packets and make their 
 *  information available to other plugins via the blackboard.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  04/06/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 *
 *  2007-07-21 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#ifndef PLUGIN_VIEWMGR_H
#define PLUGIN_VIEWMGR_H

#include <string>
#include <map>
#include <list>

#include "Plugin.h"
#include "DefFileParser.h"
#include "View.h"
#include "Entity.h"
#include "AllCigi.h"

#include "ViewCtrlP.h"
#include "ViewDefP.h"

//=========================================================
//! This plugin is responsible for maintaining the contents of the Views
//! 
class PluginViewMgr : public Plugin 
{
public:
	//=========================================================
	//! General Constructor
	//! 
	PluginViewMgr();

	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginViewMgr() throw();
	
	//=========================================================
	//! The per-frame processing that this plugin performs.
	//! This plugin's act() sanity-checks the contents of the view parameters.
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );
	
private:
	
	//=========================================================
	//! The incoming message buffer.  Retrieved from the blackboard.
	//! 
	CigiIncomingMsg *ImsgPtr;

	//=========================================================
	//! The root of the def-file tree.  Retrieved from the blackboard.
	//! 
	DefFileGroup **DefFileData;
	
	//=========================================================
	//! An entity container, containing all the entities.  
	//! Retrieved from the blackboard.
	//! 
	mpv::EntityContainer *allEntities;
	
	//=========================================================
	//! A std::map of the parameters for each of the view-groups in the system.
	//! Note - We must store information for all ViewGroups, rather than 
	//! just the group that this program's view is a member of, because 
	//! ViewCtrl and ViewDef packets are not guaranteed to arrive in 
	//! the correct order.  ex: the host may send offset data for a group 
	//! via a ViewCtrl packet, *then* make our view a member of that group 
	//! via a subsequent ViewDef packet.
	//! 
	std::map< int, mpv::ViewGroupParams * > viewGroupParamsMap;

	typedef std::map< int, mpv::RefPtr<mpv::View> > ViewMap;
	
	//=========================================================
	//! A std::map of the parameters for each of the views that this instance 
	//! of MPV will display.  Posted to the blackboard.
	//! Note - This map does *not* contain an entry for every view in the 
	//! system, only those listed in the def file.
	//! 
	ViewMap viewMap;
	
	//=========================================================
	//! The view which should be displayed when the MPV is in the standby 
	//! state.  Posted to the blackboard.
	//! 
	mpv::RefPtr<mpv::View> standbyView;

	//=========================================================
	//! The view which should be displayed when the MPV is loading a 
	//! database.  Posted to the blackboard.
	//! 
	mpv::RefPtr<mpv::View> databaseLoadingView;

	//=========================================================
	//! The packet processor for viewctrl packets
	//! 
	ViewCtrlP viewCtrlP;

	//=========================================================
	//! The packet processor for viewdef packets
	//! 
	ViewDefP viewDefP;
	
	//=========================================================
	//! Retrieves view-related preferences from the config data tree.
	//! 
	void getConfigData( void );
	
	//=========================================================
	//! Calls initViewParams on each View in viewMap
	//! 
	void initAllViewParams() ;
	
	//=========================================================
	//! Performs some sanity checking on a view's values.
	//! \param view - the view to check
	//! 
	void sanityCheckView( mpv::View *view );
	
};



#endif

