/** <pre>
 *  Plugin to add text labels to entities, for the MPV
 *  Copyright (c) 2006 Andrew Sampson
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
 *  Initial Release: 2006-03-19 Andrew Sampson
 *  
 * </pre>
 */


#ifndef LABELNODE_H
#define LABELNODE_H

#include <osg/Group>
#include <osgText/Text>
#include <string>

//=========================================================
//! This class ...
//! 
class LabelNode : public osg::Group
{
    public :
    
		//=========================================================
		//! General Constructor
		//!
        LabelNode();

		//=========================================================
		//! Copy constructor 
		//!
        LabelNode(const LabelNode&,const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY);
		
		//=========================================================
		//! 
		//! \param newText - the text to display
		//!
		void setText( const std::string &newText );

		//=========================================================
		//! 
		//! \param color - the color for the text
		//!
		void setColor( const osg::Vec3 &color );

		//=========================================================
		//! 
		//! \param newSize - the new character size
		//!
		void setSize( float newSize );

		//=========================================================
		//! 
		//! \param newPosition - the offset for the text
		//!
		void setPosition( const osg::Vec3 &newPosition );
		
		//=========================================================
		//! Macro that sets up some OSG stuff
		//!
        META_Node(osg, LabelNode);

		//=========================================================
		//! Updates the text's opacity, according to the distance between 
		//! the eyepoint and the node.
		//! \param nv - the node visitor that is acting on this node
		//!
		void traverse( osg::NodeVisitor &nv );

    protected :
		//=========================================================
		//! General Destructor (protected b/c it inherits from osg::Referenced)
		//!
        virtual ~LabelNode() {}
		
	private:
		//=========================================================
		//! The eyepoint-to-node distance where the text reaches its 
		//! maximum opacity (alphaMax).
		//!
		float fadeDistNear;

		//=========================================================
		//! The eyepoint-to-node distance beyond which the text completely 
		//! disappears.
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
		//! The color of the text.
		//!
		osg::Vec3 textColor;

		//=========================================================
		//! The text label.  We need to hold on to a reference to it so 
		//! that we can modify its opacity in traverse() and modify its 
		//! text string in setText().
		//!
		osg::ref_ptr< osgText::Text > textDrawable;

		//=========================================================
		//! Initializes member variables.  Called by constructors.
		//!
		void init();

		//=========================================================
		//! A helper function which creates textDrawable and attaches it as a 
		//! child.
		//!
		void createText();
};

#endif
