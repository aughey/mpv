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
 *  FILENAME:   NearFarManipulatorElementFactory.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  DESCRIPTION: 
 *  This class constructs EntityElements containing nodes which will 
 *  manipulate the near and far clipping plane values.
 *  
 *  2007-05-22 Andrew Sampson
 *  
 */

#include "NearFarManipulatorElementFactory.h"
#include "NearFarManipulatorElement.h"


using namespace mpv;
using namespace mpvosg;

// ================================================
// constructor
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
NearFarManipulatorElementFactory::NearFarManipulatorElementFactory() : EntityElementFactory()
{
	keyword = "near_model";
}

// ================================================
// destructor
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
NearFarManipulatorElementFactory::~NearFarManipulatorElementFactory()
{
	
}


// ================================================
// createElement
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
EntityElement *NearFarManipulatorElementFactory::createElement( 
	DefFileGroup *elementDefinition, Entity *entity )
{
	if( elementDefinition == NULL || entity == NULL ) return NULL;
	
	// sanity check
	DefFileAttrib *attr = elementDefinition->getAttribute( "element_type" );
	if( attr == NULL || attr->asString() != keyword ) return NULL;
	
	NearFarManipulatorElement *result = new NearFarManipulatorElement();
	
	if( !result->construct( elementDefinition, entity ) )
	{
		delete result;
		result = NULL;
	}
	
	return result;
}


