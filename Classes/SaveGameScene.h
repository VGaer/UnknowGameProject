#ifndef _SaveGameScene_H_
#define _SaveGameScene_H_

#include "cocos2d.h"
#include "ui\CocosGUI.h"
using namespace cocos2d::ui;
USING_NS_CC;

class SaveGameScene :public Layer
{
public:
	static Scene* scene(RenderTexture* sqr);
	CREATE_FUNC(SaveGameScene);
	virtual bool init();
private:
	Label* saveBtn;
	Label* quitBtn;
	void addSaveBtnListener(Node*);
	void addQuitBtnListener(Node*);
};

#endif