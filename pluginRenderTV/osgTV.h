/* copyright (c) 2006 all rights reserved by
 * David Fries <dfries@mail.win.org>
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
 */

#ifndef _OSG_TV_H
#define _OSG_TV_H 1

#include <osg/NodeVisitor>
#include <osg/StateAttribute>

namespace osg
{
class StateSet;
}

typedef unsigned long ulong;
#include "Video.h"

class osgTV: public osg::StateAttribute::Callback
{
public:
	// Open the specified device and grab images at a resolution of
	// width and height.  Width and textureHeight must be a power of two.
	// textureHeight also needs to be equal or greater than height.
	// The height lines are streatched to fill the rectangle causing
	// the extra lines of the textureHeight to not be displayed.  This
	// allows 480 lines of resolution to be captured even though that
	// doesn't make a power of 2.  Use 512 for textureHeight in that case.
	osgTV(const std::string &device, int width, int height,
		int textureHeight);
	osgTV(const osgTV &rhs, const osg::CopyOp &copyop=
		osg::CopyOp::SHALLOW_COPY);
	META_Object(external, osgTV);

	virtual void operator()(osg::StateAttribute *attrib,
		osg::NodeVisitor *nv);
protected:
	osgTV();
	virtual ~osgTV();
	Video V;
	// Texture height may differ from the Video capture resolution
	int TextureHeight;
	// the next frame to sync and display
	unsigned int FrameNum;
	// We can't abort in the constructor, but we can keep from accessing
	// the video device if the setup fails.
	bool Init;
	int TraversalNum;
	unsigned char *ImageData;
};

#endif // _OSG_TV_H
