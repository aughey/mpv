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
 *  FILENAME:   StarDomeNode.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class creates the star field and updates the transform associated 
 *  with it.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  2006-02-25 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#ifndef INCLUDE_STARDOMENODE
#define INCLUDE_STARDOMENODE 1

#include <osg/Group>
#include <osg/Vec3>
#include <osg/MatrixTransform>

#include "StarCatalog.h"

class StarDomeNode : public osg::Group
{
public:

	//=========================================================
	//! General Constructor
	//!
	StarDomeNode();

	//=========================================================
	//! Copy constructor 
	//!
	StarDomeNode(const StarDomeNode&,const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY);

	//=========================================================
	//! Macro that sets up some OSG stuff
	//!
	META_Node(osg, StarDomeNode);

	//=========================================================
	//! constructs the star geometry
	//!
	void init();

	//=========================================================
	//! updates the orientation and brightness of the star field
	//! \param latitude - latitude of the eyepoint
	//! \param localMeanSiderealTime - sidereal time, adjusted for local lon.
	//! \param skyIntensity - brightness of the sky dome
	//!
	void updateStars( 
		double latitude, double localMeanSiderealTime, float skyIntensity );
	
protected:
	//=========================================================
	//! General Destructor (protected b/c it inherits from osg::Referenced)
	//!
	virtual ~StarDomeNode();

private:
	
	osg::ref_ptr<osg::Geometry> starField;
	
	osg::ref_ptr<osg::MatrixTransform> starFieldTransform;
	
	//=========================================================
	//! Adjusts a star's color, desaturating it to mimic human visual 
	//! response in low-light conditions.
	//! \param I - intensity of star
	//! \param r - red
	//! \param g - green
	//! \param b - blue
	//! \return the desaturated color
	//!
	osg::Vec4 desaturateStarColor( float I, float r, float g, float b );
};

#endif
