/** <pre>
 *  The MPV Symbology Plugin Collection
 *  Copyright (c) 2007 General Dynamics Land Systems
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
 *  2007-03-30 Philip Lowman, GDLS
 *      Initial release
 *  
 *  2008-01-10 Andrew Sampson, Boeing
 *      Merged some code from GDLS symbology rendering into new plugin; part 
 *      of this class is from OSGSymbolHelper.  
 *
 * </pre>
 */

#include <iostream>


#include "SymbolImpOSG.h"
#include "MiscOSG.h"


using namespace mpv;
using namespace mpvosg;

SymbolImpOSG::SymbolImpOSG( Symbol *symbol ): SymbolImp( symbol )
{
	transform = new osg::PositionAttitudeTransform();
	geode = new osg::Geode();
	geometry = new osg::Geometry();
	stateSet = geode->getOrCreateStateSet();

	geodeSwitch = new osg::Switch();
	
	geode->addDrawable( geometry.get() );
	geodeSwitch->addChild( geode.get() );
	transform->addChild( geodeSwitch.get() );
	
	childSymbolGroup = new osg::Group();
	transform->addChild( childSymbolGroup.get() );
	
	transform->setName( "Symbol" );

	// Initialize color to white
	geometry->setColorBinding( osg::Geometry::BIND_OVERALL );
	colorArray = new osg::Vec4Array();
	color.set( 1.0f, 1.0f, 1.0f, 1.0f );
	colorArray->push_back( color );
	geometry->setColorArray( colorArray.get() );

//	texture
//	textureFilename
	
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
	symbol->colorChanged.connect( BIND_SLOT1( SymbolImpOSG::colorChanged, this ) );
	symbol->parentChanged.connect( BIND_SLOT1( SymbolImpOSG::parentChanged, this ) );
	symbol->flashStateChanged.connect( BIND_SLOT1( SymbolImpOSG::flashStateChanged, this ) );
	symbol->surfaceChanged.connect( BIND_SLOT1( SymbolImpOSG::surfaceChanged, this ) );
	symbol->stateChanged.connect( BIND_SLOT1( SymbolImpOSG::stateChanged, this ) );
	symbol->layerChanged.connect( BIND_SLOT1( SymbolImpOSG::layerChanged, this ) );
	symbol->positionChanged.connect( BIND_SLOT1( SymbolImpOSG::positionChanged, this ) );
	symbol->rotationChanged.connect( BIND_SLOT1( SymbolImpOSG::rotationChanged, this ) );
	symbol->scaleChanged.connect( BIND_SLOT1( SymbolImpOSG::scaleChanged, this ) );
	
	symbol->addedSymbol.connect( BIND_SLOT2( SymbolImpOSG::addedChildSymbol, this ) );
	symbol->removedSymbol.connect( BIND_SLOT2( SymbolImpOSG::removedChildSymbol, this ) );
	
	// handle existing child symbols (it's unlikely that the symbol has any 
	// children attached to it yet, but it's best to be robust)
	SymbolContainer::SymbolIteratorPair iterPair = symbol->getSymbols();
	SymbolContainer::SymbolMap::iterator iter;
	for( iter = iterPair.first; iter != iterPair.second; iter++ )
	{
		addedChildSymbol( symbol, iter->second.get() );
	}
}


SymbolImpOSG::~SymbolImpOSG()
{
	// sever links with parent(s)
//	detachFromParentNodes();
	// sever links with children
	childSymbolGroup->removeChildren( 0, childSymbolGroup->getNumChildren() );
}


void SymbolImpOSG::parentChanged( Symbol *symbol )
{
}


void SymbolImpOSG::surfaceChanged( Symbol *symbol )
{
}


void SymbolImpOSG::stateChanged( Symbol *symbol )
{
	updateSwitch();
}


void SymbolImpOSG::layerChanged( Symbol *symbol )
{
	stateSet->setRenderBinDetails( baseSymbol->getLayer(), "RenderBin" );
}


void SymbolImpOSG::colorChanged( Symbol *symbol )
{
	Vect4 symbolColor = baseSymbol->getColor();
	color.set( 
		symbolColor.getx(),
		symbolColor.gety(),
		symbolColor.getz(),
		symbolColor.getw() );
	colorArray->clear();
	colorArray->push_back( color );
	geometry->setColorArray( colorArray.get() );
}


void SymbolImpOSG::flashStateChanged( Symbol *symbol )
{
	updateSwitch();
}


void SymbolImpOSG::positionChanged( Symbol *symbol )
{
	Vect2 position = baseSymbol->getPosition();
	transform->setPosition( osg::Vec3d( position.getx(), position.gety(), 0.0 ) );
}


void SymbolImpOSG::rotationChanged( Symbol *symbol )
{
	float rotation = osg::DegreesToRadians( baseSymbol->getRotation() );
	osg::Quat quat;
	quat.makeRotate( rotation, 0, 0, 1 );
	transform->setAttitude( quat );
}


void SymbolImpOSG::scaleChanged( Symbol *symbol )
{
	Vect2 scale = baseSymbol->getScale();
	transform->setScale( osg::Vec3d( scale.getx(), scale.gety(), 1.0 ) );
}


osg::Node *SymbolImpOSG::getTopNode()
{
	return transform.get();
}



void SymbolImpOSG::addedChildSymbol( SymbolContainer *symbol, Symbol *childSymbol )
{
	SymbolImpOSG *symbolImpOSG = getSymbolImpOSGFromSymbol( childSymbol );
	if( symbolImpOSG == NULL )
	{
		std::cerr << "SymbolImpOSG::addedChildSymbol - warning - The newly-added child symbol either has no implementation object or has an implementation that isn't a symbolImpOSG.  Can't add new child to my scene graph.\n";
	}
	else
	{
		childSymbolGroup->addChild( symbolImpOSG->getTopNode() );
	}
}


void SymbolImpOSG::removedChildSymbol( SymbolContainer *symbol, Symbol *childSymbol )
{
	SymbolImpOSG *symbolImpOSG = getSymbolImpOSGFromSymbol( childSymbol );
	if( symbolImpOSG == NULL )
	{
		std::cerr << "SymbolImpOSG::removedChildSymbol - warning - The newly-removed child symbol either has no implementation object or has an implementation that isn't a symbolImpOSG.  Can't remove child from my scene graph.\n";
	}
	else
	{
		childSymbolGroup->removeChild( symbolImpOSG->getTopNode() );
	}
}


// ================================================
// getSymbolImpOSGFromSymbol
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
SymbolImpOSG *SymbolImpOSG::getSymbolImpOSGFromSymbol( Symbol *symbol )
{
	SymbolImpOSG *symbolImpOSG = NULL;
	
	if( symbol == NULL )
		return NULL;
	
	Symbol::SymbolImpIteratorPair iterPair = symbol->getImplementations();
	Symbol::SymbolImpList::iterator iter;
	for( iter = iterPair.first; iter != iterPair.second; iter++ )
	{
		// RTTI will hopefully protect us if imp is something 
		// other than a SymbolImpOSG
		symbolImpOSG = dynamic_cast<SymbolImpOSG*>(iter->get());
		if( symbolImpOSG != NULL )
		{
			break;
		}
	}

	return symbolImpOSG;
}


void SymbolImpOSG::updateSwitch()
{
	bool enabled = (baseSymbol->getState() == Symbol::Visible);
	bool flashState = baseSymbol->getFlashState();
	
	if( enabled && flashState )
		geodeSwitch->setAllChildrenOn();
	else
		geodeSwitch->setAllChildrenOff();
}

