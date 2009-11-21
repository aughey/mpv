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
 *  FILENAME:   IGCtrlP.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *   handle the CIGI ig control packets.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  04/04/2004 ...                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#if !defined(IGCTRLP_H)
#define IGCTRLP_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AllCigi.h"



//=========================================================
//! The class handling the CIGI ig control packets.
//!
class SyncRNIGCtrlP : public CigiBaseEventProcessor  
{
public:


	//==> Management

	//=========================================================
	//! General Constructor
	//!
	SyncRNIGCtrlP();

	//=========================================================
	//! General Destructor
	//!
	virtual ~SyncRNIGCtrlP();



	//==> Basic handler initialization and functionality


	//=========================================================
	//! Initializes the pointer to the current frame number
	//! \param dn - The pointer to the current frame number.
	//!
	void setFrameNumber( unsigned int *fn ) { framenum = fn; }

	//=========================================================
	//! The callback handler for the CIGI ig control packet
	//! \param Packet - The pointer to the ig control packet object.
	//!
	virtual void OnPacketReceived(CigiBasePacket *Packet);

private:
	

	//=========================================================
	//! A pointer to the current frame number
	//!
	unsigned int *framenum;
	

};

#endif 
