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


#include <iostream>

#include "HostToIGSignalProc.h"


// ================================================
// HostToIGSignalProc
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
HostToIGSignalProc::HostToIGSignalProc()
{
	igConnections = NULL;
}

// ================================================
// ~HostToIGSignalProc
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
HostToIGSignalProc::~HostToIGSignalProc()
{

}


// ================================================
// OnAnyHostToIG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void HostToIGSignalProc::OnAnyHostToIG(CigiBasePacket *Packet)
{
	if( !igConnections )
		return;
	
	std::list< IGConnection* >::iterator iter;
	for( iter = igConnections->begin(); iter != igConnections->end(); iter++ )
	{
		IGConnection *igc = (*iter);
		(*igc->OmsgPtr) << *Packet;
	}
	
}


// ================================================
// OnIGCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void HostToIGSignalProc::OnIGCtrl(CigiBasePacket *Packet)
{
	// IG Ctrl packets must be handled separately.  If the igctrl packet 
	// is piped ("<<") into an CigiOutgoingMsg as a CigiBasePacket instead of 
	// as a CigiBaseIGCtrl packet the CigiOutgoingMsg will not set its 
	// ValidIGCtrlSOF flag to true, and will abort() if you try to LockMsg it.

	if( !igConnections )
		return;
	
	CigiIGCtrlV3 *igCtrl = (CigiIGCtrlV3 *)Packet;
	
	std::list< IGConnection* >::iterator iter;
	for( iter = igConnections->begin(); iter != igConnections->end(); iter++ )
	{
		IGConnection *igc = (*iter);
		(*igc->OmsgPtr) << *igCtrl;
	}
	
}


// ================================================
// OnEntityCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void HostToIGSignalProc::OnEntityCtrl(CigiBasePacket *Packet)
{
	// EntityCtrl packets must be handled separately.

	if( !igConnections )
		return;
	
	CigiEntityCtrlV3 *entCtrl = (CigiEntityCtrlV3 *)Packet;
	
	std::list< IGConnection* >::iterator iter;
	for( iter = igConnections->begin(); iter != igConnections->end(); iter++ )
	{
		IGConnection *igc = (*iter);
		(*igc->OmsgPtr) << *entCtrl;
	}
	
}


// ================================================
// OnEnvCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void HostToIGSignalProc::OnEnvCtrl(CigiBasePacket *Packet)
{
	// EnvCtrl packets must be handled separately.

	if( !igConnections )
		return;
	
	CigiBaseEnvCtrl *envCtrl = (CigiBaseEnvCtrl *)Packet;
	
	std::list< IGConnection* >::iterator iter;
	for( iter = igConnections->begin(); iter != igConnections->end(); iter++ )
	{
		IGConnection *igc = (*iter);
		(*igc->OmsgPtr) << *envCtrl;
	}
	
	std::cout << "Warning: HostToIGSignalProc encountered EnvCtrl packet, which shouldn't happen\n";
}


// ================================================
// OnCelestialCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void HostToIGSignalProc::OnCelestialCtrl(CigiBasePacket *Packet)
{
	// Celestial Sphere Ctrl packets must be handled separately.

	if( !igConnections )
		return;
	
	CigiCelestialCtrlV3 *celestialCtrl = (CigiCelestialCtrlV3 *)Packet;
	
	std::list< IGConnection* >::iterator iter;
	for( iter = igConnections->begin(); iter != igConnections->end(); iter++ )
	{
		IGConnection *igc = (*iter);
		(*igc->OmsgPtr) << *celestialCtrl;
	}
	
}


// ================================================
// OnAtmosCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void HostToIGSignalProc::OnAtmosCtrl(CigiBasePacket *Packet)
{
	// Atmosphere Ctrl packets must be handled separately.

	if( !igConnections )
		return;
	
	CigiAtmosCtrlV3 *atmosCtrl = (CigiAtmosCtrlV3 *)Packet;
	
	std::list< IGConnection* >::iterator iter;
	for( iter = igConnections->begin(); iter != igConnections->end(); iter++ )
	{
		IGConnection *igc = (*iter);
		(*igc->OmsgPtr) << *atmosCtrl;
	}
	
}


