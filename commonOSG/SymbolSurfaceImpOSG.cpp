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

#include <iostream>

#include <osg/StateSet>
#include <osg/Depth>

#include "BindSlot.h"
#include "SymbolSurfaceImpOSG.h"
#include "MiscOSG.h"


using namespace mpv;
using namespace mpvosg;

// statics
osg::Vec3 SymbolSurfaceImpOSG::xAxis( 1., 0., 0. );
osg::Vec3 SymbolSurfaceImpOSG::yAxis( 0., 1., 0. );
osg::Vec3 SymbolSurfaceImpOSG::zAxis( 0., 0., 1. );


SymbolSurfaceImpOSG::SymbolSurfaceImpOSG( SymbolSurface *newSurface ) : 
	SymbolSurfaceImp( newSurface )
{
	// Note - attachmentChanged() calls symbolAdded() for each symbol 
	// attached to the surface
	attachmentChanged( surface );
	surfaceCoordinatesChanged( surface );
	billboardingChanged( surface );
	entityPositionChanged( surface );
	entityOrientationChanged( surface );
	entityDimensionsChanged( surface );
	viewPositionChanged( surface );

	surface->attachmentChanged.connect( BIND_SLOT1( SymbolSurfaceImpOSG::attachmentChanged, this ) );
	surface->surfaceCoordinatesChanged.connect( BIND_SLOT1( SymbolSurfaceImpOSG::surfaceCoordinatesChanged, this ) );
	surface->billboardingChanged.connect( BIND_SLOT1( SymbolSurfaceImpOSG::billboardingChanged, this ) );
	surface->entityPositionChanged.connect( BIND_SLOT1( SymbolSurfaceImpOSG::entityPositionChanged, this ) );
	surface->entityOrientationChanged.connect( BIND_SLOT1( SymbolSurfaceImpOSG::entityOrientationChanged, this ) );
	surface->entityDimensionsChanged.connect( BIND_SLOT1( SymbolSurfaceImpOSG::entityDimensionsChanged, this ) );
	surface->viewPositionChanged.connect( BIND_SLOT1( SymbolSurfaceImpOSG::viewPositionChanged, this ) );
	
	surface->addedSymbol.connect( BIND_SLOT2( SymbolSurfaceImpOSG::symbolAdded, this ) );
	surface->removedSymbol.connect( BIND_SLOT2( SymbolSurfaceImpOSG::symbolRemoved, this ) );
}


SymbolSurfaceImpOSG::~SymbolSurfaceImpOSG()
{
	// sever links with parent(s)
	if( topNode.valid() )
		reparentNode( topNode.get(), NULL );
	// sever links with symbols
	if( transform.valid() )
		transform->removeChildren( 0, transform->getNumChildren() );
}


// ================================================
// getSymbolSurfaceImpOSGFromSymbolSurface
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
SymbolSurfaceImpOSG *SymbolSurfaceImpOSG::getSymbolSurfaceImpOSGFromSymbolSurface( SymbolSurface *surface )
{
	SymbolSurfaceImpOSG *surfaceImpOSG = NULL;
	
	if( surface == NULL )
		return NULL;
	
	SymbolSurface::SymbolSurfaceImpIteratorPair iterPair = surface->getImplementations();
	SymbolSurface::SymbolSurfaceImpList::iterator iter;
	for( iter = iterPair.first; iter != iterPair.second; iter++ )
	{
		// RTTI will (hopefully) tell us if imp is an SymbolSurfaceImpOSG or 
		// something else.
		surfaceImpOSG = dynamic_cast<SymbolSurfaceImpOSG*>(iter->get());
		if( surfaceImpOSG != NULL )
		{
			break;
		}
	}

	return surfaceImpOSG;
}


void SymbolSurfaceImpOSG::attachmentChanged( SymbolSurface * )
{
	// Remove any symbols attached to the surface.  They'll be reattached 
	// after the surface subgraph is reconstructed.
	SymbolContainer::SymbolIteratorPair iterPair = surface->getSymbols();
	SymbolContainer::SymbolMap::iterator iter;
	for( iter = iterPair.first; iter != iterPair.second; iter++ )
	{
		symbolRemoved( surface, iter->second.get() );
	}
	
	if( surface->getAttachState() == SymbolSurface::Entity )
		setupEntityAttachedSurface();
	else if( surface->getAttachState() == SymbolSurface::View )
		setupViewAttachedSurface();

	// Add the symbols to the newly-constructed surface subgraph.
	for( iter = iterPair.first; iter != iterPair.second; iter++ )
	{
		symbolAdded( surface, iter->second.get() );
	}

}


void SymbolSurfaceImpOSG::surfaceCoordinatesChanged( SymbolSurface * )
{
	if( surface->getAttachState() == SymbolSurface::Entity )
		updateEntityAttachedTransform();
	else if( surface->getAttachState() == SymbolSurface::View )
		updateViewAttachedTransform();
}


void SymbolSurfaceImpOSG::billboardingChanged( SymbolSurface * )
{
	if( surface->getAttachState() != SymbolSurface::Entity )
		return;
	// fixme - billboarding not implemented
}


void SymbolSurfaceImpOSG::entityPositionChanged( SymbolSurface * )
{
	if( surface->getAttachState() != SymbolSurface::Entity )
		return;
	updateEntityAttachedTransform();
}


void SymbolSurfaceImpOSG::entityOrientationChanged( SymbolSurface * )
{
	if( surface->getAttachState() != SymbolSurface::Entity )
		return;
	updateEntityAttachedTransform();
}


void SymbolSurfaceImpOSG::entityDimensionsChanged( SymbolSurface * )
{
	if( surface->getAttachState() != SymbolSurface::Entity )
		return;
	updateEntityAttachedTransform();
}


void SymbolSurfaceImpOSG::viewPositionChanged( SymbolSurface * )
{
	if( surface->getAttachState() != SymbolSurface::View )
		return;
	updateViewAttachedTransform();
}


void SymbolSurfaceImpOSG::symbolAdded( SymbolContainer *, Symbol *symbol )
{
	if( !transform.valid() )
		// surface isn't ready yet
		return;
	
	SymbolImpOSG *symbolImpOSG = SymbolImpOSG::getSymbolImpOSGFromSymbol( symbol );
	if( symbolImpOSG == NULL )
	{
		std::cerr << "SymbolSurfaceImpOSG::symbolAdded - warning - The newly-added symbol either has no implementation object or has an implementation that isn't a symbolImpOSG.  Can't add symbol to my scene graph.\n";
	}
	else
	{
		transform->addChild( symbolImpOSG->getTopNode() );
	}
}


void SymbolSurfaceImpOSG::symbolRemoved( SymbolContainer *, Symbol *symbol )
{
	if( !transform.valid() )
		// surface isn't ready yet
		return;
	
	SymbolImpOSG *symbolImpOSG = SymbolImpOSG::getSymbolImpOSGFromSymbol( symbol );
	if( symbolImpOSG == NULL )
	{
		std::cerr << "SymbolSurfaceImpOSG::symbolRemoved - warning - The newly-removed symbol either has no implementation object or has an implementation that isn't a symbolImpOSG.  Can't remove symbol from my scene graph.\n";
	}
	else
	{
		transform->removeChild( symbolImpOSG->getTopNode() );
	}
}



void SymbolSurfaceImpOSG::setupEntityAttachedSurface()
{
	transform = new osg::MatrixTransform;
	
	transform->setName( "Entity-attached Symbol Surface" );
	
	// disable lighting, enable blending for the symbol surface
	osg::StateSet* stateset = transform->getOrCreateStateSet();
	stateset->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	stateset->setMode( GL_BLEND, osg::StateAttribute::ON );
	
	topNode = transform.get();
}


void SymbolSurfaceImpOSG::updateEntityAttachedTransform()
{
	Vect2 minCoord = surface->getMinimumCoordinates();
	Vect2 maxCoord = surface->getMaximumCoordinates();
	Vect2 coordDelta = maxCoord - minCoord;
	
	// places center of surface at center of entity-relative offset
	Vect2 surfaceCenterShift( 
		surface->getWidth() * -0.5, 
		surface->getHeight() * -0.5 );

	// scales the surface to the correct size, in entity coordinates
	Vect2 surfaceScale( 
		surface->getWidth() / coordDelta[0], 
		surface->getHeight() / coordDelta[1] );
	// places minCoord in the bottom-left corner of the extents
	Vect2 surfaceOriginPlacement = minCoord * -1.0;
	
	// construct a transform that converts symbol space into entity space
	Vect3 surfacePosition = surface->getPosition();
	transform->setMatrix( 
		osg::Matrix::translate( surfaceOriginPlacement[0], surfaceOriginPlacement[1], 0.0 ) * 
		osg::Matrix::scale( surfaceScale[0], surfaceScale[1], 1.0 ) * 
		osg::Matrix::translate( surfaceCenterShift[0], surfaceCenterShift[1], 0.0 ) * 
		osg::Matrix::rotate( osg::Quat( osg::DegreesToRadians(90.), xAxis ) ) * 
		osg::Matrix::rotate( osg::Quat( 
			osg::DegreesToRadians(surface->getRoll()), yAxis, 
			osg::DegreesToRadians(surface->getPitch()), xAxis, 
			osg::DegreesToRadians(surface->getYaw() * -1.0), zAxis ) ) * 
		osg::Matrix::translate( surfacePosition[1], surfacePosition[0], -surfacePosition[2] ) 
		);
}


void SymbolSurfaceImpOSG::setupViewAttachedSurface()
{
	projection = new osg::Projection;
	transform = new osg::MatrixTransform;
	
	projection->setMatrix( osg::Matrix::ortho2D( 0.0, 1.0, 0.0, 1.0 ) );
	projection->setName( "View-attached Symbol Surface" );
	projection->addChild( transform.get() );

	// turn lighting off for the symbol surface
	osg::StateSet* stateset = transform->getOrCreateStateSet();
	stateset->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	
	// ensure that the text is always on top
#if 0
	// to ensure the text appears on top we can either use osg::Depth to force 
	// the depth fragments to be placed at the front of the screen...
	stateset->setAttribute( new osg::Depth( osg::Depth::LESS, 0.0, 0.0001 ) );
#else
	// ...or disable depth test, and make sure that the text is drawn after 
	// everything else so that it always appears on top.
	stateset->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF );
	stateset->setRenderBinDetails( 11, "RenderBin" );
#endif
	
	stateset->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
	stateset->setMode( GL_BLEND, osg::StateAttribute::ON );
	
	topNode = projection.get();
}


void SymbolSurfaceImpOSG::updateViewAttachedTransform()
{
	Vect2 bottomLeftOnScreenExtent( 
		surface->getViewPositionLeft(), surface->getViewPositionBottom() );
	Vect2 topRightOnScreenExtent( 
		surface->getViewPositionRight(), surface->getViewPositionTop() );
	Vect2 onScreenExtentDelta = topRightOnScreenExtent - bottomLeftOnScreenExtent;
	
	Vect2 minCoord = surface->getMinimumCoordinates();
	Vect2 maxCoord = surface->getMaximumCoordinates();
	Vect2 coordDelta = maxCoord - minCoord;
	
	// scales the surface to the correct size, in on-screen coordinates
	Vect2 surfaceScale( 
		onScreenExtentDelta.getx() / coordDelta.getx(), 
		onScreenExtentDelta.gety() / coordDelta.gety() );
	// places minCoord in the bottom-left corner of the on-screen extents
	Vect2 surfaceOriginPlacement = minCoord * -1.0;
	
	// construct a transform that converts symbol space into a 
	// section within the on-screen coordinate system (0,0 to 1,1)
	transform->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
	transform->setMatrix( 
		osg::Matrix::translate( surfaceOriginPlacement.getx(), surfaceOriginPlacement.gety(), 0.0 ) * 
		osg::Matrix::scale( surfaceScale.getx(), surfaceScale.gety(), 1.0 ) * 
		osg::Matrix::translate( bottomLeftOnScreenExtent[0], bottomLeftOnScreenExtent[1], 0.0 ) 
		);

}
