/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2004 The Boeing Company
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
 *  FILENAME:   FollowEyePointTransform.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class is a transform node that can be used to cause a 
 *  subgraph to "follow" the eyepoint around.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  08/20/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#include <osgUtil/CullVisitor>

#include "FollowEyePointTransform.h"


// Note - some of this class was grabbed from one of the 
// OSG example programs -ads


// ================================================
// computeLocalToWorldMatrix
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
bool FollowEyePointTransform::computeLocalToWorldMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv) const
{
	osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
	if (cv)
	{
		osg::Vec3 eyePointLocal = cv->getEyeLocal();

		matrix.preMult(osg::Matrix::translate(eyePointLocal.x(),eyePointLocal.y(),eyePointLocal.z()));
	}
	return true;
}

// ================================================
// computeWorldToLocalMatrix
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
bool FollowEyePointTransform::computeWorldToLocalMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv) const
{
	osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
	if (cv)
	{
		osg::Vec3 eyePointLocal = cv->getEyeLocal();

		matrix.postMult(osg::Matrix::translate(-eyePointLocal.x(),-eyePointLocal.y(),-eyePointLocal.z()));
	}
	return true;
}


// ================================================
// computeLocalToWorldMatrix
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
bool FollowEyePointTransformWithCompression::computeLocalToWorldMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv) const
{
	osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
	if (cv)
	{
		osg::Vec3 eyePointLocal = cv->getEyeLocal();

		float zScale = fabs( eyePointLocal.z() );
		if( zScale > SKY_HIGH_ALTITUDE ) zScale = SKY_HIGH_ALTITUDE;
		if( zScale < SKY_LOW_ALTITUDE ) zScale = SKY_LOW_ALTITUDE;
		// we prevent zScale from getting too small by adding something to SHA before dividing
		zScale = 1.0 - (zScale / (SKY_HIGH_ALTITUDE + SKY_LOW_ALTITUDE));
		
		float zOffset = (SKY_DOME_RADIUS / SKY_HORIZON_DISTANCE) * eyePointLocal.z();
		if( zOffset > (SKY_DOME_RADIUS * 0.9) )
			zOffset = SKY_DOME_RADIUS * 0.9;

		matrix.preMult(osg::Matrix::translate(eyePointLocal.x(),eyePointLocal.y(),eyePointLocal.z() - zOffset));
		matrix.preMult(osg::Matrix::scale(1.0f,1.0f, zScale ));
	}
	return true;
}

// ================================================
// computeWorldToLocalMatrix
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
bool FollowEyePointTransformWithCompression::computeWorldToLocalMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv) const
{
	osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
	if (cv)
	{
		osg::Vec3 eyePointLocal = cv->getEyeLocal();

		float zScale = fabs( eyePointLocal.z() );
		if( zScale > SKY_HIGH_ALTITUDE ) zScale = SKY_HIGH_ALTITUDE;
		if( zScale < SKY_LOW_ALTITUDE ) zScale = SKY_LOW_ALTITUDE;
		// we prevent zScale from getting too small by adding something to SHA before dividing
		zScale = 1.0 - (zScale / (SKY_HIGH_ALTITUDE + SKY_LOW_ALTITUDE));

		float zOffset = (SKY_DOME_RADIUS / SKY_HORIZON_DISTANCE) * eyePointLocal.z();
		if( zOffset > (SKY_DOME_RADIUS * 0.9) )
			zOffset = SKY_DOME_RADIUS * 0.9;

		matrix.postMult(osg::Matrix::translate(-eyePointLocal.x(),-eyePointLocal.y(),-(eyePointLocal.z() - zOffset)));
		matrix.postMult(osg::Matrix::scale(1.0f,1.0f,1.0f / zScale));
	}
	return true;
}


