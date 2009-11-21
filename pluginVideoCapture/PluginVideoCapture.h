/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2007 The Boeing Company
 *  
 *  This component is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; specifically 
 *  version 2.1 of the License.
 *  
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *  
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this software; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *  
 *  FILENAME:   PluginVideoCapture.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  DESCRIPTION: 
 *  Captures the GL framebuffer, and dumps it to an image file or an mpeg 
 *  stream.
 *  
 *  2006-11-01 Philip Lowman, General Dynamics Land Systems
 *  2007-01-25 Justin Schaub, Boeing Mesa
 *  2007-04-26 Robert Bauer,  General Dynamics Land Systems
 *  2007-07-21 Andrew Sampson
 *      Changed interface to use new state machine API
 *  
 * </pre>
 */


#ifndef PLUGIN_VIDEO_CAPTURE_H
#define PLUGIN_VIDEO_CAPTURE_H

#include "Plugin.h"
#include "DefFileGroup.h"

#include <fstream>
#include <stdio.h>
#include <sys/types.h>

#if defined (HAVE_FFMPEG) || defined (HAVE_JPEG)
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif //HAVE_FFMPEG || HAVE_JPEG

#include <osg/Image>
#include <osg/ref_ptr>

#ifdef HAVE_FFMPEG
#include <ffmpeg/avcodec.h>
#include <ffmpeg/avformat.h>
#endif

#ifdef HAVE_JPEG
#include "JpegImage.h"
#endif //HAVE_JPEG

namespace VideoCapture
{
//=========================================================
//! This plugin is responsible for handling framebuffer-capture tasks, such 
//! as screenshots and streaming video.
//! 
class PluginVideoCapture : public Plugin 
{

public:

	//=========================================================
	//! General Constructor
	//! 
	PluginVideoCapture();

	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginVideoCapture() throw();
	
	//=========================================================
	//! The per-frame processing that this plugin performs.
	//! This plugin's act() will (if commanded by the user) capture the 
	//! framebuffer contents and write it to disk.
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );
	
	//=========================================================
	//! Saves the current framebuffer contents to an image file
	//! 
	void recordFrameAsImage( void );
	
	//=========================================================
	//! Toggles movie capture
	//! 
	void startStopMovie( void ) { capturing = !capturing; }
	
private:

	//=========================================================
	//! The root of the def-file tree.  Retrieved from the blackboard.
	//! 
	DefFileGroup **DefFileData;
	
	//=========================================================
	//! The directory where MPV was started. Retrieved from the blackboard.
	//! 
	std::string baseProgramDirectory;

	//=========================================================
	//! When true, the plugin will capture the framebuffer contents every frame
	//! 
	bool capturing;
	
	osg::ref_ptr<osg::Image>      m_img;

	int               m_width;
	int               m_height;
	
#ifdef HAVE_FFMPEG
	AVCodecContext    *m_context;
	AVCodec           *m_codec;
	AVFrame           *m_yuv;
	AVFrame           *m_rgb;

	uint8_t           *m_rgb_buffer;
	uint8_t           *m_yuv_buffer;

	uint8_t           *m_output_buffer;
	int                m_output_buffer_size;
#endif //HAVE_FFMPEG

#if defined (HAVE_FFMPEG) || defined (HAVE_JPEG)
	int                m_sock;
	struct sockaddr_in m_sin;
	
	void mirror(void *src, void *dest, int cols, int rows);
#endif //HAVE_FFMPEG || HAVE_JPEG

#ifdef HAVE_JPEG
	uint8_t           *m_jpeg_buffer;
	uint8_t           *m_raw_buffer;
	
	JpegImage          m_jpeg;
#endif //HAVE_JPEG
	
	int                m_send_over_network;
	std::string        m_dest_addr;
	int                m_dest_port;
	int                m_is_broadcast;
	
	int                m_write_to_file;
	std::string        m_file_name;
	std::ofstream      output_file;
	
	bool               m_jpeg_extension;

	unsigned int frame;

	std::string baseFilename;
	std::string directory;
	std::string extension;

	void resize(int width, int height);
	int sendOverNetwork(unsigned char *source, unsigned int out_size);
#ifdef HAVE_FFMPEG
	void recordFrameAsMPEG( void );
#endif

	//=========================================================
	//! retrieves configuration info from the config tree (window width, 
	//! height, screen shot file name, mpeg bitrate, etc)
	//! 
	void getConfig( void );
	
};

} //VideoCapture

#endif

