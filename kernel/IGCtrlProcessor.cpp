/** <pre>
 *  The Multi-Purpose Viewer
 *  Copyright (c) 2007 The Boeing Company
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
 *  FILENAME:   IGCtrlProcessor.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *   handle the CIGI IG Control packets.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  2007-07-04 Andrew Sampson
 *      Initial release
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#include <iostream>

#include "IGCtrlProcessor.h"

using namespace MPVKernel;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// ================================================
// IGCtrlProcessor
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
IGCtrlProcessor::IGCtrlProcessor()
{
	CommandedDatabaseNumber = NULL;
	stateMachine = NULL;
}

// ================================================
// ~IGCtrlProcessor
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
IGCtrlProcessor::~IGCtrlProcessor()
{

}


// ================================================
// OnPacketReceived
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void IGCtrlProcessor::OnPacketReceived(CigiBasePacket *Packet)
{
	CigiIGCtrlV3_2 *igc = (CigiIGCtrlV3_2 *)(Packet);
	
	int databaseNumber = abs( igc->GetDatabaseID() );
	
	if( CommandedDatabaseNumber != NULL && databaseNumber != 0 )
		*CommandedDatabaseNumber = databaseNumber;
	
	if( stateMachine != NULL )
		stateMachine->setIGMode( igc->GetIGMode() );
}

