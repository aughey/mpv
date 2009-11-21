// Combat Simulator Project
// Copyright (C) 2002-2005 The Combat Simulator Project
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

#include "Date.h"
#include "AstronomicalBody.h"

const double AstronomicalBody::_earth_radius = 6370000.0;
const double AstronomicalBody::_earth_tilt = degreesToRadians(23.4397);
const double AstronomicalBody::_cos_earth_tilt = cos(_earth_tilt);
const double AstronomicalBody::_sin_earth_tilt = sin(_earth_tilt);
const double AstronomicalBody::_epoch = simdata::SimDate(2000,1,1,0,0,0).getJulianDate();

AstronomicalBody::AstronomicalBody() {
	_alpha = _beta = _delta = _lambda = _pi = 1.0;
	_radius = 1.0;
	_stale = true;
	initLight();
}

double AstronomicalBody::refract(double h) const {
	double f = 0.000305 / (0.000305 + h * h); // localize effect at horizon (semi-arbitrary approx)
	return h + degreesToRadians(0.57 * f);
}

void AstronomicalBody::updateEquatorial() const {
	double sin_lambda = sin(_lambda);
	_alpha = atan2(sin_lambda*_cos_earth_tilt - tan(_beta)*_sin_earth_tilt, cos(_lambda));
	_delta = asin(sin(_beta)*_cos_earth_tilt + cos(_beta)*sin_lambda*_sin_earth_tilt);
	_distance = _earth_radius / _pi;
	// apparent angular radius from earth's surface
	_angular_radius = 6.2832 * _radius / (_distance - _earth_radius);
	_stale = false;
}

void AstronomicalBody::toObserver(double lat, double lmst, double &h, double &A) const {
	if (_stale) updateEquatorial();	
	double H = lmst - _alpha;
	double sin_lat = sin(lat);
	double cos_lat = cos(lat);
	double cos_H = cos(H);
	h = asin(sin_lat*sin(_delta) + cos_lat*cos(_delta)*cos_H);
	A = atan2(sin(H), cos_H*sin_lat - tan(_delta)*cos_lat);
}

void AstronomicalBody::getEliptic(double &beta, double &lambda) const {
	beta = _beta;
	lambda = _lambda;
}

void AstronomicalBody::getEquatorial(double &delta, double &alpha) const {
	if (_stale) updateEquatorial();	
	delta = _delta;
	alpha = _alpha;
}

void AstronomicalBody::initLight(/*int n*/) 
{
	m_Light.directional = true;
	m_Light.ambient.Set( 0.0, 0.0, 0.0, 1.0 );
	m_Light.diffuse.Set( 0.0, 0.0, 0.0, 1.0 );
	m_Light.specular.Set( 0.0, 0.0, 0.0, 1.0 );
}


