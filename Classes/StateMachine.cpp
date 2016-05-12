#include "StateMachine.h"

template<class T>
Idle<T> * Idle<T>::Instance()
{
	static Idle instance;
	return &instance;
}

template<class T>
void Idle<T>::Enter(T * s)
{

}

template<class T>
void Idle<T>::Excute(T * s)
{

}

template<class T>
void Idle<T>::Exit(T * s)
{

}

template<class T>
Move<T> * Move<T>::Instance()
{
	static Move instance;
	return &instance;
}

template<class T>
void Move<T>::Enter(T * s)
{

}

template<class T>
void Move<T>::Excute(T * s)
{

}

template<class T>
void Move<T>::Exit(T * s)
{

}

template<class T>
Attack<T> * Attack<T>::Instance()
{
	static Attack instance;
	return &instance;
}

template<class T>
void Attack<T>::Enter(T * s)
{

}

template<class T>
void Attack<T>::Excute(T * s)
{
}

template<class T>
void Attack<T>::Exit(T * s)
{
}

template<class T>
Dead<T> * Dead<T>::Instance()
{
	static Dead instance;
	return &instance;
}

template<class T>
void Dead<T>::Enter(T * s)
{

}

template<class T>
void Dead<T>::Excute(T * s)
{
}

template<class T>
void Dead<T>::Exit(T * s)
{
}

template<class T>
Track<T> * Track<T>::Instance()
{
	static Track instance;
	return &instance;
}

template<class T>
void Track<T>::Enter(T * s)
{

}

template<class T>
void Track<T>::Excute(T * s)
{
}

template<class T>
void Track<T>::Exit(T * s)
{
}