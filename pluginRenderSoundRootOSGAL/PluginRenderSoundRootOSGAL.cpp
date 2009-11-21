/** <pre>
 *  Plugin for the MPV to add positional audio to the scene
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
 *  Initial Release: 2007-04-07 Andrew Sampson
 *
 *  2007-07-21 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 * </pre>
 */


#include <iostream>
#include <osgAL/SoundManager>

#include "PluginRenderSoundRootOSGAL.h"



EXPORT_DYNAMIC_CLASS( PluginRenderSoundRootOSGAL )

// ================================================
// PluginRenderSoundRootOSGAL
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderSoundRootOSGAL::PluginRenderSoundRootOSGAL() : Plugin() 
{
	name_ = "PluginRenderSoundRootOSGAL";

	licenseInfo_.setLicense( LicenseInfo::LicenseLGPL );
	licenseInfo_.setOrigin( "AndrewSampson" );

	dependencies_.push_back( "PluginRenderOSG" );
	dependencies_.push_back( "PluginCameraMgrOSG" );

	cameraMatrixMap = NULL;
	
//	soundRoot = new osgAL::SoundRoot;
//	soundRoot->setName( "Sound Root Node" );
	
	// Initialize the sound manager
	// first arg for init() is the number of sound sources (This may have 
	// something to do with the hardware's limit on the number of sounds 
	// that can be played simultaneously.  If that's the case, I don't 
	// understand why the application needs to provide this information.  
	// Can't the hardware/driver be queried for this information?)
	osgAL::SoundManager::instance()->init( 16 );
	// used in attenuation calculations
	osgAL::SoundManager::instance()->getEnvironment()->setDistanceModel( openalpp::InverseDistance );
	// this value can be used to exaggerate the doppler effect
	osgAL::SoundManager::instance()->getEnvironment()->setDopplerFactor( 1 );

}


// ================================================
// ~PluginRenderSoundRootOSGAL
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginRenderSoundRootOSGAL::~PluginRenderSoundRootOSGAL() throw() 
{
	osgAL::SoundManager::instance()->shutdown();
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginRenderSoundRootOSGAL::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{

	case SystemState::BlackboardRetrieve:
		// This state is for retrieving things from the blackboard
		
		bb_->get( "CameraMatrixMap", cameraMatrixMap );
		
		{
		osg::Group *rootNode = NULL;
		bb_->get( "RootNodeOSG", rootNode );
//		rootNode->addChild( soundRoot.get() );
		}

		break;

	case SystemState::Operate:
	case SystemState::Debug:
		{
		// Update the transform for the listener.  We'll use the matrix for the 
		// first view in the cameraMatrixMap; if the MPV is currently rendering 
		// more than one view, then tough luck.  It isn't possible to have more 
		// than one listener location with OpenAL/OSGAL.
		std::map<int, osg::Matrix>::iterator iter = cameraMatrixMap->begin();
		if( iter != cameraMatrixMap->end() )
		{
			// need to use the inverse of the camera matrix... 
	//		osg::Matrix mtx = osg::Matrix::inverse( iter->second );
			osgAL::SoundManager::instance()->setListenerMatrix( iter->second );
		}
		osgAL::SoundManager::instance()->update();
		}
		break;

	default:
		break;
	}
	
}

