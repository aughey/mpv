
#ifndef __SKYCOLORS_H__
#define __SKYCOLORS_H__

#include "Colorspace.h"
#include "SkyShader.h"

class SkyColors
{
public:
	SkyColors();
	~SkyColors();

	void update( double sun_h, double sun_A, float moonBrightness );
	
	float getAverageIntensity() { return m_AverageIntensity; }
	Color getZenithColor() { return m_ZenithColor; }
	float getZenithIntensity() { return m_ZenithIntensity; }
	
	int getNumWedges() { return m_nseg; }
	int getNumSlices() { return m_nlev; }
	float *getColorArray() { return colors; }
	float *getSliceAngleArray() { return m_lev; }
	
	
private:
	int m_nlev, m_nseg;
	float *m_lev;
	int m_HorizonIndex;
	Color m_ZenithColor;
	float m_ZenithIntensity;
	float m_AverageIntensity;
	
	 // A temporary variable to help with incremental calculation of 
	 // m_AverageIntensity
	 float m_AverageIntensityTemp;

	// Used for incremental calculation of sky dome colors.
	int m_skyShadingLeftoff;
	
	// the shader object - calculates sky colors given the sun position 
	// and the moon ambient brightness
	SkyShader m_SkyShader;
	
	// the output array
	float *colors;

	void init();

	void writePPMImageFile();
};

#endif
