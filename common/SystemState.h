/*
copyright 2007 Andrew Sampson

mpv state machine - state enumeration

 *  2007-07-01 Andrew Sampson
 *      Changed interface to use new state machine API
*/


#ifndef _SYSTEM_STATE_H_
#define _SYSTEM_STATE_H_

class SystemState
{
public:
	enum ID
	{
		Init,
		BlackboardPost,
		BlackboardRetrieve,
		ConfigurationLoad,
		ConfigurationProcess,
		DatabaseLoad,
		Reset,
		Standby,
		Operate,
		Debug,
		Shutdown,
		Quit
	};
};

#endif

