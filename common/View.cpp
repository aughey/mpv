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
 *  FILENAME:   ViewParams.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  These classes encapsulate the properties associated with CIGI "views" 
 *  and "view groups".
 *  
 *  
 *  2008-01-25 Andrew Sampson
 *      Renamed ViewParams to View, and made View a proper class.  Moved 
 *      code out of PluginViewMgr and into the View class.
 *  
 * </pre>
 */

#include <iostream>

#include "View.h"

using namespace mpv;


View::View() : SymbolSurfaceContainer(),
	id( 0xff ),
	type( 0xff ),
	parallelProjection( false ),
	mirrorMode( 0 ),
	fovLeft( 0.0 ),
	fovRight( 0.0 ),
	fovTop( 0.0 ),
	fovBottom( 0.0 ),
	nearPlane( 0.0 ),
	farPlane( 0.0 ),
	groupID( 0xff ),
	entityID( 0xffff ),
	screenSizeHoriz( 0 ),
	screenSizeVert( 0 ),
	fullscreen( false ),
	viewportLeft( 0.0 ),
	viewportTop( 0.0 ),
	viewportWidth( 0.0 ),
	viewportHeight( 0.0 )
{
	errorFlags = 0;
	skipMinimumNearPlaneCheck = false;
	initialize();
}

View::~View()
{
	
}



// ================================================
// initialize
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void View::initialize( int paramFlags ) 
{
	
	if( paramFlags & ID_PARAMETERS )
	{
		setID( 0 );
	}

	if( paramFlags & TYPE_PARAMETERS )
	{
		setType( 0 );
	}

	if( paramFlags & PROJECTION_PARAMETERS )
	{
		setParallelProjection( false );
		setMirrorMode( 0 );
		setFov( -28., 28., 22.5, -22.5 );
	}

	if( paramFlags & NEAR_FAR_PLANE_PARAMETERS )
	{
		setNearFarPlanes( 3., 10000. );
	}

	if( paramFlags & TRANSFORM_PARAMETERS )
	{
		setGroupID( 0 );
		setViewOffset( Vect3( 0,0,0 ) );
		setViewRotate( Vect3( 0,0,0 ) );
		setEntityID( 0 );
	}

	if( paramFlags & RESOLUTION_PARAMETERS )
	{
		screenSizeHoriz = 800;
		screenSizeVert = 600;
		fullscreen = false;
	}

	if( paramFlags & VIEWPORT_PARAMETERS )
	{
		setViewport( 0., 0., 1., 1. );
	}
}


void View::setID( int newID )
{
	id = newID;

	if( id < 0 )
	{
		initialize( ID_PARAMETERS );
		printViewParamError( ID_PARAMETERS );
	}
}


void View::setType( int newType )
{
	if( type != newType )
	{
		type = newType;
		typeChanged( this );
	}
}


void View::setParallelProjection( bool newParallelProjection )
{
	if( parallelProjection != newParallelProjection )
	{
		parallelProjection = newParallelProjection;
		parallelProjectionChanged( this );
	}
}


void View::setMirrorMode( int newMirrorMode )
{
	if( mirrorMode != newMirrorMode )
	{
		mirrorMode = newMirrorMode;
		mirrorModeChanged( this );
	}
}


void View::setFov( 
	float newFovLeft, 
	float newFovRight, 
	float newFovTop, 
	float newFovBottom )
{
	bool changed = false;

	if( fovLeft != newFovLeft )
	{
		fovLeft = newFovLeft;
		changed = true;
	}
	if( fovRight != newFovRight )
	{
		fovRight = newFovRight;
		changed = true;
	}
	if( fovTop != newFovTop )
	{
		fovTop = newFovTop;
		changed = true;
	}
	if( fovBottom != newFovBottom )
	{
		fovBottom = newFovBottom;
		changed = true;
	}

	// check the order of the frustum edges, and make sure 
	// that a nonzero view angle is specified
	if( (fovRight - fovLeft) < 0.01 || 
		(fovTop - fovBottom) < 0.01 )
	{
		initialize( PROJECTION_PARAMETERS );
		printViewParamError( PROJECTION_PARAMETERS );
	}
	else if( changed )
	{
		// initialize() will call this method, so only emit the signal 
		// if there was no error in setting the values
		fovChanged( this );
	}
}


void View::setNearFarPlanes( float newNearPlane, float newFarPlane )
{
	bool changed = false;

	if( nearPlane != newNearPlane )
	{
		nearPlane = newNearPlane;
		changed = true;
	}
	if( farPlane != newFarPlane )
	{
		farPlane = newFarPlane;
		changed = true;
	}

	// Sanity-check the order of the planes.
	// Z-fighting increases as nearplane approaches 0.  Let's avoid that, 
	// unless the user is overriding the check.
	if( farPlane < nearPlane || nearPlane < 0.0 || 
		( nearPlane < 1.0 && !skipMinimumNearPlaneCheck ) )
	{
		initialize( NEAR_FAR_PLANE_PARAMETERS );
		printViewParamError( NEAR_FAR_PLANE_PARAMETERS );
	}
	else if( changed )
	{
		// initialize() will call this method, so only emit the signal 
		// if there was no error in setting the values
		nearFarPlanesChanged( this );
	}
}


void View::setGroupID( int newGroupID )
{
	if( groupID != newGroupID )
	{
		groupID = newGroupID;
		groupIDChanged( this );
	}
}


void View::setViewOffset( const Vect3 &newViewOffset )
{
//	if( viewOffset != newViewOffset )
	{
		viewOffset = newViewOffset;
		viewOffsetChanged( this );
	}
}


void View::setViewRotate( const Vect3 &newViewRotate )
{
//	if( viewRotate != newViewRotate )
	{
		viewRotate = newViewRotate;
		viewRotateChanged( this );
	}
}


void View::setEntityID( int newEntityID )
{
	if( entityID != newEntityID )
	{
		entityID = newEntityID;
		entityIDChanged( this );
	}
}


void View::setViewport( 
	float newViewportLeft, float newViewportTop,
	float newViewportWidth, float newViewportHeight )
{
	viewportLeft = newViewportLeft;
	viewportTop = newViewportTop;
	viewportWidth = newViewportWidth;
	viewportHeight = newViewportHeight;
	viewportChanged( this );
}


void View::setSkipMinimumNearPlaneCheck( bool skip )
{
	skipMinimumNearPlaneCheck = skip;
}


// ================================================
// printViewParamError
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void View::printViewParamError( int errorID )
{
	if( errorFlags & errorID )
		// already printed this error; bail
		return;
	
	switch( errorID )
	{
	case ID_PARAMETERS:
		std::cerr << "Error - An attempt was made (through the config files)\n"
			<< "\tto set a view's id to an invalid value.\n";
		break;
	case TYPE_PARAMETERS:
		std::cerr << "Error - An attempt was made (either by the Host or through the config files)\n"
			<< "\tto set a view's type to an invalid value.\n"
			<< "\tThe view's type has been reset to a valid value.\n";
		break;
	case PROJECTION_PARAMETERS:
		std::cerr << "Error - An attempt was made (either by the Host or through the config files)\n"
			<< "\tto set a view's projection parameters to invalid or nonsensical values.\n"
			<< "\tThe view's projection has been reset to hard-coded defaults.\n";
		break;
	case NEAR_FAR_PLANE_PARAMETERS:
		std::cerr << "Error - An attempt was made (either by the Host or through the config files)\n"
			<< "\tto set a view's near and far clipping distances to invalid or \n"
			<< "\tnonsensical values.  Note that the far clipping plane must be \n"
			<< "\tfarther away than the near clipping plane, and that the minimum \n"
			<< "\tvalue for the near clipping plane is 1.0.\n"
			<< "\tThe view's near/far planes have been reset to hard-coded defaults.\n";
		break;
	case TRANSFORM_PARAMETERS:
		std::cerr << "Error - An attempt was made (either by the Host or through the config files)\n"
			<< "\tto set a view's transform parameters (offset, rotation, target \n"
			<< "\tentity, etc) to invalid values.\n";
		break;
	case RESOLUTION_PARAMETERS:
		std::cerr << "Error - An attempt was made (through the config files)\n"
			<< "\tto set a view's resolution to invalid values.\n"
			<< "\tThe view's resolution has been reset to hard-coded defaults.\n";
		break;
	case VIEWPORT_PARAMETERS:
		std::cerr << "Error - An attempt was made (through the config files)\n"
			<< "\tto set a view's viewport (its on-screen position within the GL \n"
			<< "\tcontext) to invalid values.\n";
		break;
	default:
		break;
	}

	std::cerr << "\tNote that this error will only be reported once.\n\n";
	
	errorFlags = errorFlags | errorID;
}

