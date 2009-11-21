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
 *  FILENAME:   PositionRequestP.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  listen for Position Request packets and act on 
 *  requests for position info from motion trackers.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  10/16/2005 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#ifndef POSITIONREQUESTP_H
#define POSITIONREQUESTP_H

#include <map>

#include "AllCigi.h"
#include <CigiPositionReqV3.h>

#include "TrackerParams.h"

namespace MotionTrackerMgr
{

//=========================================================
//! This class listens for Position Request packets and acts on 
//! requests for position info from motion trackers.
//! 
class PositionRequestP : public CigiBaseEventProcessor  
{
public:

	//=========================================================
	//! General Constructor
	//! 
	PositionRequestP();

	//=========================================================
	//! General Destructor
	//! 
	virtual ~PositionRequestP();
	
	//=========================================================
	//! Initializes trackerParamsMap
	//! \param tpm - the new value for trackerParamsMap
	//! 
	void init( std::map< int, TrackerParams * > *tpm ) 
	{ trackerParamsMap = tpm; }
	
	//=========================================================
	//! Callback; processes a packet
	//! 
	virtual void OnPacketReceived(CigiBasePacket *Packet);

private:
	
	//=========================================================
	//! Map of TrackerParams, passed in from plugin
	//! 
	std::map< int, TrackerParams * > *trackerParamsMap;
	
};

}

#endif 
