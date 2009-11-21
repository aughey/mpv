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
 *  FILENAME:   Network.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *   handle the network interface.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  03/29/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#ifndef _MPV_NETWORK_INTERFACE_INCLUDED_
#define _MPV_NETWORK_INTERFACE_INCLUDED_

#if defined(WIN32) && !(defined(__CYGWIN__) || defined(__MINGW32__))
#include <winsock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/select.h>
#endif


#include "MPVCommonTypes.h"

#if defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
#define JUST_IP_ADDRESSES
#endif

//=========================================================
//! The class encapsulating the network interface.
//!
class MPVCMN_SPEC Network
{
public:


   //==> Management

   //=========================================================
   //! General Constructor
   //!
	Network();

   //=========================================================
   //! General Destructor
   //!
	~Network();


   //==> Networking methods


   //=========================================================
   //! Opens a network communication socket
   //! \param ip - The "to" internet address
   //! \param port_send - The port on which the messages are being sent
   //! \param port_rcv - the port on which messages are being received (or
   //! 0 if no receive port is to be allocated).
   //!
   //! \return Success or Failure
   //!
	bool openSocket( const char * ip, const int port_send, const int port_rcv = 0 ) ;

   //=========================================================
   //! Opens a receive only network communication socket.  Only
   //! recv will work.
   //! \param port_rcv - the port on which messages are being received
   //!
   //! \return Success or Failure
   //!
	bool openSocket(const int port_rcv ) ;

   //=========================================================
   //! Closes the socket
   //!
	void closeSocket() ;

   //=========================================================
   //! Sends a message
   //! \param sendbuff - A pointer to the buffer containing the
   //!    message to be sent.
   //! \param sendsize - The size of the message
   //!
   //! \return The number of bytes sent, or -1 on error.
   //!
	int send( unsigned char * sendbuff, int sendsize ) ;

   //=========================================================
   //! Receive a message
   //! \param rcvbuff - A pointer to the buffer to place the
   //!    incoming message.
   //! \param recvsize - The maximum size of the received message.
   //!
   //! \return The number of bytes recieved, or -1 on error.
   //!
	int recv( unsigned char * rcvbuff, int recvsize ) ;

   //=========================================================
   //! Receive a message blocking until one is received
   //! \param rcvbuff - A pointer to the buffer to place the
   //!    incoming message.
   //! \param recvsize - The maximum size of the received message.
   //!
   //! \return The number of bytes recieved, or -1 on error.
   //!
	int recvBlock( unsigned char * rcvbuff, int recvsize ) ;


private:

   //=========================================================
   //! Initializes the communication
   //! \param ipaddr - The "to" internet address
   //! \param sndport - The port on which the messages are being sent
   //! \param rcvport - the port on which messages are being received
   //!
	void InitializeComm(const char *ipaddr, const int sndport, const int rcvport);

   //=========================================================
   //! Initializes incoming communication only
   //! \param rcvport - the port on which messages are being received
   //!
	void InitializeComm(const int rcvport);

	// Berkeley Sockets stuff

   //=========================================================
   //! The send socket
   //!
	int sndsock;

   //=========================================================
   //! The receive socket
   //!
	int rcvsock;

   //=========================================================
   //! The sending socket address
   //!
#ifdef JUST_IP_ADDRESSES
	struct sockaddr_in saddr;
#else
	struct addrinfo *saddr;
#endif // JUST_IP_ADDRESSES

   //=========================================================
   //! The receiving socket address
   //!
	struct sockaddr_in raddr;
	

   //=========================================================
   //! The Valid flag
   //!
	bool valid;
	
};

#endif    //  _MPV_NETWORK_INTERFACE_INCLUDED_
