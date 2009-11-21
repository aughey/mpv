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
 *  FILENAME:   ViewCtrlP.cpp
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

// ViewCtrlP.cpp: implementation of the ViewCtrlP class.
//
//////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string.h> // for memset

#include "ViewCtrlP.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// ================================================
// ViewCtrlP()
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ViewCtrlP::ViewCtrlP()
{
	viewMap = NULL;
	viewGroupParamsMap = NULL;
}

// ================================================
// ~ViewCtrlP()
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ViewCtrlP::~ViewCtrlP()
{

}

// ================================================
// OnPacketReceived(CigiBasePacket *Packet)
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ViewCtrlP::OnPacketReceived(CigiBasePacket *Packet)
{
	CigiViewCtrlV3 *vc = (CigiViewCtrlV3 *)(Packet);
	
	if( !viewMap || !viewGroupParamsMap ) return;
	
	int groupID = vc->GetGroupID();
	if( groupID != 0 ) {
		// this packet applies to some view group
		
		ViewGroupParams * vgp = (*viewGroupParamsMap)[groupID];
		if( vgp == NULL ) {
			(*viewGroupParamsMap)[groupID] = vgp = new ViewGroupParams;
			memset( vgp, 0, sizeof( ViewGroupParams ) );
		}
		
		vgp->entityID = vc->GetEntityID();
		
		// hosts use +x=north, +y=east, -z=up, so we need to account for that
		// by swapping things around
		if( vc->GetXOffEn() )
			vgp->groupOffset[0] = vc->GetYOff();
		if( vc->GetYOffEn() )
			vgp->groupOffset[1] = vc->GetXOff();
		if( vc->GetZOffEn() )
			vgp->groupOffset[2] = -1.0 * vc->GetZOff();

		if( vc->GetYawEn() )
			vgp->groupRotate[0] = vc->GetYaw();
		if( vc->GetPitchEn() )
			vgp->groupRotate[1] = vc->GetPitch();
		if( vc->GetRollEn() )
			vgp->groupRotate[2] = vc->GetRoll();
		
	} else {
		// this packet applies to some view 

		std::map< int, RefPtr<View> >::iterator vpIter = 
			viewMap->find( vc->GetViewID() );
		
		// determine if this packet's viewID is one that we're listening for
		if( vpIter == viewMap->end() ) return;

		View *view = vpIter->second.get();
	
		view->setEntityID( vc->GetEntityID() );
		
		if( vc->GetXOffEn() || vc->GetYOffEn() || vc->GetZOffEn() )
		{
			Vect3 offset = view->getViewOffset();
			// hosts use +x=north, +y=east, -z=up, so we need to account for that
			// by swapping things around
			if( vc->GetXOffEn() )
				offset[0] = vc->GetYOff();
			if( vc->GetYOffEn() )
				offset[1] = vc->GetXOff();
			if( vc->GetZOffEn() )
				offset[2] = -1.0 * vc->GetZOff();
			view->setViewOffset( offset );
		}

		if( vc->GetYawEn() || vc->GetPitchEn() || vc->GetRollEn() )
		{
			Vect3 rotation = view->getViewRotate();
			if( vc->GetYawEn() )
				rotation[0] = vc->GetYaw();
			if( vc->GetPitchEn() )
				rotation[1] = vc->GetPitch();
			if( vc->GetRollEn() )
				rotation[2] = vc->GetRoll();
			view->setViewRotate( rotation );
		}
	}
	
}

