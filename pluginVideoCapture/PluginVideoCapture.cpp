/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2007 The Boeing Company
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
 *  FILENAME:   PluginVideoCapture.cpp
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

#include <iostream>
#include <fstream>

#include <osgDB/WriteFile>
#include <osgGA/GUIEventHandler>
#include <osg/ref_ptr>

#include <GL/gl.h>
#include <errno.h>
#include <ostream>

#include "PluginVideoCapture.h"

#ifdef WIN32
#define snprintf _snprintf
#endif

#if HAVE_JPEG
#include "JpegImage.h"
#endif //HAVE_JPEG

namespace VideoCapture
{

// KEYBOARD HANDLER ///////////////////////////////////////////////////

class KeyboardHandler: public osgGA::GUIEventHandler
{
public:
	KeyboardHandler( PluginVideoCapture *plug )
	{
		plugin = plug;
	}

	bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &)
	{
		if( plugin == NULL )
			return false;
		
		if( ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN )
		{
			int key = ea.getKey();
			if( key == 's' )
			{
				plugin->recordFrameAsImage();
				return true;
			}
			else if( key == 'S' )
			{
				plugin->startStopMovie();
				return true;
			}
		}

		return false;
	}

	virtual void getUsage( osg::ApplicationUsage &usage ) const
	{
		usage.addKeyboardMouseBinding( "s", "Take a screen shot" );
		usage.addKeyboardMouseBinding( "S", "Toggle capturing a movie" );
	}

private:
	PluginVideoCapture *plugin;
};


EXPORT_DYNAMIC_CLASS( PluginVideoCapture )

PluginVideoCapture::PluginVideoCapture() : Plugin(), 
		capturing(false),
		m_jpeg_extension(true),
		frame(0),
		baseFilename("mpv_screenshot"),
		directory("."),
		extension("jpg")
{
	name_ = "PluginVideoCapture";
	licenseInfo_.setLicense( LicenseInfo::LicenseLGPL );
	licenseInfo_.setOrigin( "Boeing+GeneralDynamics" );

	dependencies_.push_back( "PluginDefFileReader" );
	dependencies_.push_back( "PluginUserInputMgrOSGGA" );
	
	DefFileData = NULL;
	
	m_width = 1024;
	m_height = 768;

	m_write_to_file     = 0;
	m_send_over_network = 0;

#ifdef HAVE_JPEG
	m_raw_buffer   = NULL;
	m_jpeg_buffer  = NULL;

#endif //HAVE_JPEG

#ifdef HAVE_FFMPEG
	m_yuv_buffer = NULL;
	m_rgb_buffer = NULL;

	avcodec_init();
	avcodec_register_all();

	m_context = avcodec_alloc_context();

	//m_context->bit_rate = 2000000;
	m_context->bit_rate = 500000;
	m_context->width = m_width;
	m_context->height = m_height;

	m_context->time_base = (AVRational){1,25};
	m_context->gop_size = 10; // emit one intra frame every ten frames
	m_context->max_b_frames = 1;

	m_context->codec_type = CODEC_TYPE_VIDEO;

	//YUV420P is the only valid format for MPEG-2
	m_context->pix_fmt = PIX_FMT_YUV420P;

	m_output_buffer_size = 1500000;
	m_output_buffer = (uint8_t *) malloc (m_output_buffer_size);
	
	m_yuv = avcodec_alloc_frame();
	m_rgb = avcodec_alloc_frame();
	resize(m_width, m_height);

	m_codec = avcodec_find_encoder(CODEC_ID_MPEG2VIDEO);
#endif
	m_img = new osg::Image;
}

PluginVideoCapture::~PluginVideoCapture() throw() 
{
#ifdef HAVE_FFMPEG
	free (m_output_buffer);
	free (m_rgb_buffer);
	free (m_yuv_buffer);
#endif
#ifdef HAVE_JPEG
	delete [] m_raw_buffer;
	delete [] m_jpeg_buffer;
#endif //HAVE_JPEG
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginVideoCapture::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{
		case SystemState::BlackboardRetrieve:
			{
			
			bb_->get( "DefinitionData", DefFileData );
			
			char *tempStr = NULL;
			bb_->get( "BaseProgramDirectory", tempStr );
			baseProgramDirectory = tempStr;
			
			// get the list of user input callbacks
			std::list< osg::ref_ptr<osgGA::GUIEventHandler> > *eventHandlerList = NULL;
			bb_->get( "GUIEventHandlerList", eventHandlerList );
			eventHandlerList->push_back( new VideoCapture::KeyboardHandler( this ) );

			}
			break;

		case SystemState::ConfigurationProcess:
			getConfig();
			break;

		case SystemState::Operate:
		case SystemState::Debug:
			if( capturing )
			{
#ifdef HAVE_FFMPEG
				if (m_jpeg_extension)
				{
					if (m_send_over_network)
						recordFrameAsImage();
				}
				else
					recordFrameAsMPEG();
#else
				if (m_send_over_network)
					recordFrameAsImage();
#endif
			}
			break;

		default:
			break;
	}

}


void PluginVideoCapture::resize(int width, int height)
{

	m_width = width;
	m_height = height;

#ifdef HAVE_JPEG
	if (m_raw_buffer)
		delete [] m_raw_buffer;
	m_raw_buffer = new uint8_t[m_width*m_height*3];

	if (m_jpeg_buffer)
		delete [] m_jpeg_buffer;
	m_jpeg_buffer = new uint8_t[(m_width*m_height*3)*80/100];

#endif //HAVE_JPEG

#ifdef HAVE_FFMPEG
	m_context->width = m_width;
	m_context->height = m_height;

	if (m_yuv_buffer != NULL)
		free (m_yuv_buffer);
	if (m_rgb_buffer != NULL)
		free (m_rgb_buffer);

	int num_bytes = 0;
	// YUV420P Buffer
	num_bytes = avpicture_get_size(PIX_FMT_YUV420P, m_width, m_height);
	m_yuv_buffer = (uint8_t *) malloc(num_bytes);
	avpicture_fill ((AVPicture *) m_yuv, m_yuv_buffer, 
	                PIX_FMT_YUV420P, m_width, m_height);

	// RGB Buffer
	num_bytes = avpicture_get_size(PIX_FMT_RGBA32, m_width, m_height);
	m_rgb_buffer = (uint8_t *) malloc(num_bytes);
	avpicture_fill ((AVPicture *)m_rgb, m_rgb_buffer, 
	                PIX_FMT_RGBA32, m_width, m_height);
#endif
}

#if  defined (HAVE_FFMPEG) || defined (HAVE_JPEG) 
void PluginVideoCapture::mirror(void *src, void *dest, int cols, int rows)
{
	void *srcRow, *destRow;

	for (int i = 0; i < rows; i++)
	{
		srcRow = (void *)( (intptr_t) src + i * (cols * 4));
		destRow = (void *)( (intptr_t) dest + (rows - i) * (cols * 4));
		memcpy(destRow, srcRow, cols * 4);
	}
}

int PluginVideoCapture::sendOverNetwork(unsigned char *source, unsigned int out_size)
{
	
	// transmit the encoded MPEG buffer to the receiver
	unsigned int bytes_sent = sendto(m_sock, 
	                          (void *) source, out_size, 
	                          0, (struct sockaddr *) &m_sin, sizeof(m_sin));

	if (bytes_sent != out_size)
		switch (errno)
		{
			case EAFNOSUPPORT:
				std::cerr << "ERROR: PluginVideoCapture::sendOverNetwork, sendto EAFNOSUPPORT" 
				          << std::endl;
				break;
			case EAGAIN:
				std::cerr << "ERROR: PluginVideoCapture::sendOverNetwork, sendto EAGAIN" 
				          << std::endl;
				break;
			case EBADF:
				std::cerr << "ERROR: PluginVideoCapture::sendOverNetwork, sendto EBADF" 
				          << std::endl;
				break;
			case ECONNRESET:
				std::cerr << "ERROR: PluginVideoCapture::sendOverNetwork, sendto ECONNRESET" 
				          << std::endl;
				break;
			case EINTR:
				std::cerr << "ERROR: PluginVideoCapture::sendOverNetwork, sendto EINTR" 
				          << std::endl;
				break;
			case EMSGSIZE:
				std::cerr << "ERROR: PluginVideoCapture::sendOverNetwork, sendto EMSGSIZE" 
				          << std::endl;
				break;
			case ENOTCONN:
				std::cerr << "ERROR: PluginVideoCapture::sendOverNetwork, sendto ENOTCONN" 
				          << std::endl;
				break;
			case ENOTSOCK:
				std::cerr << "ERROR: PluginVideoCapture::sendOverNetwork, sendto ENOTSOCK" 
				          << std::endl;
				break;
			case EOPNOTSUPP:
				std::cerr << "ERROR: PluginVideoCapture::sendOverNetwork, sendto EOPNOTSUPP" 
				          << std::endl;
				break;
			case EPIPE:
				std::cerr << "ERROR: PluginVideoCapture::sendOverNetwork, sendto EPIPE" 
				          << std::endl;
				break;
			case EACCES:
				std::cerr << "ERROR: PluginVideoCapture::sendOverNetwork, sendto EACCES" 
				          << std::endl;
				break;
			case EDESTADDRREQ:
				std::cerr << "ERROR: PluginVideoCapture::sendOverNetwork, sendto EDESTADDRREQ" 
				          << std::endl;
				break;
			case EHOSTUNREACH:
				std::cerr << "ERROR: PluginVideoCapture::sendOverNetwork, sendto EHOSTUNREACH" 
				          << std::endl;
				break;
			case EINVAL:
				std::cerr << "ERROR: PluginVideoCapture::sendOverNetwork, sendto EINVAL" 
				          << std::endl;
				break;
			case EIO:
				std::cerr << "ERROR: PluginVideoCapture::sendOverNetwork, sendto EIO" 
				          << std::endl;
				break;
			case EISCONN:
				std::cerr << "ERROR: PluginVideoCapture::sendOverNetwork, sendto EISCONN" 
				          << std::endl;
				break;
			case ENETDOWN:
				std::cerr << "ERROR: PluginVideoCapture::sendOverNetwork, sendto ENETDOWN" 
				          << std::endl;
				break;
			case ENETUNREACH:
				std::cerr << "ERROR: PluginVideoCapture::sendOverNetwork, sendto ENETUNREACH" 
				          << std::endl;
				break;
			case ENOBUFS:
				std::cerr << "ERROR: PluginVideoCapture::sendOverNetwork, sendto ENOBUFS" 
				          << std::endl;
				break;
			case ENOMEM:
				std::cerr << "ERROR: PluginVideoCapture::sendOverNetwork, sendto ENOMEM" 
				          << std::endl;
				break;
			default:
				std::cerr << "ERROR: PluginVideoCapture::sendOverNetwork, sendto UNKNOWN ERROR " 
				          << errno 
				          << std::endl;
				break;
	}
	return bytes_sent;
}

#endif //HAVE_FFMPEG || HAVE_JPEG

#ifdef HAVE_FFMPEG

void PluginVideoCapture::recordFrameAsMPEG( void ) 
{
	int out_size;
	int bytes_sent;

	// read the OpenGL pixel buffer
	m_img->readPixels(0, 0, m_width, m_height, GL_BGRA, GL_UNSIGNED_BYTE);

	// invert the image
	mirror(m_img->data(), m_rgb_buffer, m_width, m_height);


	// use the libavcodec library to convert to a YUV image
	img_convert((AVPicture *) m_yuv, PIX_FMT_YUV420P, // destination
	            (AVPicture *) m_rgb, PIX_FMT_RGBA32,  // source
	             m_width, m_height);                  // dimensions

	// encode the video into the output buffer
	out_size = 
		avcodec_encode_video(m_context,            // options, etc. 
		                    m_output_buffer,       // dest. of encoded video
		                    m_output_buffer_size,  
		                    m_yuv);                // pointer to image data

	if (m_send_over_network)
	{
		// transmit the encoded MPEG buffer to the receiver
		bytes_sent = sendOverNetwork(m_output_buffer, out_size);
	}
	
	if (m_write_to_file)
	{
		// transmit the encoded MPEG buffer to the receiver
		output_file.write((char *) m_output_buffer, out_size);
	}

}
#endif

// ================================================
// recordFrameAsImage (originally takeSnapshot) - From General Dynamics
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginVideoCapture::recordFrameAsImage( void )
{

	// read the OpenGL pixel buffer
	m_img->readPixels(0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE);

#ifdef HAVE_JPEG
	
	if (m_send_over_network)
	{
		osg::ref_ptr<osg::Image> tmp_img = new osg::Image(*m_img);
		tmp_img->flipVertical();

		int size = m_jpeg.compress(tmp_img->data(), 
			m_jpeg_buffer, 
			tmp_img->s(), 
			tmp_img->t(), 
			80);

		sendOverNetwork(m_jpeg_buffer, size);
	}
#endif
	if (m_write_to_file)
	{

		char outfname[512];
		int result = snprintf(outfname, sizeof(outfname), "%s/%s%05u.%s",
						  directory.c_str(), baseFilename.c_str(), frame, "jpg");
		if(result >= int(sizeof(outfname)) )
		{
			outfname[sizeof(outfname)-1] = '\0';
		}
		else if (result > 0)
		{
			outfname[result] = '\0';
		}

		if(osgDB::writeImageFile(*m_img, outfname) )
		{
			std::cout << "Saved screenshot to " << outfname << std::endl;
		}
		else
		{
			std::cout << "Could not save image file " << outfname << std::endl;
		}
		++frame;
	}
}


void PluginVideoCapture::getConfig( void )
{

	DefFileGroup *root = *DefFileData;
	if(root == NULL)
		return;
	DefFileGroup *captureGroup = root->getGroupByURI( "/capture/" );
	if(captureGroup == NULL)
		return;
	DefFileGroup *windowGroup = root->getGroupByURI( "/window/" );
	if( windowGroup == NULL )
		return;
	
	DefFileAttrib *attr;

	attr = windowGroup->getAttribute( "width" );
	if( attr )
	{
		m_width = attr->asInt();
	}

	attr = windowGroup->getAttribute( "height" );
	if( attr )
	{
		m_height = attr->asInt();
	}

	attr = captureGroup->getAttribute( "screenshot_directory" );
	if ( attr )
	{
		directory = attr->asString();
	}
	else
	{
		directory = baseProgramDirectory;
	}

	attr = captureGroup->getAttribute( "screenshot_base_filename" );
	if ( attr )
	{
		baseFilename = attr->asString();
	}

	attr = captureGroup->getAttribute( "filetype" );
	if ( attr )
	{
		extension = attr->asString();
		if (extension == "jpg")
			m_jpeg_extension = true;
		else
			m_jpeg_extension = false;

	}

#ifdef HAVE_FFMPEG
	attr = captureGroup->getAttribute( "bitrate" );
	if( attr )
	{
		if(m_context != NULL)
		{
			m_context->bit_rate = attr->asInt();
		}
	}
	attr = captureGroup->getAttribute( "gopsize" );
	if( attr )
	{
		if(m_context != NULL)
			m_context->gop_size = attr->asInt();
	}

	attr = captureGroup->getAttribute( "gray" );
	if( attr )
	{
		if(m_context != NULL)
			if(attr->asInt())
				m_context->flags |= CODEC_FLAG_GRAY;
			else
				m_context->flags &= ~CODEC_FLAG_GRAY;
	}
#endif	//HAVE_FFMPEG
	attr = captureGroup->getAttribute( "write_to_file" );
	if( attr )
	{
		m_write_to_file = attr->asInt();
#ifdef HAVE_FFMPEG
		if (m_write_to_file)
		{
			attr = captureGroup->getAttribute( "file_name" );
			if( attr )
			{
				m_file_name = attr->asString();
				output_file.open(m_file_name.c_str(), std::ios::binary | std::ios::out);

				std::cout << "PluginVideoCapture::getConfig "
				          << "opened file for mpeg2 writing " << m_file_name << std::endl;
			}
		}
#endif //HAVE_FFMPEG
	}
#if defined (HAVE_JPEG) ||defined  (HAVE_FFMPEG)
	attr = captureGroup->getAttribute( "send_over_network" );
	if( attr )
	{
		m_send_over_network = attr->asInt();
		if (m_send_over_network)
		{

		attr = captureGroup->getAttribute( "dest_addr" );
		if( attr )
		{
			m_dest_addr = attr->asString();
		}

		attr = captureGroup->getAttribute( "dest_port" );
		if( attr )
		{
			m_dest_port = attr->asInt();
		}

		attr = captureGroup->getAttribute( "is_broadcast" );
		if( attr )
		{
			m_is_broadcast = attr->asInt();
		}

		// open up the socket
		m_sock = socket(AF_INET, SOCK_DGRAM, 0);
		m_sin.sin_family = AF_INET;
		m_sin.sin_port = htons(m_dest_port);

		// BROADCAST UDP
		if (m_is_broadcast)
		{
			int on=1;
			setsockopt(m_sock, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
		}

		// MULTICAST OR UNICAST ARE OK
		m_sin.sin_addr.s_addr = inet_addr(m_dest_addr.c_str());

		std::cout << "PluginVideoCapture::getConfig "
		          << "opened socket for video Streaming." << std::endl;

		}
		attr = captureGroup->getAttribute( "capture_on_start" );
		if( attr )
		{
			if (attr->asInt())
				startStopMovie();
		}

	}
#endif //HAVE_JPEG || HAVE_FFMPEG
	resize(m_width, m_height);
#ifdef HAVE_FFMPEG
	// have to open up the codec after all changes to the context to 
	// ensure options are honored by the codec
	
	if (!m_codec)
	{
		std::cerr << "PluginVideoCapture::PluginVideoCapture " 
		          << "failed to find MPEG2 codec" << std::endl;
	}


	if (avcodec_open(m_context, m_codec) < 0)
	{
		std::cerr << "PluginVideoCapture::PluginVideoCapture "
		          << "failed to open MPEG2 codec" << std::endl;
	}
#endif
	
}

} //VideoCapture
