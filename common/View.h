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
 *  04/06/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 *  
 *  2008-01-25 Andrew Sampson
 *      Renamed ViewParams to View, and made View a proper class.  Moved 
 *      code out of PluginViewMgr and into the View class.
 *  
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#ifndef VIEW_H
#define VIEW_H


#include "MPVCommonTypes.h"
#include "Vect3.h"
#include "SymbolSurfaceContainer.h"


namespace mpv
{

// forward declaration; defined at bottom of file
class ViewImp;

//=========================================================
//! This class encapsulates the properties associated with an individial view, 
//! such as the field-of-view, the view offset and rotation, etc.
//! 
class MPVCMN_SPEC View : public SymbolSurfaceContainer
{
public:
	
	//=========================================================
	//! This enum is used as input to the view initialization and 
	//! error-message methods
	//! 
	enum ViewParameterTypes
	{
		ID_PARAMETERS = 0x01,
		TYPE_PARAMETERS = 0x02,
		PROJECTION_PARAMETERS = 0x04,
		NEAR_FAR_PLANE_PARAMETERS = 0x08,
		TRANSFORM_PARAMETERS = 0x10,
		RESOLUTION_PARAMETERS = 0x20,
		VIEWPORT_PARAMETERS = 0x40,
		ALL_PARAMETERS = 0xFFFFFFFF
	};
	
	
public:
	
	boost::signal<void (View*)> typeChanged;
	boost::signal<void (View*)> parallelProjectionChanged;
	boost::signal<void (View*)> mirrorModeChanged;
	boost::signal<void (View*)> fovChanged;
	boost::signal<void (View*)> nearFarPlanesChanged;
	boost::signal<void (View*)> groupIDChanged;
	boost::signal<void (View*)> viewOffsetChanged;
	boost::signal<void (View*)> viewRotateChanged;
	boost::signal<void (View*)> entityIDChanged;
	boost::signal<void (View*)> viewportChanged;

	View();
	
	//=========================================================
	//! Initializes the values in a view.
	//! \param paramFlags - a bitfield indicating which parameters to 
	//!    initialize; defaults to all parameters
	//! 
	void initialize( int paramFlags = (ALL_PARAMETERS & ~ID_PARAMETERS) );
	
	int getID() { return id; }
	int getType() { return type; }
	bool getParallelProjection() { return parallelProjection; }
	int getMirrorMode() { return mirrorMode; }
	float getFovLeft() { return fovLeft; }
	float getFovRight() { return fovRight; }
	float getFovTop() { return fovTop; }
	float getFovBottom() { return fovBottom; }
	float getNearPlane() { return nearPlane; }
	float getFarPlane() { return farPlane; }
	int getGroupID() { return groupID; }
	Vect3 getViewOffset() { return viewOffset; }
	Vect3 getViewRotate() { return viewRotate; }
	int getEntityID() { return entityID; }
	float getViewportLeft() { return viewportLeft; }
	float getViewportTop() { return viewportTop; }
	float getViewportWidth() { return viewportWidth; }
	float getViewportHeight() { return viewportHeight; }

	void setID( int newID );
	void setType( int newType );
	void setParallelProjection( bool newParallelProjection );
	void setMirrorMode( int newMirrorMode );
	void setFov( 
		float newFovLeft, 
		float newFovRight, 
		float newFovTop, 
		float newFovBottom );
	void setNearFarPlanes( float newNearPlane, float newFarPlane );
	void setGroupID( int newGroupID );
	void setViewOffset( const Vect3 &newViewOffset );
	void setViewRotate( const Vect3 &newViewRotate );
	void setEntityID( int newEntityID );
	void setViewport( float newViewportLeft,
	                  float newViewportTop,
	                  float newViewportWidth,
	                  float newViewportHeight );
	void setSkipMinimumNearPlaneCheck( bool skip );

	void setImplementation( ViewImp *newImp ) { imp = newImp; }

	ViewImp *getImplementation() { return imp.get(); }

protected:

	// which view ID we should use
	int id;
	
	// OTW versus IR sensor versus...
	int type;
	
	// set to true to switch to parallel (orthographic) projection
	bool parallelProjection;
	
	// indicates that the view should be mirrored horizontally or vertically
	int mirrorMode;
	
	// angles for the view frustum
	float fovLeft;
	float fovRight;
	float fovTop;
	float fovBottom;
	
	// clipping planes
	float nearPlane;
	float farPlane;
	
	// The view group that the view belongs to.
	// Zero indicates that the view should not use any group's 
	// groupOffset or groupRotate.
	int groupID;
	
	// Offset vectors for the view, relative to the entity (see next).
	// These values are passed in from the Host via CIGI, and are initialized 
	// in the config files (see the view manager plugin).
	Vect3 viewOffset;
	Vect3 viewRotate; // yaw pitch roll, degrees
	
	// the ID of entity we're following around
	int entityID;
	
	// The resolution for the view.  If fullscreen is enabled, the screen will 
	// be switched to this resolution.  If fullscreen is disabled, this will 
	// determine the dimensions of the GL window.
	int screenSizeHoriz;
	int screenSizeVert;
	bool fullscreen;
	
	// The dimensions of the viewport.  Defaults to 0,0,1,1.  
	float viewportLeft;
	float viewportTop;
	float viewportWidth;
	float viewportHeight;

	//=========================================================
	//! A bitfield indicating whether one of the sanity-check error messages 
	//! has been printed already (this prevents the mpv from spamming the 
	//! console if the Host sends invalid view parameters every frame)
	//! 
	int errorFlags;

	//=========================================================
	//! If this is set to true this View will skip all validation
	//! of the minimum distance for the near clipping plane.  The
	//! user enables this option at their own risk.
	//! 
	bool skipMinimumNearPlaneCheck;

	RefPtr<ViewImp> imp;
	
protected:

	virtual ~View();

	//=========================================================
	//! Prints an error message; called if a sanity-check fails 
	//! for one of the view parameters
	//! \param errorID - one of enum ViewParameterTypes; indicates the 
	//!    type of view parameter that failed the sanity check
	//! 
	void printViewParamError( int errorID );
};


//=========================================================
//! 
//! 
class MPVCMN_SPEC ViewImp : public Referenced
{

public:
	
	ViewImp( View *_view ) : 
		Referenced(), 
		view( _view )
	{}
	

protected:
	
	virtual ~ViewImp() {}
	
	View *view;
};


//=========================================================
//! This struct contains the properties associated with a view group.
//! 
struct ViewGroupParams
{

	float groupOffset[3];
	float groupRotate[3]; // yaw pitch roll

	// the ID of entity we're following around
	int entityID;
};

}
#endif
