/** <pre>
 *  The MPV Entity Manager plugin
 *  Copyright (c) 2008 
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
 *  2008-07-13 Andrew Sampson
 *      Initial release.  Based on code extracted from PluginSymbologyMgr.
 *  
 *  
 *  </pre>
 */


#ifndef _PROCENTITYCTRL_H_
#define _PROCENTITYCTRL_H_

#include "AllCigi.h"

// forward declaration
class PluginEntityMgr;

//=========================================================
//! This class processes Entity Control packets
//! 
class ProcEntityCtrl : public CigiBaseEventProcessor
{
public:
	//=========================================================
	//! General Constructor
	//! 
	ProcEntityCtrl( PluginEntityMgr *_plugin );
	
	//=========================================================
	//! General Destructor
	//! 
	virtual ~ProcEntityCtrl();
	
	//=========================================================
	//! Callback; processes a packet
	//! 
	virtual void OnPacketReceived( CigiBasePacket *Packet );

private:
	
	//=========================================================
	//! Pointer to parent container, passed in from plugin
	//! 
	PluginEntityMgr *plugin;
};

#endif
