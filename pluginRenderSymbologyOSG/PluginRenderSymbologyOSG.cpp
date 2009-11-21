/** <pre>
 * The MPV Symbology Plugin Collection
 * Copyright (c) 2007 General Dynamics Land Systems
 * Copyright (c) 2008 The Boeing Company
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *  FILENAME:   PluginRenderSymbologyOSG.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  render symbology.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME
 *  DESCRIPTION OF CHANGE........................
 *  
 *  2007-03-30 Philip Lowman, GDLS
 *      Initial release
 *  
 *  2007-09-26 Philip Lowman
 *      Modified factory member variable so that SymbologyFactory is published
 *      via new blackboard API instead of OSGSymbologyFactory.
 *
 *  2008-01-16 Andrew Sampson, Boeing
 *      Merged some code from GDLS symbology rendering into new plugin; part 
 *      of this class is from PluginRenderSymbologyOSG.  
 *
 *  2008-08-27 Philip Lowman, GDLS
 *      Added font lookup support for text symbol
 *  
 * </pre>
 */

#include <cassert>
#include <iostream>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/MatrixTransform>
#include <osg/Projection>

#include "MPVExceptions.h"
#include "SymbolText.h"
#include "SymbolCircle.h"
#include "SymbolLine.h"

#include "SymbolSurfaceImpOSG.h"
#include "SymbolTextImpOSG.h"
#include "SymbolCircleImpOSG.h"
#include "SymbolLineImpOSG.h"
#include "GenericCache.h"

#include "PluginRenderSymbologyOSG.h"

EXPORT_DYNAMIC_CLASS( PluginRenderSymbologyOSG )

// ================================================
// PluginRenderSymbologyOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderSymbologyOSG::PluginRenderSymbologyOSG() : Plugin()
{
	name_ = "PluginRenderSymbologyOSG";

	licenseInfo_.setLicense( LicenseInfo::LicenseLGPL );
	licenseInfo_.setOrigin( "GeneralDynamics+Boeing" );

	dependencies_.push_back( "PluginSymbologyMgr" );
	dependencies_.push_back( "PluginViewMgr" );
	dependencies_.push_back( "PluginDefFileReader" );
	dependencies_.push_back( "PluginRenderOSG" );

	rootNode = NULL;
	DefFileData = NULL;
	symbols = NULL;
	symbolSurfaces = NULL;
	viewParamsMap = NULL;

	surfaceObserver = new SymbolSurfaceObserver;
	symbolObserver = new SymbolObserver;
}

// ================================================
// ~PluginRenderSymbologyOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderSymbologyOSG::~PluginRenderSymbologyOSG() throw()
{
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderSymbologyOSG::act( SystemState::ID state, StateContext &stateContext )
{
	switch ( state )
	{
	case SystemState::BlackboardPost:
		// This state is for posting things to the blackboard and other
		// initialization tasks
		break;

	case SystemState::BlackboardRetrieve:
		// This state is for retrieving things from the blackboard

		// Get the root node of OSG
		bb_->get( "RootNodeOSG", rootNode );

		// Get the Def file data
		bb_->get( "DefinitionData", DefFileData );

		// Get the symbol container
		bb_->get( "Symbols", symbols );

		// Get the symbol surface container
		bb_->get( "SymbolSurfaces", symbolSurfaces );

		// connect to signals, for notification of new symbols and 
		// new symbol surfaces
		symbolSurfaces->addedSymbolSurface.connect( BIND_SLOT2( SymbolSurfaceObserver::addedSymbolSurface, surfaceObserver.get() ) );
		symbols->addedSymbol.connect( BIND_SLOT2( SymbolObserver::addedSymbol, symbolObserver.get() ) );

		break;

	case SystemState::ConfigurationProcess:
		get_config();
		break;

	case SystemState::Operate:
	case SystemState::Debug:
		operate();
		break;

	default:
		break;
	}

}

// ================================================
// get_config
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderSymbologyOSG::get_config( void )
{
	DefFileGroup * root = *DefFileData;
	if ( root == NULL )
	{
		throw MPVPluginInitException( "DefFileData not available" );
	}

	DefFileGroup *fontsGroup = root->getGroupByURI( "/fonts/" );
	if ( fontsGroup == NULL )
	{
		std::cerr << "WARNING: fonts group not found in DEF files, all symbology text symbols will have default font" << std::endl;
	}
	else
	{
		typedef GenericCache<Cigi_uint8, osgText::Font> FontCache;
		FontCache *font_cache = FontCache::instance();

		std::list<DefFileGroup*>::iterator ditr;
		for(ditr = fontsGroup->children.begin();
				ditr != fontsGroup->children.end();
				++ditr)
		{

			DefFileAttrib *attr = 0;

			Cigi_uint8 id;
			std::string fname;

			attr = (*ditr)->getAttribute( "id" );
			if(attr)
			{
				if(attr->asInt() < 0 || attr->asInt() > 255)
				{
					std::cerr << "ERROR! id " << attr->asInt() << " outside allowable range" << std::endl;
					continue;
				}
				id = Cigi_uint8(attr->asInt());
			}
			else
			{
				std::cerr << "ERROR! font entry lacks id attribute!" << std::endl;
				continue;
			}

			attr = (*ditr)->getAttribute( "fname" );
			if(attr)
			{
				fname = attr->asString();
				if(fname.length() == 0)
				{
					std::cerr << "ERROR! font entry lacks fname!" << std::endl;
					continue;
				}
			}
			else
			{
				std::cerr << "ERROR! font entry lacks fname!" << std::endl;
				continue;
			}

			osgText::Font *font = osgText::readFontFile(fname);
			if(font)
			{
				font_cache->add(id, font);
			}
			else
			{
				std::cerr << "ERROR! Unable to read font " << fname << " (id " << int(id) << ")" << std::endl;
				continue;
			}
		}
	}
}


// ================================================
// operate
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderSymbologyOSG::operate( void )
{
	
}


// ================================================
// addedSymbolSurface
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderSymbologyOSG::SymbolSurfaceObserver::addedSymbolSurface( 
	SymbolSurfaceContainer *, SymbolSurface *surface )
{
	if( surface == NULL )
	{
		// error - why is this surface NULL?
		return;
	}

	// sanity check
	if( surface->getState() == SymbolSurface::Destroyed )
		return;

	SymbolSurfaceImpOSG *implementation = SymbolSurfaceImpOSG::getSymbolSurfaceImpOSGFromSymbolSurface( surface );
	if( implementation == NULL )
	{
//std::cerr << "PluginRenderSymbologyOSG::SymbolSurfaceObserver::addedSymbolSurface - Adding a new symbol surf imp to surface " << surface->getID() << std::endl;
		surface->addImplementation( new SymbolSurfaceImpOSG(surface) );
	}

}


// ================================================
// addedSymbol
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderSymbologyOSG::SymbolObserver::addedSymbol( 
	SymbolContainer *, Symbol *symbol )
{
	if( symbol == NULL )
	{
		// error - why is this symbol NULL?
		return;
	}

	// sanity check
	if( symbol->getState() == Symbol::Destroyed )
		return;

	SymbolImpOSG *implementation = SymbolImpOSG::getSymbolImpOSGFromSymbol( symbol );
	if( implementation == NULL )
	{
//std::cerr << "PluginRenderSymbologyOSG::SymbolObserver::addedSymbol - Adding a new symbol imp to symbol " << symbol->getID() << std::endl;
		switch( symbol->getType() )
		{
		case Symbol::Text:
			symbol->addImplementation( new SymbolTextImpOSG( static_cast<SymbolText*>(symbol) ) );
			break;
		case Symbol::Circle:
			symbol->addImplementation( new SymbolCircleImpOSG( static_cast<SymbolCircle*>(symbol) ) );
			break;
		case Symbol::Line:
			symbol->addImplementation( new SymbolLineImpOSG( static_cast<SymbolLine*>(symbol) ) );
			break;
		default:
			break;
		}
	}

}

