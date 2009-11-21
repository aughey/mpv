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
 *  FILENAME:   ImageCache.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  load and cache images.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  07/08/2005 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#include <iostream>
#include <stdio.h>

#include "ImageCache.h"



// ================================================
// ImageCache::Singleton
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ImageCache * ImageCache::Singleton = NULL;


// ================================================
// Instance
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ImageCache * ImageCache::Instance()
{
	if( Singleton == NULL )
		Singleton = new ImageCache;
	return Singleton;
}

// ================================================
// ImageCache
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ImageCache::ImageCache() 
{
}


// ================================================
// ~ImageCache
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
ImageCache::~ImageCache() 
{
}


// ================================================
// loadImage
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
osg::Image *ImageCache::loadImage( const std::string &filename )
{
	osg::ref_ptr<osg::Image> image = NULL;
	
	image = cacheMap[filename];
	if( image.valid() )
	{
//std::cout << "image " << filename << " retrieved from cache\n";
	}
	else
	{
		// image hasn't been loaded yet...
		image = osgDB::readImageFile( filename );
		cacheMap[filename] = image;
//std::cout << "image " << filename << " retrieved from disk\n";
	}
	return image.get();
}

