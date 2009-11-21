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
 *  FILENAME:   PluginEphemerisModel.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  listen for ephemeris info from the host, and interpret that info 
 *  to produce useful values, such as the angle of the ambient light, etc.
 *  
 *  MODIFICATION NOTES:
 *  DATE	  NAME										  SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  12/19/2005 Andrew Sampson							  MPV_CR_DR_1
 *  Initial Release.
 *  
 *  2007-07-14 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#include <iostream>

#include <CigiCelestialCtrl.h>

#include "PluginEphemerisModel.h"

using namespace mpv;

EXPORT_DYNAMIC_CLASS( PluginEphemerisModel )

// ================================================
// PluginEphemerisModel
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginEphemerisModel::PluginEphemerisModel() : Plugin(),
	celestialSphereCtrlProc( this ) 
{
	name_ = "PluginEphemerisModel";

	// This plugin contains code from the CSP project.
	licenseInfo_.setLicense( LicenseInfo::LicenseGPL );
	licenseInfo_.setOrigin( "CSP+Boeing+AndrewSampson" );

	dependencies_.push_back( "PluginEntityMgr" );
	dependencies_.push_back( "PluginViewMgr" );

	ImsgPtr = NULL;
	
	allEntities = NULL;
	viewMap = NULL;
	
	m_LastMoonFullUpdate = -1e+10;
	m_Latitude = 0.0;
	m_LMST = 0.0;
	m_JD = 0.0;

	sunAltitude = 0.0;
	sunAzimuth = 0.0;
	skyIntensity = 0.0;
	
	moonAltitude = 0.0;
	moonAzimuth = 0.0;
	moonPhase = 0.0;
	moonEarthShine = 0.0;
	
	// sets up the time-of-day
	initializeTime();
	
	// performs some initialization in Colorspace.cpp
	initColor();
}


// ================================================
// ~PluginEphemerisModel
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginEphemerisModel::~PluginEphemerisModel() throw() 
{
}


// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginEphemerisModel::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{
	case SystemState::BlackboardPost:
		// This state is for posting things to the blackboard and other 
		// initialization tasks
		bb_->put( "SunLight", &sunLight );
		bb_->put( "MoonLight", &moonLight );

		bb_->put( "EphemerisEyepointLatitude", &m_Latitude );
		bb_->put( "EphemerisLocalMeanSiderealTime", &m_LMST );

		bb_->put( "EphemerisSunAltitude", &sunAltitude );
		bb_->put( "EphemerisSunAzimuth", &sunAzimuth );
		bb_->put( "EphemerisSkyIntensity", &skyIntensity );

		bb_->put( "EphemerisMoonAltitude", &moonAltitude );
		bb_->put( "EphemerisMoonAzimuth", &moonAzimuth );
		bb_->put( "EphemerisMoonPhase", &moonPhase );
		bb_->put( "EphemerisMoonEarthShine", &moonEarthShine );
		
		skyDomeNumWedges = m_SkyColors.getNumWedges();
		skyDomeNumSlices = m_SkyColors.getNumSlices();
		bb_->put( "SkyDomeNumWedges", &skyDomeNumWedges );
		bb_->put( "SkyDomeNumSlices", &skyDomeNumSlices );
		bb_->put( "SkyDomeSliceAngles", m_SkyColors.getSliceAngleArray() );
		bb_->put( "SkyDomeColors", m_SkyColors.getColorArray() );
		
		break;

	case SystemState::BlackboardRetrieve:
		// This state is for retrieving things from the blackboard

		bb_->get( "CigiIncomingMsg", ImsgPtr );

		bb_->get( "AllEntities", allEntities );

		// get the view parameters
		bb_->get( "ViewMap", viewMap );
		
		if( ImsgPtr != NULL )
		{
			ImsgPtr->RegisterEventProcessor(
				CIGI_CELESTIAL_CTRL_PACKET_ID_V3,
				(CigiBaseEventProcessor *) &celestialSphereCtrlProc);
		}

		break;

	case SystemState::Operate:
	case SystemState::Debug:
		{
		simulationTime.update();
		CoordSet where = getOwnshipLocation();

		updateEphemeris( 
			degreesToRadians(where.LatX), degreesToRadians(where.LonY), 
			simulationTime );

		sunLight = m_Sun.getLight();
		moonLight = m_Moon.getLight();

		skyIntensity = m_SkyColors.getAverageIntensity();

		moonAltitude = m_Moon.getAltitude();
		moonAzimuth = m_Moon.getAzimuth();
		moonPhase = m_Moon.getPhase();
		moonEarthShine = m_Moon.getEarthShine();
		}
		break;

	default:
		break;
	}
	
}


// ================================================
// initializeTime
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginEphemerisModel::initializeTime()
{
	// The interface for the simdata time classes sucks.  You can't just 
	// set the time/date to "now" in one step.  You have to first set the 
	// time to the unix epoch, and then add the number of seconds since the 
	// epoch to it.
	simulationTime = simdata::SimDate( 1970, 1, 1, 0, 0, 0 );
	// Note: setTime and addTime actually do the same thing; one has support 
	// for timezones and the other does not.  The MPV uses UTC time, so it 
	// doesn't matter which call is used.
	simulationTime.addTime( simdata::SimDate::getSystemTime() );
	
}


// ================================================
// setTime
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginEphemerisModel::setTime( int y, int mon, int d, 
	int h, int min, int s )
{
	simulationTime = simdata::SimDate( y, mon, d, h, min, s );
}


// ================================================
// freezeTime
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginEphemerisModel::freezeTime( bool freeze )
{
	if( freeze )
		simulationTime.pause();
	else
		simulationTime.unpause();
}


/*
// ================================================
// getDateAndTime
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
simdata::SimDate PluginEphemerisModel::getDateAndTime( void )
{
	simdata::SimDate result( 1970, 1, 1, 0, 0, 0 );
	result.setTime( simdata::SimDate::getSystemTime() );
	
//std::cout << "Simulation Time is: " << result.asString() << std::endl;
	return result;
}*/


// ================================================
// getOwnshipLocation
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CoordSet PluginEphemerisModel::getOwnshipLocation( void )
{
	CoordSet result;
	
	// St. Louis :)
	result.LatX = 38.624671;
	result.LonY = -90.184963;

	// No attempt is made to find out the real lat/lon of child entities; the 
	// lat/lon of the parent is used instead.  Let's just hope that the 
	// discrepency never becomes noticable.  (The child would have to be a 
	// timezone or so away from its parent for this to be noticable :)
	
	// Note also that the attached entity for the *first* view is the entity 
	// that will be used here.  If there is more than one view, and those views 
	// are attached to different entities (hence there is more than one 
	// "ownship", if you will), then this too could create a discrepancy.
	// Of course, there are other (more noticable) problems with having two 
	// views that have been separated by a great distance (the terrain pager 
	// being one example).
	
	// find the first view
	std::map< int, RefPtr<View> >::iterator viewIter = viewMap->begin();
	if( viewIter != viewMap->end() )
	{
		View *view = viewIter->second.get();
		Entity *entity = allEntities->findEntity( view->getEntityID() );
		
		// find the top-level parent
		if( entity )
			entity = entity->getTopLevelParent();
		
		if( entity )
		{
			result = entity->getPositionGDC();
		}
	}
	
	return result;
}


// ================================================
// updateEphemeris
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginEphemerisModel::updateEphemeris( 
	double lat, double lon, simdata::SimDate const &t ) 
{
	m_LMST = t.getMST(lon);
	m_JD = t.getJulianDate();
	m_Latitude = lat;
	
	// update the sky colors
	updateSkyColors();
	
	// update the sun
	updateSun();
	
	// update the moon
	updateMoon();
	
	// update the stars
	updateStars();
}


// ================================================
// updateSkyColors
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginEphemerisModel::updateSkyColors()
{
	m_SkyColors.update( sunAltitude, sunAzimuth, m_Moon.getApparentBrightness() );
}


// ================================================
// updateSun
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginEphemerisModel::updateSun()
{
	m_Sun.updatePosition( m_JD );
	m_Sun.toObserver( m_Latitude, m_LMST, sunAltitude, sunAzimuth );

	m_Sun.updateScene( 
		sunAltitude, sunAzimuth, 
		m_SkyColors.getZenithColor(), m_SkyColors.getZenithIntensity(), 
		m_SkyColors.getAverageIntensity() );
}


// ================================================
// updateMoon
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginEphemerisModel::updateMoon()
{
//FIXME - does this hurt determinism? -ADS
	// do a full moon update every 300 seconds (orbital position and lighting)
	if( fabs( m_JD - m_LastMoonFullUpdate ) > 0.0000578705 ) // = 1.15741e-5 * 5.0 // 300.0) {
	{
		m_LastMoonFullUpdate = m_JD;
		m_Moon.updatePosition( m_JD );
	}
	m_Moon.updateScene( 
		m_Latitude, m_LMST, m_Sun.getLambda(), 
		m_SkyColors.getAverageIntensity() );
}


// ================================================
// updateStars
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginEphemerisModel::updateStars()
{
//FIXME - not done... all this will do, however, is calculate the rotation for the star dome -ADS
}



// ================================================
// CelestialSphereCtrlP::OnPacketReceived
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginEphemerisModel::CelestialSphereCtrlP::OnPacketReceived( CigiBasePacket *Packet )
{
	CigiCelestialCtrlV3 *celestialCtrl = (CigiCelestialCtrlV3*)Packet;
	
	plugin->freezeTime( !celestialCtrl->GetEphemerisEn() );
	
	if( celestialCtrl->GetDateVld() )
		plugin->setTime( 
			celestialCtrl->GetYear(),
			celestialCtrl->GetMonth(),
			celestialCtrl->GetDay(),
			celestialCtrl->GetHour(),
			celestialCtrl->GetMinute(),
			0 );
}


