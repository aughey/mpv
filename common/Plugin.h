/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2004 The Boeing Company
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
 *  03/29/2004 Andrew Sampson
 *      Initial Release.
 *  
 *  2007-07-01 Andrew Sampson
 *      Changed interface to use new state machine API
 *  
 *  2009-06-17 Andrew Sampson
 *      Split class into header and cpp files
 *  
 * </pre>
 */

#ifndef _MPV_BASE_PLUGIN_INCLUDED_
#define _MPV_BASE_PLUGIN_INCLUDED_

#include <list>
#include <string>

#include <DynamicClass.hpp>

#include "SystemState.h"
#include "StateContext.h"
#include "Blackboard.h"
#include "LicenseInfo.h"
#include "MPVCommonTypes.h"

#if defined(_MSC_VER)
   #pragma warning(push)
   #pragma warning(disable : 4251)
#endif

//=========================================================
//! The base plugin class.
//!
class MPVCMN_SPEC Plugin : public PDL::DynamicClass
{
	DECLARE_DYNAMIC_CLASS( Plugin )

public:
   //=========================================================
   //! General Constructor
   //!
	Plugin();

protected:
   //=========================================================
   //! General Destructor
   //!
	virtual ~Plugin() throw ();

public:

   //==> Basic plugin functionality

   //=========================================================
   //! Sets the pointer used to access the blackboard
   //! \param bb - A pointer to the blackboard object
   //!
	void setBlackboard( Blackboard *bb );

   //=========================================================
   //! Details the dependencies needed by this plugin
   //! \return A list of plugins that this plugin requires.
   //!
	std::list<std::string> getDependencies( void );

	//=========================================================
	//! The per-frame processing that this plugin performs
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );

   //==> Public variables


   //=========================================================
   //! A pointer to the blackboard object
   //!
	Blackboard *bb_;

   //=========================================================
   //! The name of the plugin
   //!
	std::string name_;

   //=========================================================
   //! A list of dependencies on which this plugin relies.
   //!
	std::list<std::string> dependencies_;

	//=========================================================
	//! Licensing information for this plugin.  The contents of this 
	//! need to be set by the child class.
	//!
	LicenseInfo licenseInfo_;

};


#if defined(_MSC_VER)
   #pragma warning(pop)
#endif

#endif
