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
 *  This class is responsible for creating a subgraph containing a 
 *  transform node.  It can be used to move and rotate other entity elements,
 *  which can be attached underneath.
 *  
 *  Initial Release: 2007-11-03 Andrew Sampson
 * </pre>
 */


#include "TransformNodeArticulationImp.h"

#include "TransformElement.h"

using namespace mpv;
using namespace mpvosg;

// statics
osg::Vec3 TransformElement::xAxis( 1., 0., 0. );
osg::Vec3 TransformElement::yAxis( 0., 1., 0. );
osg::Vec3 TransformElement::zAxis( 0., 0., 1. );


TransformElement::TransformElement() : EntityElement()
{
	transformNode = new osg::MatrixTransform;
}

TransformElement::~TransformElement()
{
	
}


bool TransformElement::construct( DefFileGroup *config, Entity *entity )
{
	osg::Matrixd mtx;
	mtx.makeIdentity();

	DefFileAttrib *attr;
	attr = config->getAttribute( "offset" );
	if( attr )
	{
		std::vector<float> v = attr->asFloats();
		if( v.size() >= 3 )
			mtx.setTrans( v[0], v[1], v[2] );
	}

	attr = config->getAttribute( "rotate" );
	if( attr )
	{
		std::vector<float> v = attr->asFloats();
		if( v.size() >= 3 )
			mtx.setRotate( osg::Quat( 
				osg::DegreesToRadians( v[2] ), yAxis, 		// roll
				osg::DegreesToRadians( v[1] ), xAxis, 		// pitch
				osg::DegreesToRadians( v[0] * -1.0), zAxis	// yaw
			 ) );
	}
	
	// fixme - allow user to specify full 4x4 matrix (would that be useful?)
	
	transformNode->setMatrix( mtx );
	
	// allow user to specify articulated part ID, to allow Host to 
	// control this matrix
	attr = config->getAttribute( "art_part_id" );
	if( attr )
	{
		Articulation *articulation = entity->findOrCreateArticulation( attr->asInt() );
		// check to see if the articulation was just created
		if( articulation->getEntityID() != entity->getID() )
		{
			articulation->setEntityID( entity->getID() );
		}

		articulation->addImplementation( 
			new TransformNodeArticulationImp( articulation, transformNode->asTransform() ) );
	}

	return true;
}


bool TransformElement::addChildElement( EntityElement *childElement )
{
	if( childElement == NULL ) return false;
	
	osg::Node *node = childElement->getTopNode();
	if( node == NULL ) return false;
	
	transformNode->addChild( node );
	
	// call parent
	return EntityElement::addChildElement( childElement );
}

