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
 *  FILENAME:   IGCtrlProcessor.h
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


#ifndef _IG_CTRL_PROCESSOR_H_
#define _IG_CTRL_PROCESSOR_H_

#include "AllCigi.h"
#include "StateMachine.h"

namespace MPVKernel
{

//=========================================================
//! The class handling the CIGI IG Control packets.
//!
class IGCtrlProcessor : public CigiBaseEventProcessor  
{
public:

	//=========================================================
	//! General Constructor
	//!
	IGCtrlProcessor();

	//=========================================================
	//! General Destructor
	//!
	virtual ~IGCtrlProcessor();


	//=========================================================
	//! Initializes the pointer to the database id number
	//! \param dn - The pointer to the database id number.
	//!
	void setCommandedDatabaseNumber( int *dn ) { CommandedDatabaseNumber = dn; }

	//=========================================================
	//! Initializes the pointer to the state machine
	//! \param sm - The pointer to the state machine
	//!
	void setStateMachine( StateMachine *sm ) { stateMachine = sm; }


	//=========================================================
	//! The callback handler for the CIGI ig control packet
	//! \param Packet - The pointer to the ig control packet object.
	//!
	virtual void OnPacketReceived(CigiBasePacket *Packet);

private:
	

	//=========================================================
	//! A pointer to the database id number
	//!
	int *CommandedDatabaseNumber;

	//=========================================================
	//! A pointer to the state machine
	//!
	StateMachine *stateMachine;

};

};

#endif 
