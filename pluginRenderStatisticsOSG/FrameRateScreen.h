/*




*/

#ifndef FRAME_RATE_SCREEN_H
#define FRAME_RATE_SCREEN_H

#include <osg/Geode>
#include <osgText/Text>

#include "OverlayScreen.h"

class FrameRateScreen : public mpvosg::OverlayScreen
{
public:

	FrameRateScreen();
	virtual ~FrameRateScreen();
	
	virtual void resetView( const mpv::View *viewParams );
	virtual void act( double deltaT );
	
	virtual osg::Node *getNode();
	
	
protected:

	//=========================================================
	//! 
	//! 
	osg::ref_ptr<osg::Geode> geode;
	
	float *frameRateHistory;
	int frameRateHistoryPosition;
	
	osgText::Text *largestFPSText;
	osgText::Text *currentFPSText;
	osg::Vec3Array* vertArray;
	
	void createGraph();
	void updateHistoryGraphFPS( float timeElapsed );
	void updateHistoryGraphTimeElapsed( float timeElapsed );
	void recordNewHistoryEntry( float value );

};

class FrameTimeScreen : public FrameRateScreen
{
public:

	FrameTimeScreen();
	virtual ~FrameTimeScreen();
	
	virtual void act( double deltaT );
};

#endif

