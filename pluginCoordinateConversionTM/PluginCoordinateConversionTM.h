/** <pre>
 *  MPV TM coordinate conversion plugin
 *  Copyright (c) 2008 The Boeing Company
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
 *  04/04/2004 Greg Basler
 *      Initial Release.
 *  
 *  2007-07-03 Andrew Sampson
 *      Changed interface to use new state machine API
 *  
 *  2008-07-20 Andrew Sampson
 *      Ported old pluginOSPositionConv code to the new coordinate conversion 
 *      API.  The plugin class was based on PluginCoordinateConversionMgr.
 *      The conversion code was based on "PositionConversion".
 *  
 * </pre>
 */


#ifndef _PLUGINCOORDINATECONVERSIONTM_H_
#define _PLUGINCOORDINATECONVERSIONTM_H_

#include <string>
#include <map>

#include "RefPtr.h"
#include "Plugin.h"
#include "DefFileGroup.h"
#include "CoordinateConverterTM.h"
#include "CoordinateConverterEasyTM.h"

using namespace mpv;

//=========================================================
//! This plugin performs coordinate conversion for GDC<->TM.  The real work 
//! is done in CoordinateConverterTM, an instance of which is posted to 
//! the blackboard by way of PluginCoordinateConversionMgr's 
//! CoordinateConverterMap.
//! 
class PluginCoordinateConversionTM : public Plugin 
{
public:
	//=========================================================
	//! General Constructor
	//! 
	PluginCoordinateConversionTM();

	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginCoordinateConversionTM() throw();
	
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
	//! Transverse Mercator coordinate converter.  Posted to 
	//! coordinateConverters.
	//! 
	RefPtr<CoordinateConverterTM> coordConverterTM;

	//=========================================================
	//! Easy Transverse Mercator coordinate converter.  Posted to 
	//! coordinateConverters.
	//! 
	RefPtr<CoordinateConverterEasyTM> coordConverterEasyTM;

};

#endif

