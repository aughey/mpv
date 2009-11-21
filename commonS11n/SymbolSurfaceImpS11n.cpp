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

#include <iostream>

#include "BindSlot.h"
#include "SymbolSurfaceImpS11n.h"


using namespace mpv;
using namespace mpvs11n;


SymbolSurfaceImpS11n::SymbolSurfaceImpS11n( SymbolSurface *newSurface ) : 
	SymbolSurfaceImp( newSurface )
{
	topGroup = new mpvs11n::GroupNode( "symbol_surface" );
	
	id = new mpvs11n::IntNode( "id" );
	id->set( surface->getID() );
	state = new mpvs11n::StringNode( "state" );
	attachment = new mpvs11n::StringNode( "attachment" );
	minimumU = new mpvs11n::FloatNode( "minimum_u" );
	minimumV = new mpvs11n::FloatNode( "minimum_v" );
	maximumU = new mpvs11n::FloatNode( "maximum_u" );
	maximumV = new mpvs11n::FloatNode( "maximum_v" );
	isBillboard = new mpvs11n::IntNode( "is_billboard" );
	isPerspectiveEnabled = new mpvs11n::IntNode( "is_perspective_enabled" );
	entityID = new mpvs11n::IntNode( "entity_id" );
	position = new mpvs11n::DoubleVectorNode( "position" );
	yaw = new mpvs11n::FloatNode( "yaw" );
	pitch = new mpvs11n::FloatNode( "pitch" );
	roll = new mpvs11n::FloatNode( "roll" );
	width = new mpvs11n::FloatNode( "width" );
	height = new mpvs11n::FloatNode( "height" );
	viewID = new mpvs11n::IntNode( "view_id" );
	viewPositionLeft = new mpvs11n::FloatNode( "view_position_left" );
	viewPositionRight = new mpvs11n::FloatNode( "view_position_right" );
	viewPositionTop = new mpvs11n::FloatNode( "view_position_top" );
	viewPositionBottom = new mpvs11n::FloatNode( "view_position_bottom" );
	symbolsGroup = new mpvs11n::GroupNode( "symbols" );
	
	topGroup->addChild( id.get() );
	topGroup->addChild( state.get() );
	topGroup->addChild( attachment.get() );
	topGroup->addChild( minimumU.get() );
	topGroup->addChild( minimumV.get() );
	topGroup->addChild( maximumU.get() );
	topGroup->addChild( maximumV.get() );
	topGroup->addChild( symbolsGroup.get() );
	// Note - view-attached-specific and entity-attached-specific nodes are 
	// attached to/detached from topGroup in attachmentChanged().
	
	stateChanged( surface );
	attachmentChanged( surface );
	surfaceCoordinatesChanged( surface );
	billboardingChanged( surface );
	entityPositionChanged( surface );
	entityOrientationChanged( surface );
	entityDimensionsChanged( surface );
	viewPositionChanged( surface );

	surface->stateChanged.connect( BIND_SLOT1( SymbolSurfaceImpS11n::stateChanged, this ) );
	surface->attachmentChanged.connect( BIND_SLOT1( SymbolSurfaceImpS11n::attachmentChanged, this ) );
	surface->surfaceCoordinatesChanged.connect( BIND_SLOT1( SymbolSurfaceImpS11n::surfaceCoordinatesChanged, this ) );
	surface->billboardingChanged.connect( BIND_SLOT1( SymbolSurfaceImpS11n::billboardingChanged, this ) );
	surface->entityPositionChanged.connect( BIND_SLOT1( SymbolSurfaceImpS11n::entityPositionChanged, this ) );
	surface->entityOrientationChanged.connect( BIND_SLOT1( SymbolSurfaceImpS11n::entityOrientationChanged, this ) );
	surface->entityDimensionsChanged.connect( BIND_SLOT1( SymbolSurfaceImpS11n::entityDimensionsChanged, this ) );
	surface->viewPositionChanged.connect( BIND_SLOT1( SymbolSurfaceImpS11n::viewPositionChanged, this ) );
	
	surface->addedSymbol.connect( BIND_SLOT2( SymbolSurfaceImpS11n::symbolAdded, this ) );
	surface->removedSymbol.connect( BIND_SLOT2( SymbolSurfaceImpS11n::symbolRemoved, this ) );
	
	// handle existing attached symbols (it's unlikely that the surface has any 
	// symbols attached to it yet, but it's best to be robust)
	SymbolContainer::SymbolIteratorPair iterPair = surface->getSymbols();
	SymbolContainer::SymbolMap::iterator iter;
	for( iter = iterPair.first; iter != iterPair.second; iter++ )
	{
		symbolAdded( surface, iter->second.get() );
	}
}


SymbolSurfaceImpS11n::~SymbolSurfaceImpS11n()
{
	// sever links with symbols
	symbolsGroup->removeAllChildren();
}


// ================================================
// getSymbolSurfaceImpS11nFromSymbolSurface
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
SymbolSurfaceImpS11n *SymbolSurfaceImpS11n::getSymbolSurfaceImpS11nFromSymbolSurface( SymbolSurface *surface )
{
	SymbolSurfaceImpS11n *surfaceImpS11n = NULL;
	
	if( surface == NULL )
		return NULL;
	
	SymbolSurface::SymbolSurfaceImpIteratorPair iterPair = surface->getImplementations();
	SymbolSurface::SymbolSurfaceImpList::iterator iter;
	for( iter = iterPair.first; iter != iterPair.second; iter++ )
	{
		// RTTI will (hopefully) tell us if imp is an SymbolSurfaceImpS11n or 
		// something else.
		surfaceImpS11n = dynamic_cast<SymbolSurfaceImpS11n*>(iter->get());
		if( surfaceImpS11n != NULL )
		{
			break;
		}
	}

	return surfaceImpS11n;
}


void SymbolSurfaceImpS11n::stateChanged( SymbolSurface * )
{
	switch( surface->getState() )
	{
	case SymbolSurface::Active:
		state->set( "Active" );
		break;
	case SymbolSurface::Destroyed:
		state->set( "Destroyed" );
		break;
	default:
		state->set( "Unknown" );
		break;
	}
}


void SymbolSurfaceImpS11n::attachmentChanged( SymbolSurface * )
{
	if( surface->getAttachState() == SymbolSurface::Entity )
	{
		attachment->set( "Entity" );

		topGroup->addChild( isBillboard.get() );
		topGroup->addChild( isPerspectiveEnabled.get() );
		topGroup->addChild( entityID.get() );
		topGroup->addChild( position.get() );
		topGroup->addChild( yaw.get() );
		topGroup->addChild( pitch.get() );
		topGroup->addChild( roll.get() );
		topGroup->addChild( width.get() );
		topGroup->addChild( height.get() );

		topGroup->removeChild( viewID.get() );
		topGroup->removeChild( viewPositionLeft.get() );
		topGroup->removeChild( viewPositionRight.get() );
		topGroup->removeChild( viewPositionTop.get() );
		topGroup->removeChild( viewPositionBottom.get() );
	}
	else if( surface->getAttachState() == SymbolSurface::View )
	{
		attachment->set( "View" );

		topGroup->removeChild( isBillboard.get() );
		topGroup->removeChild( isPerspectiveEnabled.get() );
		topGroup->removeChild( entityID.get() );
		topGroup->removeChild( position.get() );
		topGroup->removeChild( yaw.get() );
		topGroup->removeChild( pitch.get() );
		topGroup->removeChild( roll.get() );
		topGroup->removeChild( width.get() );
		topGroup->removeChild( height.get() );

		topGroup->addChild( viewID.get() );
		topGroup->addChild( viewPositionLeft.get() );
		topGroup->addChild( viewPositionRight.get() );
		topGroup->addChild( viewPositionTop.get() );
		topGroup->addChild( viewPositionBottom.get() );
	}
}


void SymbolSurfaceImpS11n::surfaceCoordinatesChanged( SymbolSurface * )
{
	Vect2 minimum = surface->getMinimumCoordinates();
	Vect2 maximum = surface->getMaximumCoordinates();
	minimumU->set( minimum[0] );
	minimumV->set( minimum[1] );
	maximumU->set( maximum[0] );
	maximumV->set( maximum[1] );
}


void SymbolSurfaceImpS11n::billboardingChanged( SymbolSurface * )
{
	isBillboard->set( surface->getIsBillboard() );
	isPerspectiveEnabled->set( surface->getIsPerspectiveEnabled() );
}


// FIXME - no signal for entity ID changes


void SymbolSurfaceImpS11n::entityPositionChanged( SymbolSurface * )
{
	position->set( surface->getPosition() );
}


void SymbolSurfaceImpS11n::entityOrientationChanged( SymbolSurface * )
{
	yaw->set( surface->getYaw() );
	pitch->set( surface->getPitch() );
	roll->set( surface->getRoll() );
}


void SymbolSurfaceImpS11n::entityDimensionsChanged( SymbolSurface * )
{
	width->set( surface->getWidth() );
	height->set( surface->getHeight() );
}


// FIXME - no signal for view ID changes


void SymbolSurfaceImpS11n::viewPositionChanged( SymbolSurface * )
{
	viewPositionLeft->set( surface->getViewPositionLeft() );
	viewPositionRight->set( surface->getViewPositionRight() );
	viewPositionTop->set( surface->getViewPositionTop() );
	viewPositionBottom->set( surface->getViewPositionBottom() );
}


void SymbolSurfaceImpS11n::symbolAdded( SymbolContainer *, Symbol *symbol )
{
	SymbolImpS11n *symbolImpS11n = SymbolImpS11n::getSymbolImpS11nFromSymbol( symbol );
	if( symbolImpS11n == NULL )
	{
		std::cerr << "SymbolSurfaceImpS11n::symbolAdded - warning - The newly-added symbol either has no implementation object or has an implementation that isn't a symbolImpS11n.  Can't add symbol to my scene graph.\n";
	}
	else
	{
		symbolsGroup->addChild( symbolImpS11n->getTopNode() );
	}
}


void SymbolSurfaceImpS11n::symbolRemoved( SymbolContainer *, Symbol *symbol )
{
	SymbolImpS11n *symbolImpS11n = SymbolImpS11n::getSymbolImpS11nFromSymbol( symbol );
	if( symbolImpS11n == NULL )
	{
		std::cerr << "SymbolSurfaceImpS11n::symbolRemoved - warning - The newly-removed symbol either has no implementation object or has an implementation that isn't a symbolImpS11n.  Can't remove symbol from my scene graph.\n";
	}
	else
	{
		symbolsGroup->removeChild( symbolImpS11n->getTopNode() );
	}
}


