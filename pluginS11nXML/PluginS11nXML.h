/** <pre>
 *  MPV Serialization Framework
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


#ifndef _PLUGINS11NXML_H_
#define _PLUGINS11NXML_H_

#include <string>

#include "RefPtr.h"
#include "Plugin.h"

#include "GroupNode.h"


//=========================================================
//! This plugin is currently just for testing.  It will serialize the 
//! s11n node tree in xml format.  It does so by implementing a NodeVisitor 
//! that can generate xml.
//! 
class PluginS11nXML : public Plugin 
{
public:
	//=========================================================
	//! General Constructor
	//! 
	PluginS11nXML();

protected:
	//=========================================================
	//! General Destructor
	//! 
	virtual ~PluginS11nXML() throw();
	
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
	//! Retrieved from the blackboard.
	//!
	mpvs11n::GroupNode *rootNode;


	void doStuff();
};

#endif

