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
 *  FILENAME:   AllCigi.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This includes all the Cigi Class Library header files that
 *   are necessary.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  03/29/2004 Greg Basler                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#if !defined(_ALL_CIGI_INCLUDED_)
#define _ALL_CIGI_INCLUDED_

// The X11 developers, in their infinite wisdom, named one of their macros "None".
// This conflicts with some of the enums in CCL.
#undef None

#include "CigiAnimationTable.h"
#include "CigiBaseEntityCtrl.h"
#include "CigiBaseEventProcessor.h"
#include "CigiBaseIGCtrl.h"
#include "CigiBasePacket.h"
#include "CigiBaseSOF.h"
#include "CigiDefaultPacket.h"
#include "CigiEntityCtrlV3.h"
#include "CigiErrorCodes.h"
#include "CigiException.h"
#include "CigiExceptions.h"
#include "CigiHostSession.h"
#include "CigiIGCtrlV3_2.h"
#include "CigiIGSession.h"
#include "CigiIncomingMsg.h"
#include "CigiIO.h"
#include "CigiMessage.h"
#include "CigiOutgoingMsg.h"
#include "CigiSession.h"
#include "CigiSOFV3_2.h"
#include "CigiSwapping.h"

#include "CigiBaseHatHotReq.h"
#include "CigiBaseHatHotResp.h"
#include "CigiHatHotReqV3_2.h"
#include "CigiHatHotRespV3_2.h"
#include "CigiHatHotXRespV3_2.h"
#include "CigiHatReqV1.h"
#include "CigiHatReqV2.h"
#include "CigiHatRespV1.h"
#include "CigiHatRespV2.h"
#include "CigiHotReqV2.h"
#include "CigiHotRespV2.h"

#include "CigiEnvRgnCtrlV3.h"
#include "CigiWeatherCtrlV3.h"

#include "CigiAtmosCtrl.h"
#include "CigiCelestialCtrl.h"


#endif // _ALL_CIGI_INCLUDED_
