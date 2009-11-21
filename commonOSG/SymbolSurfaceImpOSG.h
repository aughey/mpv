/** <pre>
 *  The MPV Symbology Plugin Collection
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
 *  
 *  2008-01-21 Andrew Sampson, Boeing
 *      Initial release
 *
 * </pre>
 */

#ifndef SYMBOL_SURFACE_IMP_OSG_H
#define SYMBOL_SURFACE_IMP_OSG_H

#include <osg/MatrixTransform>
#include <osg/Projection>

#include "SymbolSurface.h"
#include "Symbol.h"
#include "SymbolImpOSG.h"
#include "MPVCommonOSGTypes.h"


namespace mpvosg
{

//=========================================================
//! An OSG-specific implementation of SymbolSurfaceImp.  Contains various 
//! transforms, and an attachment point for symbols.
//! 
class MPVCMNOSG_SPEC SymbolSurfaceImpOSG : public mpv::SymbolSurfaceImp
{
public:
	
	//=========================================================
	//! Constructor
	//! 
	SymbolSurfaceImpOSG( mpv::SymbolSurface *newSurface );

	//=========================================================
	//! Returns the top node in the surface subgraph.
	//! \return the top node in the surface, suitable for attaching to 
	//!         the scene graph
	osg::Node *getTopNode() const
	{
		return topNode.get();
	}
	
	static SymbolSurfaceImpOSG *getSymbolSurfaceImpOSGFromSymbolSurface( mpv::SymbolSurface *surface );
	
protected:

	//=========================================================
	//! Destructor
	//! 
	virtual ~SymbolSurfaceImpOSG();

	void attachmentChanged( mpv::SymbolSurface *surface );
	void surfaceCoordinatesChanged( mpv::SymbolSurface *surface );
	void billboardingChanged( mpv::SymbolSurface *surface );
	void entityPositionChanged( mpv::SymbolSurface *surface );
	void entityOrientationChanged( mpv::SymbolSurface *surface );
	void entityDimensionsChanged( mpv::SymbolSurface *surface );
	void viewPositionChanged( mpv::SymbolSurface *surface );

	//=========================================================
	//! Event notification, called when a symbol is added to this surface.
	//! \param symbol - symbol that was added
	void symbolAdded( mpv::SymbolContainer *, mpv::Symbol *symbol );

	//=========================================================
	//! Event notification, called when a symbol is removed from this surface.
	//! \param symbol - symbol that was removed
	void symbolRemoved( mpv::SymbolContainer *, mpv::Symbol *symbol );

	void setupEntityAttachedSurface();
	void updateEntityAttachedTransform();
	void setupViewAttachedSurface();
	void updateViewAttachedTransform();

	//=========================================================
	//! Convenience variable
	//! 
	static osg::Vec3 xAxis;
	//=========================================================
	//! Convenience variable
	//! 
	static osg::Vec3 yAxis;
	//=========================================================
	//! Convenience variable
	//! 
	static osg::Vec3 zAxis;

	//=========================================================
	//! The top node in the surface, suitable for attaching to the scene graph
	//! 
	osg::ref_ptr<osg::Node> topNode;

	//=========================================================
	//! A transform to position the surface (relative to its attachment point)
	//! and scale the surface such that the transform maps its attachment-space 
	//! to symbol-space.  
	//! Also, the top-level symbols in this surface are attached to this node.
	//! 
	osg::ref_ptr<osg::MatrixTransform> transform;
	

	// ENTITY-ATTACHED
	

	// VIEW-ATTACHED
	
	//=========================================================
	//! A projection node, to set up the correct view volume for this surface.
	//! This is also the top node in this surface's subgraph.
	osg::ref_ptr<osg::Projection> projection;
	
};

}

#endif
