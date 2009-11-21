/** <pre>
 *  The MPV Common Library
 *  Copyright (c) 2008 
 *  
 *  This component is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; specifically 
 *  version 2.1 of the License.
 *  
 *  This software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *  
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this software; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *  
 *  
 *  2008-07-06 Andrew Sampson
 *      Initial release
 *  
 *  
 *  </pre>
 */


#include <string.h> // for memset

#include "Component.h"

using namespace mpv;


// ================================================
// Component
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Component::Component() : Referenced()
{
	id = 0xffff;
	instanceID = 0xffff;
	state = 0xff;

	memset( data, 0, sizeof( data ) );
}


// ================================================
// ~Component
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Component::~Component() 
{
	
}


// ================================================
// update
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Component::update( double timeElapsed )
{
	std::list< RefPtr<ComponentImp> >::iterator iter;
	for( iter = imps.begin(); iter != imps.end(); iter++ )
	{
		if( (*iter).valid() )
			(*iter)->update( timeElapsed );
	}
}


// ================================================
// processPacket
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Component::processPacket( CigiCompCtrlV3_3 *packet )
{
	int dataIndex = 0;
	for( unsigned int wordIndex = 0; wordIndex < 6; wordIndex++ )
	{
		for( unsigned int byteIndex = 0; byteIndex < 4; byteIndex++ )
		{
			data[dataIndex++] = packet->GetUCharCompData( wordIndex, static_cast<CigiBaseCompCtrl::BytePos>(byteIndex) );
		}
	}
	dataChanged( this );

	setState( packet->GetCompState() );

	packetReceived( this );
}


// ================================================
// processPacket
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Component::processPacket( CigiShortCompCtrlV3_3 *packet )
{
	// short control packets don't overwrite all the elements of data
	memset( data, 0, sizeof( data ) );

	int dataIndex = 0;
	for( unsigned int wordIndex = 0; wordIndex < 2; wordIndex++ )
	{
		for( unsigned int byteIndex = 0; byteIndex < 4; byteIndex++ )
		{
			data[dataIndex++] = packet->GetUCharCompData( wordIndex, static_cast<CigiBaseCompCtrl::BytePos>(byteIndex) );
		}
	}
	dataChanged( this );

	setState( packet->GetCompState() );

	packetReceived( this );
}


// ================================================
// setID
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Component::setID( Cigi_uint16 newID )
{
	id = newID;
}


// ================================================
// setInstanceID
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Component::setInstanceID( Cigi_uint16 newInstanceID )
{
	instanceID = newInstanceID;
}


// ================================================
// setName
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Component::setName( const std::string &newName )
{
	if( name != newName )
	{
		name = newName;
		nameChanged( this );
	}
}


// ================================================
// setState
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Component::setState( Cigi_uint8 newState )
{
	if( state != newState )
	{
		state = newState;
		stateChanged( this );
	}
}


// ================================================
// addImplementation
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Component::addImplementation( ComponentImp *newImp )
{
	if( newImp != NULL )
		imps.push_back( newImp );
}


