/*

Class which encapsulates a 2d overlay.  Child classes include frame rate 
display, etc.

Copyright 2007 Andrew Sampson


*/

#ifndef OVERLAY_SCREEN_H
#define OVERLAY_SCREEN_H

#include <osg/Matrixf>
#include <osg/Node>

#include "View.h"

namespace mpvosg
{

class OverlayScreen
{
public:

	OverlayScreen() {}
	virtual ~OverlayScreen() {}
	
	virtual void resetView( const mpv::View *view ) = 0;
	virtual void act( double deltaT ) = 0;
	
	const osg::Matrixf &getProjectionMatrix() const { return projectionMatrix; }
	virtual osg::Node *getNode() = 0;
	
	
protected:

	osg::Matrixf projectionMatrix;

};

}

#endif
