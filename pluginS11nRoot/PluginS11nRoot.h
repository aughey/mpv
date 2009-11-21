/** <pre>
 *  MPV Serialization Framework - Root plugin
 *  Copyright (c) 2008 Andrew Sampson
 *  
 *  This component is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; specifically 
 *  version 2.1 of the License.
 *  
 *  This software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *  
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this software; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *  
 *  
 *  2008-12-29 Andrew Sampson
 *      Initial release
 *  
 *  
 *  </pre>
 */


#ifndef _PLUGINS11NROOT_H_
#define _PLUGINS11NROOT_H_

#include <string>

#include "RefPtr.h"
#include "Plugin.h"

#include "GroupNode.h"


//=========================================================
//! This plugin has a simple but important purpose.  It posts to the 
//! blackboard the root node of the serialization tree.
//! 
class PluginS11nRoot : public Plugin 
{
public:
	//=========================================================
	//! General Constructor
	//! 
	PluginS11nRoot();

protected:
	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginS11nRoot() throw();
	
public:
	//=========================================================
	//! The per-frame processing that this plugin performs.
	//! This plugin's act() does ...
	//! \param state - The current system state
	//! \param stateContext - an object containing all the variables which 
	//!     influence state transitions
	//!
	virtual void act( SystemState::ID state, StateContext &stateContext );
	
	
private:
	
	//=========================================================
	//! The root node of the serialization tree.
	//! Posted to the blackboard.
	//!
	mpv::RefPtr<mpvs11n::GroupNode> rootNode;

};

#endif

