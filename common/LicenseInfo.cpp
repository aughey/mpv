/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2006 The Boeing Company
 *  
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *  
 *  
 *  FILENAME:   LicenseInfo.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class stores licensing information.  It is used to tag plugins 
 *  as open-source or proprietary.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  2006-12-12 Andrew Sampson
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#include "LicenseInfo.h"

// ================================================
// LicenseInfo
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
LicenseInfo::LicenseInfo()
{
	license = LicenseUnknown;
	origin = "Unknown";
}

// ================================================
// ~LicenseInfo
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
LicenseInfo::~LicenseInfo()
{

}


// ================================================
// checkPair
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
bool checkPair( LicenseInfo::LicenseEnum a1, LicenseInfo::LicenseEnum a2, 
                LicenseInfo::LicenseEnum b1, LicenseInfo::LicenseEnum b2 )
{
	if( (a1 == b1 && a2 == b2) || (a1 == b2 && a2 == b1) )
		return true;
	return false;
}

// ================================================
// isCompatible
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
bool LicenseInfo::isCompatible( 
	const LicenseInfo &other, std::string &explanation )
{
	// NOTICE - I AM NOT A LAWYER.  The results of the isCompatible method are 
	// to be viewed with skepticism.  I have attempted to document the various 
	// license incompatibilities here, but if you are in doubt as to the 
	// legitimacy/legality of a particular MPV configuration, you should get in 
	// touch with your legal department.

	bool result = false;
	explanation = "";
	
	if( other.getLicense() == license )
	{
		// stuff released under the same license is compatible
		// (two components tagged as proprietary, but are from different 
		// origins, are assumed to be OK... let's hope that they have an 
		// nda or something in place)
		result = true;
	}
	else if( 
		checkPair( other.getLicense(), license, LicenseGPL, LicenseLGPL ) || 
		checkPair( other.getLicense(), license, LicenseGPL, LicenseOSG ) ||
		checkPair( other.getLicense(), license, LicenseOSG, LicenseLGPL ) )
	{
		// the various combinations of these open source licenses are 
		// compatible
		result = true;
	}
	else if( 
		checkPair( other.getLicense(), license, LicenseProprietary, LicenseLGPL ) || 
		checkPair( other.getLicense(), license, LicenseProprietary, LicenseOSG ) )
	{
		// proprietary code can be linked with LGPL and LGPL-like (ie OSG) 
		// licensed code
		result = true;
	}
	else if( 
		checkPair( other.getLicense(), license, LicenseProprietary, LicenseGPL ) )
	{
		// proprietary code can not be linked with GPL licensed code, unless 
		// the code for both components is owned by the same entity (though 
		// I'm sure that some would argue that this is a violation regardless 
		// of the identity of the component owner)
		if( other.getOrigin() == origin )
		{
			result = true;
		}
		else
		{
			result = false;
			explanation = "GPL'd code can not be linked with proprietary code.";
		}
	}
	else
	{
		result = false;
		explanation = "There is an unrecognized combination of licenses.  "
			"Perhaps someone forgot to set the license info for a plugin?";
	}
	
	return result;
}


