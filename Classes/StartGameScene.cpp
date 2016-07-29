#include "StartGameScene.h"
#include "SettingScene.h"
#include "GameScene.h"
#include "gdShow.h"
#include "GameData.h"
#include "TalkManager.h"

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

	//开始精灵
	Sprite* startSpriteNormal = Sprite::create("startgame/start-up.png");
	Sprite* startSpriteSelected = Sprite::create("startgame/start-down.png");

	MenuItemSprite* startMenuItem = MenuItemSprite::create(startSpriteNormal,
		startSpriteSelected,
		CC_CALLBACK_1(StartGameScene::menuItemStartCallback, this));
	startMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(700, 140)));

	//设置图片菜单
	MenuItemImage* settingMenuItem = MenuItemImage::create(
		"startgame/setting-up.png",
		"startgame/setting-down.png",
		CC_CALLBACK_1(StartGameScene::menuItemSettingCallback, this));
	settingMenuItem->setPosition(Vec2(Director::getInstance()->convertToGL(Vec2(270, 300))));

	//帮助图片菜单
	MenuItemImage* helpMenuItem = MenuItemImage::create(
		"startgame/help-up.png",
		"startgame/help-down.png",
		CC_CALLBACK_1(StartGameScene::menuItemHelpCallback, this));
	helpMenuItem->setPosition(Vec2(Director::getInstance()->convertToGL(Vec2(800, 480))));

	//重新开始游戏
	MenuItemImage* restartgameMenuitem = MenuItemImage::create(
		"startgame/restart-up.png",
		"startgame/restart-down.png",
		CC_CALLBACK_1(StartGameScene::menuItemRestartCallback, this));
	restartgameMenuitem->setPosition(Vec2(Director::getInstance()->convertToGL(Vec2(450, 550))));
	if (!GameData::getInstance()->isExistSaveDoc())
	{
		restartgameMenuitem->setEnabled(false);
	}

	Menu* mu = Menu::create(startMenuItem, settingMenuItem, helpMenuItem,restartgameMenuitem, NULL);
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
	if (UserDefault::getInstance()->getBoolForKey(MUSIC_KEY)) {
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/startgamebgm.mp3", true);
		}
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
	//CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic("sound/startgamebgm.mp3");
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
		if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Blip.wav");
			}
		startmenuItem_counter->start();

		loadPlistFile();

		Scene* sc = NULL;
		//是否有玩家存档
		if (GameData::getInstance()->isExistSaveDoc()){
			sc = GameScene::loadSceneWithSaveData();
		}
		else{
			//sc = GameScene::createSceneWithId(2);
			//没有存档，切换到游戏背景介绍
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

	/*下雨背景*/
	frameCache->addSpriteFramesWithFile("Rain/Rain.plist","Rain/Rain.png");


	/*保存点*/
	frameCache->addSpriteFramesWithFile("NPC/save.plist", "NPC/save.png");

	/*树怪*/
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
	frameCache->addSpriteFramesWithFile("MonsterProj/treemonster/treemonprojdur.plist", "MonsterProj/treemonster/treemonprojdur.png");

	/*青龙*/
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
	frameCache->addSpriteFramesWithFile("MonsterProj/gdragonmonster/gdragonmonprojdur.plist", "MonsterProj/gdragonmonster/gdragonmonprojdur.png");

	/*石怪*/
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
	frameCache->addSpriteFramesWithFile("MonsterProj/bonemonster/bonemonprojdur.plist", "MonsterProj/bonemonster/bonemonprojdur.png");

	/*骷髅怪*/
	frameCache->addSpriteFramesWithFile("monster/skeletonmonster/skeletonmonsterdattack/skeletonmonsterdattack.plist", "monster/skeletonmonster/skeletonmonsterdattack/skeletonmonsterdattack.png");
	frameCache->addSpriteFramesWithFile("monster/skeletonmonster/skeletonmonsterdrun/skeletonmonsterdrun.plist", "monster/skeletonmonster/skeletonmonsterdrun/skeletonmonsterdrun.png");
	frameCache->addSpriteFramesWithFile("monster/skeletonmonster/skeletonmonsterdstatic/skeletonmonsterdstatic.plist", "monster/skeletonmonster/skeletonmonsterdstatic/skeletonmonsterdstatic.png");

	frameCache->addSpriteFramesWithFile("monster/skeletonmonster/skeletonmonsterhattack/skeletonmonsterhattack.plist", "monster/skeletonmonster/skeletonmonsterhattack/skeletonmonsterhattack.png");
	frameCache->addSpriteFramesWithFile("monster/skeletonmonster/skeletonmonsterhrun/skeletonmonsterhrun.plist", "monster/skeletonmonster/skeletonmonsterhrun/skeletonmonsterhrun.png");
	frameCache->addSpriteFramesWithFile("monster/skeletonmonster/skeletonmonsterhstatic/skeletonmonsterhstatic.plist", "monster/skeletonmonster/skeletonmonsterhstatic/skeletonmonsterhstatic.png");

	frameCache->addSpriteFramesWithFile("monster/skeletonmonster/skeletonmonsteruattack/skeletonmonsteruattack.plist", "monster/skeletonmonster/skeletonmonsteruattack/skeletonmonsteruattack.png");
	frameCache->addSpriteFramesWithFile("monster/skeletonmonster/skeletonmonsterurun/skeletonmonsterurun.plist", "monster/skeletonmonster/skeletonmonsterurun/skeletonmonsterurun.png");
	frameCache->addSpriteFramesWithFile("monster/skeletonmonster/skeletonmonsterustatic/skeletonmonsterustatic.plist", "monster/skeletonmonster/skeletonmonsterustatic/skeletonmonsterustatic.png");


	frameCache->addSpriteFramesWithFile("MonsterProj/skeletonmonster/skeletonmonproj.plist", "MonsterProj/skeletonmonster/skeletonmonproj.png");
	frameCache->addSpriteFramesWithFile("MonsterProj/skeletonmonster/skeletonmonprojdur.plist", "MonsterProj/skeletonmonster/skeletonmonprojdur.png");


	//////BOSS
	frameCache->addSpriteFramesWithFile("BossProj/eyecubeboss/eyecubebosslaser.plist", "BossProj/eyecubeboss/eyecubebosslaser.png");
}

void StartGameScene::menuItemRestartCallback(Ref* pSender)
{
	//是否有玩家存档
	if (GameData::getInstance()->isExistSaveDoc()){
		//清理存档
		GameData::getInstance()->clearAllSaveForRestartGame();

		//加载plist文件
		loadPlistFile();

		//重新读取数据
		GameData::getInstance()->restartReadDate();

		//重新读取数据后，GameData的m_mapQuestData是非存档的任务文件,考虑到主角死亡后在这个界面点
		//重新开始游戏，由于QuestDispatcher是单例，所以得手动重设QuestDispatcher里的qData
		QuestDispatcher::getInstance()->InitQuestDispathcerForPlayerDied();
		//同理对TalkManager
		TalkManager::getInstance()->InitTalkForPlayerDied();

		//切到gbshow背景
		Scene* sc = NULL;	
		sc = gdShow::createscene();
		auto reScene = TransitionJumpZoom::create(0.0f, sc);
		Director::getInstance()->replaceScene(sc);
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Blip.wav");
		MenuItem* m = (MenuItem*)pSender;
		m->setEnabled(false);
		return;
	}
	MenuItem* m = (MenuItem*)pSender;
	m->setEnabled(false);
}


