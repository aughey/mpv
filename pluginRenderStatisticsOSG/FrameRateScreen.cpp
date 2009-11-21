#include <stdio.h>
#include <string.h>
#include <iostream>

#ifdef WIN32
#define snprintf _snprintf
#endif

#include <osg/Switch>
#include <osg/StateSet>
#include <osg/Geometry>
#include <osg/Light>
#include <osg/Geode>
#include <osg/Depth>


#include "FrameRateScreen.h"


#define STAT_HISTORY_LENGTH 100
/* don't set this to anything much larger than 0.9... the max-fps text 
won't fit on the screen */
#define STAT_BOX_WIDTH 0.9
#define STAT_BOX_HEIGHT 0.9

#define STAT_BOX_MAX_FPS 240.0
#define STAT_BOX_MAX_TIME_PER_FRAME 0.1


using namespace mpv;
using namespace mpvosg;

FrameRateScreen::FrameRateScreen() :
	OverlayScreen()
{
	frameRateHistory = new float[STAT_HISTORY_LENGTH];
	memset( frameRateHistory, 0, sizeof(frameRateHistory) );
	frameRateHistoryPosition = 0;
	
	largestFPSText = NULL;
	currentFPSText = NULL;

	vertArray = NULL;
}


FrameRateScreen::~FrameRateScreen()
{
	delete [] frameRateHistory;
}


void FrameRateScreen::resetView( const View *viewParams )
{
	projectionMatrix = osg::Matrix::ortho2D( 0.0, 1.0, 0.0, 1.0 );
}


void FrameRateScreen::act( double deltaT )
{
	updateHistoryGraphFPS( deltaT );
//	updateHistoryGraphTimeElapsed( deltaT );
}


osg::Node *FrameRateScreen::getNode()
{
	if( !geode.valid() )
		createGraph();
	return geode.get();
}


// ================================================
// createGraph
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void FrameRateScreen::createGraph()
{
	geode = new osg::Geode();
	
	// turn lighting off 
	osg::StateSet* stateset = geode->getOrCreateStateSet();
	stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

#if 0
	// to ensure the hud appears on top we can either use osg::Depth to force the 
	// depth fragments to be placed at the front of the screen.
	stateset->setAttribute(new osg::Depth(osg::Depth::LESS,0.0,0.0001));
#else
	// or disable depth test, and make sure that the hud is drawn after everything 
	// else so that it always appears ontop.
	stateset->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
	stateset->setRenderBinDetails(11,"RenderBin");
#endif

	largestFPSText = new osgText::Text;
	largestFPSText->setUseDisplayList( false );
	geode->addDrawable( largestFPSText );
	largestFPSText->setFont( 0 );
	largestFPSText->setCharacterSize( 0.02 );
	largestFPSText->setPosition( osg::Vec3( STAT_BOX_WIDTH, STAT_BOX_HEIGHT, 0.0 ) );
	largestFPSText->setText( "foo" );
	
	currentFPSText = new osgText::Text;
	currentFPSText->setUseDisplayList( false );
	geode->addDrawable( currentFPSText );
	currentFPSText->setFont( 0 );
	currentFPSText->setCharacterSize( 0.02 );
	currentFPSText->setPosition( osg::Vec3( STAT_BOX_WIDTH, STAT_BOX_HEIGHT, 0.0 ) );
	currentFPSText->setText( "foo" );
	
	osg::Geometry *historyGraph = new osg::Geometry;
	historyGraph->setUseDisplayList( false );
	geode->addDrawable( historyGraph );

	osg::Vec4Array *colorarray = new osg::Vec4Array;
	colorarray->push_back( osg::Vec4( 1.0,0,0,1 ) );
	historyGraph->setColorBinding( osg::Geometry::BIND_OVERALL );
	historyGraph->setColorArray( colorarray );
	
	vertArray = new osg::Vec3Array( STAT_HISTORY_LENGTH );
	historyGraph->setVertexArray(vertArray);
	
	for( int i = 0; i < STAT_HISTORY_LENGTH; i++ )
	{
		(*vertArray)[i].set( 0., 0., 0. );
	}
	historyGraph->addPrimitiveSet( 
		new osg::DrawArrays(
			osg::PrimitiveSet::LINE_STRIP, 0, vertArray->size() ) );
	
}


void FrameRateScreen::updateHistoryGraphFPS( float timeElapsed )
{
	// the timer can give us seconds/frame, but we want frames/second
	float frameRate = timeElapsed;
//std::cout << "framerate " << frameRate << std::endl;
	// avoid divide-by-zero
	if( frameRate > 0.0 )
		frameRate = 1.0 / frameRate;
	else
		frameRate = 0.0;

	recordNewHistoryEntry( frameRate );

	float heightMultiplier = STAT_BOX_HEIGHT / STAT_BOX_MAX_FPS;
	float xIncrement = ( 1.0 / ((float)STAT_HISTORY_LENGTH) ) * STAT_BOX_WIDTH;
	float xpos = 0.0, ypos;
	int historyIdx = frameRateHistoryPosition; 
	
	for( int i = 0; i < STAT_HISTORY_LENGTH; i++ )
	{
		ypos = frameRateHistory[historyIdx] * heightMultiplier;
		if( ypos > STAT_BOX_HEIGHT )
			ypos = STAT_BOX_HEIGHT;
		(*vertArray)[i].set( xpos, ypos, 0. );
		
//		if( frameRateHistory[historyIdx] > STAT_BOX_MAX_FPS )
//			STAT_BOX_MAX_FPS = frameRateHistory[historyIdx];
		
		xpos += xIncrement;
		
		historyIdx++;
		if( historyIdx >= STAT_HISTORY_LENGTH )
			historyIdx = 0;
	}

	char tempText[256];
	snprintf( tempText, 255, "%4.2f", STAT_BOX_MAX_FPS );
	largestFPSText->setText( tempText );
	snprintf( tempText, 255, "%4.2f", frameRate );
	currentFPSText->setText( tempText );
	
	ypos = frameRate * heightMultiplier;
	if( ypos > STAT_BOX_HEIGHT )
		ypos = STAT_BOX_HEIGHT;
	currentFPSText->setPosition( osg::Vec3( STAT_BOX_WIDTH, ypos, 0.0 ) );
}


void FrameRateScreen::updateHistoryGraphTimeElapsed( float timeElapsed )
{
	recordNewHistoryEntry( timeElapsed );

	float heightMultiplier = STAT_BOX_HEIGHT / STAT_BOX_MAX_TIME_PER_FRAME;
	float xIncrement = ( 1.0 / ((float)STAT_HISTORY_LENGTH) ) * STAT_BOX_WIDTH;
	float xpos = 0.0, ypos;
	int historyIdx = frameRateHistoryPosition; 
	
	for( int i = 0; i < STAT_HISTORY_LENGTH; i++ )
	{
		ypos = frameRateHistory[historyIdx] * heightMultiplier;
		if( ypos > STAT_BOX_HEIGHT )
			ypos = STAT_BOX_HEIGHT;
		(*vertArray)[i].set( xpos, ypos, 0. );
		
		xpos += xIncrement;
		
		historyIdx++;
		if( historyIdx >= STAT_HISTORY_LENGTH )
			historyIdx = 0;
	}

	char tempText[256];
	snprintf( tempText, 255, "%4.2f", STAT_BOX_MAX_TIME_PER_FRAME );
	largestFPSText->setText( tempText );
	snprintf( tempText, 255, "%4.2f", timeElapsed );
	currentFPSText->setText( tempText );
	
	ypos = timeElapsed * heightMultiplier;
	if( ypos > STAT_BOX_HEIGHT )
		ypos = STAT_BOX_HEIGHT;
	currentFPSText->setPosition( osg::Vec3( STAT_BOX_WIDTH, ypos, 0.0 ) );
}


void FrameRateScreen::recordNewHistoryEntry( float value )
{
	frameRateHistory[frameRateHistoryPosition] = value;
	
	frameRateHistoryPosition++;
	if( frameRateHistoryPosition >= STAT_HISTORY_LENGTH )
		frameRateHistoryPosition = 0;

}



FrameTimeScreen::FrameTimeScreen() :
	FrameRateScreen()
{
}


FrameTimeScreen::~FrameTimeScreen()
{
}


void FrameTimeScreen::act( double deltaT )
{
	updateHistoryGraphTimeElapsed( deltaT );
}
