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

#ifndef __SKYSHADER_H__
#define __SKYSHADER_H__

#include "Colorspace.h"

class SkyShader {
protected:

	typedef float coeff[5];
	struct sky_c { coeff x, y, Y; };
	struct perez { float x, y, Y; };

	// control parameters
	Color m_FullMoonColor;
	float m_HaloSharpness;
	float m_NightBase;
	float m_SunsetSharpness;
	float m_OverLuminescence;
	float m_Turbidity;
	float m_SunElevation;

	// internal parameters
	sky_c m_Coefficients;
	Color m_Zenith;
	float m_MaxY;
	float m_F;
	float m_SunTheta;
	float m_SunVector[3];
	float m_AzimuthCorrection;
	perez m_PerezFactor;
	bool m_Dirty;
	
	/**
	 * Get Skylight Distribution Coefficients (c.f. Preetham et al.) for
	 * a given atmospheric turbitity.
	 *
	 * @param T Turbidity.
	 */
	void getSkyCoefficients(float T, sky_c &skylight);

	/**
	 * Perez Sky Model function.
	 *
	 * @param theta An angle (radians).
	 * @param gamma An angle (radians).
	 * @param p Skylight coefficients.
	 */
	float F(float theta, float gamma, coeff &p);

	/**
	 * Normalized Perez Sky Model function for use with precomputed sun position.
	 *
	 * @param theta View vector declination.
	 * @param gamma Angle between view vector and sun vector.
	 * @praam factor Perez precomputed scaling factor for this color component.
	 * @param z Zenith color component.
	 * @param p Skylight coefficients.
	 */
	float FastPerez(float theta, float gamma, float factor, float z, coeff &p);
	
	/**
	 * Normalized Perez Sky Model function.
	 *
	 * @param theta View vector declination.
	 * @param gamma Angle between view vector and sun vector.
	 * @praam theta_s Sun vector declination.
	 * @param z Zenith color component.
	 * @param p Skylight coefficients.
	 */
	float Perez(float theta, float gamma, float theta_s, float z, coeff &p);

	/**
	 * Get zenith color in CIE xyY colorspace.
	 *
	 * @param T Atmospheric turbidity.
	 * @param theta_s Sun vector declination.
	 */
	Color getZenith(float T, float theta_s);

	void _computeBase();

public:
	SkyShader();
	void setTurbidity(float T);
	void setSunElevation(float h);
	Color SkyColor(float elevation, float azimuth, float dark, float &intensity);
};

#endif
