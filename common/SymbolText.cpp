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
 *  2008-01-10 Andrew Sampson
 *      Initial Release.
 * </pre>
 */

#include "SymbolText.h"

using namespace mpv;


SymbolText::SymbolText() : Symbol(),
	alignment( BottomLeft ),
	orientation( LeftToRight ),
	fontID( 0 ),
	fontSize( 10.0 ),
	text( "" )
{
}


SymbolText::~SymbolText()
{
}



Symbol::SymbolType SymbolText::getType() const
{
	return Symbol::Text;
}


	
void SymbolText::setTextAlignment( SymbolText::SymbolTextAlignment newAlignment )
{
	if( alignment != newAlignment )
	{
		alignment = newAlignment;
		alignmentChanged( this );
	}
}



SymbolText::SymbolTextAlignment SymbolText::getTextAlignment() const
{
	return alignment;
}



void SymbolText::setTextOrientation( SymbolText::SymbolTextOrientation newOrientation )
{
	if( orientation != newOrientation )
	{
		orientation = newOrientation;
		orientationChanged( this );
	}
}



SymbolText::SymbolTextOrientation SymbolText::getTextOrientation() const
{
	return orientation;
}



void SymbolText::setFontID( Cigi_uint8 newFontID )
{
	if( fontID != newFontID )
	{
		fontID = newFontID;
		fontChanged( this );
	}
}



Cigi_uint8 SymbolText::getFontID() const
{
	return fontID;
}



void SymbolText::setFontSize( float newFontSize )
{
	if( fontSize != newFontSize )
	{
		fontSize = newFontSize;
		fontSizeChanged( this );
	}
}



float SymbolText::getFontSize() const
{
	return fontSize;
}



void SymbolText::setText( const std::string &newText )
{
	if( text != newText )
	{
		text = newText;
		textChanged( this );
	}
}



std::string SymbolText::getText() const
{
	return text;
}


