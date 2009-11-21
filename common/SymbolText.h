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

#ifndef SymbolText_h
#define SymbolText_h

#include "MPVCommonTypes.h"

#include <string>

#include <CigiBaseSymbolTextDef.h>

#include "Symbol.h"

namespace mpv
{

//=========================================================
//! Text symbol class.  Inherits from Symbol.
//! 
class MPVCMN_SPEC SymbolText : public Symbol
{

public:

	enum SymbolTextAlignment
	{
		TopLeft = CigiBaseSymbolTextDef::TopLeft,
		TopCenter = CigiBaseSymbolTextDef::TopCenter,
		TopRight = CigiBaseSymbolTextDef::TopRight,
		CenterLeft = CigiBaseSymbolTextDef::CenterLeft,
		Center = CigiBaseSymbolTextDef::Center,
		CenterRight = CigiBaseSymbolTextDef::CenterRight,
		BottomLeft = CigiBaseSymbolTextDef::BottomLeft,
		BottomCenter = CigiBaseSymbolTextDef::BottomCenter,
		BottomRight = CigiBaseSymbolTextDef::BottomRight
	};
	
	enum SymbolTextOrientation
	{
		LeftToRight = CigiBaseSymbolTextDef::LeftToRight,
		TopToBottom = CigiBaseSymbolTextDef::TopToBottom,
		RightToLeft = CigiBaseSymbolTextDef::RightToLeft,
		BottomToTop = CigiBaseSymbolTextDef::BottomToTop
	};

public:

	boost::signal<void (SymbolText*)> alignmentChanged;
	boost::signal<void (SymbolText*)> orientationChanged;
	boost::signal<void (SymbolText*)> fontChanged;
	boost::signal<void (SymbolText*)> fontSizeChanged;
	boost::signal<void (SymbolText*)> textChanged;


	SymbolText();
	
	virtual SymbolType getType() const;
	
	void setTextAlignment( SymbolTextAlignment newAlignment );

	SymbolTextAlignment getTextAlignment() const;

	void setTextOrientation( SymbolTextOrientation newOrientation );

	SymbolTextOrientation getTextOrientation() const;

	void setFontID( Cigi_uint8 newFontID );

	Cigi_uint8 getFontID() const;

	void setFontSize( float newFontSize );

	float getFontSize() const;

	void setText( const std::string &newText );

	std::string getText() const;

protected:

	virtual ~SymbolText();


protected:
	SymbolTextAlignment alignment;
	SymbolTextOrientation orientation;
	Cigi_uint8 fontID;
	float fontSize;
	std::string text;
};

}
#endif
