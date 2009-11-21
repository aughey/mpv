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

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif

#include "SkyDomeNode.h"
#include "SkyParameters.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <algorithm>

#include <osg/Depth>
#include <osg/Fog>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Geometry>
#include <osg/Light>
#include <osg/Texture2D>
#include <osg/TexEnv>
#include <osg/BlendFunc>
#include <osg/Version>

#ifndef OSG_VERSION_MAJOR
#  define OSG_OLD_COMPUTE_BOUND
#endif

using namespace osg;


/*
 * TODO
 *
 * tie global colormatrix to light level
 * fix moon light
 * accelerate skyshade code for fixed sun position
 * add earthshine effects
 * organize/cleanup code
 * dim the stars when the moon is full
 *
 */


/*
 * TEXDOME uses a texture to shade the sky, as opposed to skydome vertex
 * colors.  This can reduce artifacts when the sun passes near a skydome
 * vertex, but has the disadvantage of more obvious non-uniformity of
 * the skycolor due to texelization.  One solution is to use a larger
 * texture, but this requires more time to reevaluate and would have to
 * be computed over multiple frames.
 */
//#define TEXDOME



////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////



class FalseHorizon: public osg::Geometry {
public:
	FalseHorizon() {
#ifndef _MSC_VER
#warning "fixme - remove depth and light parameter stuff from here"
#endif
		m_Init = false;
		m_Segments = 0;
		m_Radius = 0.0;
		setSupportsDisplayList(false);
		osg::StateSet* dstate = new osg::StateSet;
		dstate->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
		dstate->setMode(GL_CULL_FACE, StateAttribute::OFF);
		dstate->setMode(GL_FOG, osg::StateAttribute::OFF);
		dstate->setMode(GL_CLIP_PLANE0, osg::StateAttribute::OFF);
		dstate->setMode(GL_CLIP_PLANE1, osg::StateAttribute::OFF);
		dstate->setMode(GL_CLIP_PLANE2, osg::StateAttribute::OFF);
		dstate->setMode(GL_CLIP_PLANE3, osg::StateAttribute::OFF);
		dstate->setMode(GL_CLIP_PLANE4, osg::StateAttribute::OFF);
		dstate->setMode(GL_CLIP_PLANE5, osg::StateAttribute::OFF);
		dstate->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
		osg::Depth *depth = new osg::Depth;
		depth->setFunction(osg::Depth::ALWAYS);
		depth->setRange(1.0, 1.0);
		dstate->setAttributeAndModes(depth, osg::StateAttribute::OFF);
		setStateSet(dstate);
	}

	void init(int segments) {
		if (m_Init) return;
		m_Init = true;
		m_Segments = segments;
		m_Colors = new Vec4Array(segments+1);
		m_Coords = new Vec3Array(segments+1);
		_init();
	}

	void _init() {
		DrawElementsUShort* fan = new DrawElementsUShort(PrimitiveSet::TRIANGLE_FAN);
		fan->reserve(m_Segments+2);
		fan->push_back(m_Segments);
		for (int i=0; i < m_Segments; i++) {
			fan->push_back(i);
		}
		fan->push_back(0);
		(*m_Coords)[m_Segments].set(0.0, 0.0, -1000.0);
		addPrimitiveSet(fan);
		setVertexArray(m_Coords);
		setColorArray(m_Colors);
		UShortArray *ci = new UShortArray(m_Segments+1);
		for (unsigned short i=0; i < m_Segments+1; i++) {
			(*ci)[i] = i;
		}
		setColorIndices(ci);
		setColorBinding(Geometry::BIND_PER_VERTEX);
		updateHorizon(1000.0, 120000.0);
		updateFogColor(Vec4(1.0, 1.0, 1.0, 1.0));
	}

	void updateHorizonColors(Vec4Array const &colors) {
		assert(int(colors.size()) == m_Segments);
		for (int i=0; i < m_Segments; i++) (*m_Colors)[i] = colors[i];
		dirtyDisplayList();
	}

	void updateFogColor(Vec4 const &fog) {
		m_FogColor = fog;
		(*m_Colors)[m_Segments] = fog;
	}

	void updateHorizon(float altitude, float clip) {
		float a = 0.0;
		float da = 2.0 * M_PI / m_Segments;

		float radius = std::min((float)SKY_DOME_RADIUS, std::max(1.5f*clip, sqrtf(2.0 * 6370000.0 * altitude)));

		// variation less than 2 pixels under most conditions:
		if (fabs(radius - m_Radius) < 0.0005 * m_Radius) return;
		//std::cerr << "HORIZON -> " << radius << "\n";
		m_Radius = radius;
		for (int i=0; i < m_Segments; i++) {
			(*m_Coords)[i].set(cos(a) * m_Radius, -sin(a) * m_Radius, -1000.0);
			a += da;
		}
	}

private:
	bool m_Init;
	int m_Segments;
	Vec4 m_FogColor;
	Vec4Array *m_Colors;
	Vec3Array *m_Coords;
	float m_Radius;
};

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

SkyDomeNode::SkyDomeNode(): Group() 
{
	osg::StateSet *stateSet = getOrCreateStateSet();
	
	// The stars and moon are drawn first.  The sky color is then drawn.  
	// In order to blend the stars and the sky in an additive manner, the 
	// gl blend function is set to ONE, ONE.
	osg::BlendFunc *bf = new osg::BlendFunc;
	bf->setFunction( osg::BlendFunc::ONE, osg::BlendFunc::ONE );
	stateSet->setAttributeAndModes( bf, osg::StateAttribute::ON );
}

SkyDomeNode::~SkyDomeNode() 
{
}

void SkyDomeNode::init( int numWedges, int numSlices, float *sliceAngles )
{
	m_nlev = numSlices;
	m_nseg = numWedges;
	
	int i, j;
	float x, y, z;
	float alpha, theta;

	float radius = SKY_DOME_RADIUS;

	m_Horizon = new FalseHorizon;
#ifdef TEXDOME
	m_Horizon->init(192);
#else
	m_Horizon->init(m_nseg);
#endif
	m_SkyDome = new Geometry;
	m_Colors = new Vec4Array(m_nseg*m_nlev);
	m_TexCoords = new Vec2Array(m_nseg*m_nlev);
	Vec3Array& coords = *(new Vec3Array(m_nseg*m_nlev));
	Vec4Array& colors = *m_Colors;
	Vec2Array& tcoords = *m_TexCoords;
	assert(m_nseg * m_nlev < 65536);
	UShortArray *cindex = new UShortArray(m_nseg*m_nlev);
//FIXME - leak - m_HorizonColors is created, but not attached to the scene
#ifdef TEXDOME
	m_HorizonColors = new Vec4Array(192);
#else
	m_HorizonColors = new Vec4Array(m_nseg);
#endif

	int ci = 0;

	for (i = 0; i < m_nlev; ++i) {
		for(j = 0; j < m_nseg; ++j) {
			alpha = osg::DegreesToRadians(sliceAngles[i]);
			theta = osg::DegreesToRadians((float)(j*360.0/m_nseg));

			float cos_alpha = cosf(alpha);
			float cos_theta = cosf(theta);
			float sin_theta = sinf(theta);
			x = radius * cos_alpha *  cos_theta;
			y = radius * cos_alpha * -sin_theta;
			z = radius * sinf(alpha);

			coords[ci][0] = x;
			coords[ci][1] = y;
			coords[ci][2] = z;

			colors[ci][0] = 1.0;
			colors[ci][1] = 1.0;
			colors[ci][2] = 1.0;
			colors[ci][3] = 1.0;

			tcoords[ci][0] = 0.5f + std::min(0.5f, (90.0f - sliceAngles[i]) / 180.0f) * cos_theta;
			tcoords[ci][1] = 0.5f + std::min(0.5f, (90.0f - sliceAngles[i]) / 180.0f) * sin_theta;

			(*cindex)[ci] = static_cast<unsigned short>(ci);

			++ci;
		}
	}

	DrawElementsUShort* drawElements = new DrawElementsUShort(PrimitiveSet::TRIANGLE_STRIP);
	drawElements->reserve(m_nlev*(m_nseg+1)*2);
	for (i = 0; i < m_nlev-1; ++i) {
		for (j = 0; j <= m_nseg; ++j) {
			drawElements->push_back((i+1)*m_nseg+(j%m_nseg));
			drawElements->push_back((i+0)*m_nseg+(j%m_nseg));
		}
	}

	// XXX enabling/disabling dl/vbo may impact performance
	m_SkyDome->setSupportsDisplayList(false);
	m_SkyDome->setUseDisplayList(false);
	//m_SkyDome->setUseVertexBufferObjects(false);

	m_SkyDome->addPrimitiveSet(drawElements);
	m_SkyDome->setVertexArray(&coords);
	m_SkyDome->setTexCoordArray( 0, m_TexCoords );
	m_SkyDome->setColorArray( m_Colors );
	m_SkyDome->setColorIndices(cindex);
	m_SkyDome->setColorBinding(Geometry::BIND_PER_VERTEX);
	
	if (1) {
		StateSet *dome_state = m_SkyDome->getOrCreateStateSet();

#ifdef TEXDOME
		Image *image = new Image();
		image->allocateImage(64, 64, 1, GL_RGB, GL_UNSIGNED_BYTE);
		assert(image->data() != NULL);
		memset(image->data(), 255, 64*64*3);
		// setting the internal texture format is required, but
		// its not clear how to determine the correct value.  the
		// following is just what the png loader does, setting
		// the format to the number of color bytes.
		image->setInternalTextureFormat(GL_RGB);
		Texture2D *tex = new Texture2D;
		tex->setDataVariance(osg::Object::DYNAMIC);
		tex->setImage(image);
		tex->setWrap(Texture::WRAP_S, Texture::CLAMP);
		tex->setWrap(Texture::WRAP_T, Texture::CLAMP);
		tex->setFilter(Texture::MIN_FILTER, Texture::LINEAR);
		tex->setFilter(Texture::MAG_FILTER, Texture::LINEAR);
		//tex->setFilter(Texture::MIN_FILTER, Texture::NEAREST);
		//tex->setFilter(Texture::MAG_FILTER, Texture::NEAREST);
		tex->setUseHardwareMipMapGeneration(false);
		tex->setUnRefImageDataAfterApply(false);
		tex->setInternalFormatMode(osg::Texture::USE_IMAGE_DATA_FORMAT);
		m_SkyDomeTextureImage = image;
		m_SkyDomeTexture = tex;

		dome_state->setTextureAttributeAndModes(0, m_SkyDomeTexture.get(), StateAttribute::ON);
		dome_state->setTextureAttributeAndModes(0, new TexEnv);
#endif

	}

	osg::Geode *skyDomeGeode = new osg::Geode();
	skyDomeGeode->addDrawable(m_SkyDome.get());
	addChild(skyDomeGeode);
	
	osg::Geode *horizon = new osg::Geode();
//	horizon->addDrawable(m_Horizon.get());
 	// draw horizon after sky, stars, and moon
	addChild(horizon);

	setName("Sky");
}


// TODO
// take altitude into account for turbidity
// relate night sky color to moon brightness (and altitude)
// maybe modify SkyColor it consider moon position:
//   if moon is up and sun is down (or nearly so), add in a glow and background color
//   or just use an additive textured quad in front of/behind the moon?


void SkyDomeNode::updateSky( float *skyColors )
{
	// copy sky colors from skyColors array to m_Colors
	Vec4Array& colors = *m_Colors;
	int numColorPoints = m_nlev * m_nseg;
	for( int i = 0; i < numColorPoints; ++i )
	{
		colors[i][0] = *skyColors; skyColors++;
		colors[i][1] = *skyColors; skyColors++;
		colors[i][2] = *skyColors; skyColors++;
		colors[i][3] = *skyColors; skyColors++;
	}
}







// -------------------------------------------------------------------------
// 
// DEAD CODE
// 
// -------------------------------------------------------------------------


#if 0

// this is a quick hack to explore the effect of dynamically
// altering fog color to match the sky shade at the horizon
// near the center of the field of view.  it is much better
// than a fixed fog color, but produces bad effects around
// sunrise/set for wide fields of view.  2 point interpolation
// (avg=0) seems to look better than the multipoint averaging
// implemented below for avg > 0.
osg::Vec4 Sky::getHorizonColor(float angle) {
	static int avg = 0;
	// skydome wraps around clockwise
	angle = -angle;
	if (angle < 360.0) {
		angle -= 360.0 * (int(angle / 360.0)-2);
	}
	int n = m_HorizonColors->size();
	angle *= n / 360.0;
	int idx = int(angle);
	float da = angle - idx;
	assert(m_HorizonColors);
	osg::Vec4 color;
	color = (*m_HorizonColors)[(idx-avg)%n] * (1.0-da);
	for (int i = 1-avg; i <= avg; ++i) {
		color += (*m_HorizonColors)[(idx+i)%n];
	}
	color += (*m_HorizonColors)[(idx+avg+1)%n] * da;
	color /= (2.0*avg+1.0);
	return color;
}


void Sky::updateHorizon(Vec4 const &fog_color, float altitude, float clip)
{
	m_Horizon->updateFogColor(fog_color);
	m_Horizon->updateHorizon(altitude, clip);
}

#endif
