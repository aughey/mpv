/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2007 The Boeing Company
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
 *  FILENAME:   Kernel.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This is the main portion of the kernel.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  2007-07-04 Andrew Sampson
 *      Formed a Kernel class from the code in main.cpp
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#ifndef _KERNEL_H_
#define _KERNEL_H_

#include "Network.h"  // network includes winsock2.h which must be included before windows.h

#include <stdio.h>
#include <iostream>
#include <list>
#include <queue>
#include <utility>
#include <cassert>

#ifdef WIN32
#include <Windows.h>
#include <direct.h>
#define getcwd _getcwd
#else
#include <unistd.h>
#include <sys/time.h> // for gettimeofday
#endif

#include "Blackboard.h"
#include "MPVExceptions.h"
#include "StateMachine.h"
#include "DefaultP.h"
#include "IGCtrlProcessor.h"
#include "AllCigi.h"
#include "DefFileParser.h"
#include "SystemState.h"
#include "GenerateID.h"
#include "SimpleTimer.h"
#include "MPVTimer.h"

#define RECV_BUFFER_SIZE 65536

class Kernel
{
public:
	Kernel();
	~Kernel();
	
	int init( int argc, char *argv[] );
	int run( void );
	
private:
	
	Network network;

	#ifdef WIN32 
	// VC7 seems to have a problem between our network class and the queue class that results in some overlap.
	//  we are inserting a spacer queue to resolve this problem
	std::queue< std::pair< int, unsigned char * > > barrier;
	#endif

	//=========================================================
	//! The Blackboard.  This is the most important structure in the MPV.  
	//! Any component of the MPV (plugins, the kernel) can "post" a variable 
	//! to the blackboard, along with a unique string identifier for the 
	//! variable.  Any other component of the MPV (usually just the plugins) 
	//! can retrieve a variable from the blackboard, and query or (in some 
	//! cases) modify the variable.  In this manner, data is shared throughout 
	//! the system.  
	//! Trivia: The blackboard concept originated in AI work, but you can 
	//! find variants of the concept throughout computer science.  The term, 
	//! as used in the MPV, is a little different from how it is used in the 
	//! AI field.
	//! 
	Blackboard *bb;

	//=========================================================
	//! Encapsulates the MPV's current state, as well as the actions taken 
	//! in each of the many states.
	//! 
	StateMachine stateMachine;

	//=========================================================
	//! An IG Session used only during certain system states.  It is used 
	//! only by the MPV kernel, and only for snooping on incoming CIGI traffic.
	//! The reason behind using a separate IG session during certain states is 
	//! so that the MPV can ignore irrelevant packet types when, for example, 
	//! in the Standby, Reset or Shutdown states.
	//!
	CigiIGSession internalIGSession;
	
	//=========================================================
	//! The IG Session used during normal operation.  Plugins will register 
	//! packet handlers with this session by way of the outgoing message 
	//! handler (see below).  Incoming CIGI traffic will be fed into this 
	//! object, and will be acted upon by the packet handlers.  
	//! Posted to the blackboard.
	//!
	CigiIGSession normalIGSession;
	
	//=========================================================
	//! Outgoing message handler.  Plugins can add CIGI packets to this 
	//! object.  The packets will be sent to the Host at the beginning (end) 
	//! of the frame.  Posted to the blackboard.
	//! 
	CigiOutgoingMsg *OmsgPtr;
	
	//=========================================================
	//! Incoming message handler.  Plugins can register packet handlers with 
	//! this object.  Packets received from the Host will be propagated 
	//! throughout the MPV via this mechanism.  Posted to the blackboard.
	//! 
	CigiIncomingMsg *ImsgPtr;
	
	//=========================================================
	//! An internal CIGI packet handler.  This one catches packets that 
	//! aren't otherwise handled and prints a warning message.  Not terribly 
	//! important; the only real purpose is to give the user some 
	//! notification that a particular packet/feature isn't supported.
	//! 
	DefaultP DefaultProc;
	
	//=========================================================
	//! An internal CIGI packet handler.  This handler processes IGCtrl 
	//! packets, and retrieves database-number and IG-state data.
	//! 
	MPVKernel::IGCtrlProcessor igCtrlProcessor;
	
	
	//=========================================================
	//! An array, for shuttling data from the network object to the incoming 
	//! message handler.
	//! 
	unsigned char recvBuffer[RECV_BUFFER_SIZE];
	int recvLen;

	std::queue< std::pair< int, unsigned char * > > cigiMessageQueue;

	float timeDelayLimit;
	CigiSOFV3_2 SOF;

	//=========================================================
	//! The loaded database number.  This is the number for the database 
	//! that is currently in memory.  Posted to the blackboard.
	//!
	int LoadedDatabaseNumber;

	//=========================================================
	//! The commanded database number, as set by the Host.  Posted to 
	//! the blackboard.
	//!
	int CommandedDatabaseNumber;

	//=========================================================
	//! The reported database number; this is the number sent to the Host.  
	//! Posted to the blackboard.
	//!
	int ReportedDatabaseNumber;

	//=========================================================
	//! The default database number; this is set by the user in the config 
	//! files.  Posted to the blackboard.
	//!
	int DefaultDatabaseNumber;

	std::string HostIp;
	int HostSockSendTo;
	int LocalSockListenOn;
	int Hertz;
	float busy_wait_time;

	bool shouldKernelSendNetMessages;
	
	SimpleTimer mainTimer;
	
	std::list<MPVTimer *> TmrLst;
	double timeElapsedLastFrame;
	GenerateID GenID;
	char BaseProgDir[1024];
	std::string defFileDirectory;
	std::string pathSeparator;
	
	void loadConfigFile( std::string filename );
	float timevaldiff( struct timeval *t1, struct timeval *t2 );
	void initNetwork( void );
	void initCCL( void );
	void getNetMessages( void );
	void processCigiMessage( unsigned char *message, int messageLength );
	void sendNetMessages( void );
	void delay( void );
	
};


#endif
