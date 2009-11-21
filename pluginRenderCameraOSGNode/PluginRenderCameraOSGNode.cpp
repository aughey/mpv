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
 *  FILENAME:   PluginRenderCameraOSG.cpp
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
 *  2008-02-01 Andrew Sampson
 *      Moved some code out of PluginRenderCameraOSG and into a new class, 
 *      ViewportWrapper
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#ifdef WIN32
#define _USE_MATH_DEFINES
#include <float.h>
#define isnan _isnan
#endif

#include <math.h>
#include <iostream>
#include <SDL.h>

#include <osgGA/StateSetManipulator>

#include "PluginRenderCameraOSG.h"
#include "osgSDL/SDLEventAdapter"

//=========================================================
//! The standby viewport has an entry in viewportMap, but is used only when the 
//! MPV is in Standby mode.  The STANDBY_VIEWPORT_INDEX is the standby 
//! viewport's index in viewportMap.  It is not a valid CIGI view ID, so there 
//! is no chance of an ID collision.
//! 
#define STANDBY_VIEWPORT_INDEX 65536

using namespace mpv;

// ================================================
// operator<<( std::ostream &, DisplaySettingOptions const &)
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
std::ostream &operator<<(std::ostream &out, DisplaySettingOptions const &opt)
{
	switch(opt)
	{
		case OSG_3D_EYE_SEPARATION:   out << "OSG_3D_EYE_SEPARATION"; break;
		case OSG_3D_SCREEN_WIDTH:     out << "OSG_3D_SCREEN_WIDTH"; break;
		case OSG_3D_SCREEN_HEIGHT:    out << "OSG_3D_SCREEN_HEIGHT"; break;
		case OSG_3D_SCREEN_DISTANCE:  out << "OSG_3D_SCREEN_DISTANCE"; break;
		case OSG_3D_FUSION_DISTANCE:  out << "OSG_3D_FUSION_DISTANCE"; break;
	}
	return out;
}

namespace RenderCameraOSG
{

// EVENT PROXY ////////////////////////////////////////////////////////

class EventProxyHandler: public osgGA::GUIEventHandler
{
public:
	EventProxyHandler( std::list< osg::ref_ptr<osgGA::GUIEventAdapter> > *_eventList ) 
	{
		eventList = _eventList;
	}

	bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &)
	{
		// the cast is to remove the const-ness
		eventList->push_back( (osgGA::GUIEventAdapter *)(&ea) );
		
		return false;
	}

private:
	std::list< osg::ref_ptr<osgGA::GUIEventAdapter> > *eventList;
};


// KEYBOARD HANDLER ///////////////////////////////////////////////////

class KeyboardHandler: public osgGA::GUIEventHandler
{
public:
	KeyboardHandler( PluginRenderCameraOSGNode *plug ) 
	{
		plugin = plug;
	}

	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &)
	{
		if( plugin == NULL )
			return false;
		
		if( ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN )
		{
			int key = ea.getKey();
			if( key == 'f' )
			{
				plugin->toggleFullscreen();
				return true;
			}
			else if( key == 'd' )
			{
				plugin->analyzeScene();
				return true;
			}
			else if( key == 'D' )
			{
				plugin->analyzeLODs();
				return true;
			}
			else if( key == 'h' )
			{
				// hide cursor
				int cursorState = SDL_ShowCursor( SDL_QUERY );
				SDL_ShowCursor( cursorState == SDL_DISABLE ? SDL_ENABLE : SDL_DISABLE );
				return true;
			}
			else if( key == '|' )
			{
				// change what +/- modify
				plugin->cycleThroughDisplaySettings();
				return true;
			}
			else if( key == '+' )
			{
				plugin->modifyDisplaySettings(true);
				return true;
			}
			else if( key == '-' )
			{
				plugin->modifyDisplaySettings(false);
				return true;
			}
		}

		return false;
	}

	virtual void getUsage( osg::ApplicationUsage &usage ) const
	{
		usage.addKeyboardMouseBinding( "f", "Toggle fullscreen (broken on Windows)" );
		usage.addKeyboardMouseBinding( "d", "Dump the scene graph to console" );
		usage.addKeyboardMouseBinding( "h", "Hide the mouse cursor" );
		usage.addKeyboardMouseBinding( "|", "Cycle through some display settings, mostly for stereo vision" );
		usage.addKeyboardMouseBinding( "+ and -", "Increment/decrement display settings (see '|')" );
	}

private:
	PluginRenderCameraOSGNode *plugin;
};

} // namespace





// PLUGINRENDERCAMERAOSG //////////////////////////////////////////////

EXPORT_DYNAMIC_CLASS( PluginRenderCameraOSGNode )

// ================================================
// PluginRenderCameraOSGNode
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderCameraOSGNode::PluginRenderCameraOSGNode() : Plugin() 
{
	name_ = "PluginRenderCameraOSGNode";
	licenseInfo_.setLicense( LicenseInfo::LicenseGPL );
	licenseInfo_.setOrigin( "Community" );
	dependencies_.push_back( "PluginRenderOSG" );
	dependencies_.push_back( "PluginRenderSplashScreenOSG" );
	dependencies_.push_back( "PluginViewMgr" );
	dependencies_.push_back( "PluginCameraMgrOSG" );
	dependencies_.push_back( "PluginUserInputMgrOSGGA" );

	rootNode = NULL;
	splashScreenNode = NULL;
	configData = NULL;
	viewMap = NULL;
	standbyView = NULL;
//	sendNetMessagesCB = NULL;
	cameraMatrixMap = NULL;
	eventHandlerList = NULL;
	eventList = NULL;
	
	previousState = SystemState::Quit;
	
	userRequestedQuit = false;
	haveOpenedWindow = false;
	viewer = NULL;

	calledSDLInit = false;
	useJoystick = true;
	
	windowWidth = 800;
	windowHeight = 600;
	isFullscreen = false;
	needToToggleFullscreen = false;
	
	currentDisplaySetting = 0;

	//
	// If we are in 3d mode allow the user to modify these
	// osg::DisplaySettings by using the keyboard.
	//
	if(char *envvar = getenv("OSG_STEREO")) {
		if(strcmp(envvar, "ON")==0) {
			displaySettings.push_back(OSG_3D_EYE_SEPARATION);
			//displaySettings.push_back(OSG_3D_SCREEN_WIDTH);
			//displaySettings.push_back(OSG_3D_SCREEN_HEIGHT);
			displaySettings.push_back(OSG_3D_SCREEN_DISTANCE);
			displaySettings.push_back(OSG_3D_FUSION_DISTANCE);
		}
	}

	joy = NULL;
	windowTitle = "Multi-Purpose Viewer";
}


// ================================================
// ~PluginRenderCameraOSGNode
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderCameraOSGNode::~PluginRenderCameraOSGNode() throw() 
{

	if( joy )
	{
		// Close if opened
		if( SDL_JoystickOpened(0) )
			SDL_JoystickClose(joy);
		joy = NULL;
	}

	if( calledSDLInit ) {
		SDL_Quit();
		calledSDLInit = false;
	}

}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderCameraOSGNode::act( SystemState::ID state, StateContext &stateContext )
{
	bool stateChanged = (previousState != state);
	previousState = state;
	
	switch( state )
	{
	
	case SystemState::BlackboardRetrieve:
		// This state is for retrieving things from the blackboard
		
		// get the root node from the BB
		bb_->get( "RootNodeOSG", rootNode );

		// get the splash screen node from the BB
		bb_->get( "SplashScreenRootNodeOSG", splashScreenNode );

		bb_->get( "DefinitionData", configData );

		bb_->get( "ViewMap", viewMap );

		bb_->get( "StandbyView", standbyView );
		
/*		// get the pointer to the boolean which tells the kernel if it should 
		// send the message or if one of the plugins will take care of it
		if( bb_->contains( "ShouldKernelSendNetMessagesBool" ) ) {
			bool *shouldKernelSendMsgs = (bool*)bb_->get( "ShouldKernelSendNetMessagesBool" );
			if( *shouldKernelSendMsgs == false )
			{
				// "false" indicates that some other plugin has already laid claim 
				// to SendNetMessagesCB.  We do nothing in this situation.
			}
			else
			{
				// get the function pointer for sending SOF
				if( bb_->contains( "SendNetMessagesCB" ) ) {
					sendNetMessagesCB = (void (*)())bb_->get( "SendNetMessagesCB" );

					// this tells the kernel and the other plugins that *this* plugin will 
					// take care of sending SOF
					*shouldKernelSendMsgs = false;
				}
			}
		}
*/
		
		// Use the window name from the blackboard if it exists
		{
			// I assume that the value of WindowName is valid by this stage... 
			// perhaps that is not a good assumption.
			std::string *temp = NULL;
			if( bb_->get( "WindowName", temp, false ) )
				windowTitle = *temp;
		}

		// get the camera matrices
		bb_->get( "CameraMatrixMap", cameraMatrixMap );
		
		// get the list of user input callbacks
		bb_->get( "GUIEventHandlerList", eventHandlerList );
		
		// get the list of user input events
		bb_->get( "GUIEventList", eventList );
		
		// add some callbacks to eventHandlerList
		if( eventHandlerList )
		{
			eventHandlerList->push_back( new RenderCameraOSG::KeyboardHandler( this ) );
			
			if( rootNode ) 
			{
				osgGA::StateSetManipulator *ssm = new osgGA::StateSetManipulator;
				ssm->setStateSet( rootNode->getOrCreateStateSet() );
				eventHandlerList->push_back( ssm );
			}
		}
		
		break;


	case SystemState::ConfigurationProcess:
		getConfig();
		break;
		
	case SystemState::Reset:
		// Note: openWindow definitely needs to be called *after* the 
		// ConfigurationProcess state has completed, otherwise the 
		// contents of viewMap will be missing.
		
		// openWindow will check if it's already been called
		openWindow();
		break;

	case SystemState::Standby:
	case SystemState::DatabaseLoad:
	case SystemState::Operate:
	case SystemState::Debug:
		if( stateChanged )
			changeActiveViewports( state );

		// render the scene or (if the state is Standby) 
		// render some splash screen goodness
		operate();
		break;
		
	case SystemState::Shutdown:
		// We need to delete the viewer now, rather than in the destructor, 
		// because viewer has a reference to the root scene node.  
		// Note - this may no longer be an issue...
		if( viewer )
		{
			delete viewer;
			viewer = NULL;
		}

		if( calledSDLInit )
		{
			SDL_Quit();
			calledSDLInit = false;
		}
		break;
		
	default:
		break;
	}
	
	if( userRequestedQuit )
		stateContext.userRequestedQuit = true;
}


// ================================================
// operate
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderCameraOSGNode::operate()
{
	if( needToToggleFullscreen )
	{
		isFullscreen = !isFullscreen;
		changeResolution();
		needToToggleFullscreen = false;
	}
	
	updateViewports();
	
	// have the update visitor traverse the scene
//	cg->getSceneData()->accept( updateVisitor );
	
	// check to see if we should continue
	if( viewer->isTerminated() )
	{
		userRequestedQuit = true;
	}
	else
	{
		// render the scene
		viewer->runOnce();
		
/*		// send the SOF
		if( sendNetMessagesCB )
			sendNetMessagesCB();
*/
	}

}


// ================================================
// getConfig
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderCameraOSGNode::getConfig()
{
	if( configData == NULL || *configData == NULL )
		return;
	
	DefFileGroup *windowGroup = (*configData)->getGroupByURI( "/window/" );
	if( windowGroup == NULL )
		return;
	
	DefFileAttrib *attr;

	attr = windowGroup->getAttribute( "width" );
	if( attr )
	{
		windowWidth = attr->asInt();
	}

	attr = windowGroup->getAttribute( "height" );
	if( attr )
	{
		windowHeight = attr->asInt();
	}

	attr = windowGroup->getAttribute( "fullscreen" );
	if( attr )
	{
		isFullscreen = attr->asInt();
	}

	attr = windowGroup->getAttribute( "use_joystick" );
	if( attr )
	{
		useJoystick = attr->asInt();
	}

	attr = windowGroup->getAttribute( "title" );
	if( attr )
	{
		windowTitle = attr->asString();
	}

}


// ================================================
// analyzeScene
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderCameraOSGNode::analyzeScene( osg::Node *node, int level ) 
{
	if( node == NULL ) node = rootNode;

	std::string nodeName = node->getName();
	std::string className( node->className() );
	
	for( int i = 0; i < level; i++ ) {
		printf( "\t" );
	}
	printf( "%s%s%s%s Address %p\n", 
		"Name:\t", nodeName.c_str(), "\tClass:\t", className.c_str(),
		(void *)node );
	
	osg::Group *group = node->asGroup();
	if( group ) {
		int numChildren = group->getNumChildren();
		for( int i = 0; i < numChildren; i++ ) {
			analyzeScene( group->getChild( i ), level + 1 );
		}
	}
	
}


// ================================================
// analyzeLODs
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderCameraOSGNode::analyzeLODs( osg::Node *node, int level ) 
{
	if( node == NULL ) node = rootNode;

	std::string nodeName = node->getName();
	std::string className( node->className() );
	if( className.find( "LOD" ) != std::string::npos )
	{
		for( int i = 0; i < level; i++ ) {
			printf( "\t" );
		}
		printf( "%s%s%s%s Address %p ", 
			"Name:\t", nodeName.c_str(), "\tClass:\t", className.c_str(),
			(void *)node );
		
		osg::LOD *lod = dynamic_cast<osg::LOD *>(node);
		if( lod )
		{
			int numChildren = lod->getNumChildren();
			printf( "num children under LOD node: %i\n", numChildren );
			for( int i = 0; i < numChildren; i++ ) {
				for( int j = 0; j < level; j++ ) {
					printf( "\t" );
				}
				printf( "child %i has range %f to %f\n", i, lod->getMinRange(i), lod->getMaxRange(i) );
			}
		}
		else
		{
			printf( "warning - couldn't cast a %s to a osg::LOD\n", 
				className.c_str() );
		}
	}
	
	osg::Group *group = node->asGroup();
	if( group ) {
		int numChildren = group->getNumChildren();
		for( int i = 0; i < numChildren; i++ ) {
			analyzeLODs( group->getChild( i ), level + 1 );
		}
	}
	
}


// ================================================
// openWindow
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderCameraOSGNode::openWindow()
{
	if( haveOpenedWindow )
		return;
	haveOpenedWindow = true;

	// the code for changing the screen resolution is here, because it 
	// must be *after* the screen resolution preferences are loaded from 
	// the config tree (getConfig)

	setupSDLViewer();

	setupViewports();

	changeResolution();

	if( !viewer->open() )
	{
		reportSDLError();
		fprintf( stderr, "\nExiting.\n" );
		exit( 1 );
	}

	viewer->init();

	// set the window title
	SDL_WM_SetCaption( windowTitle.c_str(), "MPV" );
}

	
// ================================================
// setupSDLViewer
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderCameraOSGNode::setupSDLViewer()
{
	if( calledSDLInit )
		return;
	
	unsigned int sdlFlags = 
		/*SDL_INIT_TIMER | */
		SDL_INIT_VIDEO | 
		SDL_INIT_NOPARACHUTE;

	if( useJoystick ) 
		sdlFlags |= SDL_INIT_JOYSTICK;
	
	if( SDL_Init( sdlFlags ) < 0 )
	{
		reportSDLError();
		fprintf( stderr, "\nExiting.\n" );
		exit( 1 );
	}

	viewer = new osgSDL::Viewer;
	calledSDLInit = true;

	if( useJoystick )
	{
		// Check for joystick
		if( SDL_NumJoysticks() > 0 )
		{
			// Open joystick
			joy = SDL_JoystickOpen(0);

			if( joy )
			{
				printf("Opened Joystick 0\n");
				printf("Name: %s\n", SDL_JoystickName(0));
				printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joy));
				printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joy));
				printf("Number of Hats: %d\n", SDL_JoystickNumHats(joy));
				printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joy));

			}
			else
				printf("Couldn't open Joystick 0\n");

		}
	}

	viewer->addEventHandler( new RenderCameraOSG::EventProxyHandler( eventList ) );
}


// ================================================
// setupViewports
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderCameraOSGNode::setupViewports()
{
	if( viewMap == NULL )
		return;
	
	// need to remove all existing viewports first
	while( viewer->numViewports() > 0 )
	{
		viewer->removeViewport( 0 );
	}
	viewportMap.clear();
	
	// for each of the CIGI view in viewMap, add a viewport
	std::map< int, RefPtr<View> >::iterator vpIter = viewMap->begin();
	for( ; vpIter != viewMap->end(); vpIter++ )
	{
		View *view = (*vpIter).second.get();
		
		ViewportWrapper *viewportWrapper = new ViewportWrapper( view );
		viewportWrapper->setSceneGraphRoot( rootNode );
		
		viewer->addViewport( viewportWrapper->getViewport() );
		viewportMap[view->getID()] = viewportWrapper;
		
	}
	
	// add one more viewport, the standby viewport
	{
		StandbyViewportWrapper *viewportWrapper = new StandbyViewportWrapper( standbyView );
		viewportWrapper->setSceneGraphRoot( splashScreenNode );

		viewer->addViewport( viewportWrapper->getViewport() );
		viewportMap[STANDBY_VIEWPORT_INDEX] = viewportWrapper;

		/*
		The horizontal extents of the orthographic view will be fixed 
		at -1.0 to 1.0.
		
		The vertical extents of the view will be adjusted, based on the aspect 
		ratio of the rendering window.  Additionally, the top and bottom 
		margins will be adjusted such that the view is centered in the window.
		
		FIXME - eventually, I plan to have the splash screen plugin provide 
		the splash scene camera matrix.
		*/
		float inverseAspectRatio = (float)windowHeight / (float)windowWidth;
		viewportWrapper->setProjectionMatrix( osg::Matrix::ortho( 
			-1.0, 1.0, 
			-inverseAspectRatio, inverseAspectRatio, 
			-1.0, 1.0 ) );
	}
	
}


// ================================================
// updateViewports
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderCameraOSGNode::updateViewports()
{
/*
	std::map< int, RefPtr<View> >::iterator vpIter = viewMap->begin();
	for( ; vpIter != viewMap->end(); vpIter++ )
	{
		View *view = (*vpIter).second.get();
		if( view == NULL )
		{
			std::cerr << "ERROR in PluginRenderCameraOSGNode::updateViewports() - viewMap contains a NULL entry\n";
			break;
		}
		
		RefPtr<ViewportWrapper> viewportWrapper = viewportMap[view->getID()];
		const osg::Matrix &camMtx = (*cameraMatrixMap)[view->getID()];
		
		viewportWrapper->update( camMtx );
	}
*/
	std::map< int, RefPtr<ViewportWrapper> >::iterator vpIter = viewportMap.begin();
	for( ; vpIter != viewportMap.end(); vpIter++ )
	{
		if( vpIter->first == STANDBY_VIEWPORT_INDEX )
			continue;

		ViewportWrapper *viewportWrapper = vpIter->second.get();
		if( viewportWrapper == NULL )
		{
			std::cerr << "ERROR in PluginRenderCameraOSGNode::updateViewports() - viewportMap contains a NULL entry\n";
			break;
		}
		
		const osg::Matrix &camMtx = (*cameraMatrixMap)[vpIter->first];
		
		viewportWrapper->update( camMtx );
	}

}


// ================================================
// changeActiveViewports
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderCameraOSGNode::changeActiveViewports( SystemState::ID state )
{
	// Note - if we ever choose to implement detection of completely-obscured 
	// viewports, then this would be the place to disable such viewports.

	std::map< int, RefPtr<ViewportWrapper> >::iterator vIter = viewportMap.begin();
	for( ; vIter != viewportMap.end(); vIter++ )
	{
		ViewportWrapper *vp = vIter->second.get();
	
		if( state == SystemState::Standby || state == SystemState::DatabaseLoad )
		{
			if( vIter->first == STANDBY_VIEWPORT_INDEX )
				vp->setEnabled( true );
			else
				vp->setEnabled( false );
		}
		else
		{
			if( vIter->first == STANDBY_VIEWPORT_INDEX )
				vp->setEnabled( false );
			else
				vp->setEnabled( true );
		}
	}
}


// ================================================
// toggleFullscreen
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderCameraOSGNode::toggleFullscreen()
{
	needToToggleFullscreen = true;
}


// ================================================
// changeResolution
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderCameraOSGNode::changeResolution()
{
	// set the resolution for the viewer
	viewer->setDisplayMode( 
		windowWidth, windowHeight, 24, 
		isFullscreen ? SDL_FULLSCREEN : 0 );

	if( isFullscreen )
		// if we've switched to fullscreen mode, hide the cursor
		SDL_ShowCursor( SDL_DISABLE );
	else
		// if we've switched to windowed mode, un-hide the cursor
		SDL_ShowCursor( SDL_ENABLE );
}


// ================================================
// cycleThroughDisplaySettings
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderCameraOSGNode::cycleThroughDisplaySettings()
{
	if(displaySettings.size() == 0) return;

	currentDisplaySetting++;
	if(currentDisplaySetting >= displaySettings.size())
	{
		currentDisplaySetting = 0;
	}
	std::cout << "Currently modifying: " << displaySettings[currentDisplaySetting] << std::endl;
}

// ================================================
// modifyDisplaySettings
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderCameraOSGNode::modifyDisplaySettings(bool increment)
{
	if(displaySettings.size() == 0) return;

	std::map< int, RefPtr<ViewportWrapper> >::iterator vIter = viewportMap.begin();
	for( ; vIter != viewportMap.end(); vIter++ )
	{
		ViewportWrapper *vpw = vIter->second.get();
		osgSDL::Viewport *vp = vpw->getViewport();
		osgUtil::SceneView *sv = vp->getSceneView();
		osg::DisplaySettings *ds = sv->getDisplaySettings();

		switch (displaySettings[currentDisplaySetting])
		{
			case OSG_3D_EYE_SEPARATION:
				{
					float val = ds->getEyeSeparation();
					if(increment) val+=0.001;
					else          val-=0.001;
					std::cout << "Setting eye separation to " << val << std::endl;
					ds->setEyeSeparation(val);
				}
				break;

			case OSG_3D_SCREEN_WIDTH:
				{
					float val = ds->getScreenWidth();
					if(increment) val+=0.1;
					else          val-=0.1;
					std::cout << "Setting screen width to " << val << std::endl;
					ds->setScreenWidth(val);
				}
				break;

			case OSG_3D_SCREEN_HEIGHT:
				{
					float val = ds->getScreenHeight();
					if(increment) val+=0.1;
					else          val-=0.1;
					std::cout << "Setting screen height to " << val << std::endl;
					ds->setScreenHeight(val);
				}
				break;

			case OSG_3D_SCREEN_DISTANCE:
				{
					float val = ds->getScreenDistance();
					if(increment) val+=0.1;
					else          val-=0.1;
					std::cout << "Setting screen distance to " << val << std::endl;
					ds->setScreenDistance(val);
				}
				break;

			case OSG_3D_FUSION_DISTANCE:
				{
					osgUtil::SceneView::FusionDistanceMode mode = sv->getFusionDistanceMode();
					float val               = sv->getFusionDistanceValue();
					if(increment)  val+=0.1;
					else           val-=0.1;
					std::cout << "Setting fusion mode to " << (int) mode << " and distance to " << val << std::endl;
					sv->setFusionDistance(mode, val);
				}
				break;
		};

		sv->setDisplaySettings(ds);
	}
}

// ================================================
// reportSDLError
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderCameraOSGNode::reportSDLError()
{
	fprintf( stderr, "ERROR - Could not initialize SDL.  SDL produced the "
		"following error message: \n\t%s\n", SDL_GetError() );
	
	fprintf( stderr, "\nThe SDL error messages are often cryptic.  Here are some "
		"errors \nyou might run into, and the likely cause for each:\n" );
	fprintf( stderr, 
		"  * \"Couldn't find matching GLX visual\" - This usually indicates that your \n"
		"    graphics card can't provide a feature that the MPV requires.  The MPV \n"
		"    requires 24 bpp, and double-buffering.  You'll get this error if, for \n"
		"    example, you're running your card at 16 bpp, or if it can't do \n"
		"    double-buffering.  Run glxinfo to see the capabilities of your card.\n" );
	fprintf( stderr, 
		"  * \"No available video device\" - There are many potential causes for \n"
		"    this error.  \n"
		"      - You will see this error if you are running the MPV on one computer, \n"
		"        and displaying its output on another (this is a feature of X11, called \n"
		"        \"remote X11\").  There are MANY ways in which this can go wrong \n"
		"        (incorrect xhost settings, bad DISPLAY variable setting, broken ssh \n"
		"        forwarding, misconfigured X server, etc).  You can narrow down the \n"
		"        problem by running a X application such as xterm; if xterm doesn't \n"
		"        work, then the MPV definitely won't work.  In any case, you shouldn't \n"
		"        be running the MPV remotely.  The performance will be terrible.\n"
		"      - You will see this error if you run the X server as one user, but run \n"
		"        the MPV as another.  Don't do that.  Again, if xterm doesn't work, \n"
		"        then the MPV definitely won't work.\n" );
	fprintf( stderr, 
		"  * Other error messages: if you don't understand your error message, and it \n"
		"    doesn't appear in the list above, then there are a few things you must try \n"
		"    before asking for help.  You need to run some simple GL applications, like \n"
		"    glxgears and osgviewer.  IF GLXGEARS AND OSGVIEWER DO NOT WORK, THEN THE \n"
		"    MPV DEFINITELY WILL NOT WORK.  If these utilities do not work, it means \n"
		"    that your problem is not an MPV issue; it is a problem with your system.\n" );
}

