/** <pre>
 *  MPV plugin to automatically manipulate entity articulations
 *  Copyright (c) 2008 Andrew Sampson
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
 *  2008-11-02 Andrew Sampson
 *      Initial release
 *  
 * </pre>
 */

#include <osg/Group>

#include "AutoArticulationElementFactory.h"
#include "AutoArticulationElement.h"

using namespace mpv;
using namespace mpvosg;

// ================================================
// constructor
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
AutoArticulationElementFactory::AutoArticulationElementFactory() : EntityElementFactory()
{
	keyword = "auto_articulation";
}

// ================================================
// destructor
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
AutoArticulationElementFactory::~AutoArticulationElementFactory()
{
	
}


// ================================================
// createElement
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
EntityElement *AutoArticulationElementFactory::createElement( 
	DefFileGroup *elementDefinition, Entity *entity )
{
	if( elementDefinition == NULL || entity == NULL ) return NULL;
	
	// sanity check
	DefFileAttrib *attr = elementDefinition->getAttribute( "element_type" );
	if( attr == NULL || attr->asString() != keyword ) return NULL;
	
	AutoArticulationElement *result = new AutoArticulationElement();
	
	if( !result->construct( elementDefinition, entity ) )
	{
		delete result;
		result = NULL;
	}
	
	return result;
}


