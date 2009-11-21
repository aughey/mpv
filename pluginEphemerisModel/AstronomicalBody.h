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


#ifndef __ASTRONOMICALBODY_H__
#define __ASTRONOMICALBODY_H__

#define _USE_MATH_DEFINES

#include <math.h>

#include "MPVLight.h"

inline double degreesToRadians( double angle ) { return angle*M_PI/180.0; }
inline float degreesToRadians( float angle ) { return angle*(float)M_PI/180.0f; }

class AstronomicalBody
{
protected:
	// the earth's tilt varies periodically in time, by about 3 degrees
	// over a 41,000 year cycle.  but we'll ignore that for now ;-)
	// that fact here.
	static const double _earth_tilt;
	static const double _cos_earth_tilt;
	static const double _sin_earth_tilt;
	static const double _earth_radius;
	static const double _epoch;
	
	mutable double _alpha, _delta;
	double  _beta, _lambda, _pi;
	double _radius;
	mutable double _angular_radius, _distance;
	mutable bool _stale;

public:
	AstronomicalBody();
	virtual ~AstronomicalBody() {}

	/**
	 * Compute atmospheric refraction
	 *
	 * Really very minor effect in the grand scheme of things.
	 */
	double refract(double h) const;

	void updateEquatorial() const;

	void toObserver(double lat, double lmst, double &h, double &A) const;

	void getEliptic(double &beta, double &lambda) const;

	void getEquatorial(double &delta, double &alpha) const;

	virtual void updatePosition(double julian_date) = 0;
	
	double getEpoch() const { return _epoch; }
	
	double getRadius() const { return _radius; }
	double getAngularRadius() const { return _angular_radius; }

	double getLambda() const { return _lambda; }
	double getBeta() const { return _beta; }
	double getPi() const { return _pi; }

	MPVLight getLight() { return m_Light; }
	void initLight(/*int n*/);

protected:

	MPVLight m_Light;
	int m_LightNum;
};


#endif
