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
 *  FILENAME:   HostToIGSignalProc.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  07/27/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#ifndef HOSTTOIGSIGNALPROC_H
#define HOSTTOIGSIGNALPROC_H

#include <stdio.h>
#include <list>

#include "WildcardSignalProcessor.h"
#include "IGConnection.h"



//=========================================================
//! 
//!
class HostToIGSignalProc : public WildcardSignalProcessor
{
public:


	//==> Management

	//=========================================================
	//! General Constructor
	//!
	HostToIGSignalProc();

	//=========================================================
	//! General Destructor
	//!
	virtual ~HostToIGSignalProc();


	//==> Basic handler initialization and functionality


	//=========================================================
	//! Initializes the pointer to the list of IG connections
	//! \param igc - The pointer to the list of IG connections
	//!
	void setIGConnectionList( std::list< IGConnection* > *igc ) { igConnections = igc; }


	//=========================================================
	//! The handler for CIGI packets sent by the host.
	//! \param Packet - The pointer to the packet object.
	//!
	virtual void OnAnyHostToIG(CigiBasePacket *Packet);

	//=========================================================
	//! The handler for CIGI packets sent by the host.
	//! \param Packet - The pointer to the packet object.
	//!
	virtual void OnIGCtrl(CigiBasePacket *Packet);
	//=========================================================
	//! The handler for CIGI packets sent by the host.
	//! \param Packet - The pointer to the packet object.
	//!
	virtual void OnEntityCtrl(CigiBasePacket *Packet);
	//=========================================================
	//! The handler for CIGI packets sent by the host.
	//! \param Packet - The pointer to the packet object.
	//!
	virtual void OnEnvCtrl(CigiBasePacket *Packet);
	//=========================================================
	//! The handler for CIGI packets sent by the host.
	//! \param Packet - The pointer to the packet object.
	//!
	virtual void OnCelestialCtrl(CigiBasePacket *Packet);
	//=========================================================
	//! The handler for CIGI packets sent by the host.
	//! \param Packet - The pointer to the packet object.
	//!
	virtual void OnAtmosCtrl(CigiBasePacket *Packet);

private:
	

	//=========================================================
	//! A pointer to the list of IG connections
	//!
	std::list< IGConnection* > *igConnections;
	

};

#endif 
