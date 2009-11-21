// Combat Simulator Project - FlightSim Demo
// Copyright (C) 2002 The Combat Simulator Project
// http://csp.sourceforge.net
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef __MOON_H__
#define __MOON_H__

#include "AstronomicalBody.h"

class Moon: public AstronomicalBody {
public:
	Moon();
	virtual void updatePosition(double julian_date);
	virtual void updateScene(double lat, double lmst, double sunLambda, float intensity);
	float getPhase() { return m_Phase; }
	float getApparentBrightness() { return m_ApparentBrightness; }
	double getAltitude() { return m_Altitude; }
	double getAzimuth() { return m_Azimuth; }
	float getEarthShine() { return m_EarthShine; }

private:
//	void _updateShading(float h, float intensity);
	void _updateLighting(double x, double y, double z, double h, float intensity);
	void _updateIllumination( double sunLambda );

	bool m_DirtyImage;
	double m_Phase;
	float m_MoonShine;
	float m_EarthShine;
	float m_SunShine;
	float m_ApparentBrightness;
	double m_Altitude;
	double m_Azimuth;
};


#endif
