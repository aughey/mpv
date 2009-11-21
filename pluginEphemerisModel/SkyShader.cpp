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

#define _USE_MATH_DEFINES
#include <cmath>

#include "SkyShader.h"

#define CUSTOM


#if 0
// CUSTOMIZATION PARAMETERS
// --------------------------------------------------------------------------
// RGB additive sky values for midnight sky color under a full moon)
#define FULLMOON_RED   0.008
#define FULLMOON_GREEN 0.035
#define FULLMOON_BLUE  0.140
// CIE luminesence (Y) looking into the sun at the zenith.
#define OVER_LUMINESCENCE 1.2 //1.5
// parameters for the attenuation function which cuts the intensity
// as the sun nears the horizon.  Large values of SUNSET_SHARPNESS localizes
// the drop at the horizon.  NIGHT_BASE is the scaling when the sun is 90
// degrees below the horizon.
#define SUNSET_SHARPNESS 4.0
#define NIGHT_BASE 0.02
// sharpness of the halo around the sun (0.1 to 2.0)
#define HALO_SHARPNESS 0.5
// --------------------------------------------------------------------------
#endif

const float TURBIDITY = 3.0f;

SkyShader::SkyShader() {
	m_OverLuminescence = 1.2;
	m_HaloSharpness = 0.5;
	m_SunsetSharpness = 4.0;
	m_NightBase = 0.02;
	m_FullMoonColor = Color(0.008, 0.035, 0.140, Color::RGB);
	setTurbidity(TURBIDITY);
	setSunElevation(0.0);
}

void SkyShader::getSkyCoefficients(float T, sky_c &skylight) {
	skylight.x[0] = -0.0193 * T - 0.2592;
	skylight.x[1] = -0.0665 * T + 0.0008;
	skylight.x[2] = -0.0004 * T + 0.2125;
	skylight.x[3] = -0.0641 * T - 0.8989;
	skylight.x[4] = -0.0033 * T + 0.0452;
	skylight.y[0] = -0.0167 * T - 0.2608;
	skylight.y[1] = -0.0950 * T + 0.0092;
	skylight.y[2] = -0.0079 * T + 0.2102;
	skylight.y[3] = -0.0441 * T - 1.6537;
	skylight.y[4] = -0.0109 * T + 0.0529;
	skylight.Y[0] =  0.1787 * T - 1.4630;
	skylight.Y[1] = -0.3554 * T + 0.4275;
	skylight.Y[2] = -0.0227 * T + 5.3251;
	skylight.Y[3] =  0.1206 * T - 2.5771;
	skylight.Y[4] = -0.0670 * T + 0.3703;
}

float SkyShader::F(float theta, float gamma, coeff &p) {
	float cos_g = cosf(gamma);
#ifdef CUSTOM
	float cos_t = fabs(cosf(theta)) + 0.09;
#else
	float cos_t = cosf(theta);
#endif
	return (1.0 + p[0]*expf(p[1]/cos_t))*(1.0+p[2]*expf(p[3]*gamma)+p[4]*cos_g*cos_g);
}

float SkyShader::FastPerez(float theta, float gamma, float factor, float z, coeff &p) {
	if (theta > 1.57) theta = 1.57;
	return factor * z * F(theta, gamma, p);
}

float SkyShader::Perez(float theta, float gamma, float theta_s, float z, coeff &p) {
	if (theta > 1.57) theta = 1.57;
	if (theta_s >  1.50) theta_s = 1.50 + ((theta_s-1.50) * 0.5);
	float denom = F(0.0, theta_s, p);
	if (denom != 0.0) z /= denom;
	return z * F(theta, gamma, p);
}

Color SkyShader::getZenith(float T, float theta_s) {
	float s = theta_s;
	float s2 = s*s;
	float s3 = s2*s;
	float t = T;
	float t2 = t*t;
	float xa =  0.00166 * s3 - 0.00375 * s2 + 0.00209 * s;
	float xb = -0.02903 * s3 + 0.06377 * s2 - 0.03202 * s + 0.00394;
	float xc =  0.11693 * s3 - 0.21196 * s2 + 0.06052 * s + 0.25886;
	float xz = xa * t2 + xb * t + xc;
	float ya =  0.00275 * s3 - 0.00610 * s2 + 0.00317 * s;
	float yb = -0.04214 * s3 + 0.08970 * s2 - 0.04153 * s + 0.00516;
	float yc =  0.15346 * s3 - 0.26756 * s2 + 0.06670 * s + 0.26688;
	float yz = ya * t2 + yb * t + yc;
	float chi = (0.4444444 - T * 0.008333333) * (M_PI - 2.0*s);
	float Yz = (4.0453 * T - 4.9710) * tan(chi) - 0.2155 * T + 2.4192;
	// Yz is in units of kcd/m^2

#ifdef CUSTOM
	// FIXME
	// Ad hoc function for post-sunset sky.
	if (Yz < 0.2) Yz = 0.2 - (0.2-Yz)*(0.2-Yz)*0.05;
	if (Yz < 0.05) Yz = 0.05;
#endif
	
	return Color(xz, yz, Yz, Color::CIExyY, false);
}


void SkyShader::setTurbidity(float T) {
	m_Turbidity = T;
	m_Dirty = true;
}

void SkyShader::setSunElevation(float h) {
	m_AzimuthCorrection = 0.0;
	if (h < -M_PI) {
		h += 2.0 * M_PI * int((M_PI - h) / (2.0 * M_PI));
	} else
	if (h > M_PI) {
		h -= 2.0 * M_PI * int((M_PI + h) / (2.0 * M_PI));
	}
	if (h > 0.5 *M_PI) {
		h = M_PI - h;
		m_AzimuthCorrection = M_PI;
	} else
	if (h < -0.5*M_PI) {
		h = -M_PI - h;
		m_AzimuthCorrection =  -M_PI;
	}
	m_SunElevation = h;
	m_Dirty = true;
}

void SkyShader::_computeBase() {
	m_Dirty = false;

	m_SunTheta = 0.5*M_PI - m_SunElevation;
	m_SunVector[0] = 0.0;
	m_SunVector[1] = sin(m_SunTheta);
	m_SunVector[2] = cos(m_SunTheta);

	getSkyCoefficients(m_Turbidity, m_Coefficients);
	
	float theta = m_SunTheta;
	if (theta >  1.50) theta = 1.50 + ((m_SunTheta-1.50) * 0.5);
	float denom;
	denom = F(0.0, theta, m_Coefficients.x);
	if (denom == 0.0) denom = 1.0;
	m_PerezFactor.x = 1.0 / denom;
	denom = F(0.0, theta, m_Coefficients.y);
	if (denom == 0.0) denom = 1.0;
	m_PerezFactor.y = 1.0 / denom;
	denom = F(0.0, theta, m_Coefficients.Y);
	if (denom == 0.0) denom = 1.0;
	m_PerezFactor.Y = 1.0 / denom;

	m_Zenith = getZenith(m_Turbidity, m_SunTheta);
	m_MaxY = 1.0;

#ifdef CUSTOM
	// only rescale down to the horizon, bad things happen if we go further
	if (m_SunTheta >= 0.5*M_PI) {
		m_MaxY = Perez(0.5*M_PI, 0.0, 0.5*M_PI, m_Zenith.getC(), m_Coefficients.Y);
	} else {
		m_MaxY = Perez(m_SunTheta, 0.0, m_SunTheta, m_Zenith.getC(), m_Coefficients.Y);
	}
#endif

#ifdef CUSTOM
	// F(sun_h) is my own attenuation function which cuts the intensity
	// as the sun nears the horizon.  Large values of SHARPNESS localize
	// the drop at the horizon.  NightBase is scaling with the sun 90
	// degrees below the horizon.
	m_F =(atan(m_SunElevation*m_SunsetSharpness)/M_PI+0.5)*(1.0-m_NightBase) + m_NightBase;
#endif

}

Color SkyShader::SkyColor(float elevation, float azimuth, float dark, float &intensity) {
	if (m_Dirty) _computeBase();
	float theta = 0.5*M_PI - elevation;
	float A = azimuth + m_AzimuthCorrection;
	float sin_theta = sinf(theta);
	float v[3] = {sinf(A)*sin_theta, cosf(A)*sin_theta, cosf(theta)};
	float dot = v[0]*m_SunVector[0]+v[1]*m_SunVector[1]+v[2]*m_SunVector[2];
	// numerical error can cause abs(dot) to exceed 1...
	if (dot < -1.0) dot = -1.0; else
	if (dot >  1.0) dot =  1.0;
	float gamma = acosf(dot);
	float ciex = FastPerez(theta, gamma, m_PerezFactor.x, m_Zenith.getA(), m_Coefficients.x);
	float ciey = FastPerez(theta, gamma, m_PerezFactor.y, m_Zenith.getB(), m_Coefficients.y);
	float cieY = FastPerez(theta, gamma, m_PerezFactor.Y, m_Zenith.getC(), m_Coefficients.Y);

#ifdef CUSTOM
	//  scale to Y = 1 looking directly at the sun
	if (m_MaxY > 0.0) cieY = m_OverLuminescence * cieY / m_MaxY;
	
	////Color txyY(ciex, ciey, cieY, Color::CIExyY, false);
	////cout << txyY << endl;

	// model for cloudy days:
	//cieY = zenith.getC() * (1+2*cos(theta))/3.0;

	// taking sqrt(Y) softens the sky a bit, spreading the blue more
	// uniformly.  this should probably be done instead by blending with
	// a single blue translucent surface across the field of view, where
	// the transparency depends on altitude
	//float oldY = cieY;
	if (cieY < 0.0) cieY = 0.0;
	cieY = powf(cieY, m_HaloSharpness) * m_F;
	//if (cieY > m_OverLuminescence) cieY = m_OverLuminescence;
	if (cieY > 1.0) cieY = 1.0;
#endif // CUSTOM

	intensity = cieY;

	Color xyY(ciex, ciey, cieY, Color::CIExyY, false);
	//cout << xyY << endl;
	Color rgb = xyY.toRGB();
	////cout << rgb << endl;
#ifdef CUSTOM
	rgb.composite(m_FullMoonColor, 1.0, dark);
	rgb.check();
#endif // CUSTOM
	return rgb;
}

