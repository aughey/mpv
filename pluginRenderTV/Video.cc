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

/* Video 4 Linux API documentation
 * http://www.linuxtv.org/downloads/video4linux/API/V4L1_API.html
 * http://www.linuxtv.org/downloads/video4linux/API/V4L2_API/
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>

#include "Video.h"

Video::Video()
{
	cleanupDataStructures();
}

Video::~Video()
{
	cleanupDevice();
}

bool Video::setDevice(const std::string &device)
{
	cleanupDevice();
	fd = open(device.c_str(), O_RDWR);
	if(fd == -1)
	{
		fprintf(stderr, "Error opening ");
		perror(device.c_str());
		return false;
	}
	// fill in the capability structure, but don't print it out
	printCapability(false);
	return true;
}

bool Video::setFormat(unsigned short i_width, unsigned short i_height,
	unsigned short depth, unsigned short palette)
{
	request.width = i_width;
	request.height = i_height;
	struct video_picture picture;;
	int result;
	result = ioctl(fd, VIDIOCGPICT, &picture);
	if(result == -1)
	{
		perror("Error getting video picture information.");
		return false;
	}

	printf("depth was %d, format %d\n", picture.depth, picture.palette);
	picture.depth = depth;
	request.format = picture.palette = palette;

	printf("setting to %d, format %d\n", depth, palette);
	result = ioctl(fd, VIDIOCSPICT, &picture);
	if(result == -1)
	{
		perror("Error setting video picture information.");
		return false;
	}

	printf("depth %d\n", picture.depth);
	const char * paletteStr;
	switch(picture.palette)
	{
	case VIDEO_PALETTE_GREY:
		paletteStr = "VIDEO_PALETTE_GREY";
		break;
	case VIDEO_PALETTE_HI240:
		paletteStr = "VIDEO_PALETTE_HI240";
		break;
	case VIDEO_PALETTE_RGB565:
		paletteStr = "VIDEO_PALETTE_RGB565";
		break;
	case VIDEO_PALETTE_RGB24:
		paletteStr = "VIDEO_PALETTE_RGB24";
		break;
	case VIDEO_PALETTE_RGB32:
		paletteStr = "VIDEO_PALETTE_RGB32";
		break;
	case VIDEO_PALETTE_RGB555:
		paletteStr = "VIDEO_PALETTE_RGB555";
		break;
	case VIDEO_PALETTE_YUV422:
		paletteStr = "VIDEO_PALETTE_YUV422";
		break;
	case VIDEO_PALETTE_YUYV:
		paletteStr = "VIDEO_PALETTE_YUYV";
		break;
	case VIDEO_PALETTE_UYVY:
		paletteStr = "VIDEO_PALETTE_UYVY";
		break;
	case VIDEO_PALETTE_YUV420:
		paletteStr = "VIDEO_PALETTE_YUV420";
		break;
	case VIDEO_PALETTE_YUV411:
		paletteStr = "VIDEO_PALETTE_YUV411";
		break;
	case VIDEO_PALETTE_RAW:
		paletteStr = "VIDEO_PALETTE_RAW";
		break;
	case VIDEO_PALETTE_YUV422P:
		paletteStr = "VIDEO_PALETTE_YUV422P";
		break;
	case VIDEO_PALETTE_YUV411P:
		paletteStr = "VIDEO_PALETTE_YUV411P";
		break;
	case VIDEO_PALETTE_YUV420P:
		paletteStr = "VIDEO_PALETTE_YUV420P";
		break;
	case VIDEO_PALETTE_YUV410P:
		paletteStr = "VIDEO_PALETTE_YUV410P";
		break;
	default:
		fprintf(stderr, "Error palette %d not recognized\n",
			picture.palette);
		return false;
	}
	printf("Palette %s\n", paletteStr);

	// it would appear that the image size doesn't need to be set in this
	// manner
#if 0
	struct video_window window;
	if(ioctl(fd, VIDIOCGWIN, &window)== -1)
	{
		fprintf(stderr,
			"Error getting video capture window information\n");
		perror("Message");
		return false;
	}
	printf("Window capture information was\n");
	printVideoCaptureInformation(&window);

	// sanity checking
	if(window.clipcount)
		fprintf(stderr, "Error clip count is %d instead of zero\n",
			window.clipcount);
	if(window.clips)
		fprintf(stderr, "Error clips is set\n");

	window.x = window.y = 0;
	window.width = width;
	window.height = height;
	printf("requesting capture of\n");
	printVideoCaptureInformation(&window);
	if(ioctl(fd, VIDIOCSWIN, &window) == -1)
	{
		fprintf(stderr,
			"Error setting video capture window information\n");
		perror("Message");
		return false;
	}
	printf("Window capture information is now\n");
	printVideoCaptureInformation(&window);
#endif // 0
	return setupMMIO();
}

bool Video::startCapture(unsigned int frame_num)
{
	request.frame = frame_num;
	if(ioctl(fd, VIDIOCMCAPTURE, &request)==-1)
	{
		fprintf(stderr, "Error capture request failed line %d: %s\n",
			__LINE__, strerror(errno));
		return false;
	}
	return true;
}

bool Video::videoSync(unsigned int frame_num)
{
	request.frame = frame_num;
	if(ioctl(fd, VIDIOCSYNC, &request)==-1)
	{
		if(errno == EINVAL)
		{
			fprintf(stderr, "Got EINVAL (lost sync)\n");
			return false;
		}
		fprintf(stderr, "Error getting video frame %d\n", errno);
		perror("Error is ");
		return false;
	}
	return true;
}

bool Video::printCapability(bool verbose)
{
	int result = ioctl(fd, VIDIOCGCAP, &capability);
	if(result == -1)
	{
		fprintf(stderr,
			"Error getting the video capability information.\n");
		perror("Message");
		return false;
	}
	if(!verbose)
		return true;
	printf("Found: %s\n", capability.name);
	printf("\tchannels %d\n"
		"\taudio devices %d\n"
		"\tmaximum %d x %d\n"
		"\tminimum %d x %d\n",
			capability.channels,
			capability.audios,
			capability.maxwidth,
			capability.maxheight,
			capability.minwidth,
			capability.minheight);
	if(capability.type & VID_TYPE_CAPTURE)
		printf("\tCan capture to memory\n");
	else
		printf("\tCan't capture to memory\n");
	if(capability.type & VID_TYPE_TUNER)
		printf("\tHas a tuner of some form\n");
	else
		printf("\tNo tuner available\n");
	if(capability.type & VID_TYPE_TELETEXT)
		printf("\tHas teletext capability\n");
	else
		printf("\tNo teletext capability\n");
	if(capability.type & VID_TYPE_OVERLAY)
		printf("\tCan overlay its image onto the frame buffer\n");
	else
		printf("\tCan't overlay its image onto the frame buffer\n");
	if(capability.type & VID_TYPE_CHROMAKEY)
		printf("\tOverlay is Chromakeyed\n");
	else
		printf("\tOverlay is not Chromakeyed\n");
	if(capability.type & VID_TYPE_CLIPPING)
		printf("\tOverlay clipping is supported\n");
	else
		printf("\tOverlay clipping is not supported\n");
	if(capability.type & VID_TYPE_FRAMERAM)
		printf("\tOverlay overwrites frame buffer memory\n");
	else
		printf("\tOverlay doen't overwrite frame buffer memory\n");
	if(capability.type & VID_TYPE_SCALES)
		printf("\tThe hardware supports image scaling\n");
	else
		printf("\tThe hardware doesn't support image scaling\n");
	if(capability.type & VID_TYPE_MONOCHROME)
		printf("\tImage capture is grey scale only\n");
	else
		printf("\tImage capture is color\n");
	if(capability.type & VID_TYPE_SUBCAPTURE)
		printf("\tCapture can be of only part of the image\n");
	else
		printf("\tCapture must be the full image\n");
	if(capability.type & VID_TYPE_MPEG_DECODER)
		printf("\tCan decode MPEG streams\n");
	else
		printf("\tMPEG decoder is not available\n");
	if(capability.type & VID_TYPE_MPEG_ENCODER)
		printf("\tCan encode MPEG streams\n");
	else
		printf("\tMPEG encoder is not available\n");
	if(capability.type & VID_TYPE_MJPEG_DECODER)
		printf("\tCan decode MJPEG streams\n");
	else
		printf("\tMJPEG decoder is not available\n");
	if(capability.type & VID_TYPE_MJPEG_ENCODER)
		printf("\tCan encode MJPEG streams\n");
	else
		printf("\tMJPEG encoder is not available\n");

	return true;
}

bool Video::printVideoSourceInfo()
{
	struct video_channel vChannel;
	int result;
	bool error = false;
	int originalChannel;

	memset(&vChannel, 0, sizeof(vChannel));

	// cache the channel it was previously set to
	result = ioctl(fd, VIDIOCGCHAN, &vChannel);
	if(result == -1)
	{
		fprintf(stderr, "Error getting Video Source Channel "
			"information.\n");
		perror("Message");
		return false;
	}
	originalChannel = vChannel.channel;
	printf("Channel was set to source %d, %s\n", vChannel.channel,
		vChannel.name);

	for( int i=0; i<capability.channels; i++)
	{
		vChannel.channel = i;
		result = ioctl(fd, VIDIOCSCHAN, &vChannel);
		if(result == -1)
		{
			fprintf(stderr, "Error setting source channel to %d.\n",
				i);
			perror("Message");
			error = true;
			continue;
		}
		result = ioctl(fd, VIDIOCGCHAN, &vChannel);
		if(result == -1)
		{
			fprintf(stderr, "Error getting Video Source Channel "
				"information for channel %d\n", i);
			perror("Message");
			error = true;
			continue;
		}
		printf("Information for source channel %d, %s\n",
			vChannel.channel, vChannel.name);
		printf("\t%d tuners\n", vChannel.tuners);
		if(vChannel.type & VIDEO_VC_TUNER)
			printf("\tChannel has tuners\n");
		else
			printf("\tNo tuners available\n");
		if(vChannel.type & VIDEO_VC_AUDIO)
			printf("\tAudio available\n");
		else
			printf("\tNo Audio\n");
		if(vChannel.type & VIDEO_TUNER_NORM)
			printf("\thas \"Norm\" setting\n");
		else
			printf("\t\"Norm\" setting isn't available\n");
		if(vChannel.type & VIDEO_TYPE_TV)
			printf("\tTV input\n");
		else
			printf("\tNot a TV input\n");
		if(vChannel.type & VIDEO_TYPE_CAMERA)
			printf("\tCamera input\n");
		else
			printf("\tNot a camera input\n");
	}

	vChannel.channel = originalChannel;
	result = ioctl(fd, VIDIOCSCHAN, &vChannel);
	if(result == -1)
	{
		fprintf(stderr, "Error setting source channel to %d.\n",
			originalChannel);
		perror("Message");
		error = true;
	}

	return !error;
}

bool Video::saveImage(unsigned int frame_num, const char * filename)
{
	if(request.format != VIDEO_PALETTE_RGB24)
	{
		printf("saveImage format must be VIDEO_PALETTE_RGB24\n");
		return false;
	}

	printf("Capturing size %d x %d\n", request.width, request.height);
	int image_file = open(filename, O_WRONLY|O_CREAT, 0644);
	if(image_file == -1)
	{
		fprintf(stderr, "Error opening %s", filename);
		perror(" for writing");
		return false;
	}

	// write out the image
	char buf[80];
	// header format for ppm
	sprintf(buf, "P6\n%d %d\n255\n", request.width, request.height);
	int len = strlen(buf);
	int result = write(image_file, buf, len);
	if( result != len)
	{
		if(result == -1)
			fprintf(stderr, "Error writing to file %s:%s",
				filename, strerror(errno));
		else
			fprintf(stderr, "Short write to file %s\n", filename);
		return false;
	}
	int image_size = request.width*request.height*3; // RGB 8 bits each
	swapEndian(frame[frame_num]);
	result = write(image_file, frame[frame_num], image_size);
	if( result != image_size)
	{
		if(result == -1)
		{
			fprintf(stderr, "Error writing to fle ");
			perror(filename);
		}
		else
		{
			fprintf(stderr, "Short write to file %s\n", filename);
		}
		return false;
	}
	close(image_file);
	printf("Image successfully captured to file %s\n", filename);
	return true;
}

void Video::cleanupDevice()
{
	if(capture)
	{
		if(munmap(capture, mmioLength))
			perror("Error unmapping the video device");
		capture=NULL;
	}
	if(fd!=-1)
	{
		close(fd);
		fd=-1;
	}
	cleanupDataStructures();
}

void Video::cleanupDataStructures()
{
	capture = NULL;
	for( int i=0; i<VIDEO_MAX_FRAME; i++)
		frame[i] = NULL;
	bufferCount = request.width = request.height = 0;
	fd = -1;
}

bool Video::setupMMIO()
{
	printf("About to memory map the device\n");
	struct video_mbuf mbuf;
	if(ioctl(fd, VIDIOCGMBUF, &mbuf)==-1)
	{
		fprintf(stderr, "Error getting MMIO information\n");
		perror("Message");
		return false;
	}
	printf("MMIO size %d, frames buffered %d\n", mbuf.size, mbuf.frames);
	capture = (unsigned char *)mmap(NULL, mbuf.size, PROT_READ | PROT_WRITE,
		MAP_SHARED, fd, 0);
	if(capture == MAP_FAILED)
	{
		perror("Error memory maping video device");
		return false;
	}
	mmioLength = mbuf.size;
	bufferCount = mbuf.frames;
	int f;

	for( f=0; f<mbuf.frames; f++)
		frame[f] = capture + mbuf.offsets[f];

	// the rest of the frames are not defined
	for( ; f<VIDEO_MAX_FRAME; f++)
		frame[f] = NULL;

	printf("video device successfully memory mapped\n");
	return true;
}

void Video::printVideoCaptureInformation(struct video_window * window)
{
	printf("\t%dx%d coordinate\n"
		"\timage size %dx%d\n"
		"\tchromakey 0x%04x\n"
		"\tflags 0x%04x\n",
		window->x, window->y,
		window->width, window->height,
		window->chromakey, window->flags);
}

void Video::swapEndian(unsigned char * image)
{
	int image_size = request.width*request.height*3; // RGB 8 bits each
	// detect endian (anyone have a big endian machine to test this on?)
	union { char c; short s; }u;
	u.s=1;
	if(u.c==0)
		return;

	const unsigned char * end = image + image_size;
	unsigned char * r = image;
	unsigned char * b = image+2;
	unsigned char tmp;
	for(;r < end; r+=3, b+=3)
	{
		tmp = *r;
		*r = *b;
		*b = tmp;
	}
}

void Video::swapEndian(unsigned char *dst, unsigned char *src)
{
	int image_size = request.width*request.height*3; // RGB 8 bits each
	// detect endian (anyone have a big endian machine to test this on?)
	union { char c; short s; }u;
	u.s=1;
	if(u.c==0)
	{
		memcpy(dst, src, image_size);
		return;
	}

	const unsigned char *end=src+image_size;
	for(;src < end; src+=3, dst+=3)
	{
		*dst=*(src+2);
		*(dst+1)=*(src+1);
		*(dst+2)=*src;
	}
}
