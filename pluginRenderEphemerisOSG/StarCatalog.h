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


#ifndef STAR_CATALOG_H
#define STAR_CATALOG_H

#include <string>
#include <vector>
#include <osg/Object>


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////


/*
 * based on a model by Preetham, Shirley, and Smits.
 * see http://www.cs.utah.edu/vissim/papers/sunsky/ for more info
 *
 */


class StarCatalog: public osg::Object {

public:

	META_Object( csp, StarCatalog );
	StarCatalog();

	StarCatalog(const StarCatalog &lhs, const osg::CopyOp&){}
	
	struct Star {
		// Ra = Right Ascension ("celestial longitude")
		// De = Declination ("celestial latitude")
		// I = Intensity
		// r,g,b = star color
		float Ra;
		float De;
		float I;
		float r;
		float g;
		float b;

		Star() {
			Ra = 0.0;
			De = 0.0;
			I = 0.0;
			r = 0.0;
			g = 0.0;
			b = 0.0;
		}
		
		Star( float _Ra, float _De, float _I, float _r, float _g, float _b ) {
			Ra = _Ra;
			De = _De;
			I = _I;
			r = _r;
			g = _g;
			b = _b;
		}
#ifdef ADS
		void serialize(simdata::Writer &writer) const {
			writer << Ra << De << I << r << g << b;
		}
		void serialize(simdata::Reader &reader) {
			reader >> Ra >> De >> I >> r >> g >> b;
		}
		bool parse(StringTokenizer &t) {
			if (t.size() < 6) return false;
			Ra = atof(t.front().c_str());
			t.pop_front();
			De = atof(t.front().c_str());
			t.pop_front();
			I = atof(t.front().c_str());
			t.pop_front();
			r = atof(t.front().c_str());
			t.pop_front();
			g = atof(t.front().c_str());
			t.pop_front();
			b = atof(t.front().c_str());
			t.pop_front();
			return true;
		}
#endif
	};

#ifdef ADS
	SIMDATA_DECLARE_STATIC_OBJECT(StarCatalog)

	void serialize(simdata::Writer &writer) const ;

	void serialize(simdata::Reader &reader) ;

	virtual void parseXML(const char *cdata) ;
#endif

	std::string m_Source;
	float m_MagnitudeCutoff;
	std::vector<Star> _stars;
};


#endif
