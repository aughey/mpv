/* copyright (c) 2003, 2006 all rights reserved by
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
#ifndef _VIDEO_H
#define _VIDEO_H 1

/* Video 4 Linux API documentation
 * http://www.linuxtv.org/downloads/video4linux/API/V4L1_API.html
 * http://www.linuxtv.org/downloads/video4linux/API/V4L2_API/
 */

// don't include the kernel's time.h
#define _LINUX_TIME_H 1
typedef unsigned long ulong;
#include <linux/videodev.h>
#include <string>

class Video{
public:
	/* The initialization order is setDevice then setFormat
	 * if all succeeds then call startCapture on all available frames,
	 * then loop videoSync, read image, startCapture
	 */
	Video();
	~Video();

	// Open the specified video device.
	bool setDevice(const std::string &device);

	// Set the image size, bytes per pixel, and pixel type.
	// Or with 2.6.9-1.667smp from Fedora Core 3, bits per pixel.
	bool setFormat(unsigned short width, unsigned short height,
		unsigned short depth, unsigned short palette);

	// Start a capturing an image on the requested frame buffer.
	bool startCapture(unsigned int frame_num);
	// Complete a capture blocking if necessary for the requested frame
	// buffer.
	bool videoSync(unsigned int frame_num);
// Information routines, the height and width values are available when the
// device is opened the buffer count is available when the format is set.
	// the minimum and maximum capture sizes
	int minWidth() { return capability.minwidth; }
	int minHeight() { return capability.minheight; }
	int getWidth() { return request.width; }
	int getHeight() { return request.height; }
	int maxWidth() { return capability.maxwidth; }
	int maxHeight() { return capability.maxheight; }
	// how many buffers can be capturing at a time
	int numBuffers() { return bufferCount; }
	// an array of pointers to the frame buffer data
	unsigned char **frameData() { return frame; }
	// get the file descriptor for the device
	int get_fd() { return fd; }

// Debug routines
	// true information was printed and stored in capability
	// false error
	bool printCapability(bool verbose);
	bool printVideoSourceInfo();
	// Write out the image on the given frame to file in ppm format.
	// The frame is expected to have VideoSync called on the frame.
	// It also expects a depth 3 RGB image format.
	bool saveImage(unsigned int frame_num, const char *filename);
	// Swap the R and B of RGB
	void swapEndian(unsigned char *image);
	// Same as above with copying it to another memory location.
	void swapEndian(unsigned char *dst, unsigned char *src);
private:
	// cleanupDevice is called before setDevice and in the class
	// destructor
	void cleanupDevice();
	// clear the data values
	void cleanupDataStructures();
	// Initialize the MMIO structures.
	bool setupMMIO();

	void printVideoCaptureInformation( struct video_window *window);

	// file descriptor of the video device
	int fd;
	// what the board can do
	struct video_capability capability;
	// mmio location
	unsigned char * capture;
	// length of the mmio region
	int mmioLength;
	// buffer location of each frame
	unsigned char * frame[VIDEO_MAX_FRAME];
	// maximum number of buffers available
	int bufferCount;
	// contains frame, width, height, and format
	struct video_mmap request;
};

#endif // _VIDEO_H
