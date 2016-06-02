#include "StartGameScene.h"
#include "SettingScene.h"
#include "GameScene.h"

Scene* StartGameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = StartGameScene::create();
	scene->addChild(layer);
	return scene;
}

bool StartGameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();

	GameData::getInstance(); // 读取文件
	
	Sprite* bg = Sprite::create("startgame/startgamebg.png");
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bg);

	//开始精灵
	Sprite* startSpriteNormal = Sprite::create("startgame/start-up.png");
	Sprite* startSpriteSelected = Sprite::create("startgame/start-down.png");

	MenuItemSprite* startMenuItem = MenuItemSprite::create(startSpriteNormal,
		startSpriteSelected,
		CC_CALLBACK_1(StartGameScene::menuItemStartCallback, this));
	startMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(700,170)));

	//设置图片菜单
	MenuItemImage* settingMenuItem = MenuItemImage::create(
		"startgame/setting-up.png",
		"startgame/setting-down.png",
		CC_CALLBACK_1(StartGameScene::menuItemSettingCallback, this));
	settingMenuItem->setPosition(Vec2(Director::getInstance()->convertToGL(Vec2(480,400))));

	//帮助图片菜单
	MenuItemImage* helpMenuItem = MenuItemImage::create(
		"startgame/help-up.png",
		"startgame/help-down.png",
		CC_CALLBACK_1(StartGameScene::menuItemHelpCallback,this));
	helpMenuItem->setPosition(Vec2(Director::getInstance()->convertToGL(Vec2(860,480))));

	Menu* mu = Menu::create(startMenuItem, settingMenuItem, helpMenuItem, NULL);
	mu->setPosition(Vec2(0,0));
	this->addChild(mu);

	return true;
}

void StartGameScene::onEnter()
{
	Layer::onEnter();
	//log("StartGameScene onEnter");
}

void StartGameScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	//log("StartGameScene onEnterTransitionDidFinish");
	/*if (UserDefault::getInstance()->getBoolForKey(MUSIC_KEY)) {
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/startgamebgm.mp3", true);
	}*/
}

void StartGameScene::onExit()
{
	Layer::onExit();
	//log("StartGameScene onExit");
}

void StartGameScene::onExitTransitionDidStart()
{
	Layer::onExitTransitionDidStart();
	//log("StartGameScene onExitTransitionDidStart");
}

void StartGameScene::cleanup()
{
	Layer::cleanup();
	//log(StartGameScene cleanup);
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic("sound/startgamebgm.mp3");
}

void StartGameScene::menuItemHelpCallback(Ref* pSender)
{
	/*if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Blip.wav");
	}*/
}

void StartGameScene::menuItemStartCallback(Ref* pSender)
{
	/*if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Blip.wav");
	}*/
	Scene* sc = NULL;
	if (GameData::getInstance()->isExistSaveDoc()){
		sc = GameScene::loadSceneWithSaveData();
	}		
	else{
		sc = GameScene::createSceneWithId(2);
	}		
	auto reScene = TransitionJumpZoom::create(1.0f, sc);
	Director::getInstance()->replaceScene(sc);
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Blip.wav");
}

void StartGameScene::menuItemSettingCallback(Ref* pSender)
{
	/*if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Blip.wav");
	}*/
	auto sc = SettingScene::createScene();
	auto reScene = TransitionJumpZoom::create(1.0f, sc);
	Director::getInstance()->pushScene(reScene);
}


