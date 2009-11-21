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
 *  This class is responsible for creating a subgraph containing positional 
 *  audio nodes.
 *  
 *  Initial Release: 2007-04-07 Andrew Sampson
 * </pre>
 */


#include <osgAL/SoundNode>
#include <osgAL/SoundManager>
#include <osgAL/SoundState>

#include "SoundElement.h"


// a static
int SoundElement::soundStateCounter = 0;


class SoundComponentCtrl : public CompCtrl
{
public:
	
	// these values were chosen to match the values used by CIGI's 
	// entity animation state
	enum SoundControlStates
	{
		Stop = 0,
		Pause = 1,
		Play = 2//,
//		Continue = 3
	};
	
	//=========================================================
	//! Constructor
	//! 
	SoundComponentCtrl() : CompCtrl()
	{
		// set modified to false, to avoid clobbering the soundnode's 
		// initial values
		modified = false;
	}
	
	//=========================================================
	//! Destructor
	//! 
	virtual ~SoundComponentCtrl()
	{
	
	}
	
	//=========================================================
	//! Called by the entity's Act 
	//! 
	virtual void Act()
	{
		static bool didPrintWarning = false;

		// check to see if we've been modified
		if( !modified )
			return;
		modified = false;

		if( !soundNode.valid() )
			return;
		
		switch( GetState() )
		{
		case Stop:
			soundNode->getSoundState()->setStopMethod( openalpp::Stopped );
			soundNode->getSoundState()->setPlay( false );
			break;
		case Pause:
			soundNode->getSoundState()->setStopMethod( openalpp::Paused );
			soundNode->getSoundState()->setPlay( false );
			break;
		case Play:
			soundNode->getSoundState()->setPlay( true );
			break;
		default:
			if( !didPrintWarning )
				std::cout << "Warning in entity sound-component-ctrl - Host sent value " 
					<< GetState() 
					<< " for entity instance " << GetEntityID()
					<< ", \n\twhich is not a valid state for a sound control.\n" 
					<< "\tGo fix your Host software!\n"
					<< "\tThis warning will only be printed once, ever.\n";
			
			didPrintWarning = true;
			break;
		}
		
		// fixme - there really needs to be an accessor in CompCtrl for 
		// retrieving the data in float form...
		union { unsigned int theInt; float theFloat; } theUnion;

		theUnion.theInt = GetCompData( 0 );
		soundNode->getSoundState()->setGain( theUnion.theFloat );
		
		theUnion.theInt = GetCompData( 1 );
		soundNode->getSoundState()->setPitch( theUnion.theFloat );
		
	}


	//=========================================================
	//! Sets the sound node that this component is associated with
	//! \param newSoundNode - the sound node
	//! 
	void setSoundNode( osgAL::SoundNode *newSoundNode )
	{
		soundNode = newSoundNode;
	}



private:
	//=========================================================
	//! The sound node that this component ctrl is associated with
	//! 
	osg::ref_ptr<osgAL::SoundNode> soundNode;

};





SoundElement::SoundElement() : EntityElement()
{
	groupNode = new osg::Group;
}

SoundElement::~SoundElement()
{
	
}


bool SoundElement::construct( DefFileGroup *config, Entity *entity )
{
	DefFileAttrib *attr = config->getAttribute( "filename" );
	if( attr )
	{
		std::string filename = attr->asString();
		float gain = 1.0f;
		float pitch = 1.0f;
		bool shouldPlay = true;
		bool shouldLoop = true;
		int priority = 0;
		float referenceDistance = 10.0f;
		float rolloffFactor = 1.0f;
		
		attr = config->getAttribute( "gain" );
		if( attr )
			gain = attr->asFloat();

		attr = config->getAttribute( "pitch" );
		if( attr )
			pitch = attr->asFloat();

		attr = config->getAttribute( "should_play" );
		if( attr )
			shouldPlay = attr->asInt();

		attr = config->getAttribute( "should_loop" );
		if( attr )
			shouldLoop = attr->asInt();

		attr = config->getAttribute( "priority" );
		if( attr )
			priority = attr->asInt();

		attr = config->getAttribute( "reference_distance" );
		if( attr )
			referenceDistance = attr->asFloat();

		attr = config->getAttribute( "rolloff_factor" );
		if( attr )
			rolloffFactor = attr->asFloat();


		// Create a sound node
		osg::ref_ptr<osgAL::SoundNode> soundNode = createSound( 
			filename, gain, pitch, shouldPlay, shouldLoop, priority,
			referenceDistance, rolloffFactor );

		// Add the sound node
		if( soundNode.valid() )
			groupNode->addChild( soundNode.get() );
		else
		{
			std::cout << "Warning - in definition for entity type " 
				<< entity->GetEntityType()
				<< " - sound file \"" << attr->asString() 
				<< "\" couldn't be loaded\n";
			return false;
		}
		
		attr = config->getAttribute( "control_type" );
		if( attr )
		{
			std::string controlType = attr->asString();
			
			/* The user has specified that the sound should be controlled 
			by the host in some way.  The sound can be configured to 
			act like a normal animation (which will start and stop 
			according to the animation values sent by the host in entity 
			control packets).  Alternatively, the sound can be configured 
			to use component control packets to control playback.  The 
			advantage to this technique, over using the entityctrl's 
			animation state, is twofold:
			- more than one sound can be controlled independently in an entity
			- gain and pitch can be specified
			*/
			
			if( controlType == "component" )
			{
				attr = config->getAttribute( "component_id" );
				if( attr )
				{
					SoundComponentCtrl *soundCtrl = new SoundComponentCtrl;
					soundCtrl->SetCompCtrlID( attr->asInt() );
					soundCtrl->setSoundNode( soundNode.get() );
					entity->AddCompCtrl( soundCtrl );
				}
				else
				{
					std::cout << "Warning - in definition for entity type " 
						<< entity->GetEntityType()
						<< " - sound element has control_type set to \"component\", " 
						<< "but \"component_id\" not specified.\n";
				}
			}
			else if( controlType == "animation" )
			{
				// fixme - animation control for not finished for sounds
				std::cout << "Warning - in definition for entity type " 
					<< entity->GetEntityType()
					<< " - sound element has control_type set to \"" << controlType
					<< "\", which has not been implemented yet.\n";
			}
			else
			{
				std::cout << "Warning - in definition for entity type " 
					<< entity->GetEntityType()
					<< " - sound element has control_type set to \"" << controlType
					<< "\", which is not a recognized value.\n";
			}
		}

	}
	else
	{
		std::cout << "Warning - in definition for entity type " 
			<< entity->GetEntityType()
			<< " - sound element is missing its \"filename\" attribute\n";
		return false;
	}
	
	
	return true;
}


bool SoundElement::addChildElement( EntityElement *childElement )
{
	if( childElement == NULL ) return false;
	
	osg::Node *node = childElement->getTopNode();
	if( node == NULL ) return false;
	
	groupNode->addChild( node );
	
	// call parent
	return EntityElement::addChildElement( childElement );
}


osg::ref_ptr<osgAL::SoundNode> SoundElement::createSound( 
	const std::string &filename,
	float gain,
	float pitch,
	bool shouldPlay,
	bool shouldLoop,
	int priority,
	float referenceDistance,
	float rolloffFactor )
{
	// parts of this were yanked from osgal's example program -ADS

	// SoundManager implements sample caching for us... nice.
	openalpp::Sample *sample = osgAL::SoundManager::instance()->getSample( filename );

	soundStateCounter++;
	char counterString[25];
	snprintf( counterString, 24, "%i", soundStateCounter ); // I hate C++. -ADS

	// Create a sound state.
	// Note - In order to use SoundManager::addSoundState correctly, the 
	// SoundState's name MUST be unique.  Even though I don't plan on 
	// using SoundManager::findSoundState, I still need to set the state's 
	// name and call SoundManager::addSoundState.
	// This is because, for whatever reason, SoundManager::addSoundState is 
	// critical for memory management/reference counting of SoundStates.
	// This is a bizarre design decision on the part of the osgAL author, and 
	// I do not agree with it.  -ADS
	std::string stateName = filename + counterString;
	osg::ref_ptr<osgAL::SoundState> soundState = new osgAL::SoundState( stateName );

	// Let the soundstate use the sample we just created
	soundState->setSample( sample );

	// Set its gain (volume) 
	soundState->setGain( gain );

	// Set its pitch (1.0 is normal speed)
	soundState->setPitch( pitch );

	// Make it play
	soundState->setPlay( shouldPlay );

	// The sound should loop over and over again
	soundState->setLooping( shouldLoop );

	// Allocate a hardware soundsource to this soundstate 
	// (it's still unclear to me how this works -ADS)
	soundState->allocateSource( priority, false );

	soundState->setReferenceDistance( referenceDistance );
	soundState->setRolloffFactor( rolloffFactor );
	soundState->apply();

	// Add the soundstate to the sound manager
	// I don't plan on using SoundManager::findSoundState, but I still need 
	// to call addSoundState, because it is used for reference counting of 
	// SoundStates. <sigh>  -ADS
	osgAL::SoundManager::instance()->addSoundState(soundState.get());

	// Create a sound node and attach the soundstate to it.
	osg::ref_ptr<osgAL::SoundNode> sound = new osgAL::SoundNode;
	sound->setSoundState(soundState.get());

	return sound;
}


