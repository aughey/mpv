#include <iostream>

#define _USE_MATH_DEFINES

#include <math.h>

#include "SkyColors.h"
#include "Date.h"
#include "AstronomicalBody.h" // for degreestoradians

SkyColors::SkyColors()
{
	init();
}

SkyColors::~SkyColors()
{
	delete [] m_lev;
	delete [] colors;
}


void SkyColors::init()
{
	m_ZenithIntensity = 0.0;
	m_AverageIntensity = 0.0;
	m_AverageIntensityTemp = 0.0;
	m_skyShadingLeftoff = 0;

	m_lev = new float[90];
	float base_elev = -10.0;
	m_nlev = 0;
	m_HorizonIndex = 0;
	for (float elev = base_elev; elev <= 90.0 && m_nlev < 90; ) {
		m_lev[m_nlev] = elev;
		m_nlev++;
		if (elev <= -5.0 || elev >= 5.0) {
			elev += 5.0;
		} else
		if (elev <  1.0) {
			elev = 1.0;
			m_HorizonIndex = m_nlev-1;
		} else {
			elev = 5.0;
		}
	}
	m_nseg = 37;
	
	colors = new float[m_nseg * m_nlev * 4];

	int offset, ci = 0;
	for( int i = 0; i < m_nlev; ++i )
	{
		for( int j = 0; j < m_nseg; ++j )
		{
			offset = ci * 4;
			colors[offset + 0] = 1.0;
			colors[offset + 1] = 1.0;
			colors[offset + 2] = 1.0;
			colors[offset + 3] = 1.0;

			++ci;
		}
	}
}

void SkyColors::update( double sun_h, double sun_A, float moonBrightness ) 
{
	double da = 2.0 * M_PI / (m_nseg);
	double min_a = 0.5*da;
	double jitter = 0.0;
	int i, j, offset, ci = 0;

//	simdata::SimTime t = simdata::SimDate::getSystemTime();
	
	m_SkyShader.setSunElevation( sun_h );
	
	float light_h = sun_h;
	// below horizon
	if (sinf(light_h) < 0.0) {
		light_h = 0.0;
	}
	// get the sky shading at the position of the sun
	m_ZenithColor = m_SkyShader.SkyColor( light_h, 0.0, 0.0, m_ZenithIntensity );

//	osg::Vec4 horizon_average;
	for( i = m_skyShadingLeftoff; i < m_skyShadingLeftoff+1 && i < m_nlev; ++i )
	{
		ci = i * m_nseg;
		double elev = degreesToRadians( m_lev[i] );
		if( elev < 0.0 ) elev = 0.0; // sub horizon colors aren't correct
		double azimuth = -sun_A - 0.5 * M_PI;
		bool at_vertex = fabs(elev - sun_h) < min_a; // this is only a rough measure
		for( j = 0; j < m_nseg; ++j )
		{
			float intensity;
			// if the sun lines up very close to a vertex, the vertex will be
			// pure white and create a noticible artifact on the surrounding
			// triangles.
			if( at_vertex && (fabs(azimuth) < min_a) )
			{
				jitter = 0.0; //da;
			} else {
				jitter = 0.0;
			}
			Color c = m_SkyShader.SkyColor(elev, azimuth+jitter, moonBrightness, intensity);
			azimuth += da;
			offset = ci * 4;
			colors[offset + 0] = c.getA();
			colors[offset + 1] = c.getB();
			colors[offset + 2] = c.getC();
			colors[offset + 3] = 1.0;
			if( i == m_HorizonIndex )
			{
//				(*m_HorizonColors)[j] = colors[ci];
//				horizon_average += colors[ci];
			}
			++ci;
			m_AverageIntensityTemp += intensity;
		}
	}
	m_skyShadingLeftoff = i;
	
	if( m_skyShadingLeftoff >= m_nlev )
	{
		// The sky colors have all been calculated, and the array of colors 
		// is now valid.

		m_skyShadingLeftoff = 0;
		
//		m_Horizon->updateHorizonColors(*m_HorizonColors);
		m_AverageIntensity = m_AverageIntensityTemp / (m_nlev*m_nseg);
		m_AverageIntensityTemp = 0.0;

//		simdata::SimTime u = simdata::SimDate::getSystemTime();
//		std::cout << "Sky update time (vertex based, not texture based): " << ((u-t)*1000.0) << " ms\n";
		
	}
}


void SkyColors::writePPMImageFile()
{
	std::cout << "P3" << std::endl;
	std::cout << m_nseg << " " << m_nlev << std::endl;
	std::cout << "255" << std::endl;
	
	for( int i = 0; i < m_nlev; ++i )
	{
		int ci = i * m_nseg;

		for( int j = 0; j < m_nseg; ++j )
		{
			int offset = ci * 4;
			std::cout 
				<< (int)(colors[offset + 0] * 255.0) << " " 
				<< (int)(colors[offset + 1] * 255.0) << " " 
				<< (int)(colors[offset + 2] * 255.0) << std::endl;
			++ci;
		}
	}
	
	
}


