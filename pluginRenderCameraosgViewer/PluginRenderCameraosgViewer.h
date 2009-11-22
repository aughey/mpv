#ifndef PLUGIN_RENDER_CAMERA_OSGVIEWER_H
#define PLUGIN_RENDER_CAMERA_OSGVIEWER_H

#include <osgViewer/Viewer>

#include "Plugin.h"
#include "Entity.h"
#include "View.h"
#include "TrackerParams.h"
#include "DefFileGroup.h"


//=========================================================
//! This plugin is responsible for creating a rendering window and 
//! rendering the scene.  
//! It uses SDL to create a rendering window and OSG to render the scene.
//! 
class PluginRenderCameraosgViewer : public Plugin 
{
public:

	//=========================================================
	//! General Constructor
	//! 
	PluginRenderCameraosgViewer();

	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginRenderCameraosgViewer() throw();
	
	//=========================================================
	//! The per-frame processing that this plugin performs.
	//! This plugin's act() will update the camera and render the scene.
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );
	
private:
  osg::ref_ptr<osgViewer::Viewer> viewer;
  osg::Node *scene;
};



#endif

