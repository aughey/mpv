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

#define _USE_MATH_DEFINES

#include <math.h>

#include "Sun.h"

Sun::Sun(): AstronomicalBody() {
	_radius = 695508000.0;
}

void Sun::updateScene(double h, double A, Color const &color, float intensity, float background) {
	float x, y, z;
	// sun direction
	z = sin(h);
	y = -cos(h);
	x = y*sin(A);
	y *= cos(A);
	_updateLighting(x, y, z, h, color, intensity, background);
}

void Sun::_updateLighting(float x, float y, float z, float /*h*/, Color const &color, float intensity, float background) {
	float specular_scale = 1.0;
	float diffuse_scale = 1.0;
	float ambient = background;
	if (ambient > 0.1) ambient = 0.1;
		
	// the sky shading at the sun's position is too blue when the sun
	// is high to use as the light color.  instead we use the approximate
	// chromaticity taken from Preetham.  near sunset/rise, this function
	// decays too rapidly and to account for secondary scattering from
	// the atmosphere that becomes important as the direct sunlight weakens.
	// so, as an approximation we blend the sunlight color and intensity
	// smoothly into the sky color at the sun's position as it nears the
	// horizon.  this misses the sharp drop in direct light as the sun
	// sets, but captures the nice glow from the horizon that persists for
	// a short time after sunset/before sunrise.  The discontinuity in
	// direct light at sunrise/set is approximated by diffuse_scale and
	// specular_scale below.
	m_Color = color;
	m_Intensity = intensity;
	if (z > 0.0) {
		// approximate atmospheric path length (1.0 at sunset)
		double atmospheric_distance = 0.03 / (z + 0.03);
		// very approximate fit to figure 7 in Preetham
		double ciex = 0.3233 + 0.08 * atmospheric_distance;
		double ciey = 0.556 + 2.3*(ciex-0.33) - 2.0*ciex*ciex;
		// completely ad-hoc
		double cieY = 1.0 - 0.4 * atmospheric_distance * atmospheric_distance;
		float weight = z * 2.0;
		if (weight > 1.0) weight = 1.0;
		m_Color.blend(Color(ciex, ciey, cieY, Color::CIExyY, false).toRGB(true), weight);
		m_Intensity = (1.0 - weight) * m_Intensity + weight * cieY;
	}

	// ad-hoc additional darkening once the sun has set... the gl light
	// representing the sun should not be as bright as the horizion.
	float scale = m_Intensity * m_Intensity * 25.0;
	if (scale > 1.0) scale = 1.0;
	float light_r = m_Color.getA() * scale;
	float light_g = m_Color.getB() * scale;
	float light_b = m_Color.getC() * scale;

	// below horizon?
	if (z < 0.0) {
		// 0.5 cuts light level in half exactly at sunset
		// (give or take the sun's diameter)
		diffuse_scale = 0.5;
		// fade out specular faster as sun drops below the horizon
		specular_scale = std::max(0.0, 0.5 + 9.0*z);
		// get the "sun shine" from the horizion, not below.
		z = 0.0;
	}

	// sunlight direction
	m_Light.direction.Set( -x, -y, -z );
	
	// place the sun far from the scene
	// Note - not needed, because directional has been set to true
	m_Light.position.Set( x*1e+6, y*1e+6, z*1e+6 );

	// set the various light components
	m_Light.ambient.Set( 0.3f * light_r + ambient,
	                     0.3f * light_g + ambient,
	                     0.3f * light_b + ambient,
	                     1.0f );
	m_Light.diffuse.Set( diffuse_scale*light_r,
	                     diffuse_scale*light_g,
	                     diffuse_scale*light_b,
	                     1.0f );
	m_Light.specular.Set( specular_scale*light_r,
	                      specular_scale*light_g,
	                      specular_scale*light_b,
	                      1.0f );

}

void Sun::updatePosition(double julian_date) {
	double dt = julian_date - getEpoch();
	double M = -3.18 + 0.98560 * dt;
	double L = M - 77.11;
	M = degreesToRadians(M);
	double C = 1.915 * sin(M) + 0.0201 * sin(2*M);
	// in AU = 1.496x10^11 m (23455 R_earth)
	// double R = 1.00140 - 0.016708*cos(M) - 0.000141*cos(2*M)
	_beta = 0.0;
	_lambda = degreesToRadians(L + C);
	_pi = 0.00004263; // just taking an average orbital radius of the earth
	_stale = true;
}


