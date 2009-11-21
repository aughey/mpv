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
 *  FILENAME:   glClearDrawable.h
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
#ifndef _GL_CLEAR_NODE_H
#define _GL_CLEAR_NODE_H 1

#include <osg/Drawable>

const std::string clear_geode_name = "glClear Geode";

/* Calls glClear with the option to clear the depth buffer. */
class glClearDrawable: public osg::Drawable
{
public:
	glClearDrawable();
	glClearDrawable(const glClearDrawable &clear, const osg::CopyOp &co);

	// define some functions for clone and string names
	META_Object(MPV,glClearDrawable)

	virtual void drawImplementation(osg::RenderInfo &renderInfo) const;
	virtual void setBound(const osg::BoundingBox &bbox) const;
private:
	virtual ~glClearDrawable() {}
};

#endif /* _GL_CLEAR_NODE_H */
