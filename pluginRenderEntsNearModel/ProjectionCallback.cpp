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
 *  FILENAME:   ProjectionCallback.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *
 *  PROGRAM DESCRIPTION:
 *  This class resets the projection node to have a different near and
 *  far clipping plane while keeping the rest of the view frustum the
 *  same.  It also updates the bounding box of the drawable responsible
 *  for clearing the depth buffer before the projection change takes place.
 *
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *
 *  03-29-2005 David Fries                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#include <iostream>

#include <osg/Projection>
#include <osgUtil/CullVisitor>
#include <osg/Geode>

#include "ProjectionCallback.h"
#include "glClearDrawable.h"

#ifdef WIN32
#define __PRETTY_FUNCTION__ "ProjectionCallback::operator()"
#endif

ProjectionCallback::ProjectionCallback(): ProjectionNear(.1), ProjectionFar(100)
{
	Traversal = -1;
	LastNode = NULL;
}

ProjectionCallback::ProjectionCallback(float near_arg, float far_arg):
	ProjectionNear(near_arg), ProjectionFar(far_arg)
{
	Traversal = -1;
	LastNode = NULL;
}

ProjectionCallback::ProjectionCallback(const ProjectionCallback &p)
{
	Init();
}


ProjectionCallback::ProjectionCallback(const ProjectionCallback &p,
	const osg::CopyOp &co): NodeCallback(p, co)
{
	Init();
}

void ProjectionCallback::Init()
{
	Projection = NULL;
	ClearGeode = NULL;
	LastNode = NULL;
	OldProjectionMatrix.makeIdentity();
	Traversal = -1;
}

void ProjectionCallback::operator() (osg::Node *node, osg::NodeVisitor *nv)
{
	/* Get the cull visitor in the cull phase. */
	osgUtil::CullVisitor *cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
	if(!cv)
		return;

	/* Each tree needs it's own callback, but the default tree
	 * clone operation does not copy callbacks, so we must detect
	 * if the callback is part of a different tree here and create
	 * a new object for the new tree.
	 */
	if(LastNode)
	{
		if(LastNode != node)
		{
			/* Create a new callback for the new tree.
			 * If the tree was copied both trees share the
			 * same callback object, if the original tree
			 * is then deleted there is only one reference
			 * to the callback object which will go away
			 * as soon as we assign the new callback
			 * object, so we must not reference anything
			 * more in this object once we set the
			 * callback.
			 */
			ProjectionCallback *p = new ProjectionCallback;
			/* Let it update the new projection. */
			(*p)(node, nv);
			node->setCullCallback(p);
			return;
		}
	}
	else
	{
		/* claim this tree */
		LastNode = node;
	}

	const int pass = nv->getTraversalNumber();
	/* Only execute once per frame. */
	if(pass == Traversal)
		return;
	Traversal = pass;

	/* Only walk the tree looking for nodes when we have to. */
	if(!Projection.get() || !ModelGraph.get() || !ClearGeode.get())
	{
		/* The graph looks like this
		 * Group
		 * |
		 * Projection
		 * |		\
		 * SubTree	Geode
		 * 		|
		 * 		glClearDrawable
		 */
		osg::Group *group = dynamic_cast<osg::Group*>(node);
		/* not found */
		if(!group)
		{
			std::cerr << "Warning: " << __PRETTY_FUNCTION__
				<< " not called from a group node\n";
			return;
		}
		if(group->getNumChildren() != 1)
		{
			std::cerr << "Error: " << __FILE__ << ":" << __LINE__
				<< ", expected only one child node\n";
			return;
		}
		osg::Projection *p =
			dynamic_cast<osg::Projection*>(group->getChild(0));
		if(!p)
		{
			std::cerr << "Error: " << __FILE__ << ":" << __LINE__
				<< ", osg::Projection node not found\n";
			return;
		}

		osg::Node *child_0 = NULL;
		osg::Node *child_1 = NULL;
		osg::Node *child_geode = NULL;

		Projection = p;

		if(p->getNumChildren() != 2)
		{
			std::cerr << "Error: " << __FILE__ << ":" << __LINE__
				<< ", expected only two child nodes\n";
			return;
		}

		/* One child node of the projection will be the
		 * subgraph the other child will be a geode that we
		 * created and named.  Figure out which is which.
		 */
		child_0 = p->getChild(0);
		child_1 = p->getChild(1);
		if(child_0->getName() == clear_geode_name)
		{
			child_geode = child_0;
			ModelGraph = child_1;
		}
		else if(child_1->getName() == clear_geode_name)
		{
			child_geode = child_1;
			ModelGraph = child_0;
		}
		else
		{
			std::cerr << "Error: " << __FILE__ << ":" << __LINE__
				<< ", the glClear geode was not found\n";
		}

		ClearGeode = dynamic_cast<osg::Geode*>(child_geode);
		if(!ClearGeode.get() || ClearGeode->getNumDrawables() != 1)
		{
			std::cerr << "Error: " << __FILE__ << ":" << __LINE__
				<< ", unexpected geode found\n";
			ClearGeode = NULL;
		}

		/* Better luck next time? */
		if(!Projection.get() || !ModelGraph.get() ||
			!ClearGeode.get())
		{
			std::cerr << "Warning: " << __PRETTY_FUNCTION__
				<< " could not find a projection, subtree, "
				"or required drawable to update\n";
			return;
		}
		BSphere.init();
	}

	/* Only update the bounding sphere if it has changed. */
	const osg::BoundingSphere &fromBound = ModelGraph->getBound();
	if(BSphere.radius() != fromBound.radius() &&
		BSphere.center() != fromBound.center())
	{
		BSphere = fromBound;

		/* Drawables use bounding boxes, nodes use bounding
		 * spheres.  The bounding boxes and sphere classes
		 * have functions to expand it by the other, but if we
		 * used those functions the volume  would be expanded twice,
		 * sphere to box, and box to sphere.  We set the box
		 * to be inside the sphere so the final box to sphere
		 * calculation will result in the same sphere volume.
		 */
		const osg::Vec3 &center = fromBound.center();
		const double r = fromBound.radius();
		/* size = 2*r/sqrt(3), half of that is r/sqrt(3) */
		const double half_side = r/sqrt(3.0);
		const osg::Vec3 v = osg::Vec3(1,1,1) * half_side;
		glClearDrawable *drawable = dynamic_cast<glClearDrawable*>
			(ClearGeode->getDrawable(0));
		drawable->setBound(osg::BoundingBox(center - v, center + v));
		ClearGeode->dirtyBound();

		#if DEBUG
		printf("Original bounding sphere center %8.4f, %8.4f, %8.4f "
			"radius %8.4f\n",
			center.x(), center.y(), center.z(), r);
		const osg::BoundingSphere bs = ClearGeode->getBound();
		const osg::Vec3 &toCenter = bs.center();
		const double toR = bs.radius();
		printf("New bounding sphere center %8.4f, %8.4f, %8.4f "
			"radius %8.4f\n",
			toCenter.x(), toCenter.y(), toCenter.z(), toR);
		#endif
	}

	/* The projection matrix doesn't change very often.  It is
	 * much faster to see if it has changed than it is to
	 * calculate a new one.
	 */
	osg::Matrix projectionMatrix = *cv->getProjectionMatrix();
	if(OldProjectionMatrix != projectionMatrix)
	{

		OldProjectionMatrix = projectionMatrix;
		double left, right, bottom, top, near, far;
		projectionMatrix.getFrustum(left, right, bottom, top, near, far);
		double close_near = ProjectionNear;
		double factor = close_near / near;
		double close_far = ProjectionFar;

		#if DEBUG
		fprintf(stderr, "projection matrix changed "
			"left, right, bottom, top, near, far,\nfrom "
			"%6.3f, %6.3f, %6.3f, %6.3f, %6.3f, %6.3f\n"
			"to   %6.3f, %6.3f, %6.3f, %6.3f, %6.3f, %6.3f\n",
			left, right, bottom, top, near, far,
			left * factor, right * factor,
			bottom * factor, top * factor, close_near, close_far);
		#endif
		projectionMatrix.makeFrustum(left * factor, right * factor,
			bottom * factor, top * factor, close_near, close_far);
		Projection->setMatrix(projectionMatrix);
	}

	/* continue with this sub-tree */
	traverse(node, nv);
}
