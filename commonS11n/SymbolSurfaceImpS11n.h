/** <pre>
 *  MPV Serialization Framework
 *  Copyright (c) 2008 The Boeing Company
 *  Copyright (c) 2009 Andrew Sampson
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
 *  2009-09-06 Andrew Sampson
 *      Copied file from commonOSG to commonS11n.  Adapted for use in S11n 
 *      subsystem.
 *
 * </pre>
 */

#ifndef SYMBOL_SURFACE_IMP_S11N_H
#define SYMBOL_SURFACE_IMP_S11N_H

#include "SymbolSurface.h"
#include "Symbol.h"

#include "DoubleVectorNode.h"
#include "FloatNode.h"
#include "GroupNode.h"
#include "IntNode.h"
#include "StringNode.h"
#include "SymbolImpS11n.h"


namespace mpvs11n
{

//=========================================================
//! 
//! 
class SymbolSurfaceImpS11n : public mpv::SymbolSurfaceImp
{
public:
	
	//=========================================================
	//! Constructor
	//! 
	SymbolSurfaceImpS11n( mpv::SymbolSurface *newSurface );

	//=========================================================
	//! Returns the top node in the surface subgraph.
	//! \return the top node in the surface, suitable for attaching to 
	//!         the serialization graph
	mpvs11n::Node *getTopNode() const
	{
		return topGroup.get();
	}
	
	static SymbolSurfaceImpS11n *getSymbolSurfaceImpS11nFromSymbolSurface( mpv::SymbolSurface *surface );
	
protected:

	//=========================================================
	//! Destructor
	//! 
	virtual ~SymbolSurfaceImpS11n();

	void stateChanged( mpv::SymbolSurface *surface );
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
	//! The top node in the surface, suitable for attaching to the serialization graph
	//! 
	mpv::RefPtr<mpvs11n::GroupNode> topGroup;

	mpv::RefPtr<mpvs11n::IntNode> id;
	mpv::RefPtr<mpvs11n::StringNode> state;
	mpv::RefPtr<mpvs11n::StringNode> attachment;
	mpv::RefPtr<mpvs11n::FloatNode> minimumU;
	mpv::RefPtr<mpvs11n::FloatNode> minimumV;
	mpv::RefPtr<mpvs11n::FloatNode> maximumU;
	mpv::RefPtr<mpvs11n::FloatNode> maximumV;
	
	// entity-attached
	mpv::RefPtr<mpvs11n::IntNode> isBillboard;
	mpv::RefPtr<mpvs11n::IntNode> isPerspectiveEnabled;
	mpv::RefPtr<mpvs11n::IntNode> entityID;
	mpv::RefPtr<mpvs11n::DoubleVectorNode> position;
	mpv::RefPtr<mpvs11n::FloatNode> yaw;
	mpv::RefPtr<mpvs11n::FloatNode> pitch;
	mpv::RefPtr<mpvs11n::FloatNode> roll;
	mpv::RefPtr<mpvs11n::FloatNode> width;
	mpv::RefPtr<mpvs11n::FloatNode> height;
	
	// view-attached
	mpv::RefPtr<mpvs11n::IntNode> viewID;
	mpv::RefPtr<mpvs11n::FloatNode> viewPositionLeft;
	mpv::RefPtr<mpvs11n::FloatNode> viewPositionRight;
	mpv::RefPtr<mpvs11n::FloatNode> viewPositionTop;
	mpv::RefPtr<mpvs11n::FloatNode> viewPositionBottom;
	
	//=========================================================
	//! Attachment point for symbols
	//! 
	mpv::RefPtr<mpvs11n::GroupNode> symbolsGroup;
	
};

}

#endif
