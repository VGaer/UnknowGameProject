#ifndef _StateMachine_H_
#define _StateMachine_H_
#include "cocos2d.h"
#include "TimeCounter.h"
#include "State.h"
USING_NS_CC;

class Monster;

class StateMachine {
private:
	Monster*	owner;
	State*	currState;
	State*	preState;
	State*	globalState;
public:
	StateMachine(Monster* sender) :owner(sender),
		currState(NULL), preState(NULL), globalState(NULL)
	{}
	virtual ~StateMachine() {}

	void SetCurrState(State* s) { currState = s; }
	void SetPreState(State* s) { preState = s; }
	void SetGlobalState(State* S) { globalState = S; }
	State* GetCurrState() { return currState; }

	void Update() const {
		if (globalState)
			globalState->Excute(owner);
		if (currState)
			currState->Excute(owner);
	}

	void ChangeState(State* newState) {
		preState = currState;
		currState->Exit(owner);
		currState = newState;
		currState->Enter(owner);
	}
	void RevertToPreState() {
		ChangeState(preState);
	}

	bool IsInstate(const State* sender) const {
		return typeid(currState) == typeid(sender);
	}

	State*  CurrentState()  const { return currState; }
	State*  GlobalState()   const { return globalState; }
	State*  PreviousState() const { return preState; }
};

class Idle : public State {
public:
	Idle() {};
	virtual void Enter(Monster*);
	virtual void Excute(Monster*);
	virtual void Exit(Monster*);
private:	
};

class Patrol : public State {
private:
	Patrol(const Patrol&);
	Patrol& operator=(const Patrol&);
public:
	Patrol() {}
	virtual void Enter(Monster*);
	virtual void Excute(Monster*);
	virtual void Exit(Monster*);
};

class Track : public State {
private:
	Track(const Track&);
	Track& operator=(const Track&);
public:
	Track() {}
	virtual void Enter(Monster*);
	virtual void Excute(Monster*);
	virtual void Exit(Monster*);
};

class Attack : public State {
private:	
	Attack(const Attack&);
	Attack& operator=(const Attack&);

public:
	Attack() {}
	virtual void Enter(Monster*);
	virtual void Excute(Monster*);
	virtual void Exit(Monster*);
};

#endif