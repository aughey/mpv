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

#include "AnimationContainer.h"

using namespace mpv;


AnimationContainer::AnimationContainer() : Referenced()
{
	
}


AnimationContainer::~AnimationContainer()
{
	// when an AnimationContainer is deleted, all Animations attached to it are deleted
//	flagAllAnimationsAsDestroyed();
}


void AnimationContainer::updateAnimations( double timeElapsed )
{
	AnimationMap::iterator iter = animations.begin();
	for( ; iter != animations.end(); iter++ )
	{
		iter->second->update( timeElapsed );
	}
}


/*void AnimationContainer::flagAllAnimationsAsDestroyed()
{
	while( !animations.empty() )
	{
		// hold on to a reference until signal emission is complete
		RefPtr<Animation> animation( animations.begin()->second );

		animation->setState( Animation::Destroyed );
		// note - callback animationChangedState will handle removal of animation 
		// from the map
	}
}*/


void AnimationContainer::addAnimation( Animation *animation )
{
	if( animation != NULL )
	{
//		animation->stateChanged.connect( SLOT( this, AnimationContainer::animationChangedState ) );
		animations[animation->getID()] = animation;

		addedAnimation( this, animation );
	}
}


void AnimationContainer::removeAnimation( Animation *animation )
{
	AnimationMap::iterator iter = animations.find( animation->getID() );
	if( iter != animations.end() )
	{
		// hold on to a reference until signal emission is complete
		RefPtr<Animation> animationReference( animation );
		
//		animation->stateChanged.disconnect( SLOT( this, AnimationContainer::animationChangedState ) );
		animations.erase( iter );
		
		removedAnimation( this, animation );
	}
}


Animation *AnimationContainer::findOrCreateAnimation( int animationID )
{
	Animation *result = findAnimation( animationID );
	if( result == NULL )
	{
		result = new Animation;
		result->setID( animationID );
		addAnimation( result );
	}
	return result;
}


/*void AnimationContainer::animationChangedState( Animation *animation )
{
	if( animation->getState() == Animation::Destroyed )
		removeAnimation( animation );
}*/

