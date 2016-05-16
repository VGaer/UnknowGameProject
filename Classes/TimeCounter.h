#ifndef  __TimeCounter_H__
#define  __TimeCounter_H__

#include "cocos2d.h"
USING_NS_CC;
class TimeCounter : public Node{
public:
	CREATE_FUNC(TimeCounter);
	virtual bool init();
	virtual void update(float dt);
	void start();
	float getCurTime();
private:
	float m_fTime;
};

#endif