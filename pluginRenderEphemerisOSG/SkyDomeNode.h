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


#ifndef __SKYDOMENODE_H__
#define __SKYDOMENODE_H__

#include <osg/Group>
#include <osg/Light>
#include <osg/Geometry>
#include <osg/Matrix>
#include <osg/MatrixTransform>


class FalseHorizon;

namespace osg {
	class Texture2D;
}

class SkyDomeNode: public osg::Group {
public:
	SkyDomeNode();

	void init( int numWedges, int numSlices, float *sliceAngles );

	void updateSky( float *skyColors );

protected:
	virtual ~SkyDomeNode();


	osg::ref_ptr<FalseHorizon> m_Horizon;
	osg::ref_ptr<osg::Geometry> m_SkyDome;
	osg::Vec2Array *m_TexCoords;
	osg::Vec4Array *m_Colors;
	osg::Vec4Array *m_HorizonColors;
	osg::ref_ptr<osg::Texture2D> m_SkyDomeTexture;
	osg::ref_ptr<osg::Image> m_SkyDomeTextureImage;

	int m_nlev, m_nseg;
	int m_HorizonIndex;

};

#endif

