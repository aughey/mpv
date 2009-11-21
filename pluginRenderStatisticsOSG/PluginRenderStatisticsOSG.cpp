/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2004 The Boeing Company
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
 *  FILENAME:   PluginRenderStatisticsOSG.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  track and display various performance statistics.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  07/06/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 *
 *  2007-07-15 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#include <stdio.h>
#include <ios>
#include <iostream>
#include <math.h>
#include <algorithm>

#ifdef WIN32
#define snprintf _snprintf
#endif

#include <osg/Switch>
#include <osg/StateSet>

#include <osg/Projection>
#include <osg/MatrixTransform>


#include "PluginRenderStatisticsOSG.h"

#include "FrameRateScreen.h"
#include "EntityScreen.h"
#include "SolidColorScreen.h"
#include "GridColorScreen.h"
#include "CheckeredColorScreen.h"
#include "GradientColorScreen.h"
#include "GridLinesScreen.h"

using namespace mpv;
using namespace mpvosg;

namespace RenderStatistics
{

// KEYBOARD HANDLER ///////////////////////////////////////////////////

class KeyboardHandler: public osgGA::GUIEventHandler
{
public:
	KeyboardHandler( PluginRenderStatisticsOSG *plug )
	{
		plugin = plug;
	}

	bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &)
	{
		if( plugin == NULL )
			return false;
		
		if( ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN )
		{
			bool didHandleEvent = false;
			// the paranoid in me doesn't want to make the assumption 
			// that KEY_F1 through KEY_F12 have seqential values... hence the 
			// switch statement below instead of something simple like 
			// cycleThroughScreen( key - osgGA::GUIEventAdapter::KEY_F1 )
			switch( ea.getKey() )
			{
			case osgGA::GUIEventAdapter::KEY_F1:
				plugin->cycleThroughScreen( 1 );
				didHandleEvent = true;
				break;
			case osgGA::GUIEventAdapter::KEY_F2:
				plugin->cycleThroughScreen( 2 );
				didHandleEvent = true;
				break;
			case osgGA::GUIEventAdapter::KEY_F3:
				plugin->cycleThroughScreen( 3 );
				didHandleEvent = true;
				break;
			case osgGA::GUIEventAdapter::KEY_F4:
				plugin->cycleThroughScreen( 4 );
				didHandleEvent = true;
				break;
			case osgGA::GUIEventAdapter::KEY_F5:
				plugin->cycleThroughScreen( 5 );
				didHandleEvent = true;
				break;
			case osgGA::GUIEventAdapter::KEY_F6:
				plugin->cycleThroughScreen( 6 );
				didHandleEvent = true;
				break;
			case osgGA::GUIEventAdapter::KEY_F7:
				plugin->cycleThroughScreen( 7 );
				didHandleEvent = true;
				break;
			case osgGA::GUIEventAdapter::KEY_F8:
				plugin->cycleThroughScreen( 8 );
				didHandleEvent = true;
				break;
			case osgGA::GUIEventAdapter::KEY_F9:
				plugin->cycleThroughScreen( 9 );
				didHandleEvent = true;
				break;
			case osgGA::GUIEventAdapter::KEY_F10:
				plugin->cycleThroughScreen( 10 );
				didHandleEvent = true;
				break;
			case osgGA::GUIEventAdapter::KEY_F11:
				plugin->cycleThroughScreen( 11 );
				didHandleEvent = true;
				break;
			case osgGA::GUIEventAdapter::KEY_F12:
				plugin->cycleThroughScreen( 12 );
				didHandleEvent = true;
				break;
			default:
				break;
			}
			
			if( didHandleEvent )
				return true;
		}

		return false;
	}

	virtual void getUsage( osg::ApplicationUsage &usage ) const
	{
		usage.addKeyboardMouseBinding( "F1", "Display keyboard shortcuts" );
		usage.addKeyboardMouseBinding( "F2 - F12", "Display various statistics and test patterns" );
	}

private:
	PluginRenderStatisticsOSG *plugin;
};


EXPORT_DYNAMIC_CLASS( PluginRenderStatisticsOSG )

// ================================================
// PluginRenderStatisticsOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderStatisticsOSG::PluginRenderStatisticsOSG() : Plugin() 
{
	name_ = "PluginRenderStatisticsOSG";
	licenseInfo_.setLicense( LicenseInfo::LicenseGPL );
	licenseInfo_.setOrigin( "Boeing" );

	dependencies_.push_back( "PluginViewMgr" );
	dependencies_.push_back( "PluginRenderOSG" );
	dependencies_.push_back( "PluginUserInputMgrOSGGA" );
	// libpluginEntityMgr is optional

	rootNode = NULL;
	eventHandlerList = NULL;

	changedScreen = false;
	currentScreen = NULL;

	overlayScreens[2].push_back( new FrameRateScreen() );
	overlayScreens[2].push_back( new FrameTimeScreen() );

	GridColorScreen *gridScreen = new GridColorScreen;
	gridScreen->setNumGridCellsWide( 3 );
	gridScreen->addColor( osg::Vec4(1,0,0,1) );
	gridScreen->addColor( osg::Vec4(0,1,0,1) );
	gridScreen->addColor( osg::Vec4(0,0,1,1) );
	overlayScreens[9].push_back( gridScreen );
	overlayScreens[9].push_back( new SolidColorScreen( osg::Vec4(1,0,0,1) ) );
	overlayScreens[9].push_back( new SolidColorScreen( osg::Vec4(0,1,0,1) ) );
	overlayScreens[9].push_back( new SolidColorScreen( osg::Vec4(0,0,1,1) ) );
	overlayScreens[9].push_back( new SolidColorScreen( osg::Vec4(1,1,1,1) ) );
	

	overlayScreens[10].push_back( new GradientColorScreen( osg::Vec4(0,0,0,1), osg::Vec4(1,1,1,1), GradientColorScreen::Vertical ) );
	overlayScreens[10].push_back( new GradientColorScreen( osg::Vec4(0,0,0,1), osg::Vec4(1,1,1,1), GradientColorScreen::Horizontal ) );

	// white, followed by 4 shades of grey
	osg::Vec3 grey( 1, 1, 1 );
	int i;
	for( i = 0; i < 5; i++ )
	{
		overlayScreens[10].push_back( new SolidColorScreen( osg::Vec4(grey, 1) ) );
		grey = grey * 0.5;
	}
	// black
	overlayScreens[10].push_back( new SolidColorScreen( osg::Vec4(0, 0, 0, 1) ) );

	// 4:3 checkerboard
	overlayScreens[10].push_back( new CheckeredColorScreen( 
		osg::Vec4(1,1,1,1), osg::Vec4(0,0,0,1), 4, 3 ) );

	// 16:9 checkerboard
	overlayScreens[10].push_back( new CheckeredColorScreen( 
		osg::Vec4(1,1,1,1), osg::Vec4(0,0,0,1), 8, 4 ) );
	
	overlayScreens[11].push_back( new GridLinesScreen() );
}


// ================================================
// ~PluginRenderStatisticsOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderStatisticsOSG::~PluginRenderStatisticsOSG() throw() 
{
	deleteOverlayScreens();
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderStatisticsOSG::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{

	case SystemState::BlackboardRetrieve:
		// This state is for retrieving things from the blackboard

		// get the root node from the BB
		bb_->get( "RootNodeOSG", rootNode );

		boxBranchNode = new osg::Group;
		boxBranchNode->setName( "Statistics-Box Branch Node" );
		rootNode->addChild( boxBranchNode.get() );

		bb_->get( "TimeElapsedLastFrame", timeElapsedLastFrame );
		
		// get the list of user input callbacks
		bb_->get( "GUIEventHandlerList", eventHandlerList );
		
		if( eventHandlerList )
		{
			eventHandlerList->push_back( new KeyboardHandler( this ) );
		}
		
		{
		EntityContainer *allEntities = NULL;
		if( bb_->get( "AllEntities", allEntities, false ) )
		{
			EntityScreen *entityScreen = new EntityScreen;
			entityScreen->setEntityList( allEntities );
			overlayScreens[3].push_back( entityScreen );
		}
		}
		
		break;

	case SystemState::Operate:
	case SystemState::Debug:
		
		// if currentScreen is new, or if viewparams have been changed, 
		// call resetView and createStatisticsBox again
//fixme - check if viewparams changed
		if( changedScreen )
		{
			if( currentScreen != NULL )
				currentScreen->resetView( NULL /*FIXME*/ );
			createStatisticsBox();

			changedScreen = false;
		}

		if( currentScreen )
		{
			currentScreen->act( fabs( *timeElapsedLastFrame ) );
		}
		break;

	default:
		break;
	}
	
}


void PluginRenderStatisticsOSG::createStatisticsBox()
{
	// remove all the nodes under boxBranchNode
	boxBranchNode->removeChild( 0, boxBranchNode->getNumChildren() );
	
	if( currentScreen != NULL )
	{
		osg::MatrixTransform* modelview_abs = new osg::MatrixTransform;
		modelview_abs->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
		modelview_abs->setMatrix( osg::Matrix::identity() );
		modelview_abs->addChild( currentScreen->getNode() );

		osg::Projection* projection = new osg::Projection;
		projection->setMatrix( currentScreen->getProjectionMatrix() );
		projection->addChild( modelview_abs );

		boxBranchNode->addChild( projection );
	}
}


void PluginRenderStatisticsOSG::cycleThroughScreen( int screenSet )
{
	changedScreen = true;

	std::map< int, std::list<OverlayScreen *> >::iterator mapIter;
	mapIter = overlayScreens.find( screenSet );
	if( mapIter == overlayScreens.end() )
	{
		currentScreen = NULL;
		return;
	}
	
	std::list<OverlayScreen *> &screenList = mapIter->second;
	if( screenList.empty() )
	{
		currentScreen = NULL;
		return;
	}
	
	if( currentScreen == NULL )
	{
		// no screen is currently set.  pick the first screen on the 
		// selected screen list.
		currentScreen = *screenList.begin();
	}
	else
	{
		// check to see if currentScreen appears on the selected screen list
		std::list<OverlayScreen *>::iterator currentScreenIter = 
			std::find( screenList.begin(), screenList.end(), currentScreen );
		if( currentScreenIter == screenList.end() )
		{
			// currentScreen isn't a member of the selected screen list.  
			// pick the first screen on the selected screen list.
			currentScreen = *screenList.begin();
		}
		else
		{
			// currentScreen is a member of the selected screen list.  
			// pick the next screen on the selected screen list.
			
			currentScreenIter++;
			if( currentScreenIter == screenList.end() )
			{
				// currentScreen was the last member of the selected screen list
				// Rather than wrapping around to the beginning of the list, 
				// we will set the currentScreen to NULL.  This will produce 
				// the following sequence when the user repeatedly presses 
				// the same screen-selection key:
				// none -> screen_N.1 -> screen_N.2 -> ... -> screen_N.M -> none
				currentScreen = NULL;
			}
			else
			{
				currentScreen = *currentScreenIter;
			}
		}
	}
}


void PluginRenderStatisticsOSG::deleteOverlayScreens()
{
	std::map< int, std::list<OverlayScreen *> >::iterator mapIter;
	std::list<OverlayScreen *>::iterator listIter;
	for( mapIter = overlayScreens.begin(); mapIter != overlayScreens.end(); mapIter++ )
	{
		for( listIter = mapIter->second.begin(); listIter != mapIter->second.end(); listIter++ )
		{
			if( *listIter != NULL )
				delete *listIter;
		}
	}
	overlayScreens.clear();
	currentScreen = NULL;
	changedScreen = true;
}


}
