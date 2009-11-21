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

#include <iostream>
#include "StarCatalog.h"

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////


/*
 * based on a model by Preetham, Shirley, and Smits.
 * see http://www.cs.utah.edu/vissim/papers/sunsky/ for more info
 *
 */


StarCatalog::StarCatalog()
{
	m_MagnitudeCutoff = 5.8;

#include "stars.h"
	
	int numStars = sizeof(starArray) / 6 / sizeof(float);
std::cout << "numstars is " << numStars << std::endl;
	_stars.resize( numStars );
	for( int i = 0; i < numStars; i++ )
	{
		_stars.push_back( Star( 
			starArray[i][0], starArray[i][1], starArray[i][2], 
			starArray[i][3], starArray[i][4], starArray[i][5] ) );
	}
}

#ifdef ADS
SIMDATA_DECLARE_STATIC_OBJECT(StarCatalog)

void StarCatalog::serialize(simdata::Writer &writer) const {
	Object::serialize(writer);
	writer << static_cast<int>(_stars.size());
	std::vector<Star>::const_iterator i;
	for (i = _stars.begin(); i != _stars.end(); i++) {
		i->serialize(writer);
	}
}

void StarCatalog::serialize(simdata::Reader &reader) {
	Object::serialize(reader);
	int n;
	reader >> n;
	_stars.resize(n);
	std::vector<Star>::iterator i;
	for (i = _stars.begin(); i != _stars.end(); i++) {
		i->serialize(reader);
	}
}

void StarCatalog::parseXML(const char *cdata) {
	StringTokenizer t(cdata, " \t\n\r");
	Star s;
	_stars.clear();
	while (s.parse(t)) {
		_stars.push_back(s);
	}
}
#endif

#ifdef ADS
SIMDATA_XML_BEGIN(StarCatalog)
	SIMDATA_DEF("source", m_Source, false)
	SIMDATA_DEF("magnitude_cutoff", m_MagnitudeCutoff, false)
SIMDATA_XML_END
#endif


