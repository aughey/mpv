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
 *  FILENAME:   JpegImage.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  DESCRIPTION: 
 *  Wraps up libjpeg
 *  
 *  2007-04-26 Robert Bauer,  General Dynamics Land Systems
 *  
 * </pre>
 */


#ifdef HAVE_JPEG

#include <string.h>

#include "JpegImage.h"

unsigned int VideoCapture::JpegImage::bytesWritten;
int 		 VideoCapture::JpegImage::bufferSize;  

using namespace std;

namespace VideoCapture
{

JpegImage::JpegImage()
{
	cinfo.err = jpeg_std_error( &jerror );
	jpeg_create_compress( &cinfo );
	cinfo.dest = ( struct jpeg_destination_mgr * )
	             ( cinfo.mem->alloc_small ) ( ( j_common_ptr ) & cinfo, JPOOL_PERMANENT,
	                 sizeof( DestMgr ) );
}

JpegImage::~JpegImage()
{
	jpeg_destroy_compress( &cinfo );
}
unsigned int JpegImage::compress( unsigned char *source,
                                  unsigned char *destination,
                                  unsigned int width,
                                  unsigned int height,
                                  unsigned int quality )
{
	bytesWritten = 0;

	bufferSize = width * height * quality / 100;

	dest = reinterpret_cast<DestMgr *>( cinfo.dest );
	dest->pub.init_destination	= initDestination;
	dest->pub.empty_output_buffer = emptyOutputBuffer;
	dest->pub.term_destination	= termDestination;
	dest->writeBuff = destination;
	dest->imageSize = width * height * 3;

	cinfo.image_width	= width;
	cinfo.image_height = height;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;

	jpeg_set_defaults( &cinfo );

	jpeg_set_quality( &cinfo, quality, false );
	jpeg_start_compress( &cinfo, TRUE );

	JSAMPROW row_pointer[ 1 ];
	int row_stride = width * 3;

	while ( cinfo.next_scanline < cinfo.image_height )
	{
		row_pointer[ 0 ] =
		    &source[ cinfo.next_scanline * row_stride ];
		jpeg_write_scanlines( &cinfo, row_pointer, 1 );
	}

	jpeg_finish_compress( &cinfo );

	return bytesWritten;
}


void JpegImage::initDestination( j_compress_ptr cinfo )
{

	DestMgr * d = reinterpret_cast<DestMgr *>( cinfo->dest );

	d->buffer = ( JOCTET * ) ( *cinfo->mem->alloc_small )
	            ( ( j_common_ptr ) cinfo, JPOOL_IMAGE, bufferSize * sizeof( JOCTET ) );

	d->pub.next_output_byte = d->buffer;
	d->pub.free_in_buffer = bufferSize;

}

boolean JpegImage::emptyOutputBuffer( j_compress_ptr cinfo )
{

	DestMgr * d = reinterpret_cast<DestMgr *>( cinfo->dest );

	memcpy( d->writeBuff + bytesWritten, d->buffer, bufferSize );
	bytesWritten += bufferSize;

	d->pub.next_output_byte = d->buffer;
	d->pub.free_in_buffer = bufferSize;

	return TRUE;
}

void JpegImage::termDestination( j_compress_ptr cinfo )
{

	DestMgr * d = reinterpret_cast<DestMgr *>( cinfo->dest );
	size_t dataLeft = bufferSize - d->pub.free_in_buffer;

	if ( dataLeft > 0 )
	{
		memcpy( d->writeBuff + bytesWritten, d->buffer, dataLeft );
		bytesWritten += static_cast<int>( dataLeft );
	}
}


} //VideoCapture

#endif //HAVE_JPEG


