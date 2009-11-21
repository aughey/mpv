/*
copyright 2007 Andrew Sampson

mpv state machine

*/

#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_

#include <map>

#include "AllCigi.h"
#include "PluginManager.h"
#include "Blackboard.h"
#include "StateContext.h"

class StateMachine
{
public:

	StateMachine( Blackboard *bb );
	~StateMachine();
	
	//! 
	void act();
	
	//! 
	bool getShouldExit() { return shouldExit; }
	
	//! 
	bool getShouldSendSOF() const;
	
	//! If true, then the Kernel should process just the IGCtrl packets, 
	//! rather than the entire CIGI message.  This is the situation for 
	//! several states, including Standby, DatabaseLoad, Shutdown, etc.
	bool getShouldIgnoreNonIGCtrl() const;
	
	//! Called by the kernel, to indicate which mode the Host has commanded.
	void setIGMode( CigiBaseIGCtrl::IGModeGrp );
	
	//! Called by the kernel, so that the kernel knows what value to put 
	//! in Start-Of-Frame packets.
	CigiBaseSOF::IGModeGrp getIGMode() const;
	
protected:

	//! Examines the variables that influence state transitions, and 
	//! sets the state accordingly.
	void examineInputs();
	
	//! Assigns currentState to a new value.  Convenient for reporting 
	//! state transitions on the console.
	void changeState( SystemState::ID newState );

	//! Contains all the variables that influence state transitions
	StateContext context;
	
	//! Manages the plugins
	PluginManager pluginManager;
	
	Blackboard *blackboard;
	
	//! A boolean indicating whether the program should exit its main loop.
	//! This will be set to true once the state machine has completed the 
	//! Quit state.
	bool shouldExit;
	
	class State
	{
	public:
		State( SystemState::ID id, StateMachine *sm );
		virtual ~State();
		
		virtual void act() = 0;
		
		SystemState::ID getStateID() const { return stateID; }
		CigiBaseSOF::IGModeGrp getIGMode() const { return igMode; }
		bool getShouldSendSOF() const { return shouldSendSOF; }
		bool getShouldIgnoreNonIGCtrl() const { return shouldIgnoreNonIGCtrl; }
		
	protected:
		SystemState::ID stateID;
		
		CigiBaseSOF::IGModeGrp igMode;
		
		StateMachine *stateMachine;

		bool shouldSendSOF;
		
		bool shouldIgnoreNonIGCtrl;
	};
	
	// These forward declarations keep this header tidy.
	class StateInit;
	class StateBlackboardPost;
	class StateBlackboardRetrieve;
	class StateConfigurationLoad;
	class StateConfigurationProcess;
	class StateDatabaseLoad;
	class StateReset;
	class StateStandby;
	class StateOperate;
	class StateDebug;
	class StateShutdown;
	class StateQuit;

	State *currentState;
	
	std::map< SystemState::ID, State * > states;
};

#endif
