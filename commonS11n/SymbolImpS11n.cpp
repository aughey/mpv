/** <pre>
 *  MPV Serialization Framework
 *  Copyright (c) 2007 General Dynamics Land Systems
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


#include "SymbolImpS11n.h"


using namespace mpv;
using namespace mpvs11n;

SymbolImpS11n::SymbolImpS11n( Symbol *symbol ): SymbolImp( symbol )
{
	topGroup = new mpvs11n::GroupNode( "symbol" );
	
	id = new mpvs11n::IntNode( "id" );
	id->set( symbol->getID() );
	type = new mpvs11n::StringNode( "type" );
	type->set( "Unknown" );
	state = new mpvs11n::StringNode( "state" );
	parentID = new mpvs11n::IntNode( "parent_id" );
	surfaceID = new mpvs11n::IntNode( "surface_id" );
	layer = new mpvs11n::IntNode( "layer" );
	position = new mpvs11n::DoubleVectorNode( "position" );
	rotation = new mpvs11n::FloatNode( "rotation" );
	scale = new mpvs11n::DoubleVectorNode( "scale" );
	flashDutyCyclePercentage = new mpvs11n::IntNode( "flash_duty_cycle_percentage" );
	flashPeriod = new mpvs11n::FloatNode( "flash_period" );
	inheritColor = new mpvs11n::IntNode( "inherit_color" );
	colorRed = new mpvs11n::IntNode( "color_red" );
	colorGreen = new mpvs11n::IntNode( "color_green" );
	colorBlue = new mpvs11n::IntNode( "color_blue" );
	colorAlpha = new mpvs11n::IntNode( "color_alpha" );
	childSymbolGroup = new mpvs11n::GroupNode( "child_symbols" );
	
	topGroup->addChild( id.get() );
	topGroup->addChild( type.get() );
	topGroup->addChild( state.get() );
	topGroup->addChild( parentID.get() );
	topGroup->addChild( surfaceID.get() );
	topGroup->addChild( layer.get() );
	topGroup->addChild( position.get() );
	topGroup->addChild( rotation.get() );
	topGroup->addChild( scale.get() );
	topGroup->addChild( flashDutyCyclePercentage.get() );
	topGroup->addChild( flashPeriod.get() );
	topGroup->addChild( inheritColor.get() );
	topGroup->addChild( colorRed.get() );
	topGroup->addChild( colorGreen.get() );
	topGroup->addChild( colorBlue.get() );
	topGroup->addChild( colorAlpha.get() );
	topGroup->addChild( childSymbolGroup.get() );
	
	// retrieve the symbol state and apply it
	parentChanged( symbol );
	surfaceChanged( symbol );
	stateChanged( symbol );
	layerChanged( symbol );
	colorChanged( symbol );
	flashStateChanged( symbol );
	positionChanged( symbol );
	rotationChanged( symbol );
	scaleChanged( symbol );

	// set up slots for change notification
	symbol->colorChanged.connect( BIND_SLOT1( SymbolImpS11n::colorChanged, this ) );
	symbol->parentChanged.connect( BIND_SLOT1( SymbolImpS11n::parentChanged, this ) );
	symbol->flashStateChanged.connect( BIND_SLOT1( SymbolImpS11n::flashStateChanged, this ) );
	symbol->surfaceChanged.connect( BIND_SLOT1( SymbolImpS11n::surfaceChanged, this ) );
	symbol->stateChanged.connect( BIND_SLOT1( SymbolImpS11n::stateChanged, this ) );
	symbol->layerChanged.connect( BIND_SLOT1( SymbolImpS11n::layerChanged, this ) );
	symbol->positionChanged.connect( BIND_SLOT1( SymbolImpS11n::positionChanged, this ) );
	symbol->rotationChanged.connect( BIND_SLOT1( SymbolImpS11n::rotationChanged, this ) );
	symbol->scaleChanged.connect( BIND_SLOT1( SymbolImpS11n::scaleChanged, this ) );
	
	symbol->addedSymbol.connect( BIND_SLOT2( SymbolImpS11n::addedChildSymbol, this ) );
	symbol->removedSymbol.connect( BIND_SLOT2( SymbolImpS11n::removedChildSymbol, this ) );
	
	// handle existing child symbols (it's unlikely that the symbol has any 
	// children attached to it yet, but it's best to be robust)
	SymbolContainer::SymbolIteratorPair iterPair = symbol->getSymbols();
	SymbolContainer::SymbolMap::iterator iter;
	for( iter = iterPair.first; iter != iterPair.second; iter++ )
	{
		addedChildSymbol( symbol, iter->second.get() );
	}
}


SymbolImpS11n::~SymbolImpS11n()
{
	// sever links with children
	childSymbolGroup->removeAllChildren();
}


void SymbolImpS11n::parentChanged( Symbol *symbol )
{
	if( baseSymbol->getIsChild() )
		parentID->set( baseSymbol->getParentID() );
	else
		parentID->set( -1 );
}


void SymbolImpS11n::surfaceChanged( Symbol *symbol )
{
	surfaceID->set( baseSymbol->getSurfaceID() );
}


void SymbolImpS11n::stateChanged( Symbol *symbol )
{
	switch( baseSymbol->getState() )
	{
	case Symbol::Hidden:
		state->set( "Hidden" );
		break;
	case Symbol::Visible:
		state->set( "Visible" );
		break;
	case Symbol::Destroyed:
		state->set( "Destroyed" );
		break;
	default:
		state->set( "Unknown" );
		break;
	}
}


void SymbolImpS11n::layerChanged( Symbol *symbol )
{
	layer->set( baseSymbol->getLayer() );
}


void SymbolImpS11n::colorChanged( Symbol *symbol )
{
	inheritColor->set( baseSymbol->getInheritColor() );
	
	Vect4 symbolColor = baseSymbol->getColor();
	colorRed->set( (int)( symbolColor[0] * 255.0 ) );
	colorGreen->set( (int)( symbolColor[1] * 255.0 ) );
	colorBlue->set( (int)( symbolColor[2] * 255.0 ) );
	colorAlpha->set( (int)( symbolColor[3] * 255.0 ) );
}


void SymbolImpS11n::flashStateChanged( Symbol *symbol )
{
	// FIXME - can't currently retrieve these values from Symbol
	flashDutyCyclePercentage->set( 100 );
	flashPeriod->set( 0.0 );
}


void SymbolImpS11n::positionChanged( Symbol *symbol )
{
	position->set( baseSymbol->getPosition() );
}


void SymbolImpS11n::rotationChanged( Symbol *symbol )
{
	rotation->set( baseSymbol->getRotation() );
}


void SymbolImpS11n::scaleChanged( Symbol *symbol )
{
	scale->set( baseSymbol->getScale() );
}


mpvs11n::Node *SymbolImpS11n::getTopNode()
{
	return topGroup.get();
}



void SymbolImpS11n::addedChildSymbol( SymbolContainer *symbol, Symbol *childSymbol )
{
	SymbolImpS11n *symbolImpS11n = getSymbolImpS11nFromSymbol( childSymbol );
	if( symbolImpS11n == NULL )
	{
		std::cerr << "SymbolImpS11n::addedChildSymbol - warning - The newly-added child symbol either has no implementation object or has an implementation that isn't a symbolImpS11n.  Can't add new child to my scene graph.\n";
	}
	else
	{
		childSymbolGroup->addChild( symbolImpS11n->getTopNode() );
	}
}


void SymbolImpS11n::removedChildSymbol( SymbolContainer *symbol, Symbol *childSymbol )
{
	SymbolImpS11n *symbolImpS11n = getSymbolImpS11nFromSymbol( childSymbol );
	if( symbolImpS11n == NULL )
	{
		std::cerr << "SymbolImpS11n::removedChildSymbol - warning - The newly-removed child symbol either has no implementation object or has an implementation that isn't a symbolImpS11n.  Can't remove child from my scene graph.\n";
	}
	else
	{
		childSymbolGroup->removeChild( symbolImpS11n->getTopNode() );
	}
}


// ================================================
// getSymbolImpS11nFromSymbol
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
SymbolImpS11n *SymbolImpS11n::getSymbolImpS11nFromSymbol( Symbol *symbol )
{
	SymbolImpS11n *symbolImpS11n = NULL;
	
	if( symbol == NULL )
		return NULL;
	
	Symbol::SymbolImpIteratorPair iterPair = symbol->getImplementations();
	Symbol::SymbolImpList::iterator iter;
	for( iter = iterPair.first; iter != iterPair.second; iter++ )
	{
		// RTTI will hopefully protect us if imp is something 
		// other than a SymbolImpS11n
		symbolImpS11n = dynamic_cast<SymbolImpS11n*>(iter->get());
		if( symbolImpS11n != NULL )
		{
			break;
		}
	}

	return symbolImpS11n;
}


