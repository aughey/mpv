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
 *  FILENAME:   WildcardSignalProcessor.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class is a CCL "signal processor"; when a packet is received, the 
 *  appropriate method in this class will be called.  This class separates 
 *  incoming packets into two major catagories: host-to-ig and ig-to-host.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  07/27/2004 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#ifndef WILDCARD_SIGNAL_PROCESSOR_H
#define WILDCARD_SIGNAL_PROCESSOR_H

#include "CigiBaseSignalProcessing.h"


//=========================================================
//! 
//!
class WildcardSignalProcessor :public CigiBaseSignalProcessing  
{
public:

	//==> Management

	//=========================================================
	//! General Constructor
	//!
	WildcardSignalProcessor() {   }

	//=========================================================
	//! General Destructor
	//!
	virtual ~WildcardSignalProcessor() {   }

	virtual void OnAnyHostToIG(CigiBasePacket *Packet) {  }
	virtual void OnAnyIGToHost(CigiBasePacket *Packet) {  }

	//==> Incoming Packet Processing Functions

	//==+> Host to IG
	virtual void OnIGCtrl(CigiBasePacket *Packet) { OnAnyHostToIG( Packet ); }
	virtual void OnArtPartCtrl(CigiBasePacket *Packet) { OnAnyHostToIG( Packet ); }
	virtual void OnAtmosCtrl(CigiBasePacket *Packet) { OnAnyHostToIG( Packet ); }
	virtual void OnCelestialCtrl(CigiBasePacket *Packet) { OnAnyHostToIG( Packet ); }
	virtual void OnCollDetSegDef(CigiBasePacket *Packet) { OnAnyHostToIG( Packet ); }
	virtual void OnCollDetVolDef(CigiBasePacket *Packet) { OnAnyHostToIG( Packet ); }
	virtual void OnCompCtrl(CigiBasePacket *Packet) { OnAnyHostToIG( Packet ); }
	virtual void OnConfClampEntityCtrl(CigiBasePacket *Packet) { OnAnyHostToIG( Packet ); }
	virtual void OnEarthModelDef(CigiBasePacket *Packet) { OnAnyHostToIG( Packet ); }
	virtual void OnEntityCtrl(CigiBasePacket *Packet) { OnAnyHostToIG( Packet ); }
	virtual void OnEnvCondReq(CigiBasePacket *Packet) { OnAnyHostToIG( Packet ); }
	virtual void OnEnvCtrl(CigiBasePacket *Packet) { OnAnyHostToIG( Packet ); }
	virtual void OnEnvRgnCtrl(CigiBasePacket *Packet) { OnAnyHostToIG( Packet ); }
	virtual void OnHatReq(CigiBasePacket *Packet) { OnAnyHostToIG( Packet ); }
	virtual void OnHatHotReq(CigiBasePacket *Packet) { OnAnyHostToIG( Packet ); }
	virtual void OnHatHotXReq(CigiBasePacket *Packet) { OnAnyHostToIG( Packet ); }
	virtual void OnHotReq(CigiBasePacket *Packet) { OnAnyHostToIG( Packet ); }
	virtual void OnLosSegReq(CigiBasePacket *Packet) { OnAnyHostToIG( Packet ); }
	virtual void OnLosVectReq(CigiBasePacket *Packet) { OnAnyHostToIG( Packet ); }
	virtual void OnMaritimeSurfaceCtrl(CigiBasePacket *Packet) { OnAnyHostToIG( Packet ); }
	virtual void OnMotionTrackCtrl(CigiBasePacket *Packet) { OnAnyHostToIG( Packet ); }
	virtual void OnPostionReq(CigiBasePacket *Packet) { OnAnyHostToIG( Packet ); }
	virtual void OnRateCtrl(CigiBasePacket *Packet) { OnAnyHostToIG( Packet ); }
	virtual void OnSensorCtrl(CigiBasePacket *Packet) { OnAnyHostToIG( Packet ); }
	virtual void OnShortArtPartCtrl(CigiBasePacket *Packet) { OnAnyHostToIG( Packet ); }
	virtual void OnShortCompCtrl(CigiBasePacket *Packet) { OnAnyHostToIG( Packet ); }
	virtual void OnSpecEffDef(CigiBasePacket *Packet) { OnAnyHostToIG( Packet ); }
	virtual void OnTerrestrialSurfaceCtrl(CigiBasePacket *Packet) { OnAnyHostToIG( Packet ); }
	virtual void OnTrajectory(CigiBasePacket *Packet) { OnAnyHostToIG( Packet ); }
	virtual void OnViewCtrl(CigiBasePacket *Packet) { OnAnyHostToIG( Packet ); }
	virtual void OnViewDef(CigiBasePacket *Packet) { OnAnyHostToIG( Packet ); }
	virtual void OnWaveCtrl(CigiBasePacket *Packet) { OnAnyHostToIG( Packet ); }
	virtual void OnWeatherCtrl(CigiBasePacket *Packet) { OnAnyHostToIG( Packet ); }

	//==+> IG to Host
	virtual void OnSOF(CigiBasePacket *Packet) { OnAnyIGToHost( Packet ); }
	virtual void OnAerosolResp(CigiBasePacket *Packet) { OnAnyIGToHost( Packet ); }
	virtual void OnAnimationStop(CigiBasePacket *Packet) { OnAnyIGToHost( Packet ); }
	virtual void OnCollDetSegResp(CigiBasePacket *Packet) { OnAnyIGToHost( Packet ); }
	virtual void OnCollDetVolResp(CigiBasePacket *Packet) { OnAnyIGToHost( Packet ); }
	virtual void OnEventNotification(CigiBasePacket *Packet) { OnAnyIGToHost( Packet ); }
	virtual void OnHatResp(CigiBasePacket *Packet) { OnAnyIGToHost( Packet ); }
	virtual void OnHatHotResp(CigiBasePacket *Packet) { OnAnyIGToHost( Packet ); }
	virtual void OnHatHotXResp(CigiBasePacket *Packet) { OnAnyIGToHost( Packet ); }
	virtual void OnHotResp(CigiBasePacket *Packet) { OnAnyIGToHost( Packet ); }
	virtual void OnIGMsg(CigiBasePacket *Packet) { OnAnyIGToHost( Packet ); }
	virtual void OnLosResp(CigiBasePacket *Packet) { OnAnyIGToHost( Packet ); }
	virtual void OnLosXResp(CigiBasePacket *Packet) { OnAnyIGToHost( Packet ); }
	virtual void OnMaritimeSurfaceResp(CigiBasePacket *Packet) { OnAnyIGToHost( Packet ); }
	virtual void OnPositionResp(CigiBasePacket *Packet) { OnAnyIGToHost( Packet ); }
	virtual void OnSensorResp(CigiBasePacket *Packet) { OnAnyIGToHost( Packet ); }
	virtual void OnSensorXResp(CigiBasePacket *Packet) { OnAnyIGToHost( Packet ); }
	virtual void OnTerrestrialSurfaceResp(CigiBasePacket *Packet) { OnAnyIGToHost( Packet ); }
	virtual void OnWeatherCondResp(CigiBasePacket *Packet) { OnAnyIGToHost( Packet ); }



	//==+> Unrecognized

	virtual void OnUnrecognized(CigiBasePacket *Packet) {  }

};

#endif
