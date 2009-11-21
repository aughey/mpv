/** <pre>
 *  Plugin to add positional audio to entities, for the MPV
 *  Copyright (c) 2007 Andrew Sampson
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
 *  This class is responsible for creating a subgraph containing positional 
 *  audio nodes.
 *  
 *  Initial Release: 2007-04-07 Andrew Sampson
 * </pre>
 */


#ifndef _SOUND_ELEMENT_H_
#define _SOUND_ELEMENT_H_

#include <osgAL/SoundNode>
#include "EntityElement.h"

class SoundElement : public EntityElement
{
public:
	
	SoundElement();
	
	virtual ~SoundElement();
	
	virtual bool construct( DefFileGroup *config, Entity *entity );
	
	virtual osg::Node *getTopNode() { return groupNode.get(); }
	
	virtual bool addChildElement( EntityElement *childElement );
	
protected:
	
	osg::ref_ptr< osg::Group > groupNode;
	
	// used for generating unique state names
	static int soundStateCounter;

	//=========================================================
	//! Creates a sound node with the given parameters.
	//! \param gain - volume multiplier; default value is 1.0
	//! \param pitch - pitch (playback speed) multiplier; default value is 1.0
	//! \param shouldPlay - set this to true if sound should start playing immediately
	//! \param shouldLoop - set this to true if sound should loop
	//! \param priority - The sound card hardware can play only a limited 
	//!        number of sounds at the same time. The priority value is used 
	//!        when there are more sounds to play than the hardware can handle.
	//!        Sounds with a higher priority are more likely to be granted a 
	//!        hardware resource than sounds with lower priority.  The base 
	//!        priority value is 0.
	//! \param referenceDistance - the sound will be unattenuated if the 
	//!        listener is closer than this distance to the sound
	//! \param rolloffFactor - this value controls distance-based attenuation 
	//!        of sounds.  The higher the value, the quicker the sound will 
	//!        attenuate as the distance between the listener and sound 
	//!        increases.  It is not clear to me if this value is a multiplier, 
	//!        an exponent, or something else.  The default value is 1.0.
	//! \return a newly constructed SoundNode, or NULL if the file pointed to 
	//!         by filename couldn't be loaded
	//! 
	osg::ref_ptr<osgAL::SoundNode> createSound( 
		const std::string &filename,
		float gain = 1.0f,
		float pitch = 1.0f,
		bool shouldPlay = true,
		bool shouldLoop = true,
		int priority = 0,
		float referenceDistance = 10.0f,
		float rolloffFactor = 1.0f );
};


#endif
