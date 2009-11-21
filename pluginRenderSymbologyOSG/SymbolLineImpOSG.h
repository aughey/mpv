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
 *  2008-02-13 Andrew Sampson, Boeing
 *      Merged some code from GDLS symbology rendering into new plugin; part 
 *      of this class is from OSGPolyLineSymbol, etc.  
 *
 * </pre>
 */


#ifndef SYMBOLLINEIMPOSG_H
#define SYMBOLLINEIMPOSG_H

#include <string>
#include <osg/PositionAttitudeTransform>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>

#include "SymbolImpOSG.h"
#include "SymbolLine.h"

//=========================================================
//! 
//! 
class SymbolLineImpOSG : public mpvosg::SymbolImpOSG
{
public:

	SymbolLineImpOSG( mpv::SymbolLine *symbol );

	virtual void update( double timeElapsed );

	void lineStyleChanged( mpv::SymbolLine *symbol );

	void verticesChanged( mpv::SymbolLine *symbol );

protected:

	virtual ~SymbolLineImpOSG();
	
};

#endif
