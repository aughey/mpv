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
 *  FILENAME:   ImageCache.h
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


#ifndef IMAGECACHE_H
#define IMAGECACHE_H

#include "Export.h"

#include <string>
#include <map>
#include <osgDB/ReadFile>
#include <osg/Image>

//=========================================================
//! This class is responsible for loading and caching images.
//! 
class OSGPARTICLE_EXPORT ImageCache {
public:

	//=========================================================
	//! Singleton Constructor
	//! 
	static ImageCache *Instance();

	//=========================================================
	//! General Destructor
	//! 
	~ImageCache();
	
	//=========================================================
	//! Loads and returns the specified image file or, if the file has been 
	//! loaded already, returns a pointer to the already-loaded instance.
	//! 
	osg::Image *loadImage( const std::string &filename );

protected:

	//=========================================================
	//! General Constructor
	//! 
	ImageCache();

private:
	
	//=========================================================
	//! A map which maps filenames to osg::Images.  This is used to 
	//! store the image corresponding to the filename, so that they don't 
	//! need to be loaded from disk more than once.  
	//! 
	std::map< std::string, osg::ref_ptr<osg::Image> > cacheMap;

	//=========================================================
	//! The singleton instance for this class
	//! 
	static ImageCache * Singleton;
	
	
};


#endif
