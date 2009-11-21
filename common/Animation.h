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
 *  FILENAME:   Animation.h
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

#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <list>

#include <CigiEntityCtrlV3.h>

#include "Referenced.h"
#include "MPVCommonTypes.h"


namespace mpv
{

// forward declaration; defined at bottom of file
class AnimationImp;


//============================================================================
//! Class to contain the animation-related values and methods.  Child classes 
//! should inherit from AnimationImp in order to act on changes to the animation 
//! state.
class MPVCMN_SPEC Animation : public Referenced
{
public:

	//=========================================================
	//! The enumeration for the Animation State flag
	//!
	enum AnimationState
	{
		Stop = CigiBaseEntityCtrl::Stop,
		Pause = CigiBaseEntityCtrl::Pause,
		Play = CigiBaseEntityCtrl::Play,
		Continue = CigiBaseEntityCtrl::Continue
	};

	//=========================================================
	//! The enumeration for the Animation Direction
	//!
	enum AnimationDirection
	{
		Forward = CigiBaseEntityCtrl::Forward,
		Backward = CigiBaseEntityCtrl::Backward
	};

	//=========================================================
	//! The enumeration for the Animation Direction
	//!
	enum AnimationLoopMode
	{
		OneShot = CigiBaseEntityCtrl::OneShot,
		Continuous = CigiBaseEntityCtrl::Continuous
	};

public:
	boost::signal<void (Animation*)> directionChanged;
	boost::signal<void (Animation*)> loopModeChanged;
	boost::signal<void (Animation*)> stateChanged;
	boost::signal<void (Animation*)> animationFinished;

	//=========================================================
	//! General Constructor
	//!
	Animation();

	//=========================================================
	//! Copy Constructor
	//!
	Animation( const Animation &other );


	int getID() const { return id; }
	AnimationDirection getDirection() const { return direction; }
	AnimationLoopMode getLoopMode() const { return loopMode; }
	AnimationState getState() const { return state; }

	//=========================================================
	//! Returns isFinished, which indicates whether the animation has run 
	//! its course.
	//! The value of this is valid only when AnimationState is Continue.
	//! Some types of animations, such as animations that have had looping 
	//! enabled, and some particle systems, will never end.
	//! \return true if the animation has played and has stopped ("on its own", 
	//! if you catch my meaning), false otherwise
	//!
	bool getIsFinished() const;


	void setID( int newID );
	void setDirection( AnimationDirection newDirection );
	void setLoopMode( AnimationLoopMode newLoopMode );
	void setState( AnimationState newState );

	//=========================================================
	//! The container for this animation will call this method once per frame
	//! \param timeElapsed the time elapsed since the last call, in seconds
	//! 
	virtual void update( double timeElapsed );

	void addImplementation( AnimationImp *newImp );
	
protected:
	//=========================================================
	//! General Destructor
	//!
	virtual ~Animation();
	
	void implementationFinishedPlaying( AnimationImp * );

	//=========================================================
	//! The ID of this animation.  CIGI doesn't currently allow for more than 
	//! one animation per entity, but I hope that that will change at some 
	//! point.
	//!
	int id;
	
	//=========================================================
	//! The animation direction, ie forward or backward
	//!
	AnimationDirection direction;

	//=========================================================
	//! The loop mode, ie oneshot, continuous
	//!
	AnimationLoopMode loopMode;

	//=========================================================
	//! Animation State, ie playing, paused, stopped
	//!
	AnimationState state;

	//=========================================================
	//! Implementation objects for this Animation.  
	//!
	std::list< RefPtr<AnimationImp> > imps;
	
};


//=========================================================
//! An implementation class, which complements Animation.
//! Those seeking to do interesting things with Animations should 
//! inherit from this class.
//! 
class MPVCMN_SPEC AnimationImp : public Referenced
{
public:
	boost::signal<void (AnimationImp*)> animationFinished;

	//=========================================================
	//! General Constructor
	//! 
	AnimationImp( Animation *_animation ) : 
		Referenced(), 
		animation( _animation ), 
		isFinished( false )
	{}
	

	virtual void update( double timeElapsed ) {}

	//=========================================================
	//! Returns isFinished, which indicates whether the animation has run 
	//! its course.
	//! The value of this is valid only when AnimationState is Continue.
	//! Some types of animations will never end.  Examples include some 
	//! types of particle systems, and animations that have had looping 
	//! enabled.
	//! \return true if the animation has played and has stopped ("on its own", 
	//! if you catch my meaning), false otherwise
	//!
	bool getIsFinished() const { return isFinished; }

protected:
	//=========================================================
	//! General Destructor
	//! 
	virtual ~AnimationImp() {}
	
	//! protected, because only child classes should be setting isFinished
	void setIsFinished( bool newIsFinished )
	{
		if( isFinished != newIsFinished )
		{
			isFinished = newIsFinished;
			if( isFinished )
				animationFinished( this );
		}
	}

	//=========================================================
	//! Pointer to the owning Animation.  
	//!
	Animation *animation;

	//=========================================================
	//! A boolean, indicating whether the animation has run its course.
	//! The value of this is valid only when AnimationState is Continue.
	//! Some types of animations will never end.  Examples include some 
	//! types of particle systems, and animations that have had looping 
	//! enabled.
	//!
	bool isFinished;

};

}
#endif
