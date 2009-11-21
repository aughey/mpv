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
 *  2008-01-14 Andrew Sampson, Boeing
 *      Merged some code from GDLS symbology rendering into new plugin; part 
 *      of this class is from OSGTextSymbol.  
 *
 *  2008-08-27 Philip Lowman, GDLS
 *      Added font lookup support for text symbol
 *
 * </pre>
 */

#include "BindSlot.h"

#include "SymbolTextImpOSG.h"

#include "GenericCache.h"
using namespace mpv;
using namespace mpvosg;

SymbolTextImpOSG::SymbolTextImpOSG( SymbolText *symbol ) : 
	SymbolImpOSG( symbol )
{
	textDrawable = new osgText::Text();
	
	// remove the stuff added to geode by SymbolImpOSG
	geode->removeDrawables( 0, geode->getNumDrawables() );

	geode->addDrawable( textDrawable.get() );

	// retrieve the symbol state and apply it
	colorChanged( symbol );
	alignmentChanged( symbol );
	orientationChanged( symbol );
	fontChanged( symbol );
	fontSizeChanged( symbol );
	textChanged( symbol );
	
	// set up slots for change notification
	symbol->colorChanged.connect( BIND_SLOT1( SymbolTextImpOSG::colorChanged, this ) );
	symbol->alignmentChanged.connect( BIND_SLOT1( SymbolTextImpOSG::alignmentChanged, this ) );
	symbol->orientationChanged.connect( BIND_SLOT1( SymbolTextImpOSG::orientationChanged, this ) );
	symbol->fontChanged.connect( BIND_SLOT1( SymbolTextImpOSG::fontChanged, this ) );
	symbol->fontSizeChanged.connect( BIND_SLOT1( SymbolTextImpOSG::fontSizeChanged, this ) );
	symbol->textChanged.connect( BIND_SLOT1( SymbolTextImpOSG::textChanged, this ) );
	
	// not using SymbolImpOSG's colorChanged
	symbol->colorChanged.disconnect( BIND_SLOT1( SymbolImpOSG::colorChanged, this ) );

}


SymbolTextImpOSG::~SymbolTextImpOSG()
{
	
}


void SymbolTextImpOSG::update( double timeElapsed )
{
	
}


void SymbolTextImpOSG::colorChanged( Symbol *symbol )
{
	Vect4 symbolColor = baseSymbol->getColor();
	
	textDrawable->setColor( osg::Vec4( 
		symbolColor.getx(),
		symbolColor.gety(),
		symbolColor.getz(),
		symbolColor.getw() ) );
}


void SymbolTextImpOSG::alignmentChanged( SymbolText *symbol )
{
	osgText::Text::AlignmentType alignment = osgText::Text::LEFT_BOTTOM;

	switch( symbol->getTextAlignment() )
	{
	case SymbolText::TopLeft:
		alignment = osgText::Text::LEFT_TOP;
		break;
	case SymbolText::TopCenter:
		alignment = osgText::Text::CENTER_TOP;
		break;
	case SymbolText::TopRight:
		alignment = osgText::Text::RIGHT_TOP;
		break;
	case SymbolText::CenterLeft:
		alignment = osgText::Text::LEFT_CENTER;
		break;
	case SymbolText::Center:
		alignment = osgText::Text::CENTER_CENTER;
		break;
	case SymbolText::CenterRight:
		alignment = osgText::Text::RIGHT_CENTER;
		break;
	case SymbolText::BottomLeft:
		alignment = osgText::Text::LEFT_BOTTOM;
		break;
	case SymbolText::BottomCenter:
		alignment = osgText::Text::CENTER_BOTTOM;
		break;
	case SymbolText::BottomRight:
		alignment = osgText::Text::RIGHT_BOTTOM;
		break;
	default:
		alignment = osgText::Text::LEFT_BOTTOM;
		break;
	}
	
	textDrawable->setAlignment( alignment );
}


void SymbolTextImpOSG::orientationChanged( SymbolText *symbol )
{
	osgText::Text::Layout layout = osgText::Text::LEFT_TO_RIGHT;

	switch( symbol->getTextOrientation() )
	{
	case SymbolText::LeftToRight:
		layout = osgText::Text::LEFT_TO_RIGHT;
		break;
	case SymbolText::TopToBottom:
		layout = osgText::Text::VERTICAL;
		break;
	case SymbolText::RightToLeft:
		layout = osgText::Text::RIGHT_TO_LEFT;
		break;
	case SymbolText::BottomToTop:
		// Note - not supported in osgText; falling back on top-to-bottom
		layout = osgText::Text::VERTICAL;
		break;
	default:
		layout = osgText::Text::LEFT_TO_RIGHT;
		break;
	}

	textDrawable->setLayout( layout );
}


void SymbolTextImpOSG::fontChanged( SymbolText *symbol )
{
	typedef GenericCache<Cigi_uint8, osgText::Font> FontCache;

	FontCache *font_cache = FontCache::instance();

	osgText::Font *font = font_cache->get( symbol->getFontID() );
	textDrawable->setFont(font);
}


void SymbolTextImpOSG::fontSizeChanged( SymbolText *symbol )
{
	textDrawable->setCharacterSize( symbol->getFontSize() );
}


void SymbolTextImpOSG::textChanged( SymbolText *symbol )
{
	textDrawable->setText( symbol->getText() );
}




