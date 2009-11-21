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
 *  FILENAME:   ProjectionCallback.h
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
#ifndef _PROJECTION_CALLBACK_H
#define _PROJECTION_CALLBACK_H 1

#include <osg/NodeCallback>

namespace osg
{
	class Node;
	class Geode;
	class Projection;
}

/* Updates the given projection node with a modified near and far
 * clipping plane.  Also updates the glClear drawable's bounding
 * sphere to be the same as the subgraph so it will be called only
 * when required.
 */
class ProjectionCallback: public osg::NodeCallback
{
public:
	ProjectionCallback();
	ProjectionCallback(float near_arg, float far_arg);
	ProjectionCallback(const ProjectionCallback &p);
	ProjectionCallback(const ProjectionCallback &p, const osg::CopyOp &co);
	virtual void operator() (osg::Node *node, osg::NodeVisitor *nv);
	META_Object(mpv, ProjectionCallback)
private:
	void Init();
	virtual ~ProjectionCallback() {}
	osg::ref_ptr<osg::Projection> Projection;
	/* This is the last node the callback was from.  It is only
	 * compared with the current one and will not be dereferenced
	 * hence there is no need to reference count it.
	 */
	osg::Node *LastNode;
	osg::ref_ptr<osg::Node> ModelGraph;
	osg::ref_ptr<osg::Geode> ClearGeode;
	osg::BoundingSphere BSphere;
	int Traversal;
	osg::Matrix OldProjectionMatrix;

	float ProjectionNear, ProjectionFar;
};

#endif /* _PROJECTION_CALLBACK_H */
