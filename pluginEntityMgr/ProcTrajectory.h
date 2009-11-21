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
 *  FILENAME:   ProcTrajectory.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *   process the Trajectory definition packets.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  04/04/2004 Greg Basler                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */



#if !defined(_PROC_TRAJECTORY_INCLUDED_)
#define _PROC_TRAJECTORY_INCLUDED_

#include "CigiBaseEventProcessor.h"
#include "Entity.h"
#include <list>

//=========================================================
//! This class processes an incoming trajectory definition packet.
//!
class ProcTrajectory : public CigiBaseEventProcessor  
{
public:


   //==> Management

   //=========================================================
   //! General Constructor
   //!
	ProcTrajectory();

   //=========================================================
   //! General Destructor
   //!
	virtual ~ProcTrajectory();



   //==> Cigi packet reception and processing


   //=========================================================
   //! A CIGI packet call back method processing the trajectory
   //!   definition packet.
   //! \param Packet - A pointer to the incoming packet.
   //!
   virtual void OnPacketReceived(CigiBasePacket *Packet);


   //==> Basic processor functionality


   //=========================================================
   //! Initializes the trajectory definition processor
   //! \param EntityTableIn - A pointer to the Entity jump table.
   //!
   void Init(Entity **EntityTableIn)
   {
      EntityTable = EntityTableIn;
   }

   //=========================================================
   //! The per-frame processing that this object performs
   //!
   void Act(void);

protected:

   //=========================================================
   //! A pointer to the entity jump table
   //!
   Entity **EntityTable;

   //=========================================================
   //! A list of orphaned trajectory definitions
   //!
   std::list<CigiTrajectoryDefV3> OrphTraj;


};

#endif // !defined(_PROC_TRAJECTORY_INCLUDED_)
