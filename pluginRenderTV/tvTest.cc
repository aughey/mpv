/* copyright (c) 2006 all rights reserved by
 * David Fries <dfries@mail.win.org>
 
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; using version 2 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 */

/* Video 4 Linux API documentation
 * http://www.linuxtv.org/downloads/video4linux/API/V4L1_API.html
 * http://www.linuxtv.org/downloads/video4linux/API/V4L2_API/
 */

#include <stdio.h>
#include <sys/select.h>
#include <string.h>

#include "Video.h"

int main(int argc, char **argv)
{
	Video v;
	if(!v.setDevice("/dev/video0"))
		return 1;

	bool print_info = false;
	const char *to_file=NULL;
	if(argc == 2 && !strcmp("--info", argv[1]))
		print_info = true;
	else if(argc == 3 && !strcmp("--tofile", argv[1]))
	{
		to_file = argv[2];
	}
	else if( argc > 1)
	{
		fprintf(stderr, "Usage: %s [ --info | --tofile file ]\n",
			argv[0]);
		return 1;
	}
			
	if( print_info)
	{
		if(!v.printCapability(true))
			return -1;
		if(!v.printVideoSourceInfo())
			return -1;
		return 0;
	}
	if(to_file)
	{
		if(!v.setFormat(v.maxWidth(), v.maxHeight(), 24,
			VIDEO_PALETTE_RGB24))
			return 1;
		unsigned int frame=0;
		if(!v.startCapture(frame))
			return 1;
		if(!v.videoSync(frame))
			return 1;
		v.saveImage(frame, to_file);
		return 0;
	}
	if(!v.setFormat(v.maxWidth(), v.maxHeight(), 3,
		VIDEO_PALETTE_RGB24))
		return 1;
	fd_set readset, tmpset;
	FD_ZERO(&readset);
	int fd=v.get_fd();
	FD_SET(fd, &readset);
	struct timeval timeout;

	memset(&timeout, 0, sizeof(timeout));
	tmpset=readset;
	printf("before capture select returned %d\n",
		select(fd+1, &tmpset, NULL, NULL, &timeout));
	unsigned int frame=0;

	memset(&timeout, 0, sizeof(timeout));
	tmpset=readset;
	v.startCapture(frame);
	printf("after capture select returned %d\n",
		select(fd+1, &tmpset, NULL, NULL, &timeout));

	memset(&timeout, 0, sizeof(timeout));
	tmpset=readset;
	v.videoSync(frame);
	printf("after sync select returned %d\n",
		select(fd+1, &tmpset, NULL, NULL, &timeout));

	struct timespec req={0, (int)(100e6)};
	nanosleep(&req, NULL);
	memset(&timeout, 0, sizeof(timeout));
	tmpset=readset;
	v.startCapture(frame);
	printf("after capture select returned %d\n",
		select(fd+1, &tmpset, NULL, NULL, &timeout));

	memset(&timeout, 0, sizeof(timeout));
	tmpset=readset;
	v.videoSync(frame);
	printf("after sync select returned %d\n",
		select(fd+1, &tmpset, NULL, NULL, &timeout));
	
	printf("set all to capture\n");
	for(int i=0; i<v.numBuffers(); ++i)
		v.startCapture(i);

	req.tv_sec=0;
	req.tv_nsec=(long)(500e6);
	nanosleep(&req, NULL);
	memset(&timeout, 0, sizeof(timeout));
	tmpset=readset;
	printf("before sync select returned %d\n",
		select(fd+1, &tmpset, NULL, NULL, &timeout));

	memset(&timeout, 0, sizeof(timeout));
	tmpset=readset;
	v.videoSync(frame);
	printf("after sync select returned %d\n",
		select(fd+1, &tmpset, NULL, NULL, &timeout));
	
	#if 0
	//if(v.setFormat(SRC_WIDTH, SRC_HEIGHT, 3, VIDEO_PALETTE_RGB24))
	// hopefully VIDEO_PALETTE_YUYV matches YUY2
	if(v.setFormat(SRC_WIDTH, SRC_HEIGHT, 3, V4L_FORMAT))
	{
		if(v.setupMMIO())
		{
			//v.captureImage("tmp/0.ppm");
			v.DoVideo();
		}
		else
			fprintf(stderr, "Error setting up memory map\n");
	}
	else
		fprintf(stderr, "setFormat failed\n");
	#endif

	return 0;
}
