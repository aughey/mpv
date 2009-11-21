/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2008 The Boeing Company
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
 *  MODIFICATION NOTES:
 *  
 *  2008-02-01 Andrew Sampson
 *      Moved some code out of PluginRenderCameraOSG and into a new class, 
 *      ViewportWrapper
 *
 *  2008-06-29 Andrew Sampson
 *      Made ViewportWrapper a child class of ViewImp
 *
 * </pre>
 */


#include <iostream>

#include <osg/FrontFace>

#include "BindSlot.h"

#include "SymbolSurfaceImpOSG.h"

#include "ViewportWrapper.h"

using namespace mpv;
using namespace mpvosg;

// ================================================
// ViewportWrapper
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ViewportWrapper::ViewportWrapper( View *v ) : ViewImp( v )
{
	projectionChanged = true;
	
	viewportNode = new osg::Group;

	viewport = new osgSDL::Viewport( viewportNode.get() );

	osgUtil::SceneView *sceneView = viewport->getSceneView();

	sceneView->setProjectionMatrixAsPerspective(45., 1.333333333, 1., 1000. );

	// a black background is needed for the star/skydome compositing, 
	// and seems more appropriate for splash screens
	sceneView->setClearColor( osg::Vec4( 0.0, 0.0, 0.0, 1.0 ) );

	// This will turn off osg's automatic near/far plane update
	sceneView->setComputeNearFarMode( osgUtil::CullVisitor::DO_NOT_COMPUTE_NEAR_FAR );

	// retrieve the view state and apply it
	typeChanged( view );
	parallelProjectionChanged( view );
	mirrorModeChanged( view );
	fovChanged( view );
	nearFarPlanesChanged( view );
	groupIDChanged( view );
	viewOffsetChanged( view );
	viewRotateChanged( view );
	entityIDChanged( view );
	viewportChanged( view );

	// set up slots for change notification
	view->typeChanged.connect( BIND_SLOT1( ViewportWrapper::typeChanged, this ) );
	view->parallelProjectionChanged.connect( BIND_SLOT1( ViewportWrapper::parallelProjectionChanged, this ) );
	view->mirrorModeChanged.connect( BIND_SLOT1( ViewportWrapper::mirrorModeChanged, this ) );
	view->fovChanged.connect( BIND_SLOT1( ViewportWrapper::fovChanged, this ) );
	view->nearFarPlanesChanged.connect( BIND_SLOT1( ViewportWrapper::nearFarPlanesChanged, this ) );
	view->groupIDChanged.connect( BIND_SLOT1( ViewportWrapper::groupIDChanged, this ) );
	view->viewOffsetChanged.connect( BIND_SLOT1( ViewportWrapper::viewOffsetChanged, this ) );
	view->viewRotateChanged.connect( BIND_SLOT1( ViewportWrapper::viewRotateChanged, this ) );
	view->entityIDChanged.connect( BIND_SLOT1( ViewportWrapper::entityIDChanged, this ) );
	view->viewportChanged.connect( BIND_SLOT1( ViewportWrapper::viewportChanged, this ) );

	// check for symbol surfaces already attached to the view and add them
	SymbolSurfaceContainer::SymbolSurfaceIteratorPair iterPair = view->getSymbolSurfaces();
	SymbolSurfaceContainer::SymbolSurfaceMap::iterator iter = iterPair.first;
	for( ; iter != iterPair.second; iter++ )
	{
		addedSymbolSurface( view, iter->second.get() );
	}

	// arrange notification of symbol surface creation/destruction
	view->addedSymbolSurface.connect( BIND_SLOT2( ViewportWrapper::addedSymbolSurface, this ) );
	view->removedSymbolSurface.connect( BIND_SLOT2( ViewportWrapper::removedSymbolSurface, this ) );
}


// ================================================
// ~ViewportWrapper
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ViewportWrapper::~ViewportWrapper()
{
	
}


// ================================================
// setSceneGraphRoot
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ViewportWrapper::setSceneGraphRoot( osg::Group *rootNode )
{
	viewportNode->addChild( rootNode );
}


// ================================================
// update
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ViewportWrapper::update( const osg::Matrix &viewMatrix )
{
	if( view == NULL )
	{
		std::cerr << "Error - in ViewportWrapper::update, view is NULL\n";
		return;
	}

	// check to see if the camera projection has been changed and, if so, 
	// recalculate the projection matrix.
	if( projectionChanged )
	{
		recalculateProjection();
		projectionChanged = false;
	}

	osgUtil::SceneView *sceneView = viewport->getSceneView();
	sceneView->setViewMatrix( viewMatrix );
}


// ================================================
// 
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ViewportWrapper::typeChanged( View *v )
{
	
}


// ================================================
// 
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ViewportWrapper::parallelProjectionChanged( View *v )
{
	projectionChanged = true;
}


// ================================================
// 
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ViewportWrapper::mirrorModeChanged( View *v )
{
	projectionChanged = true;
}


// ================================================
// 
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ViewportWrapper::fovChanged( View *v )
{
	projectionChanged = true;
}


// ================================================
// 
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ViewportWrapper::nearFarPlanesChanged( View *v )
{
	projectionChanged = true;
}


// ================================================
// 
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ViewportWrapper::groupIDChanged( View *v )
{
	
}


// ================================================
// 
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ViewportWrapper::viewOffsetChanged( View *v )
{
	
}


// ================================================
// 
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ViewportWrapper::viewRotateChanged( View *v )
{
	
}


// ================================================
// 
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ViewportWrapper::entityIDChanged( View *v )
{
	
}


// ================================================
// 
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ViewportWrapper::viewportChanged( View *v )
{
	// set up the viewport dimensions
	viewport->setLeft( view->getViewportLeft() );
	viewport->setTop( view->getViewportTop() );
	viewport->setWidth( view->getViewportWidth() );
	viewport->setHeight( view->getViewportHeight() );
}


// ================================================
// addedSymbolSurface
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ViewportWrapper::addedSymbolSurface( 
	SymbolSurfaceContainer *, SymbolSurface *surface )
{
	// retrieve nodes from symbol surface, attach them to the viewportNode
	
	// sanity check? container == view
	
	if( surface->getAttachState() != SymbolSurface::View )
	{
std::cerr << "Debug warning - in ViewportWrapper::addedSymbolSurface, symbol surface is not a view-attached surface\n";
		return;
	}
	
	SymbolSurfaceImpOSG *impOSG = SymbolSurfaceImpOSG::getSymbolSurfaceImpOSGFromSymbolSurface( surface );
	if( impOSG == NULL )
	{
		std::cerr << "Error - in ViewportWrapper::addedSymbolSurface, symbol surface \n"
			<< "has no implementation object.  It is not ready to be attached to the scene!\n";
		return;
	}
	
std::cerr << "ViewportWrapper::addedSymbolSurface - Adding a new view-attached symbol surface to the scene\n";
	if( !viewportNode->addChild( impOSG->getTopNode() ) )
	{
		std::cerr << "Error - in ViewportWrapper::addedSymbolSurface, symbol surface \n"
			<< "top node couldn't be added to scene!\n";
	}
}


// ================================================
// removedSymbolSurface
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ViewportWrapper::removedSymbolSurface( 
	SymbolSurfaceContainer *, SymbolSurface *surface )
{
	// retrieve nodes from symbol surface, remove them from the viewportNode
	
	// sanity check? container == view
	
	SymbolSurfaceImpOSG *impOSG = SymbolSurfaceImpOSG::getSymbolSurfaceImpOSGFromSymbolSurface( surface );
	if( impOSG == NULL )
	{
		std::cerr << "Error - in ViewportWrapper::removedSymbolSurface, symbol surface \n"
			<< "has no implementation object.  It can't be removed from the scene!\n";
		return;
	}
	
std::cerr << "ViewportWrapper::removedSymbolSurface - Removing a view-attached symbol surface from the scene\n";
	if( !viewportNode->removeChild( impOSG->getTopNode() ) )
	{
		std::cerr << "Error - in ViewportWrapper::removedSymbolSurface, symbol surface \n"
			<< "top node couldn't be removed from scene!\n";
	}
}


// ================================================
// recalculateProjection
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ViewportWrapper::recalculateProjection() 
{
	if( view == NULL )
	{
		std::cerr << "Error - in ViewportWrapper::recalculateProjection, view is NULL\n";
		return;
	}

	double left, right, bottom, top, nearPlane, farPlane, temp;
	
	osgUtil::SceneView *sceneView = viewport->getSceneView();
	
	nearPlane = view->getNearPlane();
	farPlane = view->getFarPlane();
	
	// a little bit of trig to calculate the corners of the 
	// near plane
	left = tan( osg::DegreesToRadians(view->getFovLeft()) ) * nearPlane;
	right = tan( osg::DegreesToRadians(view->getFovRight()) ) * nearPlane;
	bottom = tan( osg::DegreesToRadians(view->getFovBottom()) ) * nearPlane;
	top = tan( osg::DegreesToRadians(view->getFovTop()) ) * nearPlane;

	// Some of the mirrormodes cause the polygons in the scene to be 
	// "flipped around" in screen-space.  The back-face culling uses the 
	// screen-space vertex winding to determine if a poly should be culled.
	// To account for this, we will change the frontface GL setting from CCW 
	// to CW when rendering in some of the mirrormodes.
	osg::StateSet *sceneStateSet = viewportNode->getOrCreateStateSet();
	osg::FrontFace *ffStateAttr = 
		(osg::FrontFace *)sceneStateSet->getAttribute( osg::StateAttribute::FRONTFACE );
	if( !ffStateAttr ) 
	{
		ffStateAttr = new osg::FrontFace;
		sceneStateSet->setAttribute( ffStateAttr, osg::StateAttribute::ON );
	}

	switch( view->getMirrorMode() )
	{
	case 1: // horiz
		temp = left;
		left = right;
		right = temp;
		ffStateAttr->setMode( osg::FrontFace::CLOCKWISE );
		break;
	case 2: // vert
		temp = top;
		top = bottom;
		bottom = temp;
		ffStateAttr->setMode( osg::FrontFace::CLOCKWISE );
		break;
	case 3: // horiz and vert
		temp = left;
		left = right;
		right = temp;
		temp = top;
		top = bottom;
		bottom = temp;
		// fall through to default
	default:
		ffStateAttr->setMode( osg::FrontFace::COUNTER_CLOCKWISE );
		break;
	}
	
	if( view->getParallelProjection() )
	{
		sceneView->setProjectionMatrixAsOrtho( left, right, bottom, top, nearPlane, farPlane );
	}
	else
	{
		sceneView->setProjectionMatrixAsFrustum( left, right, bottom, top, nearPlane, farPlane );
	}
	
}




// ================================================
// StandbyViewportWrapper
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
StandbyViewportWrapper::StandbyViewportWrapper( View *v ) : 
	ViewportWrapper( v )
{
	osgUtil::SceneView *sceneView = viewport->getSceneView();
	sceneView->setLightingMode( osgUtil::SceneView::NO_SCENEVIEW_LIGHT );
}


// ================================================
// ~StandbyViewportWrapper
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
StandbyViewportWrapper::~StandbyViewportWrapper()
{
}

// ================================================
// setProjectionMatrix
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void StandbyViewportWrapper::setProjectionMatrix( const osg::Matrix &projMatrix )
{
	osgUtil::SceneView *sceneView = viewport->getSceneView();
	sceneView->setProjectionMatrix( projMatrix );
}


