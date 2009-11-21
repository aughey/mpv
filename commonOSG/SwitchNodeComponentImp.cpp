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

#include <iostream>

#include <osg/Switch>
#include <osgSim/MultiSwitch>

#include "BindSlot.h"

#include "SwitchNodeComponentImp.h"


using namespace mpv;


SwitchNodeComponentImp::SwitchNodeComponentImp( mpv::Component *_component, osg::Group *newSwitchNode ) : 
	ComponentImp( _component ),
	switchNode( newSwitchNode )
{
	componentStateChanged( component );
	
	component->stateChanged.connect( BIND_SLOT1( SwitchNodeComponentImp::componentStateChanged, this ) );
}


SwitchNodeComponentImp::~SwitchNodeComponentImp()
{
}


void SwitchNodeComponentImp::componentStateChanged( Component * )
{
	if( !switchNode.valid() )
		return;

	// look up the switch value, based on the component's cigi value
	int switchValue;
	std::map< int, int >::iterator iter = switchStateMap.find( component->getState() );
	if( iter != switchStateMap.end() )
	{
		switchValue = iter->second;
	}
	else
	{
		static bool didPrintWarning = false;
		if( !didPrintWarning )
			std::cout << "Warning in entity switch-node-component-ctrl - Host sent value " 
				<< (int)component->getState() 
				<< ", \n\twhich does not have a known entry " 
				<< "in this component control \n\t(control #" 
				<< component->getID() 
				<< " in entity instance " << component->getInstanceID()
				<< ")\n\tGo fix your config files!\n"
				<< "\tThis warning will only be printed once, ever.\n";

		didPrintWarning = true;
		switchValue = component->getState();
	}

	osg::Switch *ccSwitch = dynamic_cast<osg::Switch *>(switchNode.get());
	if( ccSwitch )
	{
		// turn on the child specified by the compCtrl's state
		ccSwitch->setSingleChildOn( switchValue );
	}
	else
	{
		osgSim::MultiSwitch *ccMSwitch = dynamic_cast<osgSim::MultiSwitch *>(switchNode.get());
		if( ccMSwitch )
			ccMSwitch->setSingleChildOn( 0, switchValue );
	}

}
