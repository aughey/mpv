/** <pre>
 *  Multi-Purpose Viewer - GCC coordinate conversion plugin
 *  Copyright (c) 2009 Andrew Sampson
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
 *  2009-01-17 Andrew Sampson
 *      Initial release
 *  
 *  
 *  </pre>
 */


#ifndef _PLUGINCOORDINATECONVERSIONGCC_H_
#define _PLUGINCOORDINATECONVERSIONGCC_H_

#include <string>
#include <map>

#include "RefPtr.h"
#include "Plugin.h"
#include "DefFileGroup.h"
#include "CoordinateConverterGCC.h"

using namespace mpv;

//=========================================================
//! This plugin performs coordinate conversion for GDC<->GCC.  The real work 
//! is done in CoordinateConverterGCC, an instance of which is posted to 
//! the blackboard by way of PluginCoordinateConversionMgr's 
//! CoordinateConverterMap.
//! 
class PluginCoordinateConversionGCC : public Plugin 
{
public:
	//=========================================================
	//! General Constructor
	//! 
	PluginCoordinateConversionGCC();

protected:
	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginCoordinateConversionGCC() throw();
	
public:
	//=========================================================
	//! The per-frame processing that this plugin performs.
	//! This plugin's act() does ...
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );
	
	
private:
	
	typedef std::map< 
		CoordinateSystemPair, 
		RefPtr<CoordinateConverter> > CoordinateConverterMap;

	//=========================================================
	//! The coordinate converters.  Retrieved from the blackboard.
	//! 
	CoordinateConverterMap *coordinateConverters;
	
	//=========================================================
	//! Geocentric coordinate converter.  Posted to 
	//! coordinateConverters.
	//! 
	RefPtr<CoordinateConverterGCC> coordConverterGCC;

};

#endif

