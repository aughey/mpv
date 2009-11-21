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


#ifndef SYMBOLIMPOSG_H
#define SYMBOLIMPOSG_H

#include <string>
#include <osg/PositionAttitudeTransform>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Switch>
#include <osg/Texture2D>

#include "BindSlot.h"
#include "Symbol.h"
#include "MPVCommonOSGTypes.h"


namespace mpvosg
{

//=========================================================
//! A class for common code that all of the SymbolImps inherit.
//! 
class MPVCMNOSG_SPEC SymbolImpOSG : public mpv::SymbolImp
{
public:

	SymbolImpOSG( mpv::Symbol *symbol );

	virtual void parentChanged( mpv::Symbol *symbol );

	virtual void surfaceChanged( mpv::Symbol *symbol );

	virtual void stateChanged( mpv::Symbol *symbol );

	virtual void layerChanged( mpv::Symbol *symbol );

	virtual void colorChanged( mpv::Symbol *symbol );

	virtual void flashStateChanged( mpv::Symbol *symbol );

	virtual void positionChanged( mpv::Symbol *symbol );

	virtual void rotationChanged( mpv::Symbol *symbol );

	virtual void scaleChanged( mpv::Symbol *symbol );
	
	osg::Node *getTopNode();
	
	void addedChildSymbol( mpv::SymbolContainer *symbol, mpv::Symbol *childSymbol );
	
	void removedChildSymbol( mpv::SymbolContainer *symbol, mpv::Symbol *childSymbol );

	//=========================================================
	//! Given a Symbol, this method performs various casts to extract 
	//! the implementation object (of type SymbolImpOSG).  Relies on 
	//! RTTI.  Returns NULL if the symbol's implementation object is not 
	//! the correct type, or does not exist.
	//! \param symbol - the symbol from which to retrieve the implementation
	//! \return a pointer to the implementation object, or NULL on error
	//!
	static SymbolImpOSG *getSymbolImpOSGFromSymbol( mpv::Symbol *symbol );

protected:

	virtual ~SymbolImpOSG();

	osg::ref_ptr<osg::PositionAttitudeTransform> transform;
	osg::ref_ptr<osg::Switch> geodeSwitch;
	osg::ref_ptr<osg::Geode> geode;
	osg::ref_ptr<osg::Geometry> geometry;
	osg::ref_ptr<osg::StateSet> stateSet;
	
	osg::ref_ptr<osg::Group> childSymbolGroup;

	osg::ref_ptr<osg::Vec4Array> colorArray;
	osg::Vec4d color;

	osg::ref_ptr<osg::Texture2D> texture;
	std::string textureFilename;
	
	void updateSwitch();
};

}
#endif
