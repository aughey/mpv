/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2006 The Boeing Company
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
 *  FILENAME:   PluginUserInputMgrOSGGA.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  receive user input events and distribute those events to other plugins.
 *  
 *  2006-11-18 Andrew Sampson
 *  
 *  2007-07-21 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 */


#include <iostream>
#include <osgGA/GUIActionAdapter>

#include "PluginUserInputMgrOSGGA.h"

// A dummy class to keep osgGA::GUIEventHandler::handle happy.  
class DummyActionAdapter : public osgGA::GUIActionAdapter
{
public:
	virtual void requestRedraw() {}
	virtual void requestContinuousUpdate( bool ) {}
	virtual void requestWarpPointer( float, float ) {}
};
DummyActionAdapter dummyActionAdapter;


namespace UserInputMgrOSGGA
{

// KEYBOARD HANDLER ///////////////////////////////////////////////////

class KeyboardHandler: public osgGA::GUIEventHandler
{
public:
	KeyboardHandler( PluginUserInputMgrOSGGA *plug ) 
	{
		plugin = plug;
	}

	virtual bool handle( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter & )
	{
		if( ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN )
		{
			int key = ea.getKey();
			if( key == 'q' || key == osgGA::GUIEventAdapter::KEY_Escape )
			{
				plugin->setUserRequestedQuit();
				return true;
			}
			if( key == osgGA::GUIEventAdapter::KEY_Space )
			{
				plugin->setUserRequestedDebug();
				return true;
			}
		}

		return false;
	}

	virtual void getUsage( osg::ApplicationUsage &usage ) const
	{
		usage.addKeyboardMouseBinding( "q or ESC", "Quit the MPV" );
		usage.addKeyboardMouseBinding( "SPACE", "Force transition from Standby to Debug mode" );
	}

private:
	PluginUserInputMgrOSGGA *plugin;
};

} // namespace



EXPORT_DYNAMIC_CLASS( PluginUserInputMgrOSGGA )

// ================================================
// PluginUserInputMgrOSGGA
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginUserInputMgrOSGGA::PluginUserInputMgrOSGGA() : Plugin() 
{
	name_ = "PluginUserInputMgrOSGGA";
	licenseInfo_.setLicense( LicenseInfo::LicenseLGPL );
	licenseInfo_.setOrigin( "AndrewSampson" );
	
	userRequestedQuit = false;
	userRequestedDebug = false;
	
	GUIEventHandlerList.push_back( new UserInputMgrOSGGA::KeyboardHandler( this ) );
}


// ================================================
// ~PluginUserInputMgrOSGGA
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginUserInputMgrOSGGA::~PluginUserInputMgrOSGGA() throw() 
{
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginUserInputMgrOSGGA::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{
	
	case SystemState::BlackboardPost:
		// This state is for posting things to the blackboard and other 
		// initialization tasks
		
		// post GUIEventHandlerList and GUIEventList to the blackboard
		bb_->put( "GUIEventHandlerList", &GUIEventHandlerList );
		bb_->put( "GUIEventList", &GUIEventList );

		break;

	case SystemState::Standby:
	case SystemState::Operate:
	case SystemState::Debug:
		{
		std::list< osg::ref_ptr<osgGA::GUIEventHandler> >::iterator handler;
		std::list< osg::ref_ptr<osgGA::GUIEventAdapter> >::iterator event;
		for( event = GUIEventList.begin(); event != GUIEventList.end(); event++ )
		{
			// Note that I'm intentionally ignoring the return value from the 
			// handle() call.  Normally, the return value is supposed to indicate 
			// whether the event has been handled and should not be made available 
			// to other event handlers.  I want every event to be available to 
			// every handler.
			handler = GUIEventHandlerList.begin();
			for( ; handler != GUIEventHandlerList.end(); handler++ )
			{
				(*handler)->handle( **event, dummyActionAdapter );
			}
		}

		GUIEventList.clear();
		}
		break;

	case SystemState::Shutdown:
		GUIEventHandlerList.clear();
		GUIEventList.clear();
		break;

	default:
		break;
	}
	
	if( userRequestedQuit )
	{
		stateContext.userRequestedQuit = true;
	}
	else if( userRequestedDebug && state == SystemState::Standby )
	{
		stateContext.userRequestedDebug = true;
		userRequestedDebug = false;
	}
}

