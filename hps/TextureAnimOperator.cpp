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
 *  FILENAME:   TextureAnimOperator.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This file defines the TextureAnimOperator class.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  05/28/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#include <stdio.h>

#include "TextureAnimOperator.h"
#include "ImageCache.h"

// ================================================
// TextureAnimOperator
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
TextureAnimOperator::TextureAnimOperator() 
		: osgParticleHPS::Operator() 
{
	period_ = 1. / 30.; 
	loop_ = false;
	hold_last_frame_ = false;
}


// ================================================
// addFrame
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void TextureAnimOperator::addFrame( std::string &filename )
{
	
	osg::Image *image = ImageCache::Instance()->loadImage( filename );
	if( image ) 
	{
		osg::Texture2D *texture = new osg::Texture2D;
//		texture->setDataVariance(osg::Object::DYNAMIC); // protect from being optimized away as static state.
		texture->setImage( image );
		textures_.push_back( osg::ref_ptr<osg::Texture2D>( texture ) );
	}
	else
	{
		fprintf( stderr, "Warning: could not load image \"%s\"\n", 
			filename.c_str() );
	}
}


// ================================================
// beginOperate
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void TextureAnimOperator::beginOperate( osgParticleHPS::Program *program )
{
	if(program)
	{
		osgParticleHPS::ParticleSystem* ps = program->getParticleSystem();
		if(ps)
		{
			osg::StateSet* stateset = ps->getStateSet();
			if(stateset && stateset_.get() != stateset )
			{
				stateset_ = stateset;
			}
		}
	}
}


// ================================================
// operate
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void TextureAnimOperator::operate(osgParticleHPS::Particle *P, double dt)
{
	if(!P) return;

	// do we have anything to animate?
	if( textures_.empty() )
		return;

	float partAge = P->getAge();

	int frameNum = (int)(partAge / period_);

	if( frameNum < 0 ) frameNum = 0;
	if( frameNum >= textures_.size() ) 
	{
		if( loop_ )
		{
			frameNum = frameNum % textures_.size();
		}
		else
		{
			if( hold_last_frame_ )
				frameNum = textures_.size() - 1;
			else
				frameNum = -1;
		}
	}
	stateset_->setTextureAttributeAndModes(0, textures_[frameNum].get(),
			osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE );
}

