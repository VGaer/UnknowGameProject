#ifndef __gdShow_H__
#define __gdShow_H__

#include "cocos2d.h"
USING_NS_CC;

class gdShow : public Layer
{
public:
	static Scene* createscene();
	CREATE_FUNC(gdShow);
	virtual bool init();
	void ButonCallback(Ref* resender);
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
private:
	Vector<Sprite*> vec_bg_spr;
	MenuItemImage* nextMenuItem;
};


#endif