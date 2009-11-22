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

#include <osgGA/StateSetManipulator>

#include "PluginRenderCameraOSGNode.h"

using namespace mpv;

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
	dependencies_.push_back( "PluginViewMgr" );
	dependencies_.push_back( "PluginCameraMgrOSG" );

	rootNode = NULL;
	viewMap = NULL;
//	sendNetMessagesCB = NULL;
	cameraMatrixMap = NULL;
	
  std::cout << "PluginRenderCameraOSGNode created\n";
}


// ================================================
// ~PluginRenderCameraOSGNode
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderCameraOSGNode::~PluginRenderCameraOSGNode() throw() 
{


}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderCameraOSGNode::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{
	
	case SystemState::BlackboardRetrieve:
		// This state is for retrieving things from the blackboard
		
		// get the root node from the BB
		bb_->get( "RootNodeOSG", rootNode );

		bb_->get( "ViewMap", viewMap );

		// get the camera matrices
		bb_->get( "CameraMatrixMap", cameraMatrixMap );

    std::cout << "osgNode retrieved blackboard\n";
		
		break;


	case SystemState::ConfigurationProcess:
		break;
		
	case SystemState::Reset:
    setupViewports();
		break;

	case SystemState::Standby:
	case SystemState::DatabaseLoad:
	case SystemState::Operate:
	case SystemState::Debug:

		// render the scene or (if the state is Standby) 
		// render some splash screen goodness
		operate();
		break;
		
	case SystemState::Shutdown:
		break;
		
	default:
		break;
	}
}


// ================================================
// operate
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderCameraOSGNode::operate()
{
	updateViewports();
}



// ================================================
// setupViewports
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderCameraOSGNode::setupViewports()
{
  // If we've already setup our viewports, return.
  if(!viewportMap.empty()) {
    return;
  }

  assert(viewMap);

  std::cout << "osgNode setting up viewports " << viewMap->size() << std::endl;

	// for each of the CIGI view in viewMap, add a viewport
	std::map< int, RefPtr<View> >::iterator vpIter = viewMap->begin();
	for( ; vpIter != viewMap->end(); vpIter++ )
	{
		View *view = (*vpIter).second.get();
		
		ViewportWrapper *viewportWrapper = new ViewportWrapper( view );
		viewportWrapper->setSceneGraphRoot( rootNode );
		
		//viewer->addViewport( viewportWrapper->getViewport() );

		viewportMap[view->getID()] = viewportWrapper;

    std::cout << "Created a viewport for view id " << view->getID() << std::endl;
		
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

