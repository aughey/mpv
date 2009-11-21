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
 *  FILENAME:   IGConnection.cpp
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

#include <iostream>
#include <stdio.h>
#include "IGConnection.h"


IGConnection::IGConnection()
{
	session.SetCigiVersion(3);
	session.SetSynchronous(true);
	CigiOutgoingMsg &Omsg = session.GetOutgoingMsgMgr();
	CigiIncomingMsg &Imsg = session.GetIncomingMsgMgr();

	Imsg.SetReaderCigiVersion(3);
	Imsg.UsingIteration(false);
	
	Omsg.BeginMsg();
	// prime the Omsg with an IGCtrl
	CigiIGCtrlV3 igCtrl;
	Omsg << igCtrl;

	OmsgPtr = &Omsg;
	ImsgPtr = &Imsg;
	
	waitForHost = true;
}


IGConnection::~IGConnection()
{
}	


void IGConnection::init( 
	CigiOutgoingMsg *HostOMsg, void (*sendNetMessagesCB)(void), bool isMaster )
{
	i2hSignalProc.setOutgoingMsg( HostOMsg );
	i2hSignalProc.setSendNetMessagesCB( sendNetMessagesCB );
	i2hSignalProc.setMaster( isMaster );
	
	ImsgPtr->RegisterSignalProcessor( &i2hSignalProc );
}


void IGConnection::act()
{
	// empty the receive buffer
	int recvLen = network.recv( recvBuffer, RECV_BUFFER_SIZE );
	if( recvLen > 0 ) {
		//printf( "IGConnection - recvd %i bytes\n", recvLen );
		try {
			ImsgPtr->ProcessIncomingMsg(recvBuffer,recvLen);
		}
		catch( CigiException &e ){
			std::cout << "IGConnection::act - Exception: " << e.what() << std::endl;
		}
	}


	unsigned char * sendBuffer;
	int sendLen;

	// Bundle up all of the stored packets and send them off to the IG
	// First, check to see if it has recv'd an IGCtrl packet
	if( !OmsgPtr->GetValidIGCtrlSOF() )
	{
		if( waitForHost )
		{
			// we don't want to send any packets to the IG unless the 
			// Host has sent an IG Ctrl to us first
			return;
		}
		else
		{
//printf("had to manually inject an igctrl packet\n" );
			// We haven't recv'd an IGCtrl this time.  We have to add one, or 
			// OmsgPtr will abort() when we call LockMsg().
			CigiIGCtrlV3 igCtrl;
			(*OmsgPtr) << igCtrl;
		}
	}
	OmsgPtr->LockMsg();
	sendBuffer = OmsgPtr->GetMsg( sendLen );
	int sentBytes =
		network.send( sendBuffer, sendLen );
	OmsgPtr->UnlockMsg();
	OmsgPtr->BeginMsg();

}
