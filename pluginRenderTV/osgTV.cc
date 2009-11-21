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

/* Video 4 Linux API documentation
 * http://www.linuxtv.org/downloads/video4linux/API/V4L1_API.html
 * http://www.linuxtv.org/downloads/video4linux/API/V4L2_API/
 */

#include <stdio.h>
#include <string.h>

#include <osg/Texture2D>
#include <osg/Image>

#include "osgTV.h"

osgTV::osgTV()
{
	Init=false;
}

osgTV::osgTV(const std::string &device, int width, int height,
	int textureHeight)
{
	TraversalNum=-1;
	Init=false;
	// Allocate memory for the entire texture even though we will only
	// fill height rows.
	ImageData=new unsigned char[3*width*textureHeight];
	// Setup the video device and start the capture.
	if(!V.setDevice(device))
		return;
	// bits or bytes?  It seems to depend on the kernel.
	int depth=24;
	TextureHeight=textureHeight;
	if(!V.setFormat(width, height, depth, VIDEO_PALETTE_RGB24))
		return;
	for(int i=0; i<V.numBuffers(); ++i)
		if(!V.startCapture(i))
			return;
	setName(device);
	FrameNum=0;
	Init=true;
}

osgTV::osgTV(const osgTV &rhs, const osg::CopyOp &copyop):
	osg::StateAttribute::Callback(rhs, osg::CopyOp::SHALLOW_COPY)
{
	// Only allow a shallow copy.  That way this shares the states,
	// geometry, and more importantly the video texture image.  That
	// allows one to update for all.  If it gets deleted, maybe we
	// should 
	Init=false;
}

osgTV::~osgTV()
{
	delete[]ImageData;
	ImageData=NULL;
}

void osgTV::operator()(osg::StateAttribute *attrib, osg::NodeVisitor *nv)
{
	osg::Texture2D *texture=dynamic_cast<osg::Texture2D*>(attrib);
	/* Only update the image once per draw and only if the device was
	 * opened successfully for this object.
	 */
	int tnum=nv->getTraversalNumber();
	if(!texture || !Init || tnum==TraversalNum)
		return;

	// nv->getFrameStamp()->getReferenceTime(); ...
	TraversalNum=tnum;

	// block until the image has completed loading
	V.videoSync(FrameNum);

	/* s: width
	 * t: height
	 * r: depth
	 * InternalTextureformat
	 * pixelFormat
	 * type: data type (unsigned int etc)
	 * data: char pointer
	 * mode: how to free if any
	 * packing
	 */
	osg::Image *image=new osg::Image();
	texture->setImage(image);
	unsigned char *data=V.frameData()[FrameNum];
	V.swapEndian(ImageData, data);
	image->setImage(V.getWidth(), TextureHeight, 1,
		GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE,
//			GL_RGB8, GL_BGR, GL_UNSIGNED_BYTE,
		ImageData, osg::Image::NO_DELETE);
	// TODO: v4l to texture update
	//Texture->dirtyTextureObject();

	// queue up the buffer for reuse
	V.startCapture(FrameNum);
	// next
	FrameNum=(FrameNum+1)%V.numBuffers();
}
