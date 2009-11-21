/** <pre>
 *  Entity scene graph plugin
 *  Copyright (c) 2008 Andrew Sampson
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
 *  
 *  2008-08-02 Andrew Sampson
 *      Initial release.  Split code out from ModelElement.
 *  
 * </pre>
 */

#ifndef _SEQUENCE_NODE_ANIMATION_IMP_H_
#define _SEQUENCE_NODE_ANIMATION_IMP_H_

#include <osg/Sequence>

#include "Animation.h"


//=========================================================
//! This class will control the animation state of a sequence node, according 
//! to the state changes of an Animation.
//! 
class SequenceNodeAnimationImp : public mpv::AnimationImp
{
public:
	SequenceNodeAnimationImp( mpv::Animation *anim, osg::Sequence *newSequenceNode );
	
protected:

	virtual ~SequenceNodeAnimationImp();
	
	void animationParametersChanged( mpv::Animation * );
	
	osg::ref_ptr<osg::Sequence> sequenceNode;
};


#endif
