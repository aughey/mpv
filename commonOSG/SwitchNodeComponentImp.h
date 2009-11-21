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

#ifndef _SWITCH_NODE_COMPONENT_IMP_H_
#define _SWITCH_NODE_COMPONENT_IMP_H_

#include <osg/Group>

#include "Component.h"
#include "MPVCommonOSGTypes.h"


//=========================================================
//! This class will update the state of a switch node, according to the 
//! component control values set by the Host.
//! 
class MPVCMNOSG_SPEC SwitchNodeComponentImp : public mpv::ComponentImp
{
public:
	
	//=========================================================
	//! Constructor
	//! \param newSwitch - the switch node that this component is associated with
	//! 
	SwitchNodeComponentImp( mpv::Component *_component, osg::Group *newSwitchNode );
	
	//=========================================================
	//! Adds a new entry in the switchStateMap.  When the host sets the 
	//! component control's value to cigiValue, the switch will be set 
	//! to switchValue.
	//! \param cigiValue - the component value as (potentially) set by the host
	//! \param switchValue - the value to set on the switch, if cigiValue is 
	//!     encountered
	//! 
	void addSwitchStatePair( int cigiValue, int switchValue )
	{
		switchStateMap[cigiValue] = switchValue;
	}

protected:
	//=========================================================
	//! Destructor
	//! 
	virtual ~SwitchNodeComponentImp();

	void componentStateChanged( mpv::Component * );

	//=========================================================
	//! The switch node that this component ctrl is associated with
	//! 
	osg::ref_ptr<osg::Group> switchNode;

	//=========================================================
	//! A map, which maps CIGI component control values to switch node values
	//! 
	std::map< int, int > switchStateMap;
	
};



#endif
