#include "SettingScene.h"

Scene* SettingScene::createScene()
{
	auto scene = Scene::create();
	auto layer = SettingScene::create();
	scene->addChild(layer);
	return scene;
}
bool SettingScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visiblesize = Director::getInstance()->getVisibleSize();

	Sprite* bg = Sprite::create("startgame/setting-back.png");
	bg->setPosition(Vec2(visiblesize.width / 2,visiblesize.height / 2));
	this->addChild(bg);

	//ÒôÐ§
	auto soundOnMenuItem = MenuItemImage::create(
		"startgame/on.png",
		"startgame/on.png");
	auto soundOffMenuItem = MenuItemImage::create(
		"startgame/off.png",
		"startgame/off.png");

	auto soundToggleMenuItem = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(SettingScene::menuSoundToggleCallback, this),
		soundOffMenuItem,
		soundOnMenuItem,
		NULL
		);
	soundToggleMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(818,220)));

	//±³¾°ÒôÀÖ
	auto musicOnMenuItem = MenuItemImage::create(
		"startgame/on.png",
		"startgame/on.png");
	auto musicOffMenuItem = MenuItemImage::create(
		"startgame/off.png",
		"startgame/off.png");
	auto musicToggleMenuItem = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(SettingScene::menuMusicToggleCallback, this),
		musicOffMenuItem,
		musicOnMenuItem,
		NULL);
	musicToggleMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(818, 362)));

	//OK°´Å¥
	auto okMenuItem = MenuItemImage::create(
		"startgame/ok-down.png",
		"startgame/ok-up.png",
		CC_CALLBACK_1(SettingScene::menuOkCallback, this));
	okMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(600, 510)));

	Menu* mn = Menu::create(soundToggleMenuItem, musicToggleMenuItem, okMenuItem, NULL);
	mn->setPosition(Vec2(0, 0));
	this->addChild(mn);

	UserDefault *defaults = UserDefault::getInstance();

	if (defaults->getBoolForKey(MUSIC_KEY)) {
		musicToggleMenuItem->setSelectedIndex(0);//off
	}
	else {
		musicToggleMenuItem->setSelectedIndex(1);//on
	}

	if (defaults->getBoolForKey(SOUND_KEY)) {
		soundToggleMenuItem->setSelectedIndex(0);//off
	}
	else {
		soundToggleMenuItem->setSelectedIndex(1);//on
	}

	return true;

}

void SettingScene::onEnter()
{
	Layer::onEnter();
}

void SettingScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	if (UserDefault::getInstance()->getBoolForKey(MUSIC_KEY)) {
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/startgamebgm.mp3", true);
	}
}

void SettingScene::onExit()
{
	Layer::onExit();
}

void SettingScene::onExitTransitionDidStart()
{
	Layer::onExitTransitionDidStart();
}

void SettingScene::cleanup()
{
	Layer::cleanup();
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic("sound/startgamebgm.mp3");
}

void SettingScene::menuSoundToggleCallback(Ref* pSender)
{
	UserDefault *defaults = UserDefault::getInstance();
	if (defaults->getBoolForKey(SOUND_KEY)) {
		defaults->setBoolForKey(SOUND_KEY, false);
	}
	else {
		defaults->setBoolForKey(SOUND_KEY, true);
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Blip.wav");
	}
}

void SettingScene::menuMusicToggleCallback(Ref* pSender)
{
	UserDefault *defaults = UserDefault::getInstance();
	if (defaults->getBoolForKey(SOUND_KEY)) {
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Blip.wav");
	}
	if (defaults->getBoolForKey(MUSIC_KEY)) {
		defaults->setBoolForKey(MUSIC_KEY, false);
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	}
	else {
		defaults->setBoolForKey(MUSIC_KEY, true);
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/startgamebgm.mp3");
	}
}

void SettingScene::menuOkCallback(Ref* pSender)
{
	Director::getInstance()->popScene();
	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Blip.wav");
	}
}