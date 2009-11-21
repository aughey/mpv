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
 *  FILENAME:   MoveSkyTransform.h
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

#ifndef MOVE_SKY_TRANSFORM_H
#define MOVE_SKY_TRANSFORM_H


#include <osg/Transform>
#include <osg/Matrix>
#include <osg/NodeVisitor>


/* The radius of the sky dome node.  Needs to be a value greater than the 
near clipping plane, but less than the far clipping plane. */
#define SKY_DOME_RADIUS 10000.0f


/* The altitude (in meters) below which the horizon looks normal/uncompressed. 
For altitudes above this, the misty white band at the horizon will become 
narrower. */
#define SKY_LOW_ALTITUDE 900. 

/* The altitude (in meters) above which the horizon will no longer become 
increasingly compressed. */
#define SKY_HIGH_ALTITUDE 20000.

#define SKY_MIST_SCALE_CLAMP_MIN 0.1


/* The distance from the eyepoint to the "horizon".  The horizon, in this case, 
is at some artificial distance, greater than the sky dome radius, and greater 
than the terrapage paging radius.  A value of 90000.0 gives good results, but 
the haze band drops too much and the edge of the terrain no longer blends 
smoothly with the sky.  A larger value is needed. */
#define SKY_HORIZON_DISTANCE 110000.0f


//=========================================================
//! This class is a transform node that can be used to cause a 
//! subgraph to "follow" the eyepoint around.
//! Note - most this class was grabbed from one of the 
//! OSG example programs -ads
//!
class MoveEarthySkyWithEyePointTransform : public osg::Transform
{
public:

	//=========================================================
	//! Get the transformation matrix which moves 
	//! from local coords to world coords.
	//! \param matrix - the matrix to modify
	//! \param nv - the nodevisitor; provides the eyepoint location
	//! \return Returns true on sucess
	//!
	virtual bool computeLocalToWorldMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv) const;


	//=========================================================
	//! Get the transformation matrix which moves 
	//! from world coords to local coords.
	//! \param matrix - the matrix to modify
	//! \param nv - the nodevisitor; provides the eyepoint location
	//! \return Returns true on sucess
	//! 
	virtual bool computeWorldToLocalMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv) const;
};

#endif
