/** <pre>
 *  Entity scene graph plugin
 *  Copyright (c) 2007 Andrew Sampson
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
 *  This class constructs EntityElements containing just a group node
 *  
 *  Initial Release: 2007-03-27 Andrew Sampson
 * </pre>
 */

#include <osg/Group>

#include "GroupElementFactory.h"
#include "GroupElement.h"


using namespace mpv;
using namespace mpvosg;

// ================================================
// constructor
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
GroupElementFactory::GroupElementFactory() : EntityElementFactory()
{
	keyword = "group";
}

// ================================================
// destructor
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
GroupElementFactory::~GroupElementFactory()
{
	
}


// ================================================
// createElement
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
EntityElement *GroupElementFactory::createElement( 
	DefFileGroup *elementDefinition, Entity *entity )
{
	if( elementDefinition == NULL || entity == NULL ) return NULL;
	
	// Note - Ordinarily, a sanity-check would be performed, to confirm 
	// that elementDefinition contains appropriate data for this factory.
	// However, GroupElementFactory is often used as a fallback, when 
	// no appropriate factory can be found.  Hence, the data in 
	// elementDefinition will often not match what is expected.  For this 
	// reason, no sanity check is performed here.
	
	GroupElement *result = new GroupElement();
	
	if( !result->construct( elementDefinition, entity ) )
	{
		delete result;
		result = NULL;
	}
	
	return result;
}


