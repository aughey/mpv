/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2007 The Boeing Company
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
 *  FILENAME:   Animation.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  DESCRIPTION: 
 *  Class to contain the animation-related values and methods.  Child classes 
 *  should inherit from this in order to act on changes to the animation 
 *  state.
 *  
 *  MODIFICATION NOTES:
 *  
 *  2007-03-24 Andrew Sampson
 *      Initial release.  Code extracted from Entity.
 *  
 *  2008-07-06 Andrew Sampson
 *      Rewritten to use XLObject, signals/slots, etc
 *  </pre>
 */

#include "BindSlot.h"
#include "Animation.h"

using namespace mpv;


Animation::Animation() : Referenced()
{
	id = 0xffff;
	direction = Forward;
	loopMode = OneShot;
	state = Stop;
}

Animation::Animation( const Animation &other )
{
	id = 0xffff;
	direction = other.direction;
	loopMode = other.loopMode;
	state = other.state;
}

Animation::~Animation()
{

}


bool Animation::getIsFinished() const
{
	std::list< RefPtr<AnimationImp> >::const_iterator iter;
	for( iter = imps.begin(); iter != imps.end(); iter++ )
	{
		if( ! (*iter)->getIsFinished() )
			return false;
	}
	return true;
}


void Animation::setID( int newID )
{
	id = newID;
}


void Animation::setDirection( Animation::AnimationDirection newDirection )
{
	if( direction != newDirection )
	{
		direction = newDirection;
		directionChanged( this );
	}
}


void Animation::setLoopMode( Animation::AnimationLoopMode newLoopMode )
{
	if( loopMode != newLoopMode )
	{
		loopMode = newLoopMode;
		loopModeChanged( this );
	}
}


void Animation::setState( Animation::AnimationState newState )
{
	if( state != newState )
	{
		state = newState;
		stateChanged( this );
	}
}


void Animation::update( double timeElapsed )
{
	std::list< RefPtr<AnimationImp> >::iterator iter;
	for( iter = imps.begin(); iter != imps.end(); iter++ )
	{
		if( (*iter).valid() )
			(*iter)->update( timeElapsed );
	}
}


void Animation::addImplementation( AnimationImp *newImp )
{
	if( newImp != NULL )
	{
		imps.push_back( newImp );
		
		// connect to imp's animationFinished signal
		newImp->animationFinished.connect( BIND_SLOT1( Animation::implementationFinishedPlaying, this ) );
	}
}


void Animation::implementationFinishedPlaying( AnimationImp * )
{
	// One of the imps has stopped playing.  Check all the imps, and if 
	// they've *all* stopped playing, then signal that the animation has 
	// stopped playing.
	if( getIsFinished() )
		animationFinished( this );
}


/*
void Animation::ProcEntityCtrlPckt( CigiEntityCtrlV3 *Packet )
{
	bool tempForwardPlay = (Packet->GetAnimationDir() == CigiBaseEntityCtrl::Forward);
	bool tempAnimationLoopMode = (Packet->GetAnimationLoopMode() != CigiBaseEntityCtrl::OneShot);
	AnimationStateGrp tempAnimationState = (AnimationStateGrp)Packet->GetAnimationState();
	
	if( tempForwardPlay != ForwardPlay ||
		tempAnimationLoopMode != AnimationLoopMode ||
		tempAnimationState != AnimationState )
	{
		if( isFinished && 
			(tempForwardPlay != ForwardPlay || 
			tempAnimationLoopMode == true || 
			tempAnimationState == Play) )
		{
			isFinished = false;
		}
		ForwardPlay = tempForwardPlay;
		AnimationLoopMode = tempAnimationLoopMode;
		AnimationState = tempAnimationState;
		animationParametersChanged();
	}
}
*/
