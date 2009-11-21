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


#ifndef SYMBOLIMPS11N_H
#define SYMBOLIMPS11N_H

#include <string>

#include "BindSlot.h"
#include "Symbol.h"

#include "DoubleVectorNode.h"
#include "FloatNode.h"
#include "GroupNode.h"
#include "IntNode.h"
#include "StringNode.h"

namespace mpvs11n
{

//=========================================================
//! A class for common code that all of the SymbolImps inherit.
//! 
class SymbolImpS11n : public mpv::SymbolImp
{
public:

	SymbolImpS11n( mpv::Symbol *symbol );

	virtual void parentChanged( mpv::Symbol *symbol );

	virtual void surfaceChanged( mpv::Symbol *symbol );

	virtual void stateChanged( mpv::Symbol *symbol );

	virtual void layerChanged( mpv::Symbol *symbol );

	virtual void colorChanged( mpv::Symbol *symbol );

	virtual void flashStateChanged( mpv::Symbol *symbol );

	virtual void positionChanged( mpv::Symbol *symbol );

	virtual void rotationChanged( mpv::Symbol *symbol );

	virtual void scaleChanged( mpv::Symbol *symbol );
	
	mpvs11n::Node *getTopNode();
	
	void addedChildSymbol( mpv::SymbolContainer *symbol, mpv::Symbol *childSymbol );
	
	void removedChildSymbol( mpv::SymbolContainer *symbol, mpv::Symbol *childSymbol );

	//=========================================================
	//! Given a Symbol, this method performs various casts to extract 
	//! the implementation object (of type SymbolImpS11n).  Relies on 
	//! RTTI.  Returns NULL if the symbol's implementation object is not 
	//! the correct type, or does not exist.
	//! \param symbol - the symbol from which to retrieve the implementation
	//! \return a pointer to the implementation object, or NULL on error
	//!
	static SymbolImpS11n *getSymbolImpS11nFromSymbol( mpv::Symbol *symbol );

protected:

	virtual ~SymbolImpS11n();

	mpv::RefPtr<mpvs11n::GroupNode> topGroup;
	
	mpv::RefPtr<mpvs11n::IntNode> id;
	mpv::RefPtr<mpvs11n::StringNode> type; // child classes will need to update this
	mpv::RefPtr<mpvs11n::StringNode> state;

	mpv::RefPtr<mpvs11n::IntNode> parentID;
	mpv::RefPtr<mpvs11n::IntNode> surfaceID;
	mpv::RefPtr<mpvs11n::IntNode> layer;
	mpv::RefPtr<mpvs11n::DoubleVectorNode> position;
	mpv::RefPtr<mpvs11n::FloatNode> rotation;
	mpv::RefPtr<mpvs11n::DoubleVectorNode> scale;
	
	mpv::RefPtr<mpvs11n::IntNode> flashDutyCyclePercentage;
	mpv::RefPtr<mpvs11n::FloatNode> flashPeriod;
	mpv::RefPtr<mpvs11n::IntNode> inheritColor;
	mpv::RefPtr<mpvs11n::IntNode> colorRed;
	mpv::RefPtr<mpvs11n::IntNode> colorGreen;
	mpv::RefPtr<mpvs11n::IntNode> colorBlue;
	mpv::RefPtr<mpvs11n::IntNode> colorAlpha;

	//=========================================================
	//! Attachment point for child symbols
	//! 
	mpv::RefPtr<mpvs11n::GroupNode> childSymbolGroup;
	
};

}
#endif
