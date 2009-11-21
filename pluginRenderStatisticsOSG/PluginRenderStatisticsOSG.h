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
 *  FILENAME:   PluginRenderStatisticsOSG.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  track and display various performance statistics.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  07/06/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 *
 *  2007-07-15 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#ifndef PLUGIN_RENDER_STATISTICS_OSG_H
#define PLUGIN_RENDER_STATISTICS_OSG_H

#include <list>

#include <osg/Group>
#include <osgGA/GUIEventHandler>

#include "Plugin.h"
#include "MPVTimer.h"
#include "OverlayScreen.h"

namespace RenderStatistics
{

//=========================================================
//! This plugin is responsible for tracking and displaying various performance 
//! statistics.
//! 
class PluginRenderStatisticsOSG : public Plugin 
{
public:

	//=========================================================
	//! General Constructor
	//! 
	PluginRenderStatisticsOSG();

	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginRenderStatisticsOSG() throw();
	
	//=========================================================
	//! The per-frame processing that this plugin performs
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );
	
	void cycleThroughScreen( int screenSet );
	
private:
	//=========================================================
	//! The root node of the scene graph.  Retrieved from the blackboard.
	//! 
	osg::Group *rootNode;
	
	//=========================================================
	//! The elapsed time variable, from the MPV kernel.  
	//! Retrieved from the blackboard.
	//!
	double *timeElapsedLastFrame;

	//=========================================================
	//! A list of pointers to osgGA event handlers.  Retrieved from the 
	//! blackboard.
	//! 
	std::list< osg::ref_ptr<osgGA::GUIEventHandler> > *eventHandlerList;
	
	//=========================================================
	//! The statistics-box branch of the scene graph.  Attached to the scene 
	//! root.
	//! 
	osg::ref_ptr<osg::Group> boxBranchNode;
	
	//=========================================================
	//! A boolean indicating whether currentScreen has been changed
	//! 
	bool changedScreen;
	
	mpvosg::OverlayScreen *currentScreen;
	
	std::map< int, std::list<mpvosg::OverlayScreen *> > overlayScreens;

	void createStatisticsBox();
	void deleteOverlayScreens();

};

}

#endif

