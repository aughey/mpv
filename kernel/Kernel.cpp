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
 *  03/29/2004 Andrew Sampson
 *      Initial Release.
 *  2007-07-01 Andrew Sampson
 *      Changed interface to use new state machine API
 *  2007-07-04 Andrew Sampson
 *      Formed a Kernel class from the code in main.cpp
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#include <string>
#include <stdlib.h>
#include <string.h>

#include "Kernel.h"

//#define NO_INTERNAL_QUEUING
// This next option is valid only when NO_INTERNAL_QUEUING is not defined
#ifndef NO_INTERNAL_QUEUING
//#define PROCESS_ONE_CIGI_MSG_PER_FRAME
#endif


// ================================================
// Constructor
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Kernel::Kernel() : 
	bb( new Blackboard() ),
	stateMachine( bb ),
	OmsgPtr( NULL ),
	ImsgPtr( NULL ),
	LoadedDatabaseNumber( -128 ),
	CommandedDatabaseNumber( 0 ),
	ReportedDatabaseNumber( LoadedDatabaseNumber ),
	DefaultDatabaseNumber( CommandedDatabaseNumber ),
	HostIp( "127.0.0.1" ),
	HostSockSendTo( 8005 ),
	LocalSockListenOn( 8004 ),
	Hertz( 60 ),
	busy_wait_time( 0.0f ),
	shouldKernelSendNetMessages( true ),
	timeElapsedLastFrame( 0.0 )
{
#ifdef WIN32
	pathSeparator = "\\";
#else
	pathSeparator = "/";
#endif
}


// ================================================
// Destructor
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Kernel::~Kernel()
{
	// shut down the network
	network.closeSocket();

	delete bb;
}


// ================================================
// init
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int Kernel::init( int argc, char *argv[] )
{

	if( getcwd( BaseProgDir, 1023 ) == NULL )
	{
		strcpy( BaseProgDir, "." );
	}
	
	if( getenv( "DEF_FILE_PATH" ) )
	{
		defFileDirectory = std::string( getenv( "DEF_FILE_PATH" ) ) + pathSeparator;
		std::cout << "Note - looking for config files in " 
			<< defFileDirectory << std::endl;
	}
	else
	{
		defFileDirectory = std::string( BaseProgDir ) + pathSeparator;
		std::cout << "Note - DEF_FILE_PATH environmental variable is not set; looking for config files in " 
			<< defFileDirectory << std::endl;
	}

	try
	{
		loadConfigFile( defFileDirectory + "system.def" );
	}
	catch( std::exception &e )
	{
		std::cerr << "Exception - " << e.what() << std::endl;
		std::cerr << "Error - the MPV kernel was unable to open the 'system.def' file.\n" 
			<< "If the def file is missing, then you will need to find a copy or create a new \n" 
			<< "one.  If the def file is not missing, then you probably forgot to set the \n" 
			<< "DEF_FILE_PATH environmental variable.  Just set the variable to the name of the \n" 
			<< "directory where you keep your MPV configuration files.\n";
		exit( 1 );
	}

#ifdef WIN32
	// Due to the VC7 problem
	std::pair< int, unsigned char * > bwPair;
	bwPair.first = 5;
	bwPair.second = (unsigned char *)"Test";
	barrier.push( bwPair );
#endif

	// initialize the network class
	initNetwork();

	// initialize the CIGI class library
	initCCL();

	// post the current working directory to the blackboard
	bb->put( "BaseProgramDirectory", BaseProgDir );

	// post the def file search path to the blackboard
	bb->put( "DefFileDirectory", &defFileDirectory );
	
	// post the DatabaseNumber variables to the blackboard
	bb->put( "LoadedDatabaseNumber", &LoadedDatabaseNumber );
	bb->put( "CommandedDatabaseNumber", &CommandedDatabaseNumber );
	bb->put( "ReportedDatabaseNumber", &ReportedDatabaseNumber );
	bb->put( "DefaultDatabaseNumber", &DefaultDatabaseNumber );

	// post the ID Generator
	bb->put( "GenerateID", &GenID );

	// post the timer list and the frame-time
	bb->put( "TimerList", &TmrLst );
	bb->put( "TimeElapsedLastFrame", &timeElapsedLastFrame );

	// post the sendNetMessages function to the blackboard
//	bb->put( "SendNetMessagesCB", sendNetMessages );
//	bb->put( "ShouldKernelSendNetMessagesBool", &shouldKernelSendNetMessages );

	return 0;
}


// ================================================
// run
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int Kernel::run( void )
{
	// If an error message exists it causes main to output the error message
	// after deallocating all plugins prior to returning.
	std::string shutdown_error_message;

	// main loop
	for( int i=0; !stateMachine.getShouldExit(); i++ )
	{
		mainTimer.start();
		
		try
		{
			stateMachine.act();
		}
		catch( std::exception &e )
		{
			shutdown_error_message = e.what();
			// break out of main loop
			break;
		}
		catch( const std::string &s )
		{
			shutdown_error_message = s;
			// break out of main loop
			break;
		}
		catch( ... )
		{
			shutdown_error_message = "[unknown exception]";
			// break out of main loop
			break;
		}

		// send any queued messages
		if( shouldKernelSendNetMessages && stateMachine.getShouldSendSOF() )
			sendNetMessages();

		// wait for some amount of time
		delay();

		// check to see if any network messages have arrived
		getNetMessages();
		
		mainTimer.stop();
		timeElapsedLastFrame = mainTimer.getElapsedTime();
	}

	if( shutdown_error_message != "" )
	{
		std::cout << std::endl << shutdown_error_message << std::endl;
		assert( false );
		return 1;
	}

	return 0;
}


// ================================================
// loadConfigFile
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Kernel::loadConfigFile( std::string filename )
{
	DefFileParser parser;
	DefFileGroup *root = parser.parse( filename );

	if( root == NULL )
	{
		throw MPVFileIOReadException( std::string( "Couldn't parse \"" ) + filename + "\"" );
		return;
	}

	for( std::list< DefFileGroup * >::iterator iter = root->children.begin();
	     iter != root->children.end(); iter++ )
	{
		DefFileGroup *group = ( *iter );
		if( group->getName() == "system" )
		{
			DefFileAttrib * attr;
			attr = group->getAttribute( "host_addr" );
			if( attr )
			{
				HostIp = attr->asString();
			}

			attr = group->getAttribute( "host_port" );
			if( attr )
			{
				HostSockSendTo = attr->asInt();
			}

			attr = group->getAttribute( "listen_port" );
			if( attr )
			{
				LocalSockListenOn = attr->asInt();
			}

			attr = group->getAttribute( "hertz" );
			if( attr )
			{
				Hertz = attr->asInt();
			}


			/* Set to zero (default) to busy wait for the entire
			 * time for the frames per second timer to expire or
			 * to sleep until busy_wait_time seconds and then
			 * start busy waiting for the frames per second timer
			 * to expire.
			 * Typically don't set this less than 1/(OS schedule
			 * hertz) or the start of frame will be less than
			 * requested, but setting it for a value much more
			 * than 1/(OS scheduler hertz) will not help.
			 */
			attr = group->getAttribute( "busy_wait_time" );
			if( attr )
			{
				busy_wait_time = attr->asFloat();
			}
			attr = group->getAttribute( "default_database" );
			if( attr )
			{
				DefaultDatabaseNumber = attr->asInt();
				CommandedDatabaseNumber = DefaultDatabaseNumber;
			}
		}
		else
		{
			// ignore non-system groups
		}
	}
	
	delete root;
}



// ================================================
// timevaldiff
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
float Kernel::timevaldiff( struct timeval *t1, struct timeval *t2 )
{
	return ( t2->tv_sec - t1->tv_sec ) +
	       ( t2->tv_usec - t1->tv_usec ) / 1000000.0f;
}


// ================================================
// initNetwork
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Kernel::initNetwork( void )
{

	timeDelayLimit = 1.0f / ( float ) ( Hertz );

	// hostemu-ip-addr, hostemu-socket, local-socket
	bool netstatus = network.openSocket(
		HostIp.c_str(),
		HostSockSendTo,
		LocalSockListenOn );

	if( !netstatus )
	{
		printf( "ERROR - failed to initialize network interface\n" );
		exit( 1 );
	}
	else
	{
		printf( "Successfully initialized network interface\n" );
	}

}

// ================================================
// initCCL
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Kernel::initCCL( void )
{

	// IG to Host
	CigiOutgoingMsg &Omsg = normalIGSession.GetOutgoingMsgMgr();
	CigiIncomingMsg &Imsg = normalIGSession.GetIncomingMsgMgr();
	OmsgPtr = &Omsg;
	ImsgPtr = &Imsg;

	bb->put( "CigiIGSession", &normalIGSession );
	bb->put( "CigiOutgoingMsg", &Omsg );
	bb->put( "CigiIncomingMsg", &Imsg );

	normalIGSession.SetCigiVersion( 3, 3 );
	normalIGSession.SetSynchronous( true );

	Imsg.SetReaderCigiVersion( 3, 3 );
	Imsg.UsingIteration( false );

	// set up a default handler for unhandled packets
	Imsg.RegisterEventProcessor( 0, (CigiBaseEventProcessor *) &DefaultProc );

	// set up a handler for IGCtrl packets
	igCtrlProcessor.setCommandedDatabaseNumber( &CommandedDatabaseNumber );
	igCtrlProcessor.setStateMachine( &stateMachine );
	Imsg.RegisterEventProcessor( CIGI_IG_CTRL_PACKET_ID_V3_2, 
		(CigiBaseEventProcessor *) &igCtrlProcessor );
	internalIGSession.GetIncomingMsgMgr().RegisterEventProcessor( 
		CIGI_IG_CTRL_PACKET_ID_V3_2, 
		(CigiBaseEventProcessor *) &igCtrlProcessor );

	Omsg.BeginMsg();

	// add the Start Of Frame, in preparation for the next frame
	SOF.SetIGStatus( 0 );
	SOF.SetFrameCntr( 0 );
	SOF.SetTimeStamp( 0 );
	SOF.SetTimeStampValid( false );
	SOF.SetIGMode( stateMachine.getIGMode() );
	SOF.SetDatabaseID( ReportedDatabaseNumber );
	( *OmsgPtr ) << SOF;
}


#ifdef NO_INTERNAL_QUEUING 
// ================================================
// getNetMessages
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Kernel::getNetMessages( void )
{

	recvLen = network.recv( recvBuffer, RECV_BUFFER_SIZE );
	if( recvLen > 0 )
	{
		processCigiMessage( recvBuffer, recvLen );
	}
	//else{ fprintf( stderr, "!!!recvd no packets this time 'round!!!\n" ); }
}
#else 
// ================================================
// getNetMessages
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Kernel::getNetMessages( void )
{

	unsigned char * tempBuffer = NULL;

	// first, pull all of the available messages out of the UDP buffer
	// and put them into our queue
	do
	{
		recvLen = network.recv( recvBuffer, RECV_BUFFER_SIZE );
		if( recvLen > 0 )
		{
			std::pair< int, unsigned char * > newPair;
			tempBuffer = new unsigned char[recvLen];
			memcpy( tempBuffer, recvBuffer, recvLen );
			newPair.first = recvLen;
			newPair.second = tempBuffer;
			cigiMessageQueue.push( newPair );
			//printf( "getNetMessages: sizeof temp buffer: %i\n", newPair.first );
		}
	}
	while( recvLen > 0 );

	unsigned int messagesWaiting = cigiMessageQueue.size();
	unsigned int messagesToLeaveInQueue;
	if( messagesWaiting > 1 )
		messagesToLeaveInQueue = 1;
	else if( messagesWaiting == 1 )
		messagesToLeaveInQueue = 0;
	else
	{
		//printf( "no messages were waiting in queue\n" );
		messagesToLeaveInQueue = 0;
	}

	// now, process some/all of those messages
#ifdef PROCESS_ONE_CIGI_MSG_PER_FRAME
	if( cigiMessageQueue.size() > 0 )
#else
	while( cigiMessageQueue.size() > messagesToLeaveInQueue )
#endif
	{
		std::pair< int, unsigned char * > newPair = cigiMessageQueue.front();
		recvLen = newPair.first;
		tempBuffer = newPair.second;
		cigiMessageQueue.pop();
		
		processCigiMessage( tempBuffer, recvLen );

		delete [] tempBuffer;
	}

	//else{ fprintf( stderr, "!!!recvd no packets this time 'round!!!\n" ); }
}
#endif


// ================================================
// processCigiMessage
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Kernel::processCigiMessage( unsigned char *message, int messageLength )
{
	try
	{
		/*
		In certain states, the IG is supposed to discard incoming CIGI data.
		In other states, the IG is supposed to discard all data except the 
		IG Control packet.  (Side note - For the MPV, these two situations 
		would result in mostly the same behavior, so they're handled the 
		same for now.)
		Here, the state machine is queried to see if the current state 
		requires processing the entire CIGI message or just the IG Ctrl 
		packet.  If only the IGCtrl is to be processed, then the message 
		is handled by a special, dedicated session object instead of the 
		default session object.
		*/
		if( stateMachine.getShouldIgnoreNonIGCtrl() )
			internalIGSession.GetIncomingMsgMgr().ProcessIncomingMsg( message, messageLength );
		else
			ImsgPtr->ProcessIncomingMsg( message, messageLength );
	}
	catch( CigiMissingIgControlException &theException )
	{
		std::cout << "Warning in processCigiMessage - first packet in CIGI " 
			<< "message from the host \n\twas not an IGCtrl\n";
	}
	catch( CigiException &theException )
	{
		std::cout << "processCigiMessage - Exception: " 
			<< theException.what() << std::endl;
	}
}


// ================================================
// sendNetMessages
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Kernel::sendNetMessages( void )
{
	unsigned char * sendBuffer = NULL;
	int sendLen = 0;

	// time to bundle up all of the stored packets and
	// send them off
	OmsgPtr->LockMsg();
	OmsgPtr->UpdateFrameCntr();
	sendBuffer = OmsgPtr->GetMsg( sendLen );
	int sentBytes =
	    network.send( sendBuffer, sendLen );
	OmsgPtr->UnlockMsg();
//	OmsgPtr->BeginMsg();

	/*
	printf( "sent %i bytes\n", sentBytes);
	printf( "Sending a SOF\n" );
	*/

	// add a start-of-frame to the message, in preparation for the next frame
	SOF.SetTimeStamp( 0 );
	SOF.SetTimeStampValid( false );
	SOF.SetIGMode( stateMachine.getIGMode() );
	SOF.SetDatabaseID( ReportedDatabaseNumber );
	( *OmsgPtr ) << SOF;

}


// ================================================
// delay
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
#ifdef WIN32
void Kernel::delay( void )
{
	static DWORD t1 = 0;
	static DWORD t2 = 0;
	static bool firsttimethrough = true;

	if( firsttimethrough )
	{
		t1 = GetTickCount();
		firsttimethrough = false;
	}

	do
	{
		t2 = GetTickCount();  // number of milliseconds
	}
	while( ( t2 - t1 ) < timeDelayLimit );

	t1 = t2;
}
#else
void Kernel::delay( void )
{
	static struct timeval t1;
	static struct timeval t2;
	static bool firsttimethrough = true;
	float diff;

	gettimeofday( &t2, NULL );

	if( firsttimethrough )
	{
		gettimeofday( &t1, NULL );

		firsttimethrough = false;
	}

	while( ( diff = timevaldiff( &t1, &t2 ) ) < timeDelayLimit )
	{
		if( busy_wait_time != 0.0f && timeDelayLimit - diff > busy_wait_time )
		{
			float sleep_time = timeDelayLimit - diff - busy_wait_time;
			struct timespec req;
			req.tv_sec = (time_t)sleep_time;
			req.tv_nsec = (long)((sleep_time - req.tv_sec) * 1e9);
			if( pselect( 0, NULL, NULL, NULL, &req, NULL ) == -1 )
			{
				printf( "Error calling pselect" );
				busy_wait_time = 0.0f;
			}
		}
		gettimeofday( &t2, NULL );
	}
	//printf("over frame by %f ms\n", diff - timeDelayLimit);

	t1 = t2;
}
#endif


