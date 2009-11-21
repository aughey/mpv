/*
copyright 2007 Andrew Sampson

mpv state machine - state context

*/

#ifndef _STATE_CONTEXT_H_
#define _STATE_CONTEXT_H_

#include <CigiBaseIGCtrl.h>

#include "MPVCommonTypes.h"

//! Contains all the variables that influence state transitions
class MPVCMN_SPEC StateContext
{
public:
	StateContext();
	~StateContext();
	
	// only the kernel is permitted to modify these:
	CigiBaseIGCtrl::IGModeGrp commandedIGMode;
	bool databaseLoadRequested;
	
	// plugins are allowed to modify these:
	bool databaseLoadComplete;
	bool userRequestedQuit;
	bool userRequestedDebug;
};

#endif
