#include <stdio.h>
#include <iostream>

#ifdef WIN32
#define snprintf _snprintf
#endif

#include <osg/Switch>
#include <osg/StateSet>
#include <osg/Light>
#include <osg/Geode>
#include <osg/Depth>


#include "EntityScreen.h"

using namespace mpv;
using namespace mpvosg;

EntityScreen::EntityScreen() :
	OverlayScreen()
{
	tableText = NULL;
}


EntityScreen::~EntityScreen()
{
}


void EntityScreen::resetView( const View *viewParams )
{
	projectionMatrix = osg::Matrix::ortho2D( 0.0, 1.0, 0.0, 1.0 );
}


void EntityScreen::act( double deltaT )
{
	std::map<int,std::pair<int,std::string> > typeCountMap;
	
	
	char tempText[256];
	
	if( allEntities.valid() && geode.valid() )
	{
		std::string tableTextString;
		
		// build up the entity type counts
		EntityContainer::EntityIteratorPair iterators = allEntities->getEntities();
		for( ; iterators.first != iterators.second; iterators.first++ )
		{
			Entity *entity = (*iterators.first).second.get();

			std::pair<int,std::string> &entry = typeCountMap[entity->getType()];
			entry.first++;
			if( entry.second.empty() )
			{
				entry.second = entity->getName();
			}
		}
		
		// concatenate the contents of typeCountMap into tableTextString
		std::map<int,std::pair<int,std::string> >::iterator mapIter = typeCountMap.begin();
		for( ; mapIter != typeCountMap.end(); mapIter++ )
		{
			snprintf( tempText, 255, "%-5i ", mapIter->second.first );
			tableTextString.append( tempText );
			if( mapIter->second.second.empty() )
			{
				snprintf( tempText, 255, "Type %i", mapIter->first );
				tableTextString.append( tempText );
			}
			else
				tableTextString.append( mapIter->second.second );
			tableTextString.append( "\n" );
		}
		
		tableText->setText( tableTextString.c_str() );
	}
}


osg::Node *EntityScreen::getNode()
{
	if( !geode.valid() )
		createTextBox();
	return geode.get();
}


// ================================================
// createTextBox
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void EntityScreen::createTextBox()
{
	geode = new osg::Geode();
	
	// turn lighting off for the text
	osg::StateSet* stateset = geode->getOrCreateStateSet();
	stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
	
	// ensure that the text is always on top
#if 0
	// to ensure the text appears on top we can either use osg::Depth to force 
	// the depth fragments to be placed at the front of the screen...
	stateset->setAttribute(new osg::Depth(osg::Depth::LESS,0.0,0.0001));
#else
	// ...or disable depth test, and make sure that the text is drawn after 
	// everything else so that it always appears on top.
	stateset->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
	stateset->setRenderBinDetails(11,"RenderBin");
#endif

	osgText::Text *headerText = new osgText::Text;
	headerText->setUseDisplayList( false );
	geode->addDrawable( headerText );
	headerText->setFont( 0 );
	headerText->setCharacterSize( 0.02 );
	headerText->setPosition( osg::Vec3( 0.05, 0.90, 0.0 ) );
	headerText->setAlignment( osgText::Text::LEFT_BASE_LINE );
	headerText->setText( "Count Type" );
	
	tableText = new osgText::Text;
	tableText->setUseDisplayList( false );
	geode->addDrawable( tableText );
	tableText->setFont( 0 );
	tableText->setCharacterSize( 0.02 );
	tableText->setPosition( osg::Vec3( 0.05, 0.85, 0.0 ) );
	tableText->setAlignment( osgText::Text::LEFT_BASE_LINE );
	tableText->setText( "foo" );
	
}


