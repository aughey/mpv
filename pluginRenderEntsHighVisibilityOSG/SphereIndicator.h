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
 *  FILENAME:   SphereIndicator.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class defines a node that creates a sphere, and causes that sphere 
 *  to fade as the distance from the camera decreases (yes, it disappears 
 *  when it gets closer).
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  04/20/2005 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#ifndef SPHERE_INDICATOR_H
#define SPHERE_INDICATOR_H

#include <osg/Group>
#include <osg/ShapeDrawable>


//=========================================================
//! This class defines a node that creates a sphere, and causes that sphere 
//! to fade as the distance from the camera decreases (yes, it disappears 
//! when it gets closer).
//! 
class SphereIndicator : public osg::Group
{
    public :
    
		//=========================================================
		//! General Constructor
		//!
        SphereIndicator();

		//=========================================================
		//! Constructor, taking color as argument
		//! \param color - the color for the sphere
		//!
		SphereIndicator( const osg::Vec3 &color );

		//=========================================================
		//! Copy constructor 
		//!
        SphereIndicator(const SphereIndicator&,const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY);

		//=========================================================
		//! Macro that sets up some OSG stuff
		//!
        META_Node(osg, SphereIndicator);

		//=========================================================
		//! Updates the sphere's opacity, according to the distance between 
		//! the eyepoint and the node.
		//! \param nv - the node visitor that is acting on this node
		//!
		void traverse( osg::NodeVisitor &nv );
		
		//=========================================================
		//! Sets the sphere color.  Can be called at any time.
		//! \param newSphereColor - the new color for the sphere
		//!
		void setColor( const osg::Vec3 &newSphereColor ) { sphereColor = newSphereColor; }

    protected :
		//=========================================================
		//! General Destructor (protected b/c it inherits from osg::Referenced)
		//!
        virtual ~SphereIndicator() {}
		
	private:
		//=========================================================
		//! The eyepoint-to-node distance where the sphere completely 
		//! disappears.
		//!
		float fadeDistNear;

		//=========================================================
		//! The eyepoint-to-node distance beyond which the sphere reaches its 
		//! maximum opacity (alphaMax).
		//!
		float fadeDistFar;
		
		//=========================================================
		//! The minimum alpha value.  
		//!
		float alphaMin;

		//=========================================================
		//! The maximum alpha value.
		//!
		float alphaMax;
		
		//=========================================================
		//! The color of the sphere.
		//!
		osg::Vec3 sphereColor;

		//=========================================================
		//! The sphere drawable.  We need to hold on to a reference to it so 
		//! that we can modify its opacity in traverse().
		//!
		osg::ref_ptr< osg::ShapeDrawable > shapeDrawable;

		//=========================================================
		//! Initializes member variables.  Called by constructors.
		//!
		void init();

		//=========================================================
		//! A helper function which creates shapeDrawable and attaches it as a 
		//! child.
		//!
		void createSphere();
};

#endif
