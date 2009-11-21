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
 *  2008-08-02 Andrew Sampson
 *      Initial release.  Split code out from ModelElement.
 *  
 * </pre>
 */

#include <osg/PositionAttitudeTransform>
#include <osgSim/DOFTransform>
#include <osg/MatrixTransform>

#include "BindSlot.h"

#include "TransformNodeArticulationImp.h"


osg::Vec3 TransformNodeArticulationImp::xAxis( 1.0, 0.0, 0.0 );
osg::Vec3 TransformNodeArticulationImp::yAxis( 0.0, 1.0, 0.0 );
osg::Vec3 TransformNodeArticulationImp::zAxis( 0.0, 0.0, 1.0 );

using namespace mpv;


TransformNodeArticulationImp::TransformNodeArticulationImp( mpv::Articulation *_articulation, osg::Transform *newXform ) : 
	ArticulationImp( _articulation ), 
	transformNode( newXform )
{
	
	// FIXME - set up motion limits (clamping stuff) if transform node is a DOFTransform
	// FIXME - enabling and disabling an articulated part is not implemented
	
	transformChanged( articulation );
	
	articulation->offsetChanged.connect( BIND_SLOT1( TransformNodeArticulationImp::transformChanged, this ) );
	articulation->rotationChanged.connect( BIND_SLOT1( TransformNodeArticulationImp::transformChanged, this ) );
}


TransformNodeArticulationImp::~TransformNodeArticulationImp()
{

}


void TransformNodeArticulationImp::transformChanged( Articulation * )
{
	if( !transformNode.valid() )
		return;

	Vect3 offset = articulation->getOffset();
	Vect3 rotation = articulation->getRotation();
	
	// It is likely that articulated part nodes can be any type of 
	// transform node.  However, the only type of art part node I've seen 
	// so far has been the DOFTransform nodes (found in OpenFlight models).
	// To be safe, though, we'll handle all the different transform nodes.
	if( dynamic_cast<osgSim::DOFTransform*>( transformNode.get() ) != NULL ) 
	{
		osgSim::DOFTransform *dof = dynamic_cast<osgSim::DOFTransform*>( transformNode.get() );

		// first, disable animation (animation is turned on by default 
		// for OSG's DOF nodes)
		dof->setAnimationOn( false );

		// now set the offset and rotation
		dof->setCurrentTranslate( osg::Vec3( 
			offset.gety(), 
			offset.getx(), 
			offset.getz() * -1.0 ) );

		// RMW  3/20/08  Ensure HPR ordering is per CIGI standard.
		dof->setHPRMultOrder(osgSim::DOFTransform::HPR);

		dof->setCurrentHPR( osg::Vec3( 
			osg::DegreesToRadians(rotation.getx() * -1.0),
			osg::DegreesToRadians(rotation.gety()),
			osg::DegreesToRadians(rotation.getz()) ) );
	}
	else if( transformNode->asPositionAttitudeTransform() ) 
	{
		osg::PositionAttitudeTransform *pa = 
			transformNode->asPositionAttitudeTransform();

		// set offset
		pa->setPosition( osg::Vec3( 
			offset.gety(), 
			offset.getx(), 
			offset.getz() * -1.0 ) );

		// set rotation
		pa->setAttitude( osg::Quat( 
			osg::DegreesToRadians(rotation.getz()), yAxis, 
			osg::DegreesToRadians(rotation.gety()), xAxis, 
			osg::DegreesToRadians(rotation.getx() * -1.0), zAxis ) );
	}
	else if( transformNode->asMatrixTransform() ) 
	{
		osg::MatrixTransform *mt = 
			transformNode->asMatrixTransform();
		osg::Matrix m;
		m.makeIdentity();

		// set offset
		m *= osg::Matrix::translate( 
			offset.gety(), 
			offset.getx(), 
			offset.getz() * -1.0 );

		// set rotation
		m *= osg::Matrix::rotate( 
			osg::DegreesToRadians(rotation.getz()), yAxis, 
			osg::DegreesToRadians(rotation.gety()), xAxis, 
			osg::DegreesToRadians(rotation.getx() * -1.0), zAxis );

		mt->setMatrix( m );
	}

}
