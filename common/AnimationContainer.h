/** <pre>
 *  The MPV Common Library
 *  Copyright (c) 2008
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
 *  2008-07-05 Andrew Sampson
 *      Initial release
 *
 * </pre>
 */

#ifndef ANIMATION_CONTAINER_H
#define ANIMATION_CONTAINER_H

#include <utility>
#include <map>

#include "Referenced.h"
#include "MPVCommonTypes.h"
#include "Animation.h"

namespace mpv
{

//=========================================================
//! Implements a container for animations.  The lifetime of the animations in the 
//! container will be managed, and some of the animation-related signals will 
//! be handled here.
//! 
class MPVCMN_SPEC AnimationContainer : public virtual Referenced
{
public:

	boost::signal<void (AnimationContainer*, Animation*)> addedAnimation;
	boost::signal<void (AnimationContainer*, Animation*)> removedAnimation;
	
	typedef std::map< int, RefPtr<Animation> > AnimationMap;
	typedef std::pair< AnimationMap::iterator, AnimationMap::iterator > AnimationIteratorPair;

	//=========================================================
	//! Constructor
	//! 
	AnimationContainer();

	//=========================================================
	//! Calls update() on the animations in this container
	//! 
	void updateAnimations( double timeElapsed );

	//=========================================================
	//! Flags each animation in this container as 
	//! destroyed.
	//! 
//	void flagAllAnimationsAsDestroyed();
	
	//=========================================================
	//! Adds the specified animation to this container.
	//! \param animation - the animation to add
	//! 
	void addAnimation( Animation *animation );
	
	//=========================================================
	//! Removes the specified animation from this container.  
	//! \param animation - the animation to remove
	//! 
	void removeAnimation( Animation *animation );
	
	//=========================================================
	//! Returns the begin and end iterators for the animation container.
	//! \return the begin and end iterators for the animation container.
	//! 
	AnimationIteratorPair getAnimations()
	{
		return AnimationIteratorPair( animations.begin(), animations.end() );
	}
	
	//=========================================================
	//! Searches the container for the given animation.
	//! \return a pointer to the requested Animation, or NULL if not found
	//! 
	Animation *findAnimation( int animationID )
	{
		AnimationMap::iterator mapIter = 
			animations.find( animationID );
		if( mapIter != animations.end() )
		{
			return mapIter->second.get();
		}
		return NULL;
	}
	
	//=========================================================
	//! Searches the container for the given animation.  If a matching 
	//! animation is not found, a new animation is created and added.
	//! \param animationID the id of the animation to search for
	//! \return a pointer to the requested animation
	//! 
	Animation *findOrCreateAnimation( int animationID );

#if 0
	//=========================================================
	//! Returns the underlying map data structure.  This should only be 
	//! called in very rare situations... I didn't plan on allowing 
	//! direct access to this variable.
	//! \return a pointer to the underlying map data structure
	//! 
	AnimationMap *getAnimationMap()
	{
		return &animations;
	}
#endif
	
protected:
	
	//=========================================================
	//! Destructor
	//! 
	virtual ~AnimationContainer();
	
	//=========================================================
	//! The animations attached to this object.
	//! 
	AnimationMap animations;

	//=========================================================
	//! Callback; notification of state change for a animation
	//! 
//	void animationChangedState( Animation *animation );
};

}
#endif
