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
 *  2008-07-06 Andrew Sampson
 *      Initial release
 *  
 *  
 *  </pre>
 */


#ifndef _ENTITYCOORDINATECONVERSIONOBSERVER_H_
#define _ENTITYCOORDINATECONVERSIONOBSERVER_H_

#include "CoordinateConversionObserver.h"
#include "EntityContainer.h"
#include "MPVCommonTypes.h"


using namespace mpv;

//=========================================================
//! 
//! 
class EntityCoordinateConversionObserver : public CoordinateConversionObserver
{
public:
	//=========================================================
	//! General Constructor
	//! 
	EntityCoordinateConversionObserver();
	
	void startObservingContainer( EntityContainer *entContainer );
	
	void stopObservingContainer();
	
protected:
	//=========================================================
	//! General Destructor
	//! 
	virtual ~EntityCoordinateConversionObserver();
	
	virtual void performConversionForAllObservedObjects();

	void startObservingEntity( EntityContainer*, Entity *entity );

	void stopObservingEntity( EntityContainer*, Entity *entity );

	RefPtr<EntityContainer> container;
};

#endif
