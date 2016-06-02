#ifndef _SavePoint_H_
#define _SavePoint_H_

#include "Entity.h"

class SavePoint : public Entity
{
public:
	CREATE_FUNC(SavePoint);
	virtual bool init();
private:
	void addTouchEventListener();
	bool onTouchBegan(Touch*, Event*);
	void onTouchEnded(Touch*, Event*);
};

#endif