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
 *  FILENAME:   LicenseInfo.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class stores licensing information.  It is used to tag plugins 
 *  as open-source or proprietary.
 *  NOTICE - I AM NOT A LAWYER.  The results of the isCompatible method are 
 *  to be viewed with skepticism.  I have attempted to document the various 
 *  license incompatibilities here, but if you are in doubt as to the 
 *  legitimacy/legality of a particular MPV configuration, you should get in 
 *  touch with your legal department.
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

#ifndef LICENSE_INFO_H
#define LICENSE_INFO_H

#include <string>

#include "MPVCommonTypes.h"

// NOTICE - I AM NOT A LAWYER.  The results of the isCompatible method are 
// to be viewed with skepticism.  I have attempted to document the various 
// license incompatibilities here, but if you are in doubt as to the 
// legitimacy/legality of a particular MPV configuration, you should get in 
// touch with your legal department.

//=========================================================
//! This class stores licensing information.  It is used to tag plugins
//! as open-source or proprietary.
//!
class MPVCMN_SPEC LicenseInfo
{
public:

	//=========================================================
	//! Enumeration of the most popular licenses
	//!
	enum LicenseEnum
	{
	    LicenseUnknown,
		LicenseGPL,
		LicenseLGPL,
		LicenseOSG,   // the OSG license is a more liberal version of the LGPL
		LicenseProprietary
	};


	//==> Management

	//=========================================================
	//! General Constructor
	//!
	LicenseInfo();

	//=========================================================
	//! General Destructor
	//!
	virtual ~LicenseInfo();
	
	//=========================================================
	//! Returns the license
	//! \return The license
	//!
	LicenseEnum getLicense() const { return license; }
	
	//=========================================================
	//! Returns the vendor or entity that produced this component.
	//! \return The origin of the code for this component
	//!
	std::string getOrigin() const { return origin; }
	
	//=========================================================
	//! Sets the license
	//! \param license_ - The license for this component
	//!
	void setLicense( LicenseEnum license_ ) { license = license_; }
	
	//=========================================================
	//! Sets the vendor or entity that produced this component.
	//! If the component is open source and has been touched by enough hands 
	//! that it's tough to call who wrote it, set this to "Community"
	//! \param origin_ - The origin of the code for this component
	//!
	void setOrigin( std::string origin_ ) { origin = origin_; }
	
	//=========================================================
	//! Compares the licensing info for two components and returns true if 
	//! they are compatible.
	//! \param other - This license info will be compared against "other"
	//! \param explanation - If the license information is not compatible, this 
	//!     string will be populated with an explanation of the incompatibility
	//! \return True if the components are compatible, false otherwise.
	//!
	bool isCompatible( const LicenseInfo &other, std::string &explanation );
	
protected:
	
	//=========================================================
	//! The license under which this component is published
	//!
	LicenseEnum license;
	
	//=========================================================
	//! The vendor or entity that produced this component.  If the component 
	//! is open source and has been touched by enough hands that it's tough 
	//! to call who wrote it, set this to "Community"
	//!
	std::string origin;
	
};

#endif
