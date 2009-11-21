/** <pre>
 *  Plugin to add text labels to entities, for the MPV
 *  Copyright (c) 2008 Andrew Sampson
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
 *  2008-08-18 Andrew Sampson
 *      Initial release.  Split code out from LabelElement.
 *  
 *  
 *  </pre>
 */


#include <string.h> // for memset

#include "BindSlot.h"

#include "LabelNodeComponentImp.h"

using namespace mpv;

// ================================================
// LabelNodeComponentImp
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
LabelNodeComponentImp::LabelNodeComponentImp( mpv::Component *_component, 
                                              const std::string &defaultText, 
                                              LabelNode *newLabel ) : 
	ComponentImp( _component ), 
	labelText( defaultText ), 
	labelNode( newLabel )
{
	componentDataChanged( component );
	
	component->dataChanged.connect( BIND_SLOT1( LabelNodeComponentImp::componentDataChanged, this ) );
}


// ================================================
// ~LabelNodeComponentImp
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
LabelNodeComponentImp::~LabelNodeComponentImp() 
{
	
}


void LabelNodeComponentImp::componentDataChanged( Component * )
{
//FIXME - disabled - mpv::Component has no accessors for data yet
return;
	if( !labelNode.valid() )
		return;
	
	/* character array, for storing the incoming string data.
	24 chars for the actual string, plus one "safety" char, for 
	null-termination, to prevent overruns.*/
	char labelArray[25];
	memset( labelArray, 0, sizeof(labelArray) );
	
	// retrieve the Component data
	//FIXME - mpv::Component has no accessors for data yet, and doesn't handle 
	//byte order issues either...
	
	// update labelText with the contents of the Component data
	labelText = labelArray;
	
	labelNode->setText( labelText );
}

