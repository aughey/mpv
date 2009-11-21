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

#include "Moon.h"


Moon::Moon(): AstronomicalBody() {
	m_DirtyImage = true;

	_radius = 1738000.0;
	m_Phase = 0.0;
	m_ApparentBrightness = 0.0;
	m_MoonShine = 0.0;
	m_EarthShine = 0.0;
	m_SunShine = 0.0;
	m_Altitude = 0.0;
	m_Azimuth = 0.0;
}

void Moon::updatePosition(double julian_date) {
	double dt = julian_date - getEpoch();
	double T = dt * 2.73785078713e-5; // julian centuries

	double l = 3.8104 + 8399.7091 * T;
	double m = 6.2300 + 628.3019 * T;
	double f = 1.6280 + 8433.4663 * T;
	double mp = 2.3554 + 8328.6911 * T;
	double d = 5.1985 + 7771.3772 * T;

	_lambda = \
		l
		+ 0.1098*sin(mp)
		+ 0.0222*sin(2*d-mp)
		+ 0.0115*sin(2*d)
		+ 0.0037*sin(2*mp)
		- 0.0032*sin(m)
		- 0.0020*sin(2*f)
		+ 0.0010*sin(2*d-2*mp)
		+ 0.0010*sin(2*d-m-mp)
		+ 0.0009*sin(2*d+mp)
		+ 0.0008*sin(2*d-m)
		+ 0.0007*sin(mp-m)
		- 0.0006*sin(d)
		- 0.0005*sin(m+mp);
	_beta = \
		+ 0.0895*sin(f)
		+ 0.0049*sin(mp+f)
		+ 0.0048*sin(mp-f)
		+ 0.0030*sin(2*d-f)
		+ 0.0010*sin(2*d+f-mp)
		+ 0.0008*sin(2*d-f-mp)
		+ 0.0006*sin(2*d+f);
	/*
	_pi = \
		+ 0.016593
		+ 0.000904 * cos(mp)
		+ 0.000166 * cos(2*d - mp)
		+ 0.000137 * cos(2*d)
		+ 0.000049 * cos(2*mp)
		+ 0.000015 * cos(2*d + mp)
		+ 0.000009 * cos(2*d - m);
	*/
	// save some time: for now no distance scaling variation
	_pi = 0.016593;
	_stale = true;
}


void Moon::updateScene(double lat, double lmst, double sunLambda, float intensity) {
	double x, y, z;

	toObserver(lat, lmst, m_Altitude, m_Azimuth);

//FIXME - note to self - the logic here regarding m_DirtyImage has me 
// scratching my head... need to look into it -ADS
	if (m_DirtyImage && m_Altitude > -0.2) {
		m_DirtyImage = false;	
		_updateIllumination(sunLambda);
	} else if (m_Altitude < -0.2 && !m_DirtyImage) {
		m_DirtyImage = true;
	}
	//_updateShading(m_Altitude, intensity);
	y = -cos(m_Altitude);
	x = y*sin(m_Azimuth);
	y *= cos(m_Azimuth);
	z = sin(m_Altitude);

	_updateLighting(x, y, z, m_Altitude, intensity);
	
}


void Moon::_updateIllumination( double sunLambda ) {
	m_Phase = (sunLambda - getLambda());
	float sun_angle = getBeta() * 0.38/159.6; // (distance ratio moon:sun)
	float a0 = 0.5 * m_Phase;
	float a1 = 0.5 * (M_PI - m_Phase);
	float f0 = fabs(tan(0.5*a0));
	float f1 = fabs(tan(0.5*a1));
	m_EarthShine = 0.19 * 0.5;
	if (f0 > 0.0) {
		m_EarthShine *= 1.0 - sin(a0)*tan(a0)*log(1.0/f0);
	}
	m_SunShine = 1905.0;
	if (f1 > 0.0) {
		m_SunShine *= 1.0 - sin(a1)*tan(a1)*log(1.0/f1);
	}
	float rd = _radius / _distance;
	m_MoonShine = 0.67 * 0.072 * rd * rd * (m_EarthShine + m_SunShine);
	//cout << a0 << ":" << a1 << "|" << m_EarthShine << ":" << m_SunShine << ":" << m_MoonShine << endl;
//	_maskPhase(m_Phase, sun_angle);
}


void Moon::_updateLighting(double x, double y, double z, double h, float intensity) {
	float brightness = 0.0;
	float horizon = 0.0;
	if (h > 0.0) {
	 	horizon = 1.2 * h / (0.4 + h);
	}
	// moon direction
	if (z < 0.0) {
		z = 0.0;
		brightness = 0.0;
	}

	// moonlight direction
	m_Light.direction.Set( -x, -y, -z );
	
	// place the moon far from the scene
	// Note - not needed, because directional has been set to true
	m_Light.position.Set( x*1e+6, y*1e+6, z*1e+6 );
	
	// brightess should reach a maximum of approximately 1.0 during a fullmoon.
	brightness = 6.5e+2 * m_MoonShine * (1.0 - 40.0 * intensity * intensity * intensity) * horizon;
	//cout << brightness << ", shine=" << m_MoonShine << ", I=" << intensity << ", hor=" << horizon << endl;

	if (brightness < 0.0) brightness = 0.0;
	if (brightness > 1.0) brightness = 1.0;

	float a_r, a_g, a_b;
	//a_r = 0.17 * brightness; a_g = 0.18 * brightness; a_b = 0.20 * brightness;
	a_r = 0.25 * brightness; a_g = 0.27 * brightness; a_b = 0.28 * brightness;

	// set the various light components
	//cout << brightness << ":" << a_b << endl;
	m_Light.ambient.Set( a_r*0.2, a_g*0.2, a_b*0.2, 1.0 );
	m_Light.diffuse.Set( a_r, a_g, a_b, 1.0 );
	m_Light.specular.Set( 0, 0, 0, 1 );
	
	m_ApparentBrightness = brightness;

}




/*
// This one seems to be unused...
void Moon::_updateShading(float h, float intensity) {
	static int xx = 0;
	if (xx++ % 5) return;
	if (h < -0.2) return;
	if (h < 0.0) h = 0.0;
	float hs = 2.0 - 2.0*sqrt(h);
	if (hs > 2.0) hs = 2.0;
	if (hs < 1.0) hs = 1.0;
	float DAY = 1.0-2.0*intensity*intensity*hs;
	//float DAY = 1.0-1.5*intensity*hs;
	if (DAY < 0.0) DAY = 0.0;
	float gibbous = 1.0 - h * 8.0;
	if (gibbous < 0.0) gibbous = 0.0;
	float R = 1.0 - gibbous * 0.412 * 0.5;
	float G = 1.0 - gibbous * 0.717 * 0.5;
	float B = 1.0 - gibbous * 0.999 * 0.5;
	osg::Vec4Array* colours = dynamic_cast<Vec4Array*>(m_Moon->getColorArray());
	assert(colours);
	(*colours)[0].set(R, G, B, DAY);
	m_Moon->setColorArray(colours);
	m_Moon->dirtyDisplayList();
}
*/

