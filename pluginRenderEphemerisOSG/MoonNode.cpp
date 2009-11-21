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

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif

#include <iostream>
#include <cassert>

#include <osg/Depth>
#include <osgDB/ReadFile>
#include <osg/BlendFunc>

#include "SkyParameters.h"
//#include "ExceptionBase.h"
#include "MoonNode.h"

using namespace osg;


//SIMDATA_EXCEPTION(DataError)

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////


bool AstroBillboard::computeMatrix(Matrix& modelview, const Vec3& eye_local, const Vec3& pos_local) const {
	Matrix matrix;
	Vec3 to_eye(eye_local-pos_local);
	Vec3 to_eye_projection = to_eye^_axis;
	if (to_eye_projection.normalize() > 0.0) {
		float rotation_angle = M_PI+acos(to_eye_projection*_onormal);
		Vec3 pivot = _onormal^to_eye_projection;
		matrix.makeRotate(rotation_angle, pivot);
	}
	matrix = _orient * matrix;
	matrix.setTrans(pos_local);
	modelview.preMult(matrix);
	return true;
}

void AstroBillboard::setAxes(const Vec3& axis, const Vec3 &up, const Vec3 &normal) {
	_axis = axis;
	_axis.normalize();
	updateCache();
	_normal = normal;
	_normal.normalize();
	Vec3 up_ = up;
	up_.normalize();
	_normal = up_ ^ (_normal ^ up_);
	Vec3 rot = _axis ^ up_;
	float angle = acos(up_*_axis);
	_orient.makeRotate(-angle, rot);
	Matrix inv;
	inv.makeRotate(angle, rot);
	_normal = inv * _normal;
	_onormal = _normal ^ _axis;
	_onormal.normalize();
}


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

MoonNode::MoonNode() : osg::Group()
{
	haveTexture = false;
	m_DirtyImage = true;
	m_Latitude = -100.0;
	m_EarthShine = 0.0;

	m_RenderDistance = SKY_DOME_RADIUS;
	float x, y;
	x = y = 1.0;

	// set up the drawstate.
	m_MoonStateSet = new osg::StateSet;


	osg::BlendFunc *bf = new osg::BlendFunc;
	bf->setFunction(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA);
	m_MoonStateSet->setAttributeAndModes(bf, osg::StateAttribute::ON);

	// set up the geoset.
	Geometry* geom = new Geometry;
	geom->setStateSet( m_MoonStateSet.get() );

	Vec3Array* coords = new Vec3Array(4);
	(*coords)[0].set(-x,0.0f,y);
	(*coords)[1].set(-x,0.0f,-y);
	(*coords)[2].set(x,0.0f,-y);
	(*coords)[3].set(x,0.0f,y);
	geom->setVertexArray(coords);

	Vec2Array* tcoords = new Vec2Array(4);
	(*tcoords)[0].set(0.0f,1.0f);
	(*tcoords)[1].set(0.0f,0.0f);
	(*tcoords)[2].set(1.0f,0.0f);
	(*tcoords)[3].set(1.0f,1.0f);
	geom->setTexCoordArray(0,tcoords);

	osg::Vec4Array* colours = new osg::Vec4Array(1);
	(*colours)[0].set(1.0f,1.0f,1.0,1.0f);
	geom->setColorArray(colours);
	geom->setColorBinding(Geometry::BIND_OVERALL);

	geom->addPrimitiveSet(new DrawArrays(PrimitiveSet::QUADS,0,4));

	m_Moon = geom;
	
	m_Transform = new osg::MatrixTransform;
	addChild( m_Transform.get() );

	m_Billboard = new AstroBillboard;
	m_Billboard->setCullingActive(false);
	m_Billboard->setStateSet( m_MoonStateSet.get() );
	m_Billboard->addDrawable(m_Moon.get());
	m_Billboard->setAxes(osg::Vec3(0.0,1.0,0.0), osg::Vec3(0.0,0.0,1.0), osg::Vec3(0.0,1.0,0.0));
	m_Transform->addChild(m_Billboard.get());
	m_Transform->setStateSet( m_MoonStateSet.get() );
	m_Transform->setCullingActive(false);

}


MoonNode::MoonNode(const MoonNode &mn,const osg::CopyOp &copyop) : 
	osg::Group( mn, copyop )
{
	std::cerr << "MoonNode: Error - Copy constructor is not implemented\n";
}


void MoonNode::updateMoon(
	double lat, double moon_h, double moon_A, double phase, float earthshine,
	float moonRadius, float distance ) 
{
	m_EarthShine = earthshine;

	if (lat != m_Latitude) {
		m_Billboard->setAxes(osg::Vec3(0.0,cos(lat), sin(lat)), osg::Vec3(0.0,0.0,1.0), osg::Vec3(0.0,1.0,0.0));
		m_Latitude = lat;
	}

//FIXME - note to self - the logic here regarding m_DirtyImage has me 
// scratching my head... need to look into it -ADS
	if (m_DirtyImage && moon_h > -0.2) {
		m_DirtyImage = false;
		_maskPhase( phase, 0.0 );
	} else if (moon_h < -0.2 && !m_DirtyImage) {
		m_DirtyImage = true;
	}

	double x, y, z;
	y = -cos(moon_h);
	x = y*sin(moon_A);
	y *= cos(moon_A);
	z = sin(moon_h);

	double scale = m_RenderDistance * moonRadius / distance;
	osg::Matrix S = osg::Matrix::scale(scale, scale, scale);
	osg::Matrix T = osg::Matrix::translate(x*m_RenderDistance, y*m_RenderDistance, z*m_RenderDistance);
	m_Transform->setMatrix(S*T);
}


/**
 * Generate a texture with the correct lunar phase.
 *
 * The original full moon image is masked with alpha equal to 1 in the
 * shadow and a soft terminus.  Beta, the angle of the moon above the
 * ecliptic plane is not used, since the effect is relatively small and
 * the computation much simpler without it.
 */
void MoonNode::_maskPhase(float phi, float beta) 
{
	if( !haveTexture )
	{
		return;
	}
	
	{
		int n = (int) (phi / (2.0*M_PI));
		phi -= n * 2.0 * M_PI;
		//cout << "MOON PHASE = " << phi << endl;
	}
	osg::Image *phased = new osg::Image(*(m_Image.get()), CopyOp::DEEP_COPY_IMAGES);
	int i, j;
	int width = phased->s();
	int height = phased->t();
	unsigned char* data = phased->data();
	// alpha component
	data += 3;
	// adhoc, gives a maximum value of 10 for a full earth (new moon)
	float dark = m_EarthShine * 105.0;
	if (dark < 0.0) dark = 0.0;
	if (dark > 10.0) dark = 10.0;
	unsigned char darkalpha = (unsigned char) dark;
	float terminus = 1000.0;
	float sz = 1.9999 / height;
	float sx = 1.9999 / width;
	assert(phased->getPixelFormat() == GL_RGBA);
	// beta has an extremely small effect on the shading (most likely sub-pixel)
	// and since i'm not certain whether to use + or - beta, i'm just setting it
	// equal to zero for now.
	beta = 0.0;
	float cos_phi = cos(phi);
	float A = sin(phi);
	float B = cos_phi * cos(beta);
	float C = cos_phi * sin(beta);
	for (j = 0; j < height; j++) {
		float z = j*sz-1.0;
		float edge = 1.0-z*z;
		for (i = 0; i < width; i++) {
			float x = i*sx-1.0;
			float x2 = x*x;
			int c = int(*data);
			if (x2 > edge) {
				// outside the disc
				c = 0;
			} else {
				// inside the disc
				float y = sqrt(edge - x2);
				float projection = A*x + B*y + C*z;
				// angle away from the terminus (perpendular to illumination)
				float alpha = -asin(projection);
				int shade = int(alpha * terminus) + 128;
				if (shade < 0) shade = 0; else
				if (shade > 255) shade = 255;
				if (shade < c) c = shade;
				// FIXME relate to earthshine... i'm guessing something like
				// 10-15 would be a good upper bound.
				if (c < darkalpha) c = darkalpha;
			}
			*data = (unsigned char) c;
			data += 4;
		}
	}
	m_Phased = phased;
	m_Texture->setImage(m_Phased.get());
}


void MoonNode::loadTexture( const std::string &filename )
{
	m_Image = osgDB::readImageFile( filename );
	if( !m_Image.valid() )
	{
		std::cerr << "MoonNode: Couldn't load moon texture " << filename << std::endl;
		haveTexture = false;
		return;
	}

	// set up the texture.
	m_Texture = new osg::Texture2D;
	m_Texture->setImage(m_Image.get());

	m_MoonStateSet->setTextureAttributeAndModes(0, m_Texture.get(),osg::StateAttribute::ON);

	haveTexture = true;
}
