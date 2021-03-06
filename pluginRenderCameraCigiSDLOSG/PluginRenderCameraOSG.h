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
 *  FILENAME:   PluginRenderCameraOSG.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  create a rendering window, update the camera position and render the scene.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  03/29/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 *
 *  2006-11-05 Andrew Sampson
 *      Moved all of the camera matrix manipulation out of this plugin and 
 *      into a separate plugin, pluginCameraMgrOSG.
 *
 *  2007-07-15 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#ifndef PLUGIN_RENDER_CAMERA_OSG_H
#define PLUGIN_RENDER_CAMERA_OSG_H

#include <map>
#include <list>
#include <osg/Vec3d>
#include <osg/Matrix>
#include <osg/Group>
#include <osgUtil/SceneView>
#include <osg/LOD>

#include "Plugin.h"
#include "Entity.h"
#include "View.h"
#include "TrackerParams.h"
#include "DefFileGroup.h"

#include "osgSDL/Viewer"
#include "osgSDL/Viewport"

#include "ViewportWrapper.h"

enum DisplaySettingOptions
{
	OSG_3D_EYE_SEPARATION,
	OSG_3D_SCREEN_WIDTH,
	OSG_3D_SCREEN_HEIGHT,
	OSG_3D_SCREEN_DISTANCE,
	OSG_3D_FUSION_DISTANCE,
};

//=========================================================
//! This plugin is responsible for creating a rendering window and 
//! rendering the scene.  
//! It uses SDL to create a rendering window and OSG to render the scene.
//! 
class PluginRenderCameraCigiSDLOSG : public Plugin 
{
public:

	//=========================================================
	//! General Constructor
	//! 
	PluginRenderCameraCigiSDLOSG();

	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginRenderCameraCigiSDLOSG() throw();
	
	//=========================================================
	//! The per-frame processing that this plugin performs.
	//! This plugin's act() will update the camera and render the scene.
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );
	
	//=========================================================
	//! dumps the entire scene graph to the console
	//! 
	void analyzeScene( osg::Node *node=NULL, int level=0 ) ;

	//=========================================================
	//! dumps info about LODs in the scene to the console
	//! 
	void analyzeLODs( osg::Node *node=NULL, int level=0 ) ;

	//=========================================================
	//! Enables or disables the various viewports, based on the system state
	//! \param state - The current system state
	//!
	void changeActiveViewports( SystemState::ID state );

	void toggleFullscreen();

	void cycleThroughDisplaySettings();
	void modifyDisplaySettings(bool increment = true);


private:
	//=========================================================
	//! The root node of the scene graph.  Retrieved from the blackboard.
	//! 
	osg::Group *rootNode;
	
	//=========================================================
	//! The root node of the splash screen.  Retrieved from the blackboard.
	//! 
	osg::Group *splashScreenNode;
	
	//=========================================================
	//! The root of the def-file tree.  Retrieved from the blackboard.
	//! 
	DefFileGroup **configData;

	//=========================================================
	//! Map containing the view parameters (used for frustum setup,
	//! entity-following, etc).  Retrieved from the blackboard.
	//! 
	std::map< int, mpv::RefPtr<mpv::View> > *viewMap;
	
	//=========================================================
	//! The view used when in Standby mode.  Retrieved from the blackboard.
	//! 
	mpv::View *standbyView;

/*	//=========================================================
	//! The function pointer for sending SOF.  Retrieved from the blackboard.
	//! 
	void (*sendNetMessagesCB)(void);
*/

	//=========================================================
	//! The camera matrices, one for each view.  Each one of these is a 
	//! complete modelview matrix.  Retrieved from the blackboard.
	//! 
	std::map< int, osg::Matrix > *cameraMatrixMap;

	//=========================================================
	//! A list of osgGA event handlers.  This plugin needs to add its 
	//! event handler to this list in order to receive input events.
	//! (Yes, I realize that this is ironic, since those very events are 
	//! originating in this plugin... I'm just keeping things consistent.)
	//! Retrieved from the blackboard.
	//! 
	std::list< osg::ref_ptr<osgGA::GUIEventHandler> > *eventHandlerList;
	
	//=========================================================
	//! A list of osgGA input events.  This plugin will add the events it 
	//! recieves from SDL to this list, so that those events can be distributed 
	//! to other plugins.  Retrieved from the blackboard.
	//! 
	std::list< osg::ref_ptr<osgGA::GUIEventAdapter> > *eventList;
	
	//=========================================================
	//! The system state for the previous frame.  Only used in act(), to 
	//! determine when the system changes state.
	//! 
	SystemState::ID previousState;
	
	//=========================================================
	//! Boolean that indicates that the user has requested that the program 
	//! exit.  If this is set to true, then the plugin will set stateContext's 
	//! userRequestedQuit flag to true.
	//! 
	bool userRequestedQuit;
	
	//=========================================================
	//! Boolean that indicates whether openWindow has been called yet.
	//! 
	bool haveOpenedWindow;

	//=========================================================
	//! Map containing osgSDL Viewports.  Each viewport has a FOV, a transform 
	//! positioning its view within the scene, a position within the 2d window, 
	//! etc.  Each entry in this map corresponds to one of the CIGI views that 
	//! the MPV is rendering.  Additionally, there is one entry that does not 
	//! correspond to a CIGI view: the standby viewport.  
	//! 
	std::map< int, mpv::RefPtr<ViewportWrapper> > viewportMap;
	
	//=========================================================
	//! osgSDL Viewer.  Used to create the rendering window.
	//! 
	osgSDL::Viewer *viewer;
	
	//=========================================================
	//! used to prevent SDLQuit from being called twice.
	//! 
	bool calledSDLInit;
	
	//=========================================================
	//! If set to true, the SDL joystick system will be initialized.
	//! If a joystick is found, it can be used to move the camera around 
	//! when in local-camera-control mode.
	//! 
	bool useJoystick;
	
	//=========================================================
	//! Called by act() when the system is in the Operate state.  This is the 
	//! method that actually does the rendering.
	//! 
	void operate();
	
	//=========================================================
	//! retrieves configuration info from the config tree (width, 
	//! height, etc)
	//! 
	void getConfig();

	//=========================================================
	//! Calls setupSDLViewer, setupViewports, etc.
	//! 
	void openWindow();
	
	//=========================================================
	//! initializes SDL and creates the viewer object
	//! 
	void setupSDLViewer();

	//=========================================================
	//! Sets up the viewportMap, gives the viewports some default settings, etc
	//! 
	void setupViewports();

	//=========================================================
	//! Updates the view transformation matrices for each of the viewports.
	//! 
	void updateViewports();

	//=========================================================
	//! Changes the resolution of the GL context.  Performed at startup, 
	//! and whenever the user toggles fullscreen.
	//! 
	void changeResolution();

	//=========================================================
	//! Prints out the results of SDL_GetError, along with some explanation of 
	//! the various error messages.
	//!
	void reportSDLError();

	//=========================================================
	//! the window width, in pixels
	//!
	int windowWidth;

	//=========================================================
	//! the window height, in pixels
	//!
	int windowHeight;

	//=========================================================
	//! a flag indicating whether the user has requested that the fullscreen 
	//! state be changed
	//!
	bool needToToggleFullscreen;
	
	//=========================================================
	//! a flag indicating whether the mpv is fullscreen
	//!
	bool isFullscreen;

	//=========================================================
	//! the SDL joystick 
	//!
	SDL_Joystick *joy;
	
	
	//=========================================================
	//! Used to index into the displaySettings vector and select
	//! which option to manipulate.
	//! 
	unsigned int currentDisplaySetting;

	//=========================================================
	//! A vector of DisplaySettingOptions that can be used to
	//! manipulate 3d settings.
	//! 
	std::vector<DisplaySettingOptions> displaySettings;
	
	//=========================================================
	//! The string that is displayed in the window's title bar.
	//! 
	std::string windowTitle;
};



#endif

