#ifndef _SavePoint_H_
#define _SavePoint_H_

#include "Entity.h"

class SavePoint : public Entity
{
public:
	CREATE_FUNC(SavePoint);
	virtual bool init();
	void setSceneLockState(bool);
private:
	bool isSaving;
	bool isQuiting;
	bool isSavingEnd;
	bool isQuitingEnd;
	Label* saveBtn;
	Label* quitBtn;
	Node* gameLayer;
	Node* layer; // ¸²¸ÇÕû¸ö²ã
	void save();
	void quit();
	void addTouchEventListener();
	bool onTouchBegan(Touch*, Event*);
	void onTouchEnded(Touch*, Event*);
};

#endif