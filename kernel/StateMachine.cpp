/*
copyright 2007 Andrew Sampson

mpv state machine

*/

#include <iostream>
#include <string>

#include "StateMachine.h"

StateMachine::State::State( SystemState::ID id, StateMachine *sm ) : 
	stateID( id ),
	igMode( CigiBaseSOF::OfflineMaint ),
	stateMachine( sm ),
	shouldSendSOF( true ),
	shouldIgnoreNonIGCtrl( false )
{
	
}

StateMachine::State::~State()
{
	
}


class StateMachine::StateInit : public StateMachine::State
{
public:
	StateInit( SystemState::ID id, StateMachine *sm ) : State( id, sm )
	{
		igMode = CigiBaseSOF::OfflineMaint;
		shouldSendSOF = false;
		shouldIgnoreNonIGCtrl = true;
	}
	
	virtual ~StateInit()
	{
	
	}

	virtual void act()
	{
		stateMachine->pluginManager.loadPlugins();
	}

};

class StateMachine::StateBlackboardPost : public StateMachine::State
{
public:
	StateBlackboardPost( SystemState::ID id, StateMachine *sm ) : State( id, sm )
	{
		igMode = CigiBaseSOF::OfflineMaint;
		shouldSendSOF = false;
		shouldIgnoreNonIGCtrl = true;
	}
	
	virtual ~StateBlackboardPost()
	{
	
	}

	virtual void act()
	{
		// stateMachine->blackboard->unlock();
		stateMachine->pluginManager.act( stateID, stateMachine->context );
		// stateMachine->blackboard->lock();
	}

};

class StateMachine::StateBlackboardRetrieve : public StateMachine::State
{
public:
	StateBlackboardRetrieve( SystemState::ID id, StateMachine *sm ) : State( id, sm )
	{
		igMode = CigiBaseSOF::OfflineMaint;
		shouldSendSOF = false;
		shouldIgnoreNonIGCtrl = true;
	}
	
	virtual ~StateBlackboardRetrieve()
	{
	
	}

	virtual void act()
	{
		stateMachine->pluginManager.act( stateID, stateMachine->context );
	}

};

class StateMachine::StateConfigurationLoad : public StateMachine::State
{
public:
	StateConfigurationLoad( SystemState::ID id, StateMachine *sm ) : State( id, sm )
	{
		igMode = CigiBaseSOF::OfflineMaint;
		shouldSendSOF = false;
		shouldIgnoreNonIGCtrl = true;
	}
	
	virtual ~StateConfigurationLoad()
	{
	
	}

	virtual void act()
	{
		stateMachine->pluginManager.act( stateID, stateMachine->context );
	}

};

class StateMachine::StateConfigurationProcess : public StateMachine::State
{
public:
	StateConfigurationProcess( SystemState::ID id, StateMachine *sm ) : State( id, sm )
	{
		igMode = CigiBaseSOF::OfflineMaint;
		shouldSendSOF = false;
		shouldIgnoreNonIGCtrl = true;
	}
	
	virtual ~StateConfigurationProcess()
	{
	
	}

	virtual void act()
	{
		stateMachine->pluginManager.act( stateID, stateMachine->context );
	}

};

class StateMachine::StateDatabaseLoad : public StateMachine::State
{
public:
	StateDatabaseLoad( SystemState::ID id, StateMachine *sm ) : State( id, sm )
	{
		igMode = CigiBaseSOF::Operate;
		shouldIgnoreNonIGCtrl = true;
	}
	
	virtual ~StateDatabaseLoad()
	{
	
	}

	virtual void act()
	{
		stateMachine->pluginManager.act( stateID, stateMachine->context );
	}

};

class StateMachine::StateReset : public StateMachine::State
{
public:
	StateReset( SystemState::ID id, StateMachine *sm ) : State( id, sm )
	{
		igMode = CigiBaseSOF::Reset;
		shouldIgnoreNonIGCtrl = true;
	}
	
	virtual ~StateReset()
	{
	
	}

	virtual void act()
	{
		stateMachine->pluginManager.act( stateID, stateMachine->context );
	}

};

class StateMachine::StateStandby : public StateMachine::State
{
public:
	StateStandby( SystemState::ID id, StateMachine *sm ) : State( id, sm )
	{
		igMode = CigiBaseSOF::Standby;
		shouldIgnoreNonIGCtrl = true;
	}
	
	virtual ~StateStandby()
	{
	
	}

	virtual void act()
	{
		stateMachine->pluginManager.act( stateID, stateMachine->context );
	}

};

class StateMachine::StateOperate : public StateMachine::State
{
public:
	StateOperate( SystemState::ID id, StateMachine *sm ) : State( id, sm )
	{
		igMode = CigiBaseSOF::Operate;
	}
	
	virtual ~StateOperate()
	{
	
	}

	virtual void act()
	{
		stateMachine->pluginManager.act( stateID, stateMachine->context );
	}

};

//! For now, Debug is the same as Operate
class StateMachine::StateDebug : public StateMachine::StateOperate
{
public:
	StateDebug( SystemState::ID id, StateMachine *sm ) : StateOperate( id, sm )
	{
		igMode = CigiBaseSOF::debug;
	}
	
	virtual ~StateDebug()
	{
	
	}

};

class StateMachine::StateShutdown : public StateMachine::State
{
public:
	StateShutdown( SystemState::ID id, StateMachine *sm ) : State( id, sm )
	{
		igMode = CigiBaseSOF::OfflineMaint;
		shouldSendSOF = false;
		shouldIgnoreNonIGCtrl = true;
	}
	
	virtual ~StateShutdown()
	{
	
	}

	virtual void act()
	{
		stateMachine->pluginManager.act( stateID, stateMachine->context );
	}

};

class StateMachine::StateQuit : public StateMachine::State
{
public:
	StateQuit( SystemState::ID id, StateMachine *sm ) : State( id, sm )
	{
		igMode = CigiBaseSOF::OfflineMaint;
		shouldSendSOF = false;
		shouldIgnoreNonIGCtrl = true;
	}
	
	virtual ~StateQuit()
	{
	
	}

	virtual void act()
	{
//		stateMachine->pluginManager.closeAllPlugins();
		stateMachine->shouldExit = true;
	}

};


StateMachine::StateMachine( Blackboard *bb ) : 
	blackboard( bb ),
	shouldExit( false ),
	currentState( NULL )
{
	states[SystemState::Init] = new StateInit( SystemState::Init, this );
	states[SystemState::BlackboardPost] = new StateBlackboardPost( SystemState::BlackboardPost, this );
	states[SystemState::BlackboardRetrieve] = new StateBlackboardRetrieve( SystemState::BlackboardRetrieve, this );
	states[SystemState::ConfigurationLoad] = new StateConfigurationLoad( SystemState::ConfigurationLoad, this );
	states[SystemState::ConfigurationProcess] = new StateConfigurationProcess( SystemState::ConfigurationProcess, this );
	states[SystemState::DatabaseLoad] = new StateDatabaseLoad( SystemState::DatabaseLoad, this );
	states[SystemState::Reset] = new StateReset( SystemState::Reset, this );
	states[SystemState::Standby] = new StateStandby( SystemState::Standby, this );
	states[SystemState::Operate] = new StateOperate( SystemState::Operate, this );
	states[SystemState::Debug] = new StateDebug( SystemState::Debug, this );
	states[SystemState::Shutdown] = new StateShutdown( SystemState::Shutdown, this );
	states[SystemState::Quit] = new StateQuit( SystemState::Quit, this );
	
	pluginManager.setBlackboard( blackboard );
}

StateMachine::~StateMachine()
{
	std::map< SystemState::ID, State * >::iterator iter;
	for( iter = states.begin(); iter != states.end(); iter++ )
	{
		delete (*iter).second;
	}
	states.clear();
}


void StateMachine::act()
{
	examineInputs();

	if( currentState == NULL )
		throw std::string("Invalid state");

	currentState->act();
}


void StateMachine::examineInputs()
{
	if( currentState == NULL )
		changeState( SystemState::Init );
	else
	{
		switch( currentState->getStateID() )
		{
		case SystemState::Init:
			changeState( SystemState::BlackboardPost );
			break;
		case SystemState::BlackboardPost:
			changeState( SystemState::BlackboardRetrieve );
			break;
		case SystemState::BlackboardRetrieve:
			changeState( SystemState::ConfigurationLoad );
			break;
		case SystemState::ConfigurationLoad:
			changeState( SystemState::ConfigurationProcess );
			break;
		case SystemState::ConfigurationProcess:
			changeState( SystemState::Reset );
			break;
		case SystemState::DatabaseLoad:
			if( context.databaseLoadComplete )
			{
				context.databaseLoadComplete = false;
				if( context.commandedIGMode == CigiBaseIGCtrl::debug )
					changeState( SystemState::Debug );
				else
					changeState( SystemState::Operate );
			}
			else if( context.userRequestedQuit )
			{
				changeState( SystemState::Shutdown );
			}
			break;
		case SystemState::Reset:
			changeState( SystemState::Standby );
			break;
		case SystemState::Standby:
			if( context.userRequestedQuit )
			{
				changeState( SystemState::Shutdown );
			}
			else if( context.commandedIGMode == CigiBaseIGCtrl::Operate )
			{
				changeState( SystemState::Operate );
			}
			else if( context.commandedIGMode == CigiBaseIGCtrl::debug )
			{
				changeState( SystemState::Debug );
			}
			else if( context.userRequestedDebug )
			{
				context.commandedIGMode = CigiBaseIGCtrl::debug;
				changeState( SystemState::Debug );
			}
			break;
		case SystemState::Operate:
			if( context.userRequestedQuit )
			{
				changeState( SystemState::Shutdown );
			}
			else if( context.commandedIGMode == CigiBaseIGCtrl::Reset )
			{
				changeState( SystemState::Reset );
			}
			else if( context.databaseLoadRequested )
			{
				context.databaseLoadRequested = false;
				changeState( SystemState::DatabaseLoad );
			}
			else if( context.commandedIGMode == CigiBaseIGCtrl::debug )
			{
				changeState( SystemState::Debug );
			}
			break;
		case SystemState::Debug:
			if( context.userRequestedQuit )
			{
				changeState( SystemState::Shutdown );
			}
			else if( context.commandedIGMode == CigiBaseIGCtrl::Reset )
			{
				changeState( SystemState::Reset );
			}
			else if( context.databaseLoadRequested )
			{
				context.databaseLoadRequested = false;
				changeState( SystemState::DatabaseLoad );
			}
			else if( context.commandedIGMode == CigiBaseIGCtrl::Operate )
			{
				changeState( SystemState::Operate );
			}
			break;
		case SystemState::Shutdown:
			// FIXME - should spend several frames in this state
			changeState( SystemState::Quit );
			break;
		case SystemState::Quit:
			currentState = NULL;
			break;
		default:
			break;
		}
	}
}

void StateMachine::changeState( SystemState::ID newState )
{
	currentState = states[newState];
	
	std::cout << "** Transitioned to state ";
	std::string stateName;
	switch( newState )
	{
	case SystemState::Init:
		stateName = "Init";
		break;
	case SystemState::BlackboardPost:
		stateName = "BlackboardPost";
		break;
	case SystemState::BlackboardRetrieve:
		stateName = "BlackboardRetrieve";
		break;
	case SystemState::ConfigurationLoad:
		stateName = "ConfigurationLoad";
		break;
	case SystemState::ConfigurationProcess:
		stateName = "ConfigurationProcess";
		break;
	case SystemState::DatabaseLoad:
		stateName = "DatabaseLoad";
		break;
	case SystemState::Reset:
		stateName = "Reset";
		break;
	case SystemState::Standby:
		stateName = "Standby";
		break;
	case SystemState::Operate:
		stateName = "Operate";
		break;
	case SystemState::Debug:
		stateName = "Debug";
		break;
	case SystemState::Shutdown:
		stateName = "Shutdown";
		break;
	case SystemState::Quit:
		stateName = "Quit";
		break;
	default:
		stateName = "UNKNOWN";
		break;
	}
	std::cout << stateName << std::endl;
}


bool StateMachine::getShouldSendSOF() const
{
	if( currentState == NULL )
		return false;
	return currentState->getShouldSendSOF();
}

bool StateMachine::getShouldIgnoreNonIGCtrl() const
{
	if( currentState == NULL )
		return false;
	return currentState->getShouldIgnoreNonIGCtrl();
}

void StateMachine::setIGMode( CigiBaseIGCtrl::IGModeGrp mode )
{
	context.commandedIGMode = mode;
}

CigiBaseSOF::IGModeGrp StateMachine::getIGMode() const
{
	if( currentState == NULL )
		return CigiBaseSOF::OfflineMaint;
	return currentState->getIGMode();
}
