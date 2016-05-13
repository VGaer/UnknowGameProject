#include "TimeCounter.h"

bool TimeCounter::init()
{
	m_fTime = 0;
	return true;
}

void TimeCounter::update(float dt)
{
	m_fTime += dt;
}

float TimeCounter::getCurTime()
{
	return m_fTime;
}

void TimeCounter::start()
{
	m_fTime = 0;
	this->scheduleUpdate();
}