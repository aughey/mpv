/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2008 The Boeing Company
 *  
 *  This component is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; specifically 
 *  version 2.1 of the License.
 *  
 *  This software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *  
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this software; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *  
 *  2008-01-17 Andrew Sampson
 *      Initial Release.
 * </pre>
 */

#ifndef SYMBOL_SURFACE_H
#define SYMBOL_SURFACE_H

#include "MPVCommonTypes.h"

#include <map>
#include <CigiBaseSymbolSurfaceDef.h>

#include "Referenced.h"
#include "Symbol.h"
#include "SymbolContainer.h"
#include "ComponentContainer.h"
#include "Vect2.h"
#include "Vect3.h"

namespace mpv
{

class SymbolSurfaceImp;

class MPVCMN_SPEC SymbolSurface : 
			public ComponentContainer,
			public SymbolContainer
{
public:
	enum SurfaceState
	{
		Active = CigiBaseSymbolSurfaceDef::Active,
		Destroyed = CigiBaseSymbolSurfaceDef::Destroyed
	};
	
	enum SurfaceAttachState
	{
		Entity = CigiBaseSymbolSurfaceDef::EntityAttached,
		View = CigiBaseSymbolSurfaceDef::ViewAttached
	};
	
	typedef std::list< RefPtr<SymbolSurfaceImp> > SymbolSurfaceImpList;
	typedef std::pair< SymbolSurfaceImpList::iterator, SymbolSurfaceImpList::iterator > SymbolSurfaceImpIteratorPair;
	
public:

	boost::signal<void (SymbolSurface*)> stateChanged;
	boost::signal<void (SymbolSurface*)> attachmentChanged;
	boost::signal<void (SymbolSurface*)> surfaceCoordinatesChanged;
	// entity-attached
	boost::signal<void (SymbolSurface*)> billboardingChanged;
	boost::signal<void (SymbolSurface*)> entityPositionChanged;
	boost::signal<void (SymbolSurface*)> entityOrientationChanged;
	boost::signal<void (SymbolSurface*)> entityDimensionsChanged;
	// view-attached
	boost::signal<void (SymbolSurface*)> viewPositionChanged;

	SymbolSurface();
	
	int getID() const { return id; }
	SurfaceState getState() const { return state; }
	SurfaceAttachState getAttachState() const { return attachment; }
	Vect2 getMinimumCoordinates() const { return minimumCoordinates; }
	Vect2 getMaximumCoordinates() const { return maximumCoordinates; }
	
	// entity-attached
	bool getIsBillboard() const { return isBillboard; }
	bool getIsPerspectiveEnabled() const { return isPerspectiveEnabled; }
	int getEntityID() const { return entityID; }
	Vect3 getPosition() const { return position; }
	float getYaw() const { return yaw; }
	float getPitch() const { return pitch; }
	float getRoll() const { return roll; }
	float getWidth() const { return width; }
	float getHeight() const { return height; }
	
	// view-attached
	int getViewID() const { return viewID; }
	float getViewPositionLeft() const { return viewPositionLeft; }
	float getViewPositionRight() const { return viewPositionRight; }
	float getViewPositionTop() const { return viewPositionTop; }
	float getViewPositionBottom() const { return viewPositionBottom; }
	
	
	void setID( int newID );
	void setState( SurfaceState newState );
	void setAttachState( SurfaceAttachState newAttachment );
	void setMinimumCoordinates( const Vect2 &newMinimumCoordinates );
	void setMaximumCoordinates( const Vect2 &newMaximumCoordinates );
	
	// entity-attached
	void setIsBillboard( bool newIsBillboard );
	void setIsPerspectiveEnabled( bool newIsPerspectiveEnabled );
	void setEntityID( int newEntityID );
	void setPosition( const Vect3 &newPosition );
	void setYaw( float newYaw );
	void setPitch( float newPitch );
	void setRoll( float newRoll );
	void setWidth( float newWidth );
	void setHeight( float newHeight );
	
	// view-attached
	void setViewID( int newViewID );
	void setViewPositionLeft( float newViewPositionLeft );
	void setViewPositionRight( float newViewPositionRight );
	void setViewPositionTop( float newViewPositionTop );
	void setViewPositionBottom( float newViewPositionBottom );
	
	void addImplementation( SymbolSurfaceImp *newImplementation );
	
	SymbolSurfaceImpIteratorPair getImplementations()
	{
		return SymbolSurfaceImpIteratorPair( imps.begin(), imps.end() );
	}
	
protected:

	virtual ~SymbolSurface();
	
	int id;
	SurfaceState state;
	SurfaceAttachState attachment;
	
	// entity-attached
	bool isBillboard;
	bool isPerspectiveEnabled;
	int entityID;
	Vect3 position;
	float yaw;
	float pitch;
	float roll;
	float width;
	float height;
	
	// view-attached
	int viewID;
	float viewPositionLeft;
	float viewPositionRight;
	float viewPositionTop;
	float viewPositionBottom;
	
	Vect2 minimumCoordinates;
	Vect2 maximumCoordinates;
	
	//=========================================================
	//! Implementation objects for this SymbolSurface.  
	//!
	SymbolSurfaceImpList imps;
	
};


class MPVCMN_SPEC SymbolSurfaceImp : public Referenced
{
public:
	SymbolSurfaceImp( SymbolSurface *newSurface );
	
protected:
	virtual ~SymbolSurfaceImp();

	SymbolSurface *surface;
};

}
#endif
