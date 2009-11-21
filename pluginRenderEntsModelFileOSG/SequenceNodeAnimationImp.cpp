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

#include "BindSlot.h"

#include "SequenceNodeAnimationImp.h"


using namespace mpv;

SequenceNodeAnimationImp::SequenceNodeAnimationImp( Animation *anim, osg::Sequence *newSequenceNode ) : 
	AnimationImp( anim ), 
	sequenceNode( newSequenceNode )
{
	animationParametersChanged( animation );
	
	animation->directionChanged.connect( BIND_SLOT1( SequenceNodeAnimationImp::animationParametersChanged, this ) );
	animation->loopModeChanged.connect( BIND_SLOT1( SequenceNodeAnimationImp::animationParametersChanged, this ) );
	animation->stateChanged.connect( BIND_SLOT1( SequenceNodeAnimationImp::animationParametersChanged, this ) );
}


SequenceNodeAnimationImp::~SequenceNodeAnimationImp()
{
}


void SequenceNodeAnimationImp::animationParametersChanged( Animation * )
{
	if( !sequenceNode.valid() )
		return;

	// We'll assume that most model animations are meant to be played back 
	// at about 30 fps (hence the 0.033333).
	// The second argument to setDuration is the number of times to repeat 
	// the animation.  Positive values are treated as you would expect, and 
	// a value of -1 indicates infinite repetition.
	sequenceNode->setDuration( 0.033333, 
		(animation->getLoopMode() == Animation::Continuous) ? -1 : 1 );

	// osg::Sequence::setMode() will ignore the following state transitions:
	//    - STOP->PAUSE
	//    - STOP->RESUME
	// These special cases are handled below.
	osg::Sequence::SequenceMode currentMode = sequenceNode->getMode();
	switch( animation->getState() )
	{
	case Animation::Play:
		sequenceNode->setMode( osg::Sequence::START );
		break;
	case Animation::Pause:
		if( currentMode == osg::Sequence::STOP )
		{
			sequenceNode->setMode( osg::Sequence::START );
		}
		sequenceNode->setMode( osg::Sequence::PAUSE );
		break;
	case Animation::Continue:
		if( currentMode == osg::Sequence::STOP )
		{
			sequenceNode->setMode( osg::Sequence::START );
		}
		sequenceNode->setMode( osg::Sequence::RESUME );
		break;
	case Animation::Stop:
	default:
		sequenceNode->setMode( osg::Sequence::STOP );
		break;
	}

}
