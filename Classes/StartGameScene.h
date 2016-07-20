#ifndef _StartGameScene_H__
#define _StartGameScene_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "TimeCounter.h"

USING_NS_CC;

class StartGameScene : public Layer
{
public:
	static Scene* createScene();

	void loadPlistFile(); // 应该放在游戏开始界面加载。只加载一次
	virtual bool init();
	virtual void onEnter();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	virtual void onExitTransitionDidStart();
	virtual void cleanup();

	void menuItemHelpCallback(Ref* pSender);
	void menuItemStartCallback(Ref* pSender);
	void menuItemSettingCallback(Ref* pSender);
	void menuItemRestartCallback(Ref* pSender);
	CREATE_FUNC(StartGameScene);
	TimeCounter* startmenuItem_counter;
};

#endif