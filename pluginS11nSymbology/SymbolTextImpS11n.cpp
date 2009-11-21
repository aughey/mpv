/** <pre>
 *  MPV Serialization Framework
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
 *      Initial release
 *
 * </pre>
 */

#include "BindSlot.h"

#include "SymbolTextImpS11n.h"

using namespace mpv;
using namespace mpvs11n;

SymbolTextImpS11n::SymbolTextImpS11n( SymbolText *symbol ) : 
	SymbolImpS11n( symbol )
{
	type->set( "Text" ); // "type" exists in parent class
	
	alignment = new mpvs11n::StringNode( "alignment" );
	orientation = new mpvs11n::StringNode( "orientation" );
	fontID = new mpvs11n::IntNode( "font_id" );
	fontSize = new mpvs11n::FloatNode( "font_size" );
	text = new mpvs11n::StringNode( "text" );
	
	topGroup->addChild( alignment.get() );
	topGroup->addChild( orientation.get() );
	topGroup->addChild( fontID.get() );
	topGroup->addChild( fontSize.get() );
	topGroup->addChild( text.get() );
	
	// retrieve the symbol state and apply it
	alignmentChanged( symbol );
	orientationChanged( symbol );
	fontChanged( symbol );
	fontSizeChanged( symbol );
	textChanged( symbol );
	
	// set up slots for change notification
	symbol->alignmentChanged.connect( BIND_SLOT1( SymbolTextImpS11n::alignmentChanged, this ) );
	symbol->orientationChanged.connect( BIND_SLOT1( SymbolTextImpS11n::orientationChanged, this ) );
	symbol->fontChanged.connect( BIND_SLOT1( SymbolTextImpS11n::fontChanged, this ) );
	symbol->fontSizeChanged.connect( BIND_SLOT1( SymbolTextImpS11n::fontSizeChanged, this ) );
	symbol->textChanged.connect( BIND_SLOT1( SymbolTextImpS11n::textChanged, this ) );
}


SymbolTextImpS11n::~SymbolTextImpS11n()
{
	
}


void SymbolTextImpS11n::update( double timeElapsed )
{
	
}


void SymbolTextImpS11n::alignmentChanged( SymbolText *symbol )
{
	switch( symbol->getTextAlignment() )
	{
	case SymbolText::TopLeft:
		alignment->set( "TopLeft" );
		break;
	case SymbolText::TopCenter:
		alignment->set( "TopCenter" );
		break;
	case SymbolText::TopRight:
		alignment->set( "TopRight" );
		break;
	case SymbolText::CenterLeft:
		alignment->set( "CenterLeft" );
		break;
	case SymbolText::Center:
		alignment->set( "Center" );
		break;
	case SymbolText::CenterRight:
		alignment->set( "CenterRight" );
		break;
	case SymbolText::BottomLeft:
		alignment->set( "BottomLeft" );
		break;
	case SymbolText::BottomCenter:
		alignment->set( "BottomCenter" );
		break;
	case SymbolText::BottomRight:
		alignment->set( "BottomRight" );
		break;
	default:
		alignment->set( "Unknown" );
		break;
	}
}


void SymbolTextImpS11n::orientationChanged( SymbolText *symbol )
{
	switch( symbol->getTextOrientation() )
	{
	case SymbolText::LeftToRight:
		orientation->set( "LeftToRight" );
		break;
	case SymbolText::TopToBottom:
		orientation->set( "TopToBottom" );
		break;
	case SymbolText::RightToLeft:
		orientation->set( "RightToLeft" );
		break;
	case SymbolText::BottomToTop:
		orientation->set( "BottomToTop" );
		break;
	default:
		orientation->set( "Unknown" );
		break;
	}
}


void SymbolTextImpS11n::fontChanged( SymbolText *symbol )
{
	fontID->set( symbol->getFontID() );
}


void SymbolTextImpS11n::fontSizeChanged( SymbolText *symbol )
{
	fontSize->set( symbol->getFontSize() );
}


void SymbolTextImpS11n::textChanged( SymbolText *symbol )
{
	text->set( symbol->getText() );
}


