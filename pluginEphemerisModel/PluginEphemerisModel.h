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
 *  FILENAME:	PluginEphemerisModel.h
 *  LANGUAGE:	C++
 *  CLASS:		UNCLASSIFIED
 *  PROJECT:	 Multi-Purpose Viewer
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


#ifndef PLUGIN_EPHEMERISMODEL_H
#define PLUGIN_EPHEMERISMODEL_H

#include <string>
#include <iostream>

#include "RefPtr.h"
#include "Plugin.h"
#include "AllCigi.h"
#include "CoordSet.h"
#include "Entity.h"
#include "View.h"

#include "Date.h"
#include "SkyColors.h"
#include "Sun.h"
#include "Moon.h"


//=========================================================
//! The class encapsulating the EphemerisModel plugin.
//!
class PluginEphemerisModel : public Plugin 
{
public:

	//=========================================================
	//! General Constructor
	//!
	PluginEphemerisModel();

	//=========================================================
	//! General Destructor
	//!
	virtual ~PluginEphemerisModel() throw();

	//=========================================================
	//! The per-frame processing that this plugin performs.
	//! This plugin's act() will perform lighting calculations based 
	//! on the time of day, the date, and the ownship's location.
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );
	
private:
	

	//=========================================================
	//! The Cigi incoming message object.  Retrieved from the blackboard.
	//!
	CigiIncomingMsg *ImsgPtr;
	
	//=========================================================
	//! An entity container containing all active entities. 
	//! This data is necessary for finding out 
	//! the lat/lon of the attached entity (ownship).  
	//! Retrieved from the blackboard.
	//! 
	mpv::EntityContainer *allEntities;

	//=========================================================
	//! View parameters.  This data is neccessary for finding out which entity 
	//! the camera is attached to.  Retrieved from the blackboard.
	//!
	std::map< int, mpv::RefPtr<mpv::View> > *viewMap;
	
	//=========================================================
	//! Lighting parameters from sun.  Posted to the blackboard.
	//!
	MPVLight sunLight;
	
	//=========================================================
	//! Lighting parameters from moon.  Posted to the blackboard.
	//!
	MPVLight moonLight;
	
	//=========================================================
	//! Local Mean Sidereal Time.  Units: unknown.  
	//! Posted to the blackboard.
	//!
	double m_LMST;
	
	//=========================================================
	//! Altitude of the sun in the sky.  Units: radians.  
	//! Posted to the blackboard.
	//!
	double m_Latitude;
	
	//=========================================================
	//! Altitude of the sun in the sky.  Units: radians.  
	//! Posted to the blackboard.
	//!
	double sunAltitude;
	
	//=========================================================
	//! Azimuth of the sun in the sky.  Units: radians.  
	//! Posted to the blackboard.
	//!
	double sunAzimuth;
	
	//=========================================================
	//! Average light intensity of the sky dome.  Units: unknown.  
	//! Posted to the blackboard.
	//!
	float skyIntensity;
	
	//=========================================================
	//! Altitude of the moon in the sky.  Units: radians.  
	//! Posted to the blackboard.
	//!
	double moonAltitude;
	
	//=========================================================
	//! Azimuth of the moon in the sky.  Units: radians.  
	//! Posted to the blackboard.
	//!
	double moonAzimuth;
	
	//=========================================================
	//! The phase of the moon.  Units: unknown.  
	//! Posted to the blackboard.
	//!
	double moonPhase;
	
	//=========================================================
	//! The amount of light reflected off of the earth toward the moon, 
	//! then reflected back toward the earth.  Only noticable on the 
	//! dark parts of the moon, not illuminated directly by the sun.
	//! Units: unknown.  Posted to the blackboard.
	//!
	float moonEarthShine;
	
	//=========================================================
	//! The number of color samples around the perimeter of the sky dome. 
	//! This information is needed in order to make use of the sky dome 
	//! color array.  Posted to the blackboard.
	//!
	int skyDomeNumWedges;
	
	//=========================================================
	//! The number of color samples along the vertical axis of the sky dome. 
	//! This information is needed in order to make use of the sky dome 
	//! color array.  Posted to the blackboard.
	//!
	int skyDomeNumSlices;
	

	SkyColors m_SkyColors;
	Sun m_Sun;
	Moon m_Moon;

	//=========================================================
	//! The current simulation time, in Julian Date form.  Units: unknown.  
	//!
	double m_JD;

	//=========================================================
	//! The time delta since the last time the moon position was updated, 
	//! in Julian Date form.  Units: unknown.  
	//!
	double m_LastMoonFullUpdate;
	
	
	//=========================================================
	//! The current simulation time.  This will start off as as the time 
	//! commanded by the host and will increment as the program runs.
	//!
	simdata::SimDate simulationTime;
	
	//=========================================================
	//! Initializes simulationTime, using the current wall time as the
	//! default
	//!
	void initializeTime();
	
	//=========================================================
	//! Sets the time to a specific value.  Called by the CIGI packet 
	//! handler.
	//! \param y   - year
	//! \param mon - month
	//! \param d   - day
	//! \param h   - hour (GMT)
	//! \param min - minute
	//! \param s   - second
	//!
	void setTime( int y, int mon, int d, int h, int min, int s );

	//=========================================================
	//! Freezes or unfreezes the simulationTime.  Called by the CIGI packet 
	//! handler.
	//! \param freeze - if true, simulationTime will be paused; 
	//!                 if false, simulationTime will be unpaused
	//!
	void freezeTime( bool freeze );

	//=========================================================
	//! Returns the position for the entity to which the view is attached.
	//!
	CoordSet getOwnshipLocation( void );

	void updateEphemeris( double lat, double lon, simdata::SimDate const &t );
	void updateSkyColors();
	void updateSun();
	void updateMoon();
	void updateStars();
	

	//=========================================================
	//! This class listens for Celestial Sphere Control packets
	//! 
	class CelestialSphereCtrlP : public CigiBaseEventProcessor  
	{
	public:

		//=========================================================
		//! General Constructor
		//! 
		CelestialSphereCtrlP( PluginEphemerisModel *_plugin )
			: plugin( _plugin ) {}

		//=========================================================
		//! General Destructor
		//! 
		virtual ~CelestialSphereCtrlP() {}

		//=========================================================
		//! Callback; processes a packet
		//! 
		virtual void OnPacketReceived(CigiBasePacket *Packet);

	private:

		//=========================================================
		//! Pointer to parent container, passed in from plugin
		//! 
		PluginEphemerisModel *plugin;

	};
	
	CelestialSphereCtrlP celestialSphereCtrlProc;
};



#endif

