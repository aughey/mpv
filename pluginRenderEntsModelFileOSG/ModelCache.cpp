/** <pre>
 *  Entity scene graph plugin
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
 *  2008-06-29 Andrew Sampson
 *      Initial release
 *  
 * </pre>
 */


#include "ModelCache.h"


ModelCache::ModelCache() : osg::Referenced()
{
	
}


ModelCache::~ModelCache()
{
	
}


void ModelCache::add( const std::string &filename, osg::Node *node )
{
	modelCache[filename] = node;
}


osg::Node *ModelCache::get( const std::string &filename )
{
	std::map< std::string, osg::ref_ptr<osg::Node> >::iterator iter;
	iter = modelCache.find( filename );
	if( iter == modelCache.end() )
		return NULL;
	else
		return iter->second.get();
}


