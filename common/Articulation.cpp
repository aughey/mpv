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


#include "Articulation.h"

using namespace mpv;


// ================================================
// Articulation
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Articulation::Articulation() : Referenced()
{
	id = 0xffff;
	entityID = 0xffff;
	enabled = true;
	
}


// ================================================
// ~Articulation
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Articulation::~Articulation() 
{
	
}


// ================================================
// update
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Articulation::update( double timeElapsed )
{
	std::list< RefPtr<ArticulationImp> >::iterator iter;
	for( iter = imps.begin(); iter != imps.end(); iter++ )
	{
		if( (*iter).valid() )
			(*iter)->update( timeElapsed );
	}
}


// ================================================
// setID
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Articulation::setID( Cigi_uint16 newID )
{
	id = newID;
}


// ================================================
// setEntityID
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Articulation::setEntityID( Cigi_uint16 newEntityID )
{
	entityID = newEntityID;
}


// ================================================
// setName
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Articulation::setName( const std::string &newName )
{
	if( name != newName )
	{
		name = newName;
		nameChanged( this );
	}
}


// ================================================
// setEnabled
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Articulation::setEnabled( bool newEnabled )
{
	if( enabled != newEnabled )
	{
		enabled = newEnabled;
		enabledChanged( this );
	}
}


// ================================================
// setOffset
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Articulation::setOffset( const Vect3 &newOffset )
{
//	if( offset != newOffset )
	{
		offset = newOffset;
		offsetChanged( this );
	}
}


// ================================================
// setRotation
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Articulation::setRotation( const Vect3 &newRotation )
{
//	if( rotation != newRotation )
	{
		rotation = newRotation;
		rotationChanged( this );
	}
}


// ================================================
// setOffsetVelocity
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Articulation::setOffsetVelocity( const Vect3 &newOffsetVelocity )
{
//	if( offsetVelocity != newOffsetVelocity )
	{
		offsetVelocity = newOffsetVelocity;
		offsetVelocityChanged( this );
	}
}


// ================================================
// setRotationVelocity
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Articulation::setRotationVelocity( const Vect3 &newRotationVelocity )
{
//	if( rotationVelocity != newRotationVelocity )
	{
		rotationVelocity = newRotationVelocity;
		rotationVelocityChanged( this );
	}
}


// ================================================
// addImplementation
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void Articulation::addImplementation( ArticulationImp *newImp )
{
	if( newImp != NULL )
		imps.push_back( newImp );
}


