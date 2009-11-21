/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2006 Andrew Sampson
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
 *  2006-10-29 Andrew Sampson
 *      Initial release
 *  
 *  2008-08-17 Andrew Sampson
 *      Ported to new Component interface
 *  
 */

#include <stdio.h>
#include <iostream>

#include "BindSlot.h"

#include "ScaleComponentImp.h"

using namespace mpv;

// ================================================
// ScaleComponentImp
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ScaleComponentImp::ScaleComponentImp( mpv::Component *_component, osg::MatrixTransform *newTransform ) : 
	ComponentImp( _component ),
	transform( newTransform )
{
	componentDataChanged( component );
	
	component->dataChanged.connect( BIND_SLOT1( ScaleComponentImp::componentDataChanged, this ) );
}


// ================================================
// 
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ScaleComponentImp::~ScaleComponentImp()
{
}


// ================================================
// 
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void ScaleComponentImp::componentDataChanged( mpv::Component * )
{
	if( !transform.valid() )
		return;

/*FIXME - Component has no mechanism for accessing component data
		// turn the unsigned integer component ctrl value into a float 
		// FIXME - this is code/functionality that belongs in CompCtrl, not here.
		union 
		{
			// FIXME - this makes assumptions about the lengths of variables - not 64-bit safe
			unsigned int intValue;
			float floatValue;
		} intFloatUnion;
		
		intFloatUnion.intValue = GetCompData( 0 );
		
		float scaleFactor = intFloatUnion.floatValue;
		if( scaleFactor < 0.01 || scaleFactor > 1000.0 )
		{
			std::cerr << "Error - the Host tried to set the scale for an entity to " 
				<< scaleFactor << ", which is an illegal value.\n"
				<< "\tReseting scale to 1.0.\n";
			scaleFactor = 1.0;
		}
		
		osg::Matrix matrix;
		matrix.makeScale( scaleFactor, scaleFactor, scaleFactor );
		transform->setMatrix( matrix );
		
		// FIXME - this code neglects to fix the normals for the scaled models; as a 
		// result, the lighting for the scaled models will be wrong
*/
}


