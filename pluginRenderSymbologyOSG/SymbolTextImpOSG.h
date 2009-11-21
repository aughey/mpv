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
 * </pre>
 */


#ifndef SYMBOLTEXTIMPOSG_H
#define SYMBOLTEXTIMPOSG_H

#include <string>
#include <osg/PositionAttitudeTransform>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osgText/Text>

#include "SymbolImpOSG.h"
#include "SymbolText.h"

//=========================================================
//! 
//! 
class SymbolTextImpOSG : public mpvosg::SymbolImpOSG
{
public:

	SymbolTextImpOSG( mpv::SymbolText *symbol );

	virtual void update( double timeElapsed );

	// Can't use the SymbolImpOSG version; this class doesn't use 
	// SymbolImpOSG::geometry
	virtual void colorChanged( mpv::Symbol *symbol );

	virtual void alignmentChanged( mpv::SymbolText *symbol );

	virtual void orientationChanged( mpv::SymbolText *symbol );

	virtual void fontChanged( mpv::SymbolText *symbol );

	virtual void fontSizeChanged( mpv::SymbolText *symbol );

	virtual void textChanged( mpv::SymbolText *symbol );

protected:

	virtual ~SymbolTextImpOSG();
	
	osg::ref_ptr<osgText::Text> textDrawable;
};

#endif
