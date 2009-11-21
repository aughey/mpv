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
 *  FILENAME:   JpegImage.h
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


#ifndef _JPEG_IMAGE_
#define _JPEG_IMAGE_

#ifdef HAVE_JPEG

#include <stdio.h>
#include <jpeglib.h>

namespace VideoCapture
{
//=========================================================
//! The class encapsulating Jpeg image compression
//!
class JpegImage
{
public:

	//=========================================================
	//! General Constructor
	//!
	JpegImage();

	//=========================================================
	//! General Destructor
	//!
	~JpegImage();
	//=========================================================
	//! compress an image into a jpeg
	//! \param source  - buffer containing the source image.
	//! \param width   - the width of the source image.
	//! \param height  - the hieght of the source image.
	//! \param quality - the quality of the compression.
	unsigned int compress( unsigned char *source,
	                       unsigned char *destination,
	                       unsigned int width,
	                       unsigned int height,
	                       unsigned int quality );

protected:

private:

	typedef struct
	{
		struct jpeg_destination_mgr pub;
		JOCTET* buffer;
		unsigned char* writeBuff;
		int imageSize;
	}
	DestMgr;

	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr	jerror;
	unsigned char	**buffer;
	DestMgr *dest;
	static unsigned int bytesWritten;
	static int bufferSize;

	METHODDEF( boolean ) emptyOutputBuffer( j_compress_ptr cinfo );
	METHODDEF( void ) initDestination( j_compress_ptr cinfo );
	METHODDEF( void ) termDestination( j_compress_ptr cinfo );

}; //JpegImage

} //VideoCapture

#endif //HAVE_JPEG
#endif //JpegImage
