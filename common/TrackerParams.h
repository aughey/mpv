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
 *  FILENAME:   TrackerParams.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This struct contains the data necessary to store motion tracker 
 *  state-changes sent by the host.
 *  
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  06/28/2005 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 * </pre>
 *  The Boeing Company
 *  1.0
 */

#ifndef TRACKERPARAMS_H
#define TRACKERPARAMS_H

#include <string.h> // for memset

//=========================================================
//! This struct contains the data necessary to store motion tracker 
//! state-changes sent by the host.
//! 
struct TrackerParams
{
	//=========================================================
	//! Constructor
	//! 
	TrackerParams()
	{
		
		memset( this, 0, sizeof(TrackerParams) );
		
		trackerID = 0;
		enableTracker = true;
		boresight = false;
		enableX = true;
		enableY = true;
		enableZ = true;
		enableRoll = true;
		enablePitch = true;
		enableYaw = true;
		viewGroupID = 0;
		viewID = 0;
		useQuaternion = false;
		
		// Set to zero rotation.
		trackerRotateQuat[0] = 0.0;
		trackerRotateQuat[1] = 0.0;
		trackerRotateQuat[2] = 0.0;
		trackerRotateQuat[3] = 1.0;
		
		// set the matrices to the identity
		boresightPreMtx[0] = boresightPreMtx[5] = 
		boresightPreMtx[10] = boresightPreMtx[15] = 1.0;
		boresightPostMtx[0] = boresightPostMtx[5] = 
		boresightPostMtx[10] = boresightPostMtx[15] = 1.0;
		transformMtx[0] = transformMtx[5] = 
		transformMtx[10] = transformMtx[15] = 1.0;
	}
	
	enum TrackerPositionRequestStatus
	{
		PositionReqNone = 0,
		PositionReqOneShot,
		PositionReqContinuous
	};

	//=========================================================
	//! The specific motion tracker that this set of parameters applies to
	//! 
	int trackerID;
	
	//=========================================================
	//! A flag indicating whether this tracker should be enabled
	//! 
	bool enableTracker;
	
	//=========================================================
	//! A flag indicating that this tracker should be boresighted
	//! 
	bool boresight;
	
	
	//=========================================================
	//! A flag indicating whether the tracker's X-component should be used.
	//! 
	bool enableX;

	//=========================================================
	//! A flag indicating whether the tracker's Y-component should be used.
	//! 
	bool enableY;

	//=========================================================
	//! A flag indicating whether the tracker's Z-component should be used.
	//! 
	bool enableZ;


	//=========================================================
	//! A flag indicating whether the tracker's roll-component should be used.
	//! 
	bool enableRoll;

	//=========================================================
	//! A flag indicating whether the tracker's pitch-component should be used.
	//! 
	bool enablePitch;

	//=========================================================
	//! A flag indicating whether the tracker's yaw-component should be used.
	//! 
	bool enableYaw;
	
	
	//=========================================================
	//! The ID of the View Group that this tracker should manipulate.  If 
	//! this value is 0, it indicates that the tracker should be manipulating 
	//! an individual view, rather than a view group.
	//! 
	int viewGroupID;
	
	//=========================================================
	//! The ID of the View that this tracker should manipulate.  If 
	//! viewGroupID is non-zero, then the tracker should be manipulating a 
	//! View Group rather than an individual View, and this parameter should 
	//! be ignored.
	//! 
	int viewID;
	
	//=========================================================
	//! The Host can request the position and orientation for a motion 
	//! tracker.  This variable stores the status for this.  If the Host 
	//! makes a single ("one-shot") request, this variable will be set to 
	//! PositionReqOneShot, and will be reset to PositionReqNone after the 
	//! request has been processed.  If it is set to PositionReqContinuous, 
	//! it will remain so until the Host changes it.
	//! 
	TrackerPositionRequestStatus positionRequestStatus;
	
	//=========================================================
	//! A flag indicating whether the tracker's orientation is expressed 
	//! using a quaternion.  Note that if useQuaternion is enabled, the 
	//! enableRoll, enablePitch, and enableYaw flags will not be honored.
	//! 
	bool useQuaternion;

	//=========================================================
	//! The tracker's orientation, expressed using a quaternion.  
	//! This value is ignored if useQuaternion is false.
	//! 
	float trackerRotateQuat[4];

	//=========================================================
	//! The tracker's orientation, expressed using Euler angles.
	//! This value is ignored if useQuaternion is true.
	//! Units are degrees.
	//! 
	float trackerRotateRPY[3];

	//=========================================================
	//! An absolute yaw for when yaw 0 doesn't point to the front
	//! of the crew station.
	//! 
	float trackerAbsoluteYaw;
	
	//=========================================================
	//! offset vector for this motion tracker
	//! 
	float trackerOffset[3];
	
	//=========================================================
	//! A matrix that stores part of the boresight transformation for this 
	//! tracker.
	//! The trackerAbsoluteYaw value is used to calculate this matrix.
	//! 
	double boresightPreMtx[16];
	
	//=========================================================
	//! A matrix that stores part of the boresight transformation for this 
	//! tracker.
	//! The above trackerRotate, trackerOffset, etc values are used to 
	//! calculate this matrix.
	//! 
	double boresightPostMtx[16];
	
	//=========================================================
	//! Complete, boresighted transformation matrix for this tracker.
	//! The above trackerRotate, trackerOffset, and boresight matrices are 
	//! used to calculate this matrix.
	//! 
	double transformMtx[16];
	
	//=========================================================
	//! A flag indicating that the contents of trackerTransform are 
	//! valid and non-trivial.  If this is false, then the camera plugin 
	//! should ignore transformMtx.
	//! 
	bool transformMtxValid;
	
};

#endif 
