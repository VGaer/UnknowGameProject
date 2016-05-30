#ifndef __SettingScene_H__
#define __SettingScene_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "SystemHeader.h"

USING_NS_CC;

class SettingScene : public Layer
{
	bool isEffect;
public:
	static Scene* createScene();
	virtual bool init();
	virtual void onEnter();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	virtual void onExitTransitionDidStart();
	virtual void cleanup();

	void menuSoundToggleCallback(Ref* pSender);
	void menuMusicToggleCallback(Ref* pSender);
	void menuOkCallback(Ref* pSender);

	CREATE_FUNC(SettingScene);
};


#endif 