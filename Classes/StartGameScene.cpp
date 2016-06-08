#include "StartGameScene.h"
#include "SettingScene.h"
#include "GameScene.h"
#include "gdShow.h"

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

	Sprite* bg = Sprite::create("startgame/startgamebg.png");
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bg);

	//¿ªÊ¼¾«Áé
	Sprite* startSpriteNormal = Sprite::create("startgame/start-up.png");
	Sprite* startSpriteSelected = Sprite::create("startgame/start-down.png");

	MenuItemSprite* startMenuItem = MenuItemSprite::create(startSpriteNormal,
		startSpriteSelected,
		CC_CALLBACK_1(StartGameScene::menuItemStartCallback, this));
	startMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(700, 170)));

	//ÉèÖÃÍ¼Æ¬²Ëµ¥
	MenuItemImage* settingMenuItem = MenuItemImage::create(
		"startgame/setting-up.png",
		"startgame/setting-down.png",
		CC_CALLBACK_1(StartGameScene::menuItemSettingCallback, this));
	settingMenuItem->setPosition(Vec2(Director::getInstance()->convertToGL(Vec2(480, 400))));

	//°ïÖúÍ¼Æ¬²Ëµ¥
	MenuItemImage* helpMenuItem = MenuItemImage::create(
		"startgame/help-up.png",
		"startgame/help-down.png",
		CC_CALLBACK_1(StartGameScene::menuItemHelpCallback, this));
	helpMenuItem->setPosition(Vec2(Director::getInstance()->convertToGL(Vec2(860, 480))));

	Menu* mu = Menu::create(startMenuItem, settingMenuItem, helpMenuItem, NULL);
	mu->setPosition(Vec2(0, 0));
	this->addChild(mu);

	startmenuItem_counter = TimeCounter::create();
	this->addChild(startmenuItem_counter);

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
	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Blip.wav");
	}
}

void StartGameScene::menuItemStartCallback(Ref* pSender)
{
	if (startmenuItem_counter->getCurTime() == 0.0f || startmenuItem_counter->getCurTime() > 10.0f){
		/*if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Blip.wav");
			}*/
		startmenuItem_counter->start();

		loadPlistFile();

		Scene* sc = NULL;
		//ÊÇ·ñÓÐÍæ¼Ò´æµµ
		if (GameData::getInstance()->isExistSaveDoc()){
			sc = GameScene::loadSceneWithSaveData();
		}
		else{
			//sc = GameScene::createSceneWithId(2);
			//Ã»ÓÐ´æµµ£¬ÇÐ»»µ½ÓÎÏ·±³¾°½éÉÜ
			sc = gdShow::createscene();
		}
		auto reScene = TransitionJumpZoom::create(0.0f, sc);
		Director::getInstance()->replaceScene(sc);
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Blip.wav");
		auto m = (MenuItemSprite*)pSender;
		m->setEnabled(false);
	}
}

void StartGameScene::menuItemSettingCallback(Ref* pSender)
{
	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Blip.wav");
	}
	auto sc = SettingScene::createScene();
	auto reScene = TransitionJumpZoom::create(1.0f, sc);
	Director::getInstance()->pushScene(reScene);
}

void StartGameScene::loadPlistFile()
{
	SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
	/*±£´æµã*/
	frameCache->addSpriteFramesWithFile("NPC/save.plist", "NPC/save.png");

	/*Ê÷¹Ö*/
	frameCache->addSpriteFramesWithFile("monster/treemonster/treemonsterdattack/treemonsterdattack.plist", "monster/treemonster/treemonsterdattack/treemonsterdattack.png");
	frameCache->addSpriteFramesWithFile("monster/treemonster/treemonsterdrun/treemonsterdrun.plist", "monster/treemonster/treemonsterdrun/treemonsterdrun.png");
	frameCache->addSpriteFramesWithFile("monster/treemonster/treemonsterdstatic/treemonsterdstatic.plist", "monster/treemonster/treemonsterdstatic/treemonsterdstatic.png");

	frameCache->addSpriteFramesWithFile("monster/treemonster/treemonsterhattack/treemonsterhattack.plist", "monster/treemonster/treemonsterhattack/treemonsterhattack.png");
	frameCache->addSpriteFramesWithFile("monster/treemonster/treemonsterhrun/treemonsterhrun.plist", "monster/treemonster/treemonsterhrun/treemonsterhrun.png");
	frameCache->addSpriteFramesWithFile("monster/treemonster/treemonsterhstatic/treemonsterhstatic.plist", "monster/treemonster/treemonsterhstatic/treemonsterhstatic.png");

	frameCache->addSpriteFramesWithFile("monster/treemonster/treemonsteruattack/treemonsteruattack.plist", "monster/treemonster/treemonsteruattack/treemonsteruattack.png");
	frameCache->addSpriteFramesWithFile("monster/treemonster/treemonsterurun/treemonsterurun.plist", "monster/treemonster/treemonsterurun/treemonsterurun.png");
	frameCache->addSpriteFramesWithFile("monster/treemonster/treemonsterustatic/treemonsterustatic.plist", "monster/treemonster/treemonsterustatic/treemonsterustatic.png");

	frameCache->addSpriteFramesWithFile("monster/treemonster/treemonsterubigskill/treemonsterubigskill.plist", "monster/treemonster/treemonsterubigskill/treemonsterubigskill.png");
	frameCache->addSpriteFramesWithFile("monster/treemonster/treemonsterdbigskill/treemonsterdbigskill.plist", "monster/treemonster/treemonsterdbigskill/treemonsterdbigskill.png");
	frameCache->addSpriteFramesWithFile("monster/treemonster/treemonsterhbigskill/treemonsterhbigskill.plist", "monster/treemonster/treemonsterhbigskill/treemonsterhbigskill.png");

	frameCache->addSpriteFramesWithFile("MonsterProj/treemonster/treemonproj.plist", "MonsterProj/treemonster/treemonproj.png");

	/*ÇàÁú*/
	frameCache->addSpriteFramesWithFile("monster/gdragonmonster/gdragonmonsterdattack/gdragonmonsterdattack.plist", "monster/gdragonmonster/gdragonmonsterdattack/gdragonmonsterdattack.png");
	frameCache->addSpriteFramesWithFile("monster/gdragonmonster/gdragonmonsterdrun/gdragonmonsterdrun.plist", "monster/gdragonmonster/gdragonmonsterdrun/gdragonmonsterdrun.png");
	frameCache->addSpriteFramesWithFile("monster/gdragonmonster/gdragonmonsterdstatic/gdragonmonsterdstatic.plist", "monster/gdragonmonster/gdragonmonsterdstatic/gdragonmonsterdstatic.png");

	frameCache->addSpriteFramesWithFile("monster/gdragonmonster/gdragonmonsterhattack/gdragonmonsterhattack.plist", "monster/gdragonmonster/gdragonmonsterhattack/gdragonmonsterhattack.png");
	frameCache->addSpriteFramesWithFile("monster/gdragonmonster/gdragonmonsterhrun/gdragonmonsterhrun.plist", "monster/gdragonmonster/gdragonmonsterhrun/gdragonmonsterhrun.png");
	frameCache->addSpriteFramesWithFile("monster/gdragonmonster/gdragonmonsterhstatic/gdragonmonsterhstatic.plist", "monster/gdragonmonster/gdragonmonsterhstatic/gdragonmonsterhstatic.png");

	frameCache->addSpriteFramesWithFile("monster/gdragonmonster/gdragonmonsteruattack/gdragonmonsteruattack.plist", "monster/gdragonmonster/gdragonmonsteruattack/gdragonmonsteruattack.png");
	frameCache->addSpriteFramesWithFile("monster/gdragonmonster/gdragonmonsterurun/gdragonmonsterurun.plist", "monster/gdragonmonster/gdragonmonsterurun/gdragonmonsterurun.png");
	frameCache->addSpriteFramesWithFile("monster/gdragonmonster/gdragonmonsterustatic/gdragonmonsterustatic.plist", "monster/gdragonmonster/gdragonmonsterustatic/gdragonmonsterustatic.png");

	frameCache->addSpriteFramesWithFile("MonsterProj/gdragonmonster/gdragonmonproj.plist", "MonsterProj/gdragonmonster/gdragonmonproj.png");

	/*Ê¯¹Ö*/
	frameCache->addSpriteFramesWithFile("monster/bonemonster/bonemonsterdattack/bonemonsterdattack.plist", "monster/bonemonster/bonemonsterdattack/bonemonsterdattack.png");
	frameCache->addSpriteFramesWithFile("monster/bonemonster/bonemonsterdrun/bonemonsterdrun.plist", "monster/bonemonster/bonemonsterdrun/bonemonsterdrun.png");
	frameCache->addSpriteFramesWithFile("monster/bonemonster/bonemonsterdstatic/bonemonsterdstatic.plist", "monster/bonemonster/bonemonsterdstatic/bonemonsterdstatic.png");

	frameCache->addSpriteFramesWithFile("monster/bonemonster/bonemonsterhattack/bonemonsterhattack.plist", "monster/bonemonster/bonemonsterhattack/bonemonsterhattack.png");
	frameCache->addSpriteFramesWithFile("monster/bonemonster/bonemonsterhrun/bonemonsterhrun.plist", "monster/bonemonster/bonemonsterhrun/bonemonsterhrun.png");
	frameCache->addSpriteFramesWithFile("monster/bonemonster/bonemonsterhstatic/bonemonsterhstatic.plist", "monster/bonemonster/bonemonsterhstatic/bonemonsterhstatic.png");

	frameCache->addSpriteFramesWithFile("monster/bonemonster/bonemonsteruattack/bonemonsteruattack.plist", "monster/bonemonster/bonemonsteruattack/bonemonsteruattack.png");
	frameCache->addSpriteFramesWithFile("monster/bonemonster/bonemonsterurun/bonemonsterurun.plist", "monster/bonemonster/bonemonsterurun/bonemonsterurun.png");
	frameCache->addSpriteFramesWithFile("monster/bonemonster/bonemonsterustatic/bonemonsterustatic.plist", "monster/bonemonster/bonemonsterustatic/bonemonsterustatic.png");

	frameCache->addSpriteFramesWithFile("MonsterProj/bonemonster/bonemonproj.plist", "MonsterProj/bonemonster/bonemonproj.png");
}


