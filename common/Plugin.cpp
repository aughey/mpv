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


#include "Plugin.h"

Plugin::Plugin()
{
	bb_ = NULL;
}


Plugin::~Plugin() throw ()
{
}


void Plugin::setBlackboard( Blackboard *bb )
{
	bb_ = bb;
}


std::list<std::string> Plugin::getDependencies( void )
{
	return dependencies_;
}


void Plugin::act( SystemState::ID state, StateContext &stateContext )
{
}

