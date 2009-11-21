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
 *  FILENAME:   MoveDummyTerrainTransform.cpp
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

#include "MoveDummyTerrainTransform.h"



// ================================================
// computeLocalToWorldMatrix
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
bool MoveDummyTerrainTransform::computeLocalToWorldMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv) const
{
	osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
	if (cv)
	{
		osg::Vec3 eyePointLocal = cv->getEyeLocal();
		int x, y;
		x = (int)eyePointLocal.x();
		y = (int)eyePointLocal.y();
		x /= 1000;
		x *= 1000;
		y /= 1000;
		y *= 1000;

		matrix.preMult(osg::Matrix::translate(x,y,0.));
	}
	return true;
}

// ================================================
// computeWorldToLocalMatrix
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
bool MoveDummyTerrainTransform::computeWorldToLocalMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv) const
{
	osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
	if (cv)
	{
		osg::Vec3 eyePointLocal = cv->getEyeLocal();
		int x, y;
		x = (int)eyePointLocal.x();
		y = (int)eyePointLocal.y();
		x /= 1000;
		x *= 1000;
		y /= 1000;
		y *= 1000;

		matrix.postMult(osg::Matrix::translate(-x,-y,0.));
	}
	return true;
}


