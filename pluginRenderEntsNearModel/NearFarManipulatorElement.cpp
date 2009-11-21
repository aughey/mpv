/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2007 The Boeing Company
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
 *  FILENAME:   NearFarManipulatorElement.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class creates nodes which, when inserted into an entity's subgraph, 
 *  will manipulate the near and far clipping plane values.
 *  
 *  03-29-2005 David Fries
 *      Initial implementation.
 *  
 *  2007-05-22 Andrew Sampson
 *      Ported to new entity-construction system.
 */

#include <limits.h>
#include <iostream>
#include <osg/Projection>
#include <osg/Geode>

#include "NearFarManipulatorElement.h"

#include "glClearDrawable.h"
#include "ProjectionCallback.h"


using namespace mpv;
using namespace mpvosg;

NearFarManipulatorElement::NearFarManipulatorElement() : EntityElement()
{
}


NearFarManipulatorElement::~NearFarManipulatorElement()
{
	
}


bool NearFarManipulatorElement::construct( DefFileGroup *config, Entity *entity )
{
	/* Some things like a canopy are well within the normal near clipping
	 * plane and would be completely clipped out of the scene.  The
	 * following code is designed to reset the near and far clipping plane
	 * to draw a subtree that is near to the camera.
	 *
	 * generate the following tree.
	 * The graph looks like this
	 * Group
	 * |
	 * Projection
	 * |		\
	 * Geode	SubTree
	 * |
	 * glClearDrawable
	 *
	 * The group node has a callback to update the projection matrix in
	 * the cull phase to a frustum with a different near and far clipping
	 * plane.  It also updates the glClearDrawable & Geode's bounding box
	 * to match that of the SubTree.
	 *
	 * The geode sets the render bin to be the next to last bin so the
	 * drawable callback gets called after the rest of the scene and
	 * before the model.  The drawable callback clears the depth buffer.
	 *
	 * The SubTree is set to the last rendering bin and will be rendered
	 * with the updated projection matrix.
	 *
	 * The projection's order of operation is apply state set, set
	 * projection matrix, call callbacks, then pop operations.  That
	 * requires the projection update callback to take place before the
	 * transform node.
	 */

	double near = .1, far = 30;
	DefFileAttrib *attr;
	
	attr = config->getAttribute("near");
	if(attr)
		near = attr->asFloat();

	attr = config->getAttribute("far");
	if(attr)
		far = attr->asFloat();

	if( near > far )
	{
		std::cerr << "Error when setting up near/far-clipping-plane manipulator - " 
			<< "\n\tnear value (" << near << ") is greater than far value ("
			<< far << ")." << std::endl;
		return false;
	}
	
	subgraph = new osg::Group;
	nearFarTree = new osg::Group;

	nearFarTree->setCullCallback(new ProjectionCallback(near, far));
	/* draw after all the normal objects in the scene */
	nearFarTree->getOrCreateStateSet()->setRenderBinDetails(INT_MAX,
		"RenderBin");

	/* group -> projection -> subgraph */
	osg::Projection *projection = new osg::Projection;
	nearFarTree->addChild(projection);

	osg::Geode *geode = new osg::Geode;
	geode->setName(clear_geode_name);
	/* first sub redering bin to draw */
	geode->getOrCreateStateSet()->setRenderBinDetails(INT_MIN,
		"RenderBin");
	geode->addDrawable(new glClearDrawable);
	projection->addChild(geode);

	osg::StateSet *ss = subgraph->getOrCreateStateSet();
	/* The drawing order is after the depth buffer clear call
	 * and before other items like the hud are drawn.
	 */
	ss->setRenderBinDetails(INT_MIN+1, "RenderBin");
	/* No fog as close as this will be. */
	ss->setMode(GL_FOG, osg::StateAttribute::OFF);
	projection->addChild( subgraph.get() );

	return true;
}


bool NearFarManipulatorElement::addChildElement( EntityElement *childElement )
{
	if( childElement == NULL ) return false;
	
	osg::Node *node = childElement->getTopNode();
	if( node == NULL ) return false;
	
	subgraph->addChild( node );
	
	// call parent
	return EntityElement::addChildElement( childElement );
}


