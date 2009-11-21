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

#ifndef _TRANSFORM_NODE_ARTICULATION_IMP_H_
#define _TRANSFORM_NODE_ARTICULATION_IMP_H_

#include <osg/Transform>

#include "Articulation.h"

#include "MPVCommonOSGTypes.h"

//=========================================================
//! This class will update the state of a transform node, according to the 
//! position and orientation of an articulated part.
//! 
class MPVCMNOSG_SPEC TransformNodeArticulationImp : public mpv::ArticulationImp
{
public:
	
	//! \param newXform - the transform node that this articulated part is associated with
	//! 
	TransformNodeArticulationImp( mpv::Articulation *_articulation, osg::Transform *newXform );
	
	
protected:
	virtual ~TransformNodeArticulationImp();

	void transformChanged( mpv::Articulation * );

	//=========================================================
	//! The transform node that this articulated part is associated with
	//! 
	osg::ref_ptr<osg::Transform> transformNode;

	//=========================================================
	//! Convenience variable
	//! 
	static osg::Vec3 xAxis;
	//=========================================================
	//! Convenience variable
	//! 
	static osg::Vec3 yAxis;
	//=========================================================
	//! Convenience variable
	//! 
	static osg::Vec3 zAxis;

};

#endif
