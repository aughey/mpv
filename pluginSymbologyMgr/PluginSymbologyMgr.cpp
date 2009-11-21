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
 *  
 *  FILENAME:   PluginSymbologyMgr.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  maintain state for CIGI symbology.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  2008-01-10 Andrew Sampson
 *      Initial Release.
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#include <iostream>
#include <string.h> // for memcpy

#include <CigiSymbolSurfaceDefV3_3.h>
#include <CigiSymbolCtrlV3_3.h>
#include <CigiShortSymbolCtrlV3_3.h>
#include <CigiSymbolTextDefV3_3.h>
#include <CigiSymbolCircleDefV3_3.h>
#include <CigiSymbolLineDefV3_3.h>
#include <CigiSymbolCloneV3_3.h>

#include "PluginSymbologyMgr.h"
#include "SymbolText.h"
#include "SymbolCircle.h"
#include "SymbolLine.h"

#ifdef WIN32
#define isnan _isnan
#endif


EXPORT_DYNAMIC_CLASS( PluginSymbologyMgr )

// ================================================
// PluginSymbologyMgr
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginSymbologyMgr::PluginSymbologyMgr() : Plugin(),
	symbologySurfaceDefProc( this ),
	symbologyCtrlProc( this ),
	shortSymbologyCtrlProc( this ),
	textSymbolDefinitionProc( this ),
	circleSymbolDefinitionProc( this ),
	lineSymbolDefinitionProc( this ),
	symbolCloneDefinitionProc( this )
{
	name_ = "PluginSymbologyMgr";

	licenseInfo_.setLicense( LicenseInfo::LicenseLGPL );
	licenseInfo_.setOrigin( "Boeing" );

	dependencies_.push_back( "PluginDefFileReader" );
	dependencies_.push_back( "PluginViewMgr" );

	ImsgPtr = NULL;
	OmsgPtr = NULL;
	IGSession = NULL;
	DefFileData = NULL;
	timeElapsedLastFrame = NULL;
	viewMap = NULL;
	allEntities = NULL;
	
	symbols = new SymbolContainer;
	symbolSurfaces = new SymbolSurfaceContainer;
}


// ================================================
// ~PluginSymbologyMgr
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginSymbologyMgr::~PluginSymbologyMgr() throw() 
{
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginSymbologyMgr::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{
	case SystemState::BlackboardPost:
		// This state is for posting things to the blackboard and other 
		// initialization tasks
		
		// post the symbols
		bb_->put( "Symbols", symbols.get() );
		
		// post the symbol surfaces
		bb_->put( "SymbolSurfaces", symbolSurfaces.get() );
		break;

	case SystemState::BlackboardRetrieve:
		// This state is for retrieving things from the blackboard

		bb_->get( "CigiIncomingMsg", ImsgPtr );
		bb_->get( "CigiOutgoingMsg", OmsgPtr );
		bb_->get( "CigiIGSession", IGSession );
		bb_->get( "DefinitionData", DefFileData );
		bb_->get( "TimeElapsedLastFrame", timeElapsedLastFrame );
		bb_->get( "ViewMap", viewMap );
		bb_->get( "AllEntities", allEntities );

		if( ImsgPtr != NULL )
		{
			// Register for incoming Symbology Surface Definition Packets
			ImsgPtr->RegisterEventProcessor( CIGI_SYMBOL_SURFACE_DEF_PACKET_ID_V3_3, &symbologySurfaceDefProc );
			// Register for incoming Symbology Control Packets
			ImsgPtr->RegisterEventProcessor( CIGI_SYMBOL_CONTROL_PACKET_ID_V3_3, &symbologyCtrlProc );
			// Register for incoming Short Symbology Control Packets
			ImsgPtr->RegisterEventProcessor( CIGI_SHORT_SYMBOL_CONTROL_PACKET_ID_V3_3, &shortSymbologyCtrlProc );
			// Register for incoming Symbology Definition Packets
			ImsgPtr->RegisterEventProcessor( CIGI_SYMBOL_TEXT_DEFINITION_PACKET_ID_V3_3, &textSymbolDefinitionProc );
			ImsgPtr->RegisterEventProcessor( CIGI_SYMBOL_CIRCLE_DEFINITION_PACKET_ID_V3_3, &circleSymbolDefinitionProc );
			ImsgPtr->RegisterEventProcessor( CIGI_SYMBOL_LINE_DEFINITION_PACKET_ID_V3_3, &lineSymbolDefinitionProc );
			ImsgPtr->RegisterEventProcessor( CIGI_SYMBOL_CLONE_PACKET_ID_V3_3, &symbolCloneDefinitionProc );
		}
		break;

	case SystemState::ConfigurationProcess:
		processConfigData();
		break;

	case SystemState::Operate:
	case SystemState::Debug:
		operate();
		break;

	case SystemState::Reset:
	case SystemState::Shutdown:
		symbols->flagAllSymbolsAsDestroyed();
		symbolSurfaces->flagAllSurfacesAsDestroyed();
		break;

	default:
		break;
	}
	
}


// ================================================
// operate
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginSymbologyMgr::operate()
{

	SymbolContainer::SymbolIteratorPair iterators = symbols->getSymbols();
	for( ; iterators.first != iterators.second; iterators.first++ )
	{
		Symbol *symbol = (*iterators.first).second.get();

		// skip over symbols that do not exist
		if( symbol == NULL )
			continue;
		
		
		processSymbol( symbol );
		
	}
}


// ================================================
// processConfigData
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginSymbologyMgr::processConfigData()
{
	DefFileGroup *root = *DefFileData;

	if(root == NULL)
	{
		return;
	}

	
}


// ================================================
// processSymbol
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginSymbologyMgr::processSymbol( Symbol *symbol )
{
	symbol->update( *timeElapsedLastFrame );
}



// ================================================
// reparentSymbol
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginSymbologyMgr::reparentSymbol( int symbolID, bool isChild, int newParentID )
{
	// find the symbol, its old parent (if applicable), and its new parent 
	// (if applicable)
	
	Symbol *symbol = symbols->findSymbol( symbolID );
	if( symbol == NULL )
	{
		// error - symbol not found
		return;
	}
	
	// detach the symbol from its old parent
	if( symbol->getIsChild() )
	{
		// get symbol's old parent
		Symbol *oldParent = symbol->getParent();
		if( oldParent == NULL )
		{
			// error - inconsistent symbol state
		}
		else
		{
			// remove the symbol from oldParent
			oldParent->removeSymbol( symbol );
		}
	}
	
	// if symbol is now a child, attach it to its new parent
	if( isChild )
	{
		// find the new parent
		Symbol *newParent = symbols->findSymbol( newParentID );
		if( newParent == NULL )
		{
			// error - new parent not found
// FIXME - return error status
			return;
		}
		
		symbol->setParent( isChild, newParentID, newParent );
		newParent->addSymbol( symbol );
	}
	// if the symbol is now top-level, clear its parent
	else
	{
		symbol->setParent( isChild, 0xffff, NULL );
	}
}


// ================================================
// addSymbolToSurface
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginSymbologyMgr::addSymbolToSurface( Symbol *symbol, int surfaceID )
{
	SymbolSurface *surface = symbolSurfaces->findSymbolSurface( surfaceID );
	if( surface != NULL )
	{
		surface->addSymbol( symbol );
	}
}


// ================================================
// removeSymbolFromSurface
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginSymbologyMgr::removeSymbolFromSurface( Symbol *symbol, int surfaceID )
{
	SymbolSurface *surface = symbolSurfaces->findSymbolSurface( surfaceID );
	if( surface != NULL )
	{
		surface->removeSymbol( symbol );
	}
}


// ================================================
// addSymbolSurface
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginSymbologyMgr::addSymbolSurface( SymbolSurface *surface )
{
	if( surface == NULL )
		return;
	
std::cerr << "PluginSymbologyMgr::addSymbolSurface\n";
	// It is important that this is called (and the resulting 
	// SymbolSurfaceContainer::addedSymbolSurface signal is emitted) before 
	// the surface is added to the View or Entity container.  Doing so 
	// allows a SymbolSurfaceImp to be added to the new surface before 
	// other parts of the system are notified of the addition.
	symbolSurfaces->addSymbolSurface( surface );
}


// ================================================
// attachSymbolSurface
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginSymbologyMgr::attachSymbolSurface( SymbolSurface *surface )
{
	if( surface == NULL )
		return;
	
std::cerr << "PluginSymbologyMgr::attachSymbolSurface - surface " << surface->getID() << "\n";

	SymbolSurfaceContainer *container = getSurfaceContainer( surface );

	// add surface to the entity or view that it is associated with
	if( container == NULL )
		std::cerr << "Error - when attaching symbol surface " 
			<< surface->getID() 
			<< ", could not find symbol container to attach to\n";
	else
		container->addSymbolSurface( surface );
	
}


// ================================================
// detachSymbolSurface
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginSymbologyMgr::detachSymbolSurface( SymbolSurface *surface )
{
	if( surface == NULL )
		return;
	
std::cerr << "PluginSymbologyMgr::detachSymbolSurface - surface " << surface->getID() << "\n";

	SymbolSurfaceContainer *container = getSurfaceContainer( surface );
	
	// remove surface from the entity or view that it is associated with
	if( container == NULL )
		std::cerr << "Error - when detaching symbol surface " 
			<< surface->getID() 
			<< ", could not find symbol container to detach from\n";
	else
		container->removeSymbolSurface( surface );
	
}


// ================================================
// getSurfaceContainer
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
SymbolSurfaceContainer *PluginSymbologyMgr::getSurfaceContainer( SymbolSurface *surface )
{
	SymbolSurfaceContainer *container = NULL;
	
	if( surface->getAttachState() == SymbolSurface::Entity )
	{
		Entity *entity = allEntities->findEntity( surface->getEntityID() );
		if( entity != NULL )
		{
			container = entity;
		}
		else
		{
			// error - entity not found
			std::cerr << "Error - for symbol surface " 
				<< surface->getID() << ", could not find entity " 
				<< surface->getEntityID() << "\n";
		}
	}
	else if( surface->getAttachState() == SymbolSurface::View )
	{
		std::map< int, RefPtr<View> >::iterator iter = viewMap->find( surface->getViewID() );
		if( iter != viewMap->end() )
		{
			container = iter->second.get();
		}
		else
		{
			// error - view not found
			std::cerr << "Error - for symbol surface " 
				<< surface->getID() << ", could not find view " 
				<< surface->getViewID() << "\n";
		}
	}

	return container;
}

	
// ================================================
// cloneSymbol
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginSymbologyMgr::cloneSymbol( 
	int sourceID, int destinationID, bool sourceIsIGDefined )
{
	RefPtr<Symbol> sourceSymbol;
	
	if( !sourceIsIGDefined )
	{
		// clone from an existing symbol
		
		sourceSymbol = symbols->findSymbol( sourceID );
	}
	else
	{
		// clone from an IG-defined, built-in symbol
		
		// FIXME - not implemented yet
	}

	if( sourceSymbol.valid() )
	{
		// time to make a copy

		switch( sourceSymbol->getType() )
		{
		case Symbol::Text:
			{
			SymbolText *src = dynamic_cast<SymbolText*>(sourceSymbol.get());
			SymbolText *dest = createSymbol<SymbolText>( destinationID, Symbol::Text );
			dest->setTextAlignment( src->getTextAlignment() );
			dest->setTextOrientation( src->getTextOrientation() );
			dest->setFontID( src->getFontID() );
			dest->setFontSize( src->getFontSize() );
			dest->setText( src->getText() );
			}
			break;
		case Symbol::Circle:
			{
			SymbolCircle *src = dynamic_cast<SymbolCircle*>(sourceSymbol.get());
			SymbolCircle *dest = createSymbol<SymbolCircle>( destinationID, Symbol::Circle );
			dest->setDrawingStyle( src->getDrawingStyle() );
			dest->setLineWidth( src->getLineWidth() );
			dest->setStipplePattern( src->getStipplePattern() );
			dest->setStipplePatternLength( src->getStipplePatternLength() );
			dest->removeAllCircles();
			for( unsigned int i = 0; i < src->getNumCircles(); i++ )
			{
				dest->addCircle( src->getCircle( i ) );
			}
			}
			break;
		case Symbol::Line:
			{
			SymbolLine *src = dynamic_cast<SymbolLine*>(sourceSymbol.get());
			SymbolLine *dest = createSymbol<SymbolLine>( destinationID, Symbol::Line );
			dest->setPrimitiveType( src->getPrimitiveType() );
			dest->setLineWidth( src->getLineWidth() );
			dest->setStipplePattern( src->getStipplePattern() );
			dest->setStipplePatternLength( src->getStipplePatternLength() );
			dest->removeAllVertices();
			for( unsigned int i = 0; i < src->getNumVertices(); i++ )
			{
				dest->addVertex( src->getVertex( i ) );
			}
			}
			break;
		default:
			break;
		}

	}
	else
	{
		std::cout << "PluginSymbologyMgr - warning - could not find "
			<< ( sourceIsIGDefined ? "IG-defined" : "existing" )
			<< " symbol " << sourceID
			<< "\n\twhen processing SymbolCloneDefinition for symbol " 
			<< destinationID 
			<< "\n\tDiscarding packet.\n";
	}
}


// ================================================
// SymbologySurfaceDefP::OnPacketReceived
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginSymbologyMgr::SymbologySurfaceDefP::OnPacketReceived( CigiBasePacket *Packet )
{

	CigiSymbolSurfaceDefV3_3 *surfaceDef = static_cast<CigiSymbolSurfaceDefV3_3 *> (Packet);
	
	bool isNewSurface = false;
	bool attachmentHasChanged = false;
	
	RefPtr<SymbolSurface> surface = 
		plugin->symbolSurfaces->findSymbolSurface( surfaceDef->GetSurfaceID() );
	if( !surface.valid() )
	{
		// symbol surface not found; create a new surface
		surface = new SymbolSurface;

		surface->setID( surfaceDef->GetSurfaceID() );
		
		isNewSurface = true;
	}
	
	surface->setState( (SymbolSurface::SurfaceState)surfaceDef->GetSurfaceState() );

	if( surface->getState() == SymbolSurface::Destroyed )
		// just setting the state is enough; the event callback system will 
		// take care of removing the surface from the various containers
		return;

	SymbolSurface::SurfaceAttachState newAttachState = (SymbolSurface::SurfaceAttachState)surfaceDef->GetAttached();
	
	/*
	If the surface...
	- was attached to an entity, and is now attached to a view
	- was attached to a view, and is now attached to an entity
	- was attached to entity A, and is now attached to entity B
	- was attached to view A, and is now attached to view B
	...then the surface needs to be reparented
	*/
	if( !isNewSurface )
	{
		if( newAttachState != surface->getAttachState() )
		{
			attachmentHasChanged = true;
		}
		else
		{
			if( surface->getAttachState() == SymbolSurface::Entity )
			{
				if( surface->getEntityID() != surfaceDef->GetEntityID() )
					attachmentHasChanged = true;
			}
			else if( surface->getAttachState() == SymbolSurface::View )
			{
				if( surface->getViewID() != surfaceDef->GetViewID() )
					attachmentHasChanged = true;
			}
		}
		
		if( attachmentHasChanged )
		{
			// detaching must be performed now, before we clobber the 
			// surface's previous values
			plugin->detachSymbolSurface( surface.get() );
		}
	}
	
	surface->setAttachState( newAttachState );

	surface->setMinimumCoordinates( Vect2( 
		surfaceDef->GetMinU(), 
		surfaceDef->GetMinV() ) );
	surface->setMaximumCoordinates( Vect2( 
		surfaceDef->GetMaxU(), 
		surfaceDef->GetMaxV() ) );

	if( surface->getAttachState() == SymbolSurface::Entity )
	{
		surface->setIsBillboard( 
			surfaceDef->GetBillboardState() == CigiBaseSymbolSurfaceDef::Billboard );
		surface->setIsPerspectiveEnabled( 
			surfaceDef->GetPerspectiveGrowth() == CigiBaseSymbolSurfaceDef::Enable );
		surface->setEntityID( surfaceDef->GetEntityID() );
		surface->setPosition( Vect3( 
			surfaceDef->GetXOffset(),
			surfaceDef->GetYOffset(),
			surfaceDef->GetZOffset() ) );
		surface->setYaw( surfaceDef->GetYaw() );
		surface->setPitch( surfaceDef->GetPitch() );
		surface->setRoll( surfaceDef->GetRoll() );
		surface->setWidth( surfaceDef->GetWidth() );
		surface->setHeight( surfaceDef->GetHeight() );
	}
	else if( surface->getAttachState() == SymbolSurface::View )
	{
		surface->setViewID( surfaceDef->GetViewID() );
		surface->setViewPositionLeft( surfaceDef->GetLeftEdgePosition() );
		surface->setViewPositionRight( surfaceDef->GetRightEdgePosition() );
		surface->setViewPositionTop( surfaceDef->GetTopEdgePosition() );
		surface->setViewPositionBottom( surfaceDef->GetBottomEdgePosition() );
	}
	
	if( isNewSurface )
	{
		// It is important that this is called (and the resulting 
		// SymbolSurfaceContainer::addedSymbolSurface signal is emitted) before 
		// the surface is added to the View or Entity container.  Doing so 
		// allows a SymbolSurfaceImp to be added to the new surface before 
		// other parts of the system are notified of the addition.
		plugin->addSymbolSurface( surface.get() );
	}
	
	if( isNewSurface || attachmentHasChanged )
	{
		plugin->attachSymbolSurface( surface.get() );
	}
}



// ================================================
// SymbologyCtrlP::OnPacketReceived
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginSymbologyMgr::SymbologyCtrlP::OnPacketReceived( CigiBasePacket *Packet )
{

	CigiSymbolCtrlV3_3 *symbolCtrl = static_cast<CigiSymbolCtrlV3_3 *> (Packet);
	RefPtr<Symbol> symbol = plugin->symbols->findSymbol( symbolCtrl->GetSymbolID() );
	if( !symbol.valid() )
	{
		// error - symbol not found
		return;
	}
	
	symbol->setFullControlPacketRecvd( true );
	
	// symbol state
	// signal handling in SymbolContainer will take care of removing Destroyed 
	// symbols from the plugin, symbol surfaces, and parent symbols.
	symbol->setState( (Symbol::SymbolState)symbolCtrl->GetSymbolState() );
	if( symbol->getState() == Symbol::Destroyed )
	{
		// go no further
		return;
	}
	
	// symbol hierarchy (it is important that this is processed 
	// before the flash or color state is processed, as these attributes 
	// are affected by the symbol hierarchy)
	bool isChild = (symbolCtrl->GetAttachState() == CigiBaseSymbolCtrl::Attach);
	bool changedTopLevelStatus = ( symbol->getIsChild() != isChild );
	bool changedParent =         ( symbol->getIsChild() &&
	                             ( symbol->getParentID() != symbolCtrl->GetParentSymbolID() ) );
	if( changedTopLevelStatus || changedParent )
	{
		plugin->reparentSymbol( symbol->getID(), isChild, symbolCtrl->GetParentSymbolID() );
// FIXME - error handling - what if reparenting fails; for example, new parent doesn't exist?
	}
	
	
	// symbol surface management
	// Only top-level symbols need to be stored in Surface objects.
	// Symbols need to be added/removed from Surfaces when:
	// - their surface ID changes (and they are top-level)
	// - their top-level status changes (isChild)
	// - they are created/destroyed
	if( changedTopLevelStatus || 
		symbol->getSurfaceID() != symbolCtrl->GetSurfaceID() )
	{
		if( symbol->getIsChild() )
		{
			// symbol is now a child; remove it from its current surface
			plugin->removeSymbolFromSurface( symbol.get(), symbol->getSurfaceID() );
		}
		else
		{
			// symbol is now top-level...
			// ...remove it from its current surface
			plugin->removeSymbolFromSurface( symbol.get(), symbol->getSurfaceID() );
			// ...and add it to the appropriate surface
			plugin->addSymbolToSurface( symbol.get(), symbolCtrl->GetSurfaceID() );
		}
	}
	// (don't clobber symbol's surfaceID until finished removing it from 
	// old surface...)
	symbol->setSurfaceID( symbolCtrl->GetSurfaceID() );
	
	
	// flash control
	symbol->setFlashDutyCyclePercentage( symbolCtrl->GetFlashDutyCycle() );
	symbol->setFlashPeriod( symbolCtrl->GetFlashPeriod() );
	if( symbolCtrl->GetFlashCtrl() == CigiBaseSymbolCtrl::Reset )
		symbol->resetFlashSequence();
	
	
	// color
	symbol->setInheritColor( symbolCtrl->GetInheritColor() == CigiBaseSymbolCtrl::Inherit );
	symbol->setColor( 
		symbolCtrl->GetRed(),
		symbolCtrl->GetGreen(),
		symbolCtrl->GetBlue(),
		symbolCtrl->GetAlpha() );

	// layering
	symbol->setLayer( symbolCtrl->GetLayer() );

	// transform
	symbol->setPosition( symbolCtrl->GetUPosition(), symbolCtrl->GetVPosition() );
	symbol->setRotation( symbolCtrl->GetRotation() );
	symbol->setScale( symbolCtrl->GetScaleU(), symbolCtrl->GetScaleV() );

}



bool getDatumFromShortSymbolCtrl( 
	const CigiBaseShortSymbolCtrl *packet, 
	CigiBaseShortSymbolCtrl::DatumTypeGrp datumType, 
	Cigi_uint32 &result )
{
	if( 
		datumType != CigiBaseShortSymbolCtrl::SurfaceIdDatumType && 
		datumType != CigiBaseShortSymbolCtrl::ParentIdDatumType && 
		datumType != CigiBaseShortSymbolCtrl::LayerDatumType && 
		datumType != CigiBaseShortSymbolCtrl::FlashDutyCycleDatumType )
	{
		// error - invalid datumType for uint
		return false;
	}
	
	for( int i = 0; i < 2; i++ )
	{
		if( datumType != packet->GetDatumType( i ) )
			continue;
		
		result = packet->GetUIntDatum( i );
		return true;
	}
	
	return false;
}


bool getDatumFromShortSymbolCtrl( 
	const CigiBaseShortSymbolCtrl *packet, 
	CigiBaseShortSymbolCtrl::DatumTypeGrp datumType, 
	float &result )
{
	if( 
		datumType != CigiBaseShortSymbolCtrl::FlashPeriodDatumType && 
		datumType != CigiBaseShortSymbolCtrl::UPositionDatumType && 
		datumType != CigiBaseShortSymbolCtrl::VPositionDatumType && 
		datumType != CigiBaseShortSymbolCtrl::RotationDatumType && 
		datumType != CigiBaseShortSymbolCtrl::ScaleUDatumType && 
		datumType != CigiBaseShortSymbolCtrl::ScaleVDatumType )
	{
		// error - invalid datumType for float
		return false;
	}
	
	for( int i = 0; i < 2; i++ )
	{
		if( datumType != packet->GetDatumType( i ) )
			continue;
		
		result = packet->GetFloatDatum( i );
		return true;
	}
	
	return false;
}


bool getDatumFromShortSymbolCtrl( 
	// can't make packet const, because CigiBaseShortSymbolCtrl::GetColorDatum isn't const
	CigiBaseShortSymbolCtrl *packet, 
	CigiBaseShortSymbolCtrl::DatumTypeGrp datumType, 
	Vect4 &result )
{
	if( datumType != CigiBaseShortSymbolCtrl::ColorDatumType )
	{
		// error - invalid datumType for uint
		return false;
	}
	
	for( int i = 0; i < 2; i++ )
	{
		if( datumType != packet->GetDatumType( i ) )
			continue;
		
		Cigi_uint8 r, g, b, a;
		packet->GetColorDatum( i, r, g, b, a );
		result = Vect4( 
			(float)r / 255.0, 
			(float)g / 255.0, 
			(float)b / 255.0, 
			(float)a / 255.0 );
		return true;
	}
	
	return false;
}


// ================================================
// ShortSymbologyCtrlP::OnPacketReceived
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginSymbologyMgr::ShortSymbologyCtrlP::OnPacketReceived( CigiBasePacket *Packet )
{

	CigiShortSymbolCtrlV3_3 *symbolCtrl = static_cast<CigiShortSymbolCtrlV3_3 *> (Packet);
	RefPtr<Symbol> symbol = plugin->symbols->findSymbol( symbolCtrl->GetSymbolID() );
	if( !symbol.valid() )
	{
		// error - symbol not found
		return;
	}
	
	// symbol control packet
	// A full symbol control packet must be sent for this symbol before 
	// short symbol controls can be sent.  If no such packet has been sent 
	// for this symbol, then ignore this short symbol control.
	if( !symbol->getFullControlPacketRecvd() )
	{
		// error - symbol not fully defined; no full Symbol Control for this 
		// symbol was received
		return;
	}
	
	// symbol state
	// signal handling in SymbolContainer will take care of removing Destroyed 
	// symbols from the plugin, symbol surfaces, and parent symbols.
	symbol->setState( (Symbol::SymbolState)symbolCtrl->GetSymbolState() );
	if( symbol->getState() == Symbol::Destroyed )
	{
		// go no further
		return;
	}
	
	// symbol hierarchy (it is important that this is processed 
	// before the flash or color state is processed, as these attributes 
	// are affected by the symbol hierarchy)
	bool isChild = (symbolCtrl->GetAttachState() == CigiBaseSymbolCtrl::Attach);
	bool changedTopLevelStatus = ( symbol->getIsChild() != isChild );
	bool changedParent =         false;
	Cigi_uint32 commandedParent;
	if( getDatumFromShortSymbolCtrl( 
		symbolCtrl, CigiBaseShortSymbolCtrl::ParentIdDatumType, commandedParent ) )
	{
		changedParent =          ( symbol->getIsChild() &&
		                         ( symbol->getParentID() != commandedParent ) );
	}
	if( changedTopLevelStatus || changedParent )
	{
		plugin->reparentSymbol( symbol->getID(), isChild, commandedParent );
// FIXME - error handling - what if reparenting fails; for example, new parent doesn't exist?
	}
	
	
	// symbol surface management
	// Only top-level symbols need to be stored in Surface objects.
	// Symbols need to be added/removed from Surfaces when:
	// - their surface ID changes (and they are top-level)
	// - their top-level status changes (isChild)
	// - they are created/destroyed
	Cigi_uint32 commandedSurface = symbol->getSurfaceID();
	getDatumFromShortSymbolCtrl( 
		symbolCtrl, CigiBaseShortSymbolCtrl::SurfaceIdDatumType, commandedSurface );
	if( changedTopLevelStatus || 
		symbol->getSurfaceID() != commandedSurface )
	{
		if( symbol->getIsChild() )
		{
			// symbol is now a child; remove it from its current surface
			plugin->removeSymbolFromSurface( symbol.get(), symbol->getSurfaceID() );
		}
		else
		{
			// symbol is now top-level...
			// ...remove it from its current surface
			plugin->removeSymbolFromSurface( symbol.get(), symbol->getSurfaceID() );
			// ...and add it to the appropriate surface
			plugin->addSymbolToSurface( symbol.get(), commandedSurface );
		}
	}
	// (don't clobber symbol's surfaceID until finished removing it from 
	// old surface...)
	symbol->setSurfaceID( commandedSurface );
	
	
	// flash control
	Cigi_uint32 flashDutyCycle;
	float flashPeriod;
	if( getDatumFromShortSymbolCtrl( symbolCtrl, CigiBaseShortSymbolCtrl::FlashDutyCycleDatumType, flashDutyCycle ) )
		symbol->setFlashDutyCyclePercentage( flashDutyCycle );
	if( getDatumFromShortSymbolCtrl( symbolCtrl, CigiBaseShortSymbolCtrl::FlashPeriodDatumType, flashPeriod ) )
		symbol->setFlashPeriod( flashPeriod );
	if( symbolCtrl->GetFlashCtrl() == CigiBaseSymbolCtrl::Reset )
		symbol->resetFlashSequence();
	
	
	// color
	Vect4 color;
	symbol->setInheritColor( symbolCtrl->GetInheritColor() == CigiBaseSymbolCtrl::Inherit );
	if( getDatumFromShortSymbolCtrl( symbolCtrl, CigiBaseShortSymbolCtrl::ColorDatumType, color ) )
		symbol->setColor( color );

	// layering
	Cigi_uint32 layer;
	if( getDatumFromShortSymbolCtrl( symbolCtrl, CigiBaseShortSymbolCtrl::LayerDatumType, layer ) )
		symbol->setLayer( layer );

	// transform
	Vect2 position = symbol->getPosition();
	float positionU = position.getx();
	float positionV = position.gety();
	getDatumFromShortSymbolCtrl( symbolCtrl, CigiBaseShortSymbolCtrl::UPositionDatumType, positionU );
	getDatumFromShortSymbolCtrl( symbolCtrl, CigiBaseShortSymbolCtrl::VPositionDatumType, positionV );
	symbol->setPosition( positionU, positionV );

	Vect2 scale = symbol->getScale();
	float scaleU = scale.getx();
	float scaleV = scale.gety();
	getDatumFromShortSymbolCtrl( symbolCtrl, CigiBaseShortSymbolCtrl::ScaleUDatumType, scaleU );
	getDatumFromShortSymbolCtrl( symbolCtrl, CigiBaseShortSymbolCtrl::ScaleVDatumType, scaleV );
	symbol->setScale( scaleU, scaleV );

	float rotation = symbol->getRotation();
	if( getDatumFromShortSymbolCtrl( symbolCtrl, CigiBaseShortSymbolCtrl::RotationDatumType, rotation ) )
		symbol->setRotation( rotation );

}



// ================================================
// TextSymbolDefinitionP::OnPacketReceived
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginSymbologyMgr::TextSymbolDefinitionP::OnPacketReceived( CigiBasePacket *Packet )
{

	CigiSymbolTextDefV3_3 *textDefinition = static_cast<CigiSymbolTextDefV3_3 *> (Packet);
	SymbolText *symbol = plugin->createSymbol<SymbolText>( textDefinition->GetSymbolID(), Symbol::Text );

	symbol->setTextAlignment( (SymbolText::SymbolTextAlignment)textDefinition->GetAlignment() );
	symbol->setTextOrientation( (SymbolText::SymbolTextOrientation)textDefinition->GetOrientation() );
	symbol->setFontID( textDefinition->GetFontID() );
	symbol->setFontSize( textDefinition->GetFontSize() );
	symbol->setText( textDefinition->GetText() );

}



// ================================================
// CircleSymbolDefinitionP::OnPacketReceived
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginSymbologyMgr::CircleSymbolDefinitionP::OnPacketReceived( CigiBasePacket *Packet )
{

	CigiSymbolCircleDefV3_3 *circleDefinition = static_cast<CigiSymbolCircleDefV3_3 *> (Packet);
	SymbolCircle *symbol = plugin->createSymbol<SymbolCircle>( circleDefinition->GetSymbolID(), Symbol::Circle );

	symbol->setDrawingStyle( (SymbolCircle::SymbolCircleDrawStyle)circleDefinition->GetDrawingStyle() );
	symbol->setLineWidth( circleDefinition->GetLineWidth() );
	symbol->setStipplePattern( circleDefinition->GetStipplePattern() );
	symbol->setStipplePatternLength( circleDefinition->GetStipplePatternLen() );
	symbol->removeAllCircles();
	for( int i = 0; i < circleDefinition->GetCircleCount(); i++ )
	{
		CigiBaseCircleSymbolData *cigiCircle = circleDefinition->GetCircle( i );
		SymbolCircle::Circle circle;
		circle.centerPosition.Set( cigiCircle->GetCenterUPosition(), cigiCircle->GetCenterVPosition() );
		circle.radius = cigiCircle->GetRadius();
		circle.innerRadius = cigiCircle->GetInnerRadius();
		circle.startAngle = cigiCircle->GetStartAngle();
		circle.endAngle = cigiCircle->GetEndAngle();
		symbol->addCircle( circle );
	}

}



// ================================================
// LineSymbolDefinitionP::OnPacketReceived
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginSymbologyMgr::LineSymbolDefinitionP::OnPacketReceived( CigiBasePacket *Packet )
{

	CigiSymbolLineDefV3_3 *lineDefinition = static_cast<CigiSymbolLineDefV3_3 *> (Packet);
	SymbolLine *symbol = plugin->createSymbol<SymbolLine>( lineDefinition->GetSymbolID(), Symbol::Line );

	symbol->setPrimitiveType( (SymbolLine::SymbolLinePrimitiveType)lineDefinition->GetPrimitive() );
	symbol->setLineWidth( lineDefinition->GetLineWidth() );
	symbol->setStipplePattern( lineDefinition->GetStipplePattern() );
	symbol->setStipplePatternLength( lineDefinition->GetStipplePatternLen() );
	symbol->removeAllVertices();
	for( int i = 0; i < lineDefinition->GetVertexCount(); i++ )
	{
		CigiBaseVertexSymbolData *vertex = lineDefinition->GetVertex( i );
		symbol->addVertex( vertex->GetVertexU(), vertex->GetVertexV() );
	}

}



// ================================================
// SymbolCloneDefinitionP::OnPacketReceived
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginSymbologyMgr::SymbolCloneDefinitionP::OnPacketReceived( CigiBasePacket *Packet )
{

	CigiSymbolCloneV3_3 *cloneDefinition = static_cast<CigiSymbolCloneV3_3 *> (Packet);
	
	plugin->cloneSymbol( 
		cloneDefinition->GetSourceID(), 
		cloneDefinition->GetSymbolID(), 
		cloneDefinition->GetSourceType() == CigiBaseSymbolClone::SymbolSrc );
}




