/** <pre>
 * MPV symbology stress test utility
 * Copyright (c) 2007 General Dynamics Land Systems
 * Copyright (c) 2008 Andrew Sampson
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 * 
 * Revision history:
 * 
 * 2008-04-12  Andrew Sampson
 *     Initial version.  Based in part on symbologyTest.cpp, which was in turn 
 *     based on the GDLS symbology test.
 * 
 */


#include <iostream>
#include <cassert>
#include <cmath>
#include <cstring>


#include "SymbologyStress.h"
#include "SymbologyStressTestFunctor.h"
#include "Misc.h"


#if WIN32
#define strcasecmp _stricmp
#endif



using namespace std;


class WaitForIGFunctor : public Functor
{
public:
	WaitForIGFunctor( CigiIncomingMsg &i, CigiOutgoingMsg &o ) : 
		Functor( i, o )
	{
		// set up a callback for SOF packets
		incoming.RegisterEventProcessor( 
			CIGI_SOF_PACKET_ID_V3_2, (CigiBaseEventProcessor *)&sofProcessor );
	}
	virtual ~WaitForIGFunctor()
	{
		incoming.UnregisterEventProcessor( 
			CIGI_SOF_PACKET_ID_V3_2, (CigiBaseEventProcessor *)&sofProcessor );
	}
	
	virtual bool operator()( double deltaT )
	{
		// return true to continue waiting
		return !sofProcessor.igIsInOperateMode;
	}


protected:
	class SOFProcessor : public CigiBaseEventProcessor  
	{
	public:

		SOFProcessor()
			: igIsInOperateMode( false ) {}

		virtual ~SOFProcessor() {}

		virtual void OnPacketReceived( CigiBasePacket *Packet )
		{
			CigiSOFV3_2 *sof = (CigiSOFV3_2 *)(Packet);
			igIsInOperateMode = ( 
				sof->GetIGMode() == CigiBaseSOF::Operate || 
				sof->GetIGMode() == CigiBaseSOF::debug );
		}

		bool igIsInOperateMode;
	};
	SOFProcessor sofProcessor;

};


class SendSurfaceDefinitionFunctor : public Functor
{
public:
	SendSurfaceDefinitionFunctor( CigiIncomingMsg &i, CigiOutgoingMsg &o ) : 
		Functor( i, o )
	{
	}
	virtual ~SendSurfaceDefinitionFunctor()
	{
	}
	
	virtual bool operator()( double deltaT )
	{
		surfaceDef.SetSurfaceID( SURFACE_ID );
		surfaceDef.SetSurfaceState( CigiBaseSymbolSurfaceDef::Active );
		surfaceDef.SetAttached( CigiBaseSymbolSurfaceDef::ViewAttached );

		surfaceDef.SetViewID( 0 );

		surfaceDef.SetMinU( SURFACE_MIN_U );
		surfaceDef.SetMinV( SURFACE_MIN_V );
		surfaceDef.SetMaxU( SURFACE_MAX_U );
		surfaceDef.SetMaxV( SURFACE_MAX_V );

		// set up the surface so that it covers most, but not all, of the view
		surfaceDef.SetTopEdgePosition(    0.9 );
		surfaceDef.SetBottomEdgePosition( 0.1 );

		// left and right extents are adjusted so that the surface is square, 
		// given the aspect ratio of the IG's view
// fixme - assuming a 4:3 ratio
		float aspectRatio = 4.0/3.0;
		float width = (0.9 - 0.1) / aspectRatio;
		
		surfaceDef.SetLeftEdgePosition(   0.5 - width/2.0 );
		surfaceDef.SetRightEdgePosition(  0.5 + width/2.0 );

		/*
		Note - you can account for aspect ratio by either adjusting the 
		surface extents and leaving the min/max uv values alone, or by 
		leaving the surface extents alone and adjusting the min/max uv 
		values.  The choice is yours.  I'd recommend the first option, since 
		it isn't a great idea to mess with your uv coordinates.

		Above, I account for aspect but don't do a very good job of it.  
		I adjust the width of the surface such that it is square on-screen.
		I assume that the view ratio is 4:3; if it isn't, then the symbols 
		will appear stretched.  No big deal; it's just a test.
		*/

		outgoing << surfaceDef;

		// return true to continue calling this functor
		return false;
	}

protected:
	CigiSymbolSurfaceDefV3_3 surfaceDef;
};


SymbologyStress::SymbologyStress() : 
	frame( 0 ), 
	session(), 
	incoming( session.GetIncomingMsgMgr() ), 
	outgoing( session.GetOutgoingMsgMgr() ), 
	incomingBufferSize( 0 )
{

	session.SetCigiVersion( 3, 3 );
	session.SetSynchronous( false );

	IG.SetFrameCntr( frame );
	IG.SetIGMode( CigiBaseIGCtrl::Operate );
	IG.SetDatabaseID( 0 ); // 0 = no database change requested

	outgoing.BeginMsg();

	incoming.SetReaderCigiVersion( 3, 3 );
	incoming.UsingIteration( false );
	
}


SymbologyStress::~SymbologyStress()
{
	network.closeSocket();
}


void SymbologyStress::init()
{

	std::string destIP( "127.0.0.1" );
	int destPort = 8004;
	int recvPort = 8005;
	bool success = network.openSocket( destIP.c_str(), destPort, recvPort );
	if( !success )
	{
		cout << "Unable to open socket (send: "
		<< destIP << ":" << destPort << " recv: " << recvPort << ")" << endl;
		exit( 1 );
	}
	

}


void SymbologyStress::runFunctor( Functor &functor )
{
	bool shouldContinue = true;
	
	// prime the pump
	timer.start();
	
	do
	{
		
		//
		// Process incoming messages
		//

		incomingBufferSize = network.recvBlock( incomingBuffer, sizeof( incomingBuffer ) );
		if( incomingBufferSize <= 0 )
		{
			// some kind of error
			std::cerr << "There was a networking error\n";
			// bail, try again
			return;
		}
		try
		{
			incoming.ProcessIncomingMsg( incomingBuffer, incomingBufferSize );
		}
		catch( CigiException & te )
		{
			cout << "Exception = " << te.what() << endl;
		}

		//
		// Create outgoing messages
		//

		IG.SetFrameCntr( ++frame );
		try
		{
			outgoing << IG;
		}
		catch( CigiException & te )
		{
			cout << __FILE__ << ":" << __LINE__ << " " << te.what() << endl;
		}
		
		timer.stop();

		//
		// Do stuff
		//

		shouldContinue = functor( timer.getElapsedTime() );


		timer.start();

		//
		// Send outgoing messages
		//

		int msg_len;
		outgoing.LockMsg();
		unsigned char *cigi_buffer = outgoing.GetMsg( msg_len );

		int retval = network.send( cigi_buffer, msg_len );
		if( retval == -1 )
		{
			cout << "Error sending!" << endl;
		}

		outgoing.UnlockMsg();
//		outgoing.BeginMsg();

	} while( shouldContinue );
	
}


int SymbologyStress::run()
{
	
	cout << "Waiting for the IG to send SOF and transition to Operate mode." << endl;
	
	// wait until the IG is ready
	{
	WaitForIGFunctor waitForIG( incoming, outgoing );
	runFunctor( waitForIG );
	}
		
	cout << "IG is ready.  Beginning tests." << endl;
	
	
	// set up the symbol surface
	{
	SendSurfaceDefinitionFunctor sendSurfaceDef( incoming, outgoing );
	runFunctor( sendSurfaceDef );
	}
	
	
	// run the test
	{
	SymbologyStressTestFunctor symbologyStressTest( incoming, outgoing );
	runFunctor( symbologyStressTest );
	}
	
	
	return 0;
}



int main( void )
{
	SymbologyStress symbologyStress;
	symbologyStress.init();
	return symbologyStress.run();
}

