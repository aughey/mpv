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
 *  FILENAME:   ViewDefP.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  listen for ViewDefinition packets and update the Views accordingly.
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

// ViewDefP.cpp: implementation of the ViewDefP class.
//
//////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string.h> // for memset

#include "ViewDefP.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// ================================================
// ViewDefP()
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ViewDefP::ViewDefP()
{
	viewMap = NULL;
	viewGroupParamsMap = NULL;
}

// ================================================
// ~ViewDefP()
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ViewDefP::~ViewDefP()
{

}

// ================================================
// OnPacketReceived(CigiBasePacket *Packet)
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ViewDefP::OnPacketReceived(CigiBasePacket *Packet)
{
	CigiViewDefV3 *vd = (CigiViewDefV3 *)(Packet);
	
	if( !viewMap || !viewGroupParamsMap ) return;

	std::map< int, RefPtr<View> >::iterator vpIter = 
		viewMap->find( vd->GetViewID() );

	// determine if this packet's viewID is one that we're listening for
	if( vpIter == viewMap->end() ) return;
	
	View *view = vpIter->second.get();

	int groupID = vd->GetGroupID();
	view->setGroupID( groupID );
	if( groupID != 0 ) {
		
		ViewGroupParams * vgp = (*viewGroupParamsMap)[groupID];
		if( vgp == NULL ) {
			(*viewGroupParamsMap)[groupID] = vgp = new ViewGroupParams;
			memset( vgp, 0, sizeof( ViewGroupParams ) );
		}
		
	}
	
	if( vd->GetFOVNearEn() || vd->GetFOVFarEn() )
	{
		float near = view->getNearPlane();
		float far = view->getFarPlane();
		if( vd->GetFOVNearEn() )
			near = vd->GetFOVNear();
		if( vd->GetFOVFarEn() )
			far = vd->GetFOVFar();
		view->setNearFarPlanes( near, far );
	}
	
	if( vd->GetFOVLeftEn() || vd->GetFOVRightEn() || 
	    vd->GetFOVTopEn() || vd->GetFOVBottomEn() )
	{
		float fovLeft = view->getFovLeft();
		float fovRight = view->getFovRight();
		float fovTop = view->getFovTop();
		float fovBottom = view->getFovBottom();
		if( vd->GetFOVLeftEn() )
			fovLeft = vd->GetFOVLeft();
		if( vd->GetFOVRightEn() )
			fovRight = vd->GetFOVRight();
		if( vd->GetFOVTopEn() )
			fovTop = vd->GetFOVTop();
		if( vd->GetFOVBottomEn() )
			fovBottom = vd->GetFOVBottom();
		view->setFov( fovLeft, fovRight, fovTop, fovBottom );
	}

	view->setParallelProjection( CigiBaseViewDef::Orthographic == vd->GetProjectionType() );

	view->setMirrorMode( vd->GetMirrorMode() );
}

