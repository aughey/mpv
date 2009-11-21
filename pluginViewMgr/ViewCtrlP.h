/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2004 The Boeing Company
 *  
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *  
 *  
 *  FILENAME:   ViewCtrlP.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  listen for ViewControl packets and update the Views accordingly.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  04/06/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */

// ViewCtrlP.h: interface for the ViewCtrlP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(VIEWCTRLP_H)
#define VIEWCTRLP_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>

#include "AllCigi.h"
#include "View.h"
#include "CigiViewCtrlV3.h"

using namespace mpv;

//=========================================================
//! This class listens for ViewControl packets and updates the contents of 
//! the Views.
//! 
class ViewCtrlP : public CigiBaseEventProcessor  
{
public:

	//=========================================================
	//! General Constructor
	//! 
	ViewCtrlP();

	//=========================================================
	//! General Destructor
	//! 
	virtual ~ViewCtrlP();
	
	//=========================================================
	//! Initializes viewMap and viewGroupParamsMap
	//! \param vpm - the new value for viewMap
	//! \param vgpm - the new value for viewGroupParamsMap
	//! 
	void init( 
		std::map< int, RefPtr<View> > *vpm, 
		std::map< int, ViewGroupParams *> *vgpm ) 
	{ viewMap = vpm; viewGroupParamsMap = vgpm; }
	
	//=========================================================
	//! Callback; processes a packet
	//! 
	virtual void OnPacketReceived(CigiBasePacket *Packet);

private:
	
	//=========================================================
	//! Map of Views, passed in from plugin
	//! 
	std::map< int, RefPtr<View> > *viewMap;
	
	//=========================================================
	//! Map of ViewGroupParams, passed in from plugin
	//! 
	std::map< int, ViewGroupParams* > *viewGroupParamsMap;
};

#endif 
