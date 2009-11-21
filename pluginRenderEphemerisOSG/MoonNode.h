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

#ifndef __MOON_NODE_H__
#define __MOON_NODE_H__

#include <string>
#include <osg/Group>
#include <osg/Billboard>
#include <osg/Geometry>
//#include <osg/ColorMatrix>
#include <osg/Texture2D>
#include <osg/MatrixTransform>

// OSG's billboard implementation is incomplete as of 0.9.3, and does
// not properly implement axial rotations with an arbitrary 'up' vector.
// This class provides that functionality.
class AstroBillboard: public osg::Billboard {
public:
	virtual bool computeMatrix(osg::Matrix& modelview, const osg::Vec3& eye_local, const osg::Vec3& pos_local) const;
	void setAxes(const osg::Vec3& axis, const osg::Vec3 &up, const osg::Vec3 &normal);
protected:
	osg::Vec3 _onormal;
	osg::Matrix _orient;
};


class MoonNode : public osg::Group 
{
public:
	//=========================================================
	//! General Constructor 
	//!
	MoonNode();

	//=========================================================
	//! Copy constructor 
	//!
	MoonNode(const MoonNode &mn,const osg::CopyOp &copyop=osg::CopyOp::SHALLOW_COPY);

	//=========================================================
	//! Macro that sets up some OSG stuff
	//!
	META_Node(osg, MoonNode);
	
	//=========================================================
	//! 
	//!
	void updateMoon( 
		double lat, double moon_h, double moon_A, double phase, float earthshine,
		float moonRadius, float distance );

	//=========================================================
	//! Loads the specified texture file and applies it to the moon 
	//! geometry.
	//! \param filename - the filename of the texture to load.  can be an 
	//!        absolute path, or some file within the OSG_FILE_PATH
	//!
	void loadTexture( const std::string &filename );

private:

	void _maskPhase(float phase, float beta);

	osg::ref_ptr<osg::MatrixTransform> m_Transform;
	osg::ref_ptr<AstroBillboard> m_Billboard;
	osg::ref_ptr<osg::Image> m_Image;
	osg::ref_ptr<osg::Image> m_Phased;
	osg::ref_ptr<osg::Texture2D> m_Texture;
	osg::ref_ptr<osg::Geometry> m_Moon;
	osg::ref_ptr<osg::StateSet> m_MoonStateSet;

	bool haveTexture;
	bool m_DirtyImage;
	double m_Latitude;
	float m_RenderDistance;
	float m_EarthShine;

};

#endif
