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
 *  FILENAME:   PluginRenderOSG.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  set up the OSG scene root node and post it to the blackboard.
 * 
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  03/29/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 *
 *  2007-07-15 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#include <iostream>

#include "PluginRenderOSG.h"
#include "SystemState.h"


void analyzeScene( osg::Node *node, int level=0 ) ;
void analyzeScene( osg::Node *node, int level ) 
{

	std::string nodeName = node->getName();
	std::string className( node->className() );
	
	for( int i = 0; i < level; i++ ) {
		printf( "\t" );
	}
	printf( "%s%s%s%s Address %p\n", 
		"Name:\t", nodeName.c_str(), "\tClass:\t", className.c_str(),
		(void *)node );
	
	osg::Group *group = node->asGroup();
	if( group ) {
		int numChildren = group->getNumChildren();
		for( int i = 0; i < numChildren; i++ ) {
			analyzeScene( group->getChild( i ), level + 1 );
		}
	}
	
}



EXPORT_DYNAMIC_CLASS( PluginRenderOSG )

// ================================================
// PluginRenderOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderOSG::PluginRenderOSG() : Plugin() 
{
	name_ = "PluginRenderOSG";
	licenseInfo_.setLicense( LicenseInfo::LicenseGPL );
	licenseInfo_.setOrigin( "Boeing" );
	rootNode = new osg::Group;
	rootNode->setName( "Root Node" );
	
	// Thread-safe reference counting is needed by any plugin that 
	// uses OSG and threads.  Shame on the OSG developers for not 
	// enabling this by default.
	osg::Referenced::setThreadSafeReferenceCounting( true );
}


// ================================================
// ~PluginRenderOSG
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderOSG::~PluginRenderOSG() throw() 
{
	// when the rootNode ref_ptr falls out of scope (ie after this object is 
	// deleted) the node it points to will automatically be deleted.

}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderOSG::act( SystemState::ID state, StateContext &stateContext )
{
	// The RenderOSG plugin doesn't do much... manipulation of the scene 
	// graph is done by other plugins (such as the terrain and entity plugins) 
	// and rendering itself is actually performed by the camera plugin.

	switch( state )
	{

	case SystemState::BlackboardPost:
		// This state is for posting things to the blackboard and other 
		// initialization tasks
		bb_->put( "RootNodeOSG", rootNode.get() );

		break;

	case SystemState::Shutdown:
		// It is vitally important that all of the children of the root node 
		// are detached at this point.  If we could ensure that this plugin's 
		// destructor was called first, we wouldn't need this piece of code.
		// Check out the README.renderPluginGotchas file for a complete 
		// explanation.
		rootNode->removeChild( 0, rootNode->getNumChildren() );
		break;

	default:
		break;
	}
	
}

