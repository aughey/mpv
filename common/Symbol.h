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

#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#include <list>

#include <CigiTypes.h>
#include <CigiBaseSymbolCtrl.h>

#include "Referenced.h"
#include "Vect2.h"
#include "Vect4.h"
#include "ComponentContainer.h"
#include "SymbolContainer.h"
#include "MPVCommonTypes.h"


namespace mpv
{

// forward declaration; defined at bottom of file
class SymbolImp;


//=========================================================
//! This class serves as a base for all of the symbol types.  It contains 
//! state, as sent by the Host, such as symbol position, color, etc.  It 
//! also contains state pertaining to the symbol hierarchy.
//! Symbol inherits from SymbolContainer so that child symbols can be 
//! attached.  Child symbols are added via addSymbol(), removed via 
//! removeSymbol(), etc.  Child symbol addition and removal can be monitored 
//! using the signals provided by SymbolContainer.
//! 
class MPVCMN_SPEC Symbol : 
			public ComponentContainer,
			public SymbolContainer
{
public:


	class FlashState
	{

	public:
		boost::signal<void ()> stateChanged;
		
		FlashState();
		
		~FlashState();

		void update( double timeElapsed );

		void resetSequence();

		void setDutyCyclePercentage( Cigi_uint8 newPercentage );

		void setPeriod( float newPeriod );

		bool getState() const;

		bool isFlashing() const;


	protected:
		Cigi_uint8 dutyCyclePercentage;
		float period;
		float timeNow;
		bool on;
	};
	

	enum SymbolType
	{
		Unknown,
		Text,
		Circle,
		Line
	};
	
	
	enum SymbolState
	{
		Hidden = CigiBaseSymbolCtrl::Hidden,
		Visible = CigiBaseSymbolCtrl::Visible,
		Destroyed = CigiBaseSymbolCtrl::Destroyed
	};

	typedef std::list< RefPtr<SymbolImp> > SymbolImpList;
	typedef std::pair< SymbolImpList::iterator, SymbolImpList::iterator > SymbolImpIteratorPair;

public:

	boost::signal<void (Symbol*)> colorChanged;
	boost::signal<void (Symbol*)> parentChanged;
	boost::signal<void (Symbol*)> flashStateChanged;
	boost::signal<void (Symbol*)> surfaceChanged;
	boost::signal<void (Symbol*)> stateChanged;
	boost::signal<void (Symbol*)> layerChanged;
	boost::signal<void (Symbol*)> positionChanged;
	boost::signal<void (Symbol*)> rotationChanged;
	boost::signal<void (Symbol*)> scaleChanged;

	Symbol();
	
	virtual void update( double timeElapsed );

	void setID( Cigi_uint16 newID );

	Cigi_uint16 getID() const;

	void setInheritColor( bool newInheritColor );

	bool getInheritColor() const;

	void setState( SymbolState newState );

	SymbolState getState() const;
	
	//! 
	void setFullControlPacketRecvd( bool truth );

	bool getFullControlPacketRecvd() const;

	bool getIsChild() const;

	Cigi_uint16 getParentID() const;

	//! This method sets isChild, parentID, and parent all at once, 
	//! rather than setting these attributes through separate methods.
	//! The reason for this is so that the operation is atomic, and hence 
	//! there is no chance of putting the symbol in an inconsistent state 
	//! (like if isChild were set to true, but parent were set to NULL).
	//! \param newIsChild - if set to true, the symbol will be a child; if 
	//!        false, the other parameters will be ignored
	//! \param newParentID - 
	//! \param newParent - 
	//! 
	void setParent( 
		bool newIsChild, Cigi_uint16 newParentID, Symbol *newParent );

	Symbol * getParent() const;

	void setSurfaceID( Cigi_uint16 newSurfaceID );

	Cigi_uint16 getSurfaceID() const;

	void setLayer( Cigi_uint8 newLayer );

	Cigi_uint8 getLayer() const;

	void setPosition( float u, float v );

	void setPosition( const Vect2 &pos );

	Vect2 getPosition() const;

	void setRotation( float newRotation );

	float getRotation() const;

	void setColor( Cigi_uint8 red, Cigi_uint8 green, Cigi_uint8 blue, Cigi_uint8 alpha );

	void setColor( const Vect4 &newColor );

	Vect4 getColor() const;

	void setScale( float u, float v );

	void setScale( const Vect2 &newScale );

	Vect2 getScale() const;
	
	virtual SymbolType getType() const = 0;

	void resetFlashSequence();

	void setFlashDutyCyclePercentage( Cigi_uint8 newPercentage );

	void setFlashPeriod( float newPeriod );

	bool getFlashState() const;

	void addImplementation( SymbolImp *newImp );
	
	SymbolImpIteratorPair getImplementations()
	{
		return SymbolImpIteratorPair( imps.begin(), imps.end() );
	}
	
protected:

	virtual ~Symbol();

	bool isFlashing() const;
	
	void parentColorChanged( Symbol * );
	void parentFlashStateChanged( Symbol * );
	
	void forwardFlashStateChanged();
	

protected:
	// fixme - should change to int, so that initial value can be set to something invalid
	Cigi_uint16 id;
	SymbolState state;

	//! In order for a symbol to be placed on-screen, a Definition packet and 
	//! a full Symbol Control packet must be sent to the IG for that symbol.  
	//! It is not enough to send just a Definition, or just a Definition and a 
	//! Short Symbol Control for that symbol; doing so would leave several 
	//! symbol attributes uninitialized.  For that reason, this variable 
	//! exists to keep track of whether a full Symbol Control has been sent 
	//! for this symbol.
	bool fullControlPacketRecvd;

	bool isChild;
	bool inheritColor;
	// fixme - should change to int, so that initial value can be set to something invalid
	Cigi_uint16 parentID;
	Symbol *parent;
	// fixme - should change to int, so that initial value can be set to something invalid
	Cigi_uint16 surfaceID;
	Cigi_uint8 layer;
	FlashState flashState;
	Vect2 position;
	float rotation;
	Vect4 color;
	Vect2 scale;

	//=========================================================
	//! Implementation objects for this Symbol.  
	//!
	SymbolImpList imps;

};



//=========================================================
//! 
//! 
class MPVCMN_SPEC SymbolImp : public Referenced
{

public:
	
	SymbolImp( Symbol *symbol ) : 
		Referenced(), 
		baseSymbol( symbol )
	{}
	

	virtual void update( double timeElapsed ) {}

protected:
	
	virtual ~SymbolImp() {}
	
	Symbol *baseSymbol;
};

}
#endif
