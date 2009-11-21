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
 *  FILENAME:   glClearDrawable.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *
 *  PROGRAM DESCRIPTION:
 *  This class clears the depth buffer at most once per frame even if multiple
 *  instances of this object are called in a frame.
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

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#endif

#include <GL/gl.h>
#include "glClearDrawable.h"

glClearDrawable::glClearDrawable()
{
	/* No use caching the glClear call. */
	setUseDisplayList(false);
}

glClearDrawable::glClearDrawable(const glClearDrawable &clear,
	const osg::CopyOp &co): Drawable(clear, co)
{
}

void glClearDrawable::setBound(const osg::BoundingBox &bbox) const
{
	_boundingBox = bbox;
	_boundingBoxComputed = true;
}

void glClearDrawable::drawImplementation(osg::RenderInfo &renderInfo) const
{
	static int s_frame_num;
	int frame = renderInfo.getState()->getFrameStamp()->getFrameNumber();
	/* frame_num is a static class member allowing us to only
	 * execute the glClear once per draw no matter how many of
	 * those nodes are in the scene.
	 */
	if(frame == s_frame_num)
		return;
	s_frame_num = frame;

	glClear(GL_DEPTH_BUFFER_BIT);

#if DEBUG >= 10
	const osg::Vec3 &center = _bbox.center();
	printf("Calling glClear(GL_DEPTH_BUFFER_BIT);\n"
		"center (%7.4f, %7.4f, %7.4f) r %7.4f frame %d\n",
		center.x(), center.y(), center.z(), _bbox.radius(),
		frame);
#endif
}
