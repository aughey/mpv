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
 *  FILENAME:   SkyDomeNode.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class defines a sky dome node.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  04/26/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#ifndef INCLUDE_SKYDOMENODE
#define INCLUDE_SKYDOMENODE 1

#include <osg/Group>



class SkyDomeNode : public osg::Group
{
    public :
    
		//=========================================================
		//! General Constructor
		//!
        SkyDomeNode();

		//=========================================================
		//! Copy constructor 
		//!
        SkyDomeNode(const SkyDomeNode&,const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY);

		//=========================================================
		//! Macro that sets up some OSG stuff
		//!
        META_Node(osg, SkyDomeNode);

    protected :
		//=========================================================
		//! General Destructor (protected b/c it inherits from osg::Referenced)
		//!
        virtual ~SkyDomeNode() {}
		
	private:
		
		//=========================================================
		//! Creates the skydome geometry
		//!
		osg::Node *makeSky();
};

#endif
