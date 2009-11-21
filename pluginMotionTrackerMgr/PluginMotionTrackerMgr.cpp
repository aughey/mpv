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
 *  FILENAME:   PluginMotionTrackerMgr.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Multi-Purpose Viewer
 *  
 *  PROGRAM DESCRIPTION: 
 *  This class contains the data and methods necessary to
 *  listen for MotionTrackerControl packets and make the 
 *  information available to other plugins via the blackboard.
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  06/28/2005 Andrew Sampson                       MPV_CR_DR_1
 *  Initial Release.
 *
 *  2007-07-14 Andrew Sampson
 *      Changed interface to use new state machine API
 *
 * </pre>
 *  The Boeing Company
 *  1.0
 */


#include <iostream>
#include <string.h> // for memcpy

#ifdef WIN32
#define isnan _isnan
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include <osg/Matrix>
#include <osg/Vec3d>
#include <osg/Vec3>
#include <osg/Quat>

#include <CigiPositionRespV3.h>

#include "PluginMotionTrackerMgr.h"
#include "MPVExceptions.h"

// Prototypes
void Camera_to_Euler(const osg::Quat &q, osg::Vec3d &euler);
void convertRotMtxToEuler( const osg::Matrix &m, osg::Vec3 &euler );


EXPORT_DYNAMIC_CLASS( PluginMotionTrackerMgr )

// ================================================
// PluginMotionTrackerMgr
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginMotionTrackerMgr::PluginMotionTrackerMgr() : Plugin() 
{
	name_ = "PluginMotionTrackerMgr";
	licenseInfo_.setLicense( LicenseInfo::LicenseGPL );
	licenseInfo_.setOrigin( "Boeing" );
	dependencies_.push_back( "PluginDefFileReader" );

	ImsgPtr = NULL;
	OmsgPtr = NULL;
	
}

// ================================================
// ~PluginMotionTrackerMgr() throw() 
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
PluginMotionTrackerMgr::~PluginMotionTrackerMgr() throw() 
{
	// delete contents of trackerParamsMap
	std::map< int, TrackerParams * >::iterator tpmIter;
	for( tpmIter = trackerParamsMap.begin(); 
		tpmIter != trackerParamsMap.end(); tpmIter++ )
	{
		if( (*tpmIter).second != NULL )
			delete (*tpmIter).second;
	}

}

// ================================================
// act
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginMotionTrackerMgr::act( SystemState::ID state, StateContext &stateContext )
{
	switch( state )
	{
	case SystemState::BlackboardPost:
		// This state is for posting things to the blackboard and other 
		// initialization tasks
		
		// post the trackerParams
		bb_->put( "TrackerParamsMap", &trackerParamsMap );
		break;

	case SystemState::BlackboardRetrieve:
		// This state is for retrieving things from the blackboard

		bb_->get( "DefinitionData", DefFileData );

		bb_->get( "CigiIncomingMsg", ImsgPtr );

		bb_->get( "CigiOutgoingMsg", OmsgPtr );
		
		if( ImsgPtr != NULL ) {

			ImsgPtr->RegisterEventProcessor(
				CIGI_MOTION_TRACK_CTRL_PACKET_ID_V3,
				(CigiBaseEventProcessor *) &motionTrackerCtrlP);
			
			ImsgPtr->RegisterEventProcessor(
				CIGI_POSITION_REQ_PACKET_ID_V3,
				(CigiBaseEventProcessor *) &positionRequestP);
			
			motionTrackerCtrlP.init( &trackerParamsMap );
			positionRequestP.init( &trackerParamsMap );

		}
		break;

	case SystemState::ConfigurationProcess:
		processConfigData();
		break;

	case SystemState::Operate:
	case SystemState::Debug:
		for( std::map< int, TrackerParams * >::iterator iter = trackerParamsMap.begin(); 
			iter != trackerParamsMap.end(); iter++ )
		{
			TrackerParams *tp = (*iter).second;

			// skip over trackers that do not exist
			if( tp == NULL )
				continue;

			// skip over trackers that are not enabled
			if( !tp->enableTracker )
			{
				tp->transformMtxValid = false;
				continue;
			}

			processTracker( tp );
			replyToPositionRequests( tp );
		}
		break;

	default:
		break;
	}
	
}


// ================================================
// processConfigData
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginMotionTrackerMgr::processConfigData( void )
{
	DefFileGroup *root = *DefFileData;

	if(root == NULL)
	{
		return;
	}

	
}


// ================================================
// processTracker
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginMotionTrackerMgr::processTracker( TrackerParams *trackerParams )
{

	// NOTE - this function does not honor the enableRoll,Pitch,Yaw flags when 
	// the trackerparams is using quaternions

	bool trackerMtxIsNonTrivial = false;
	osg::Matrix boresightPreMtx;
	osg::Matrix boresightPostMtx;
	osg::Quat trackerRotateQuat;
	osg::Matrix trackerOffsetMtx;
	osg::Vec3d trackerOffsetMask;

	if( trackerParams->useQuaternion )
	{
		trackerRotateQuat.set(
			trackerParams->trackerRotateQuat[0],
			trackerParams->trackerRotateQuat[1],
			trackerParams->trackerRotateQuat[2],
			trackerParams->trackerRotateQuat[3]);
	}
	else
	{
		double yaw = osg::DegreesToRadians((double)(trackerParams->trackerRotateRPY[2])) * 
			(trackerParams->enableYaw ? 1.0 : 0.0 );
		double pitch = osg::DegreesToRadians((double)(trackerParams->trackerRotateRPY[1])) * 
			(trackerParams->enablePitch ? 1.0 : 0.0 );
		double roll = osg::DegreesToRadians((double)(trackerParams->trackerRotateRPY[0])) * 
			(trackerParams->enableRoll ? 1.0 : 0.0 );
		
		trackerRotateQuat.makeRotate( 
			yaw         , osg::Vec3d( 0, 0, 1 ),
			pitch * -1.0, osg::Vec3d( 1, 0, 0 ),
			roll  * -1.0, osg::Vec3d( 0, 1, 0 ) );
	}
	
	trackerOffsetMask[0] = trackerParams->enableX ? 1.0 : 0.0;
	trackerOffsetMask[1] = trackerParams->enableY ? 1.0 : 0.0;
	trackerOffsetMask[2] = trackerParams->enableZ ? 1.0 : 0.0;
	trackerOffsetMtx = osg::Matrix::translate( 
		(double)(-1.0 * trackerOffsetMask[0] * trackerParams->trackerOffset[0]),
		(double)(-1.0 * trackerOffsetMask[1] * trackerParams->trackerOffset[1]),
		(double)(-1.0 * trackerOffsetMask[2] * trackerParams->trackerOffset[2]) );

	// copy boresight matrices out of trackerParams
	boresightPreMtx.set( trackerParams->boresightPreMtx );
	boresightPostMtx.set( trackerParams->boresightPostMtx );

	// check to see if boresight is enabled
	if( trackerParams->boresight )
	{
		/* The order is,
		 * sensor^-1 * current tracker * boresight^-1 * sensor
		 * where sensor is a rotation matrix with only the yaw
		 * of the boresight angles being used (because the InterSense
		 * tracker has an absolute yaw reference and not a
		 * relative yaw reference).
		 * When a boresight is completed,
		 * boresightPreMtx = sensor^-1
		 * boresightPostMtx = boresight^-1 * sensor
		 */

		osg::Matrix boresight = trackerOffsetMtx;
		boresight *= osg::Matrix(trackerRotateQuat);

		osg::Matrix sensor = osg::Matrix::rotate(
			osg::DegreesToRadians(trackerParams->trackerAbsoluteYaw),
				osg::Vec3d( 0., 0., 1. ) );

		boresightPreMtx = osg::Matrix::inverse(sensor);
		boresightPostMtx = osg::Matrix::inverse(boresight) * sensor;
		
		// copy boresightPreMtx and boresightPostMtx back into trackerParams
		memcpy( trackerParams->boresightPreMtx, boresightPreMtx.ptr(), sizeof(double)*16 );
		memcpy( trackerParams->boresightPostMtx, boresightPostMtx.ptr(), sizeof(double)*16 );
	}
	/*
	osg::Vec3d ypr;
	Camera_to_Euler(trackerRotateQuat, ypr);
	printf("ypr %f %f %f\n", ypr.x(), ypr.y(), ypr.z());
	*/


	// test the offset and rotation values to see if we really need to 
	// apply the additional transforms to the camMtx
	if( trackerParams->trackerOffset[0] != 0.0 || 
		trackerParams->trackerOffset[1] != 0.0 || 
		trackerParams->trackerOffset[2] != 0.0 )
		trackerMtxIsNonTrivial = true;
	else if( !trackerRotateQuat.zeroRotation() )
		trackerMtxIsNonTrivial = true;

	if( trackerMtxIsNonTrivial ) 
	{
		osg::Matrix trackerMatrix = trackerOffsetMtx;
	
		trackerMatrix *= osg::Matrix(trackerRotateQuat);
		trackerMatrix = boresightPreMtx * trackerMatrix * boresightPostMtx;

/*osg::Vec3 ypr;
convertRotMtxToEuler( trackerMatrix, ypr );
printf("rpy %f %f %f\n", ypr.x(), ypr.y(), ypr.z());*/

		// copy trackerMatrix into trackerParams->transformMtx
		memcpy( trackerParams->transformMtx, trackerMatrix.ptr(), sizeof(double)*16 );

		trackerParams->transformMtxValid = true;
	}
	else
	{
		trackerParams->transformMtxValid = false;
	}

}


// ================================================
// replyToPositionRequests
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void PluginMotionTrackerMgr::replyToPositionRequests( TrackerParams *trackerParams )
{
	if( trackerParams->positionRequestStatus != TrackerParams::PositionReqNone )
	{
		CigiPositionRespV3 response;
		
		response.SetObjectClass( CigiBasePositionResp::MotionTracker );
		response.SetObjectID( trackerParams->trackerID );
		
		// FIXME - this method sends the un-boresighted position and orientation
		// and as a result, also doesn't honor the enable x,y,z,r,p,y flags
		response.SetLatOrXoff( trackerParams->trackerOffset[0] );
		response.SetLonOrYoff( trackerParams->trackerOffset[1] );
		response.SetAltOrZoff( trackerParams->trackerOffset[2] );
		
		if( trackerParams->useQuaternion )
		{
			// FIXME - quaternions aren't easily converted to eulers
			response.SetRoll( 0.0 );
			response.SetPitch( 0.0 );
			response.SetYaw( 0.0 );
		}
		else
		{
			response.SetRoll( trackerParams->trackerRotateRPY[0] );
			response.SetPitch( trackerParams->trackerRotateRPY[1] );
			response.SetYaw( trackerParams->trackerRotateRPY[2] );
		}
		
		(*OmsgPtr) << response;
		
		// clear the flag, now that we've processed the request
		if( trackerParams->positionRequestStatus == TrackerParams::PositionReqOneShot )
			trackerParams->positionRequestStatus = TrackerParams::PositionReqNone;
	}
}


void NED_to_Euler(const osg::Matrix &m, osg::Vec3d &euler)
{
	/* Remember osg::Matrix operator() is column major, m(column, row)
	 */
	double ratio, pitch, c_p, yaw, roll; 
	pitch = asin(-m(0,2));
	c_p = cos(pitch);

	/* avoid divide by zero */
	const double limit = 1e-8;
	if(c_p >= 0 && c_p < limit)
		c_p = limit;
	if(c_p <= 0 && c_p >-limit)
		c_p = -limit;
	bool positive_cos_pitch = c_p > 0;

	/* check for out of bounds values before doing acos */
	ratio = m(0,0)/c_p;
	if(ratio < -1)
		yaw = M_PI;
	else if(ratio > 1)
		yaw = 0;
	else
		yaw = acos(ratio);

	/* If asin(m(0,1)/c_p) is negative, yaw = 2*PI - yaw asin is
	 * negative if it's argument is negative, the argument is
	 * negative if the sign of numerator or denominator differ, so
	 * just check that and skip the cost of the math.
	 */
	if((m(0,1) > 0) != positive_cos_pitch)
		yaw = 2* M_PI - yaw;

	/* check for out of bounds values before doing acos */
	ratio = m(2,2)/c_p;
	if(ratio < -1)
		roll = M_PI;
	else if(ratio > 1)
		roll = 0;
	else 
		roll = acos(ratio);
	/* Same goes for roll as for yaw. */
	if((m(1,2) > 0) != positive_cos_pitch)
		roll = 2*M_PI - roll;
	euler.x() = yaw;
	euler.y() = pitch;
	euler.z() = roll;

	// Debugging
	if(isnan(yaw) || isnan(pitch) || isnan(roll))
	{
		printf("pitch %f, cos(pitch) %f, m(0,0) %f, m(0,1) %f\n",
			pitch, c_p, m(0,0), m(0,1));
		printf("m(0,0)/c_p %f, m(0,1)/c_p %f\n",
			m(0,0)/c_p, m(0,1)/c_p);
		const double yaw2 = asin(m(0,1)/c_p);

		printf("pitch %f, cos(pitch) %f, m(2,2) %f, m(1,2) %f\n",
			pitch, c_p, m(1,2), m(2,2));
		printf("m(2,2)/c_p %f, m(1,2)/c_p %f\n",
			m(2,2)/c_p, m(1,2)/c_p);
		const double roll2 = asin(m(1,2)/c_p);

		printf("yaw %f or %f, pitch %f, roll %f or %f\n",
			osg::RadiansToDegrees(yaw),
			osg::RadiansToDegrees(yaw2),
			osg::RadiansToDegrees(pitch),
			osg::RadiansToDegrees(roll),
			osg::RadiansToDegrees(roll2));
	}
}

void Camera_to_Euler(const osg::Quat &q, osg::Vec3d &euler)
{
	/* The matrix produced by doing the camera rotations, that is
	 * yaw around z, negative pitch around y, negative roll around
	 * x differs by a transpose and a negative cos(pitch)*sin(yaw)
	 * term in the column 1 row 0 position for the five matrix
	 * elements that NED_to_Euler looks at.
	 * The North East Down matrix is produced by roll around x
	 * pitch around y, yaw around z rotation order.
	 */
	osg::Matrix cam(q);
	osg::Matrix ned(
		cam(0,0),	-1*cam(1,0),	1*cam(2,0),	0,
		0,		0,		cam(2,1),	0,
		0,		0,		cam(2,2),	0,
		0,		0,		0,		0);

	NED_to_Euler(ned, euler);

	#if DEBUG > 10
	//printMatrix(cam, "Before transpose and negation\n");
	printMatrix(ned, "After transpose and negation\n");
	#endif
}



#define ARCCOS_SANITY( theta ) \
if( theta > 0.9999 ) theta = 0.9999;\
if( theta < -0.9999 ) theta = -0.9999;

// ================================================
// convertRotMtxToEuler
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void convertRotMtxToEuler( const osg::Matrix &m, osg::Vec3 &euler ) 
{
	osg::Vec3 yaxis( 0., 1., 0. ), zaxis( 0., 0., 1. );
	float heading, pitch, roll;
	
	osg::Vec3 headingv, pitchv, hpnormalv, v;

	pitchv = m.preMult( yaxis );
	headingv = pitchv;
	// zero-out the z component
	headingv[2] = 0.;
	// normalize
	headingv.normalize();
	
	/////////////////////////////////////////////////////////////////
	// HEADING
	/////////////////////////////////////////////////////////////////
	
	// the dot product of the two vectors will get us the cosine of the 
	// angle between them
	float cosH = headingv * yaxis; // dot product
	ARCCOS_SANITY( cosH )
	heading = acos(cosH);
	
	// the cross product of the two vectors will get us the vector normal 
	// to them
	v = headingv ^ yaxis; // cross product
	if( v[2] > 0. )
	{
		heading *= -1.;
	}
	
	
	/////////////////////////////////////////////////////////////////
	// PITCH
	/////////////////////////////////////////////////////////////////
	
	float cosP = pitchv * headingv; // dot product
	ARCCOS_SANITY( cosP )
	pitch = acos(cosP);

	if( pitchv[2] < 0. )
	{
		pitch *= -1.;
	}


	/////////////////////////////////////////////////////////////////
	// ROLL
	/////////////////////////////////////////////////////////////////
	
	hpnormalv = pitchv ^ headingv;
	hpnormalv.normalize();
	// If pitch is negative, hpnormalv will point in the "wrong" direction.
	// In this situation, we'll negate hpnormalv.  If we didn't do this, 
	// we'd get roll values that change sign as pitch changes from positive to 
	// negative or vice versa (yuck).
	if( pitch < 0. )
		hpnormalv *= -1.;
		
	osg::Vec3 zm;
	zm = m.preMult( zaxis );

/*
cout << "vv color " << "zm" << " 0.5 0.5 1.0\n";
cout << "vv set zm " << zm  << endl;
cout << "vv color pitchv 1 0 1\n";
cout << "vv set pitchv " << pitchv  << endl;
cout << "vv color headingv 1 .5 1\n";
cout << "vv set headingv " << headingv  << endl;
cout << "vv color " << "hpnormalv" << " 1. 1. 1.\n";
cout << "vv set hpnormalv " << hpnormalv << endl;
*/
	
	float cosR = zm * hpnormalv;
	ARCCOS_SANITY( cosR )
	roll = acos(cosR);
	// if the normal is roughly coincident with pitchv, then we need 
	// to negate the sign of the roll angle
	osg::Vec3 rollplanenormalv = zm ^ hpnormalv;
	rollplanenormalv.normalize();
//cout << "vv color " << "rollplanenormalv" << " .3 .3 .3\n";
//cout << "vv set rollplanenormalv " << rollplanenormalv << endl;
	if( pitchv * rollplanenormalv > 0. )
	{
		roll *= -1.;
	}
	
	euler.set( roll, pitch, heading );
//cout << "r p h " << euler << endl;
}




