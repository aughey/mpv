/*
copyright 2007 Andrew Sampson

mpv state machine - state context

*/

#include "StateContext.h"

StateContext::StateContext()
{
	commandedIGMode = CigiBaseIGCtrl::Standby;
	databaseLoadRequested = false;
	databaseLoadComplete = false;
	userRequestedQuit = false;
	userRequestedDebug = false;
}


StateContext::~StateContext()
{
}
