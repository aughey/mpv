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
 *  FILENAME:   IGConnection.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class encapsulates the connection between the manager and an IG 
 *  channel.
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

#ifndef IG_CONNECTION_H
#define IG_CONNECTION_H

#define RECV_BUFFER_SIZE 65536

#include "AllCigi.h"

#include "Network.h"

#include "IGToHostSignalProc.h"

class IGConnection
{
public:
	IGConnection();
	~IGConnection();
	void act();
	void init( CigiOutgoingMsg *HostOMsg, void (*sendNetMessagesCB)(void), bool isMaster );
	
	CigiHostSession session;
	CigiOutgoingMsg *OmsgPtr;
	CigiIncomingMsg *ImsgPtr;
	Network network;
	unsigned char recvBuffer[RECV_BUFFER_SIZE];
	IGToHostSignalProc i2hSignalProc;
	
	// if true, no traffic should be sent to IG until Host sends IGControl
	bool waitForHost;
};

#endif
