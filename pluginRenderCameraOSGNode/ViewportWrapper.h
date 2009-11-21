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


#ifndef VIEWPORTWRAPPER_H
#define VIEWPORTWRAPPER_H


#include <osg/Matrix>
#include <osg/Group>
#include <osgUtil/SceneView>

#include "osgSDL/Viewport"

#include "View.h"

//=========================================================
//! A class to encapsulate a viewport.  This class was created to make it 
//! easier to manage per-view unique additions to the scene (like 
//! symbol surfaces).  It also makes the window-rendering plugin cleaner, and 
//! will probably encourage code re-use if I ever create a (non-SDL) 
//! window-rendering plugin.
//! 
class ViewportWrapper : public mpv::ViewImp
{
public:
	ViewportWrapper( mpv::View *v );

	void setSceneGraphRoot( osg::Group *rootNode );
// testing only - remove when done
osg::Group * getSceneGraph(){ return viewportNode.get(); }
	
	//=========================================================
	//! yuck... returns viewport
	//! \return viewport
	//!
	osgSDL::Viewport *getViewport() { return viewport.get(); }
	
	//=========================================================
	//! Calls recalculateProjection as needed, and updates the camera 
	//! transformation matrix.
	//! \param viewMatrix - a modelview matrix
	//! 
	virtual void update( const osg::Matrix &viewMatrix );
	
	void setEnabled( bool enabled ) { viewport->setEnabled( enabled ); }
	
	virtual void typeChanged( mpv::View *v );
	virtual void parallelProjectionChanged( mpv::View *v );
	virtual void mirrorModeChanged( mpv::View *v );
	virtual void fovChanged( mpv::View *v );
	virtual void nearFarPlanesChanged( mpv::View *v );
	virtual void groupIDChanged( mpv::View *v );
	virtual void viewOffsetChanged( mpv::View *v );
	virtual void viewRotateChanged( mpv::View *v );
	virtual void entityIDChanged( mpv::View *v );
	virtual void viewportChanged( mpv::View *v );

protected:
	virtual ~ViewportWrapper();

	void addedSymbolSurface( mpv::SymbolSurfaceContainer *, mpv::SymbolSurface *surface );
	void removedSymbolSurface( mpv::SymbolSurfaceContainer *, mpv::SymbolSurface *surface );
	
	//=========================================================
	//! Recalculates the camera's projection matrix
	//! 
	void recalculateProjection();

	osg::ref_ptr<osgSDL::Viewport> viewport;
	osg::ref_ptr<osg::Group> viewportNode;
	
	//! Indicates if the projection has changed; the various signal handlers 
	//! will set this to true if there have been any changes to the view 
	//! frustum.
	bool projectionChanged;
};



//=========================================================
//! This class differs from its parent ViewportWrapper only in that it 
//! allows a projection matrix to be specified directly.  (Ordinarily, the 
//! projection matrix is calculated based on the View parameters.)  
//! There are also some minor differences in how the scene lighting is 
//! set up for this standby viewport.
//! 
class StandbyViewportWrapper : public ViewportWrapper
{
public:
	StandbyViewportWrapper( mpv::View *v );

	void setProjectionMatrix( const osg::Matrix &projMatrix );

	//=========================================================
	//! A stubbed-out replacement for ViewportWrapper::update
	//! \param viewMatrix - a modelview matrix (unused)
	//! 
	virtual void update( const osg::Matrix &viewMatrix ) { }
	
protected:
	virtual ~StandbyViewportWrapper();

};

#endif
