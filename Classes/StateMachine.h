#pragma once
#include "cocos2d.h"
USING_NS_CC;
#include"State.h"
template<class T>
class StateMachine {
private:
	T*	owner;
	State<T>*	currState;
	State<T>*	preState;
	State<T>*	globalState;
public:
	StateMachine(T* sender) :owner(sender),
		currState(NULL),
		preState(NULL),
		globalState(NULL)
	{}
	virtual ~StateMachine() {}

	void SetCurrState(State<T>* s) { currState = s; }
	void SetPreState(State<T>* s) { preState = s; }
	void SetGlobalState(State<T>* S) { globalState = S; }
	State<T>* GetCurrState() { return currState; }

	void Update() const {
		if (globalState)
			globalState->Excute(owner);
		if (currState)
			currState->Excute(owner);
	}

	void ChangeState(State<T>* newState) {
		preState = currState;
		currState->Exit(owner);
		currState = newState;
		currState->Enter(owner);
	}
	void RevertToPreState() {
		ChangeState(preState);
	}

	bool IsInstate(const State<T>* sender) const {
		return typeid(currState) == typeid(sender);
	}

	State<T>*  CurrentState()  const { return currState; }
	State<T>*  GlobalState()   const { return globalState; }
	State<T>*  PreviousState() const { return preState; }
};

template<class T>
class Idle : public State<T> {
private:
	Idle(){}

	Idle(const Idle&);
	Idle& operator=(const Idle&);

public:
	static Idle* Instance();

	virtual void Enter(T* s);
	virtual void Excute(T* s);
	virtual void Exit(T* s);
};

template<class T>
class Move : public State<T> {
private:
	Move() {}

	Move(const Move&);
	Move& operator=(const Move&);

public:
	static Move* Instance();

	virtual void Enter(T* s);
	virtual void Excute(T* s);
	virtual void Exit(T* s);
};

template<class T>
class Attack : public State<T> {
private:
	Attack() {}

	Attack(const Attack&);
	Attack& operator=(const Attack&);

public:
	static Attack* Instance();

	virtual void Enter(T* s);
	virtual void Excute(T* s);
	virtual void Exit(T* s);
};

template<class T>
class Dead : public State<T> {
private:
	Dead() {}

	Dead(const Dead&);
	Dead& operator=(const Dead&);

public:
	static Dead* Instance();

	virtual void Enter(T* s);
	virtual void Excute(T* s);
	virtual void Exit(T* s);
};

template<class T>
class Track : public State<T> {
private:
	Track() {}

	Track(const Track&);
	Track& operator=(const Track&);

public:
	static Track* Instance();

	virtual void Enter(T* s);
	virtual void Excute(T* s);
	virtual void Exit(T* s);
};

