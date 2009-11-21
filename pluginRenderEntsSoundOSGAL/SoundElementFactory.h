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
 *  This class constructs SoundElements
 *  
 *  Initial Release: 2007-04-07 Andrew Sampson
 * </pre>
 */


#ifndef _SOUND_ELEMENT_FACTORY_H_
#define _SOUND_ELEMENT_FACTORY_H_

#include "EntityElementFactory.h"


class SoundElementFactory : public EntityElementFactory
{
public:
	SoundElementFactory();
	virtual ~SoundElementFactory();
	
	virtual EntityElement *createElement( 
		DefFileGroup *elementDefinition, Entity *ent );
	
	
protected:
	
};

#endif
