#include "GameScene.h"
#include "Monster.h"
#include "Graph.h"
#include "GameData.h"
#include "Pop.h"
#include "algorithm"
#include "SceneIdManager.h"
#include "BarManager.h"
#include "SavePoint.h"
#include <sstream>
#include "G2U.h"
#include "MonsterBarManager.h"
#include "Talk.h"
#include "TalkManager.h"

int GameScene::sceneId = 2;
bool GameScene::OnlyOnceSetScale = false;

string convertToString(double x)
{
	ostringstream o;
	if (o << x)
		return o.str();
	else
		return "error";
}

bool comp(Entity* a, Entity*b)
{
	if (a->getPositionY() > b->getPositionY())
		return true;
	else
		return false;
}

GameScene::GameScene()
{
	m_player = NULL;
}

Scene* GameScene::createSceneWithId(int sceneId)
{
	GameScene::sceneId = sceneId;
	auto scene = Scene::create();
	auto layer = new GameScene();
	if (layer && layer->init(sceneId))
	{
		layer->autorelease();
		scene->addChild(layer);
	}
	return scene;
}

Scene* GameScene::loadSceneWithSaveData()
{
	auto scene = Scene::create();
	auto layer = new GameScene();
	if (layer && layer->init())
	{
		layer->autorelease();
		scene->addChild(layer);
	}
	return scene;
}

bool GameScene::init()
{
	auto size = Director::getInstance()->getVisibleSize();

	/*读存档的任务数据*/
	GameData::getInstance()->readQuestSaveData();
	auto saveData = GameData::getInstance()->getPlayerData();

	switch (saveData->sceneId)
	{
	case 2:{
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/theme.mp3",true);
		break;
	}
	case 3:	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/wildmap.mp3", true);
		break;
	}
	case 4:{
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/wildmap.mp3", true);
		break;
	}
	case 5:{
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/wildmap.mp3", true);
		break;
	}
	case 6:{
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/cave.mp3", true);
		break;
	}
	case 7:{
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/cave.mp3", true);
		break;
	}
	case 8:{
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/cave.mp3", true);
		break;
	}
	case 9:{
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/wildmap.mp3", true);
		break;
	}
	case 10:{
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/wildmap.mp3", true);
		break;
	}
	case 11:{
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/grave.mp3", true);
		break;
	}

	}

	setMapInfo(saveData->sceneId);

	///需要先创建怪物，在addPlayer，因为在addPlayer的时候，会创建主角开始时的对话框，此时要锁定怪物，但是怪物还没创建出来的话就锁定不了了，造成怪物可以动
	/*判断开头任务是否完成，是的话不添加怪物*/
	//且地图id为主角家2
	if (QuestDispatcher::getInstance()->getqData()[4]->status != QuestStatus::finish && GameScene::sceneId == 2)
	{
		string monname[3] = { "treemonster", "gdragonmonster", "bonemonster" };
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				ValueMap monpos;
				if (objGroup){
					monpos = objGroup->getObject("Mon" + convertToString(i * 2 + j + 1));
				}
				float monposx = monpos["x"].asFloat();
				float monposy = monpos["y"].asFloat();
				addMonster(monname[i], Vec2(monposx, monposy));
				m_monster->monsterIdForBar = i * 2 + j;
				auto monbar = BarManager::getInstance()->create("UI/Enemy_hp_bar2.png", m_monster->monsterIdForBar, m_monster->monMaxHp);
				//添加到怪物精灵血条管理器
				auto monsterbarmanager = MonsterBarManager::getInstance();
				monsterbarmanager->getmonsterBarVec().pushBack(monbar);
				monbar->setAnchorPoint(Vec2(0.5, 1));
				monbar->setPosition(Vec2(size.width / 2 - 10, size.height));
				this->addChild(monbar, this->getChildren().size());
				if (monname[i] == "treemonster")
				{
					auto monLabelname = Label::create(gb2312_to_utf8("树怪"), "Arial", 25);
					monLabelname->setColor(Color3B::ORANGE);
					monbar->addChild(monLabelname);
					monLabelname->setPosition(Vec2(43, 48));
				}
				else if (monname[i] == "gdragonmonster")
				{
					auto monLabelname = Label::create(gb2312_to_utf8("青龙"), "Arial", 25);
					monLabelname->setColor(Color3B::ORANGE);
					monbar->addChild(monLabelname);
					monLabelname->setPosition(Vec2(43, 48));
				}
				else if (monname[i] == "bonemonster")
				{
					auto monLabelname = Label::create(gb2312_to_utf8("石怪"), "Arial", 25);
					monLabelname->setColor(Color3B::ORANGE);
					monbar->addChild(monLabelname);
					monLabelname->setPosition(Vec2(43, 48));
				}
			}
		}
	}
	//非主角家，都创建怪物
	else if (GameScene::sceneId != 2)
	{
		string monname[3] = { "treemonster", "gdragonmonster", "bonemonster" };
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				ValueMap monpos = objGroup->getObject("Mon" + convertToString(i * 2 + j + 1));
				float monposx = monpos["x"].asFloat();
				float monposy = monpos["y"].asFloat();
				addMonster(monname[i], Vec2(monposx, monposy));
				m_monster->monsterIdForBar = i * 2 + j;
				auto monbar = BarManager::getInstance()->create("UI/Enemy_hp_bar2.png", m_monster->monsterIdForBar, m_monster->monMaxHp);
				//添加到怪物精灵血条管理器
				auto monsterbarmanager = MonsterBarManager::getInstance();
				monsterbarmanager->getmonsterBarVec().pushBack(monbar);
				monbar->setAnchorPoint(Vec2(0.5, 1));
				monbar->setPosition(Vec2(size.width / 2 - 10, size.height));
				this->addChild(monbar, this->getChildren().size());
				if (monname[i] == "treemonster")
				{
					auto monLabelname = Label::create(gb2312_to_utf8("树怪"), "Arial", 25);
					monLabelname->setColor(Color3B::ORANGE);
					monbar->addChild(monLabelname);
					monLabelname->setPosition(Vec2(43, 48));
				}
				else if (monname[i] == "gdragonmonster")
				{
					auto monLabelname = Label::create(gb2312_to_utf8("青龙"), "Arial", 25);
					monLabelname->setColor(Color3B::ORANGE);
					monbar->addChild(monLabelname);
					monLabelname->setPosition(Vec2(43, 48));
				}
				else if (monname[i] == "bonemonster")
				{
					auto monLabelname = Label::create(gb2312_to_utf8("石怪"), "Arial", 25);
					monLabelname->setColor(Color3B::ORANGE);
					monbar->addChild(monLabelname);
					monLabelname->setPosition(Vec2(43, 48));
				}
			}
		}
	}

	addPlayer(saveData);

	AfterPlayerSetMonsterBindPlayer();

	auto savePoint = SavePoint::create();
	savePoint->setPosition(Point(500, 350));
	m_map->addChild(savePoint, 100);

	auto npcMap = GameData::getInstance()->getMapIDtoNpcData();
	for (auto& i : npcMap) {
		if (i.first == sceneId) {
			addNpc(i.second);
		}
		else	continue;
	}

	auto questDisp = QuestDispatcher::getInstance();
	addChild(questDisp);
	//判断是否存在defeat任务,主角死后回开始界面开始游戏，单例QuestDispatcher已经存在，此时必须判断然后手动开启定时器。
	if (questDisp->IsExistDefeatQuest())
	{
		questDisp->schedule(schedule_selector(QuestDispatcher::questsUpdate));
	}
	
	TalkManager::getInstance();

	scheduleUpdate();
	this->schedule(schedule_selector(GameScene::MonHP_MPBar_Update), 0.2f);
	return true;
}

bool GameScene::init(int sceneId)
{
	auto size = Director::getInstance()->getVisibleSize();

	switch (sceneId)
	{
	case 2:{
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/theme.mp3", true);
		break;
	}
	case 3:	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/wildmap.mp3", true);
		break;
	}
	case 4:{
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/wildmap.mp3", true);
		break;
	}
	case 5:{
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/wildmap.mp3", true);
		break;
	}
	case 6:{
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/cave.mp3", true);
		break;
	}
	case 7:{
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/cave.mp3", true);
		break;
	}
	case 8:{
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/cave.mp3", true);
		break;
	}
	case 9:{
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/wildmap.mp3", true);
		break;
	}
	case 10:{
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/wildmap.mp3", true);
		break;
	}
	case 11:{
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/grave.mp3", true);
		break;
	}

	}

	setMapInfo(sceneId);

	/*判断开头任务是否完成，是的话不添加怪物,否则添加怪物*/
	//且地图id为主角家2
	if (QuestDispatcher::getInstance()->getqData()[4]->status != QuestStatus::finish && GameScene::sceneId == 2)
	{
		string monname[3] = { "treemonster", "gdragonmonster", "bonemonster" };
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				ValueMap monpos;
				if (objGroup){
					monpos = objGroup->getObject("Mon" + convertToString(i * 2 + j + 1));
				}	 
				float monposx = monpos["x"].asFloat();
				float monposy = monpos["y"].asFloat();
				addMonster(monname[i], Vec2(monposx, monposy));
				m_monster->monsterIdForBar = i * 2 + j;
				auto monbar = BarManager::getInstance()->create("UI/Enemy_hp_bar2.png", m_monster->monsterIdForBar, m_monster->monMaxHp);
				//添加到怪物精灵血条管理器
				auto monsterbarmanager = MonsterBarManager::getInstance();
				monsterbarmanager->getmonsterBarVec().pushBack(monbar);
				monbar->setAnchorPoint(Vec2(0.5, 1));
				monbar->setPosition(Vec2(size.width / 2 - 10, size.height));
				this->addChild(monbar, this->getChildren().size());
				if (monname[i] == "treemonster")
				{
					auto monLabelname = Label::create(gb2312_to_utf8("树怪"), "Arial", 25);
					monLabelname->setColor(Color3B::ORANGE);
					monbar->addChild(monLabelname);
					monLabelname->setPosition(Vec2(43, 48));
				}
				else if (monname[i] == "gdragonmonster")
				{
					auto monLabelname = Label::create(gb2312_to_utf8("青龙"), "Arial", 25);
					monLabelname->setColor(Color3B::ORANGE);
					monbar->addChild(monLabelname);
					monLabelname->setPosition(Vec2(43, 48));
				}
				else if (monname[i] == "bonemonster")
				{
					auto monLabelname = Label::create(gb2312_to_utf8("石怪"), "Arial", 25);
					monLabelname->setColor(Color3B::ORANGE);
					monbar->addChild(monLabelname);
					monLabelname->setPosition(Vec2(43, 48));
				}
			}
		}
	}
	//非主角家，都创建怪物
	else if (GameScene::sceneId != 2)
	{
		string monname[3] = { "treemonster", "gdragonmonster", "bonemonster" };
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				ValueMap monpos = objGroup->getObject("Mon" + convertToString(i * 2 + j + 1));
				float monposx = monpos["x"].asFloat();
				float monposy = monpos["y"].asFloat();
				addMonster(monname[i], Vec2(monposx, monposy));
				m_monster->monsterIdForBar = i * 2 + j;
				auto monbar = BarManager::getInstance()->create("UI/Enemy_hp_bar2.png", m_monster->monsterIdForBar, m_monster->monMaxHp);
				//添加到怪物精灵血条管理器
				auto monsterbarmanager = MonsterBarManager::getInstance();
				monsterbarmanager->getmonsterBarVec().pushBack(monbar);
				monbar->setAnchorPoint(Vec2(0.5, 1));
				monbar->setPosition(Vec2(size.width / 2 - 10, size.height));
				this->addChild(monbar, this->getChildren().size());
				if (monname[i] == "treemonster")
				{
					auto monLabelname = Label::create(gb2312_to_utf8("树怪"), "Arial", 25);
					monLabelname->setColor(Color3B::ORANGE);
					monbar->addChild(monLabelname);
					monLabelname->setPosition(Vec2(43, 48));
				}
				else if (monname[i] == "gdragonmonster")
				{
					auto monLabelname = Label::create(gb2312_to_utf8("青龙"), "Arial", 25);
					monLabelname->setColor(Color3B::ORANGE);
					monbar->addChild(monLabelname);
					monLabelname->setPosition(Vec2(43, 48));
				}
				else if (monname[i] == "bonemonster")
				{
					auto monLabelname = Label::create(gb2312_to_utf8("石怪"), "Arial", 25);
					monLabelname->setColor(Color3B::ORANGE);
					monbar->addChild(monLabelname);
					monLabelname->setPosition(Vec2(43, 48));
				}
			}
		}
	}

	float playerX;
	float playerY;
	
	/*加载主角坐标*/
	//最初开始游戏时
	if (Player::getInstance()->gamescenedir == "none")
	{
		ValueMap playerPointMap = objGroup->getObject("PlayerPoint");
		playerX = playerPointMap["x"].asFloat();
		playerY = playerPointMap["y"].asFloat();
	}
	else
	{
		ValueMap playerPointMap = objGroup->getObject("playerpoint" + Player::getInstance()->gamescenedir);
		playerX = playerPointMap["x"].asFloat();
		playerY = playerPointMap["y"].asFloat();
		//重设gamescenedir为none
		Player::getInstance()->gamescenedir = "none";
	}

	addPlayer(Vec2(playerX, playerY));

	AfterPlayerSetMonsterBindPlayer();

	/*创建主角hpmp条*/
	if (BarManager::getInstance()->getParent() != NULL)
	{
		BarManager::getInstance()->removeFromParent();
		this->addChild(BarManager::getInstance());
		auto bar = BarManager::getInstance()->create("UI/PlayerBar_hp.png", "UI/PlayerBar_mp.png", m_player->m_hp, m_player->m_mp);		
		bar->setAnchorPoint(Vec2(0, 1));
		bar->setPosition(0, size.height);
		//创建行动条
		auto act = BarManager::getInstance()->create("UI/inventoryBtn.png", "UI/questBtn.png");
		act->setAnchorPoint(Vec2(0.5, 0));
		act->setPosition(size.width / 2 + 30, 0);
		this->addChild(act, 100);

		auto exp = BarManager::getInstance()->create("UI/exp.png", 100 * m_player->m_playerlevel);
		exp->setAnchorPoint(Vec2(0.5, 0));
		exp->setPosition(size.width / 2, act->getContentSize().height - 5);

		auto playerbar = BarManager::getInstance()->getPlayerBars();
		if (playerbar != NULL)
		{
			playerbar->m_hp->setPercentage(m_player->m_hp / m_player->getCurMaxHp());
			BarManager::getInstance()->setBarLabel(playerbar->l_hp, m_player->m_hp, m_player->getCurMaxHp());
			playerbar->m_mp->setPercentage(m_player->m_mp / m_player->getCurMaxMp());
			BarManager::getInstance()->setBarLabel(playerbar->l_mp, m_player->m_mp, m_player->getCurMaxMp());
			playerbar->m_exp->setPercentage(m_player->m_exp / (100 * m_player->m_playerlevel));
			BarManager::getInstance()->setBarLabel(playerbar->l_exp, m_player->m_exp, 100 * m_player->m_playerlevel);
		}
		this->addChild(bar, 100);
		this->addChild(exp, 100);
	}
	else
	{
		this->addChild(BarManager::getInstance());
		auto bar = BarManager::getInstance()->create("UI/PlayerBar_hp.png", "UI/PlayerBar_mp.png", m_player->m_hp, m_player->m_mp);
		bar->setAnchorPoint(Vec2(0, 1));
		bar->setPosition(0, size.height);
		//创建行动条
		auto act = BarManager::getInstance()->create("UI/inventoryBtn.png", "UI/questBtn.png");
		act->setAnchorPoint(Vec2(0.5, 0));
		act->setPosition(size.width / 2 + 30, 0);
		this->addChild(act, 100);

		auto exp = BarManager::getInstance()->create("UI/exp.png", 100 * m_player->m_playerlevel);
		exp->setAnchorPoint(Vec2(0.5, 0));
		exp->setPosition(size.width / 2, act->getContentSize().height - 5);


		auto playerbar = BarManager::getInstance()->getPlayerBars();
		if (playerbar != NULL)
		{
			playerbar->m_hp->setPercentage(m_player->m_hp / m_player->getCurMaxHp());
			BarManager::getInstance()->setBarLabel(playerbar->l_hp, m_player->m_hp, m_player->getCurMaxHp());
			playerbar->m_mp->setPercentage(m_player->m_mp / m_player->getCurMaxMp());
			BarManager::getInstance()->setBarLabel(playerbar->l_mp, m_player->m_mp, m_player->getCurMaxMp());
			playerbar->m_exp->setPercentage(m_player->m_exp / (100 * m_player->m_playerlevel));
			BarManager::getInstance()->setBarLabel(playerbar->l_exp, m_player->m_exp, 100 * m_player->m_playerlevel);
		}
		this->addChild(bar, 100);
		this->addChild(exp, 100);
	}


	auto savePoint = SavePoint::create();
	savePoint->setPosition(Point(500, 350));
	m_map->addChild(savePoint, 100);

	auto questDisp = QuestDispatcher::getInstance();
	addChild(questDisp);
	//判断是否存在defeat任务，当没存挡的主角死后到开始界面开始游戏，开头清主角家园怪时，由于QuestDispatcher单例已经存在，所以得手动开定时器了
	if (questDisp->IsExistDefeatQuest())
	{
		questDisp->schedule(schedule_selector(QuestDispatcher::questsUpdate));
	}

	auto npcMap = GameData::getInstance()->getMapIDtoNpcData();
	for (auto& i : npcMap) {
		if (i.first == sceneId) {
			addNpc(i.second);
		}
		else	continue;
	}

	TalkManager::getInstance();

	scheduleUpdate();
	this->schedule(schedule_selector(GameScene::MonHP_MPBar_Update), 0.2f);
	firstEnterTalk();
	return true;
}

void GameScene::setMapInfo(int id)
{
	GameScene::sceneId = id;
	if (SceneIdManager::getInstance()->map_sceneIdToSceneData.find(id) != SceneIdManager::getInstance()->map_sceneIdToSceneData.end())
	{
		m_map = TMXTiledMap::create(SceneIdManager::getInstance()->map_sceneIdToSceneData[id].name);
		m_map->getLayer("barrier")->setVisible(false);
		addChild(m_map, 0, 1);
		auto graph = Graph::getInstance();
		graph->setTildMap(m_map);
		graph->init(Point(16, 20));
		objGroup = m_map->getObjectGroup("objects");
	}
}

void GameScene::addPlayer(Point pos, int direction)
{
	/*主角没死时过图切换场景*/
	Player* player = Player::getInstance();
	if (player->getParent() != NULL)
	{
		player->removeFromParentAndCleanup(false);

		//切换场景时主角的初始化,不需要初始化方向了

		player->setEnableAction(true);
		//开启所有定时器 
		player->setPosition(pos);
		player->setTiledMap(m_map);
		//创建K键发射物
		player->createSwordWave();
		m_player = player;
		return;
	}

	/*这部分为第一次开始游戏或者主角死后回到开始游戏界面的开始游戏(都是没存档时),即没存档时主角出生在ID为2号的地图*/
	player->setTiledMap(m_map);
	player->init();
	player->setPosition(pos);

	//设置地图的时候，如果是id == 2的地图，就可以调用主角开始时的对话框
	//并且对话框是根据主句位置设置的，主角位置得先初始化好了
	if (m_map && GameScene::sceneId == 2)
		QuestDispatcher::getInstance()->twoIdSceneTalk(m_map);

	if (OnlyOnceSetScale == false)
	{
		player->getSprite()->setScale(player->getPlayer_magnification());
		player->getSprite()->setAnchorPoint(Vec2(0.5, 0));
		player->getSprite()->setPosition(Vec2(player->getContentSize().width * player->getPlayer_magnification() / 2,
			0));
		player->setContentSize(player->getContentSize() * player->getPlayer_magnification());
		player->setAnchorPoint(Vec2(0.5, 0));
	
		//player的init已经有设置direction了。
		//player->setPlayerDir(direction);
		OnlyOnceSetScale = true;
	}
	
	m_player = player;

}

void GameScene::addPlayer(PlayerData* saveData)
{
	auto size = Director::getInstance()->getVisibleSize();
	Player* player = Player::getInstance();
	player->setTiledMap(m_map);
	player->init();
	/*初始化玩家mp hp*/
	player->setPlayer_hp(saveData->hp);
	player->setPlayer_mp(saveData->mp);
	/*初始化玩家等级*/
	player->m_playerlevel = saveData->level;
	/*初始化玩家经验*/
	player->m_exp = saveData->exp;
	/*初始化玩家的最大HP和最大MP*/
	player->getCurMaxHp() = 100 + (saveData->level - 1) * 20;
	player->getCurMaxMp() = 100 + (saveData->level - 1) * 35;

	this->addChild(BarManager::getInstance());
	auto playerbar = BarManager::getInstance()->getPlayerBars();
	//创建HP,MPbar
	auto bar = BarManager::getInstance()->create("UI/PlayerBar_hp.png", "UI/PlayerBar_mp.png", player->m_hp, player->m_mp);
	bar->setAnchorPoint(Vec2(0, 1));
	bar->setPosition(0, size.height);
	this->addChild(bar, 100);
	//创建行动条
	auto act = BarManager::getInstance()->create("UI/inventoryBtn.png", "UI/questBtn.png");
	act->setAnchorPoint(Vec2(0.5, 0));
	act->setPosition(size.width / 2 + 30, 0);
	this->addChild(act, 100);
	//创建经验条
	auto exp = BarManager::getInstance()->create("UI/exp.png", 100 * saveData->level);
	exp->setAnchorPoint(Vec2(0.5, 0));
	exp->setPosition(size.width / 2, act->getContentSize().height - 5);
	this->addChild(exp, 100);
	//初始化主角经验条
	//初始化主角血条和HP条

	if (playerbar != NULL)
	{
		playerbar->m_hp->setPercentage(saveData->hp / player->getCurMaxHp());
		BarManager::getInstance()->setBarLabel(playerbar->l_hp, saveData->hp, player->getCurMaxHp());
		playerbar->m_mp->setPercentage(saveData->mp / player->getCurMaxMp());
		BarManager::getInstance()->setBarLabel(playerbar->l_mp, saveData->mp, player->getCurMaxMp());
		playerbar->m_exp->setPercentage(saveData->exp / (100 * saveData->level));
		BarManager::getInstance()->setBarLabel(playerbar->l_exp, saveData->exp, 100 * saveData->level);
	}

	if (OnlyOnceSetScale == false)
	{
		player->getSprite()->setScale(player->getPlayer_magnification());
		player->getSprite()->setAnchorPoint(Vec2(0.5, 0));
		player->getSprite()->setPosition(Vec2(player->getContentSize().width * player->getPlayer_magnification() / 2,
			0));
		player->setContentSize(player->getContentSize() * player->getPlayer_magnification());
		player->setAnchorPoint(Vec2(0.5, 0));
		OnlyOnceSetScale = true;
	}
	
	player->setPosition(Point(saveData->posX, saveData->posY));

	//设置地图的时候，如果是id == 2的地图，就可以调用主角开始时的对话框
	//并且对话框是根据主句位置设置的，主角位置得先初始化好了
	if (m_map && GameScene::sceneId == 2)
		QuestDispatcher::getInstance()->twoIdSceneTalk(m_map);

	//存档的PlayerDir
	player->setPlayerDir(saveData->direction);
	/*
	其他属性
	*/

	m_player = player;
}

void GameScene::addNpc(vector<NpcsData*> nData)
{
	for (auto &i : nData) {
		ValueMap pos = objGroup->getObject("NPC" + convertToString(i->id));
		float posx = pos["x"].asFloat();
		float posy = pos["y"].asFloat();
		m_npc = NPC::create(i->name);
		m_npc->setAnchorPoint(Vec2(.5f, .5f));
		m_npc->setTiledMap(m_map);
		m_npc->setPosition(Vec2(posx, posy));
		m_npc->setPlayer(m_player);
		m_map->addChild(m_npc, (int)m_map->getChildren().size());
		auto p = m_npc->getPosition();
		p = CC_POINT_POINTS_TO_PIXELS(p);
		m_npc->setVertexZ(-((p.y + 64) / 64));
		auto pop = Pop::create(Vec2(m_npc->getPosition().x, m_npc->getPosition().y - 50));
		m_map->addChild(pop, 3);
		PopManager::getInstance()->getPopsMap()[i->name] = pop;
		NpcManager::getInstance()->getNpcsVec().pushBack(m_npc);
	}
}

void GameScene::addMonster(const std::string& name, Point pos)
{
	//怪物创建
	m_monster = Monster::create(name);
	m_map->addChild(m_monster, (int)m_map->getChildren().size());
	m_monster->setPosition(pos);
	//添加到怪物管理器
	MonsterManager::getInstance()->getMonsterVec().pushBack(m_monster);
	//怪物的各种初始化
	m_monster->getSprite()->setScaleY(1.5);
	m_monster->getSprite()->setAnchorPoint(Vec2(0.5, 0));
	m_monster->setContentSize(m_monster->getContentSize() * 1.5);
	m_monster->getSprite()->setPosition(Vec2(m_monster->getContentSize().width / 2, 0));
	m_monster->setAnchorPoint(Vec2(0.5, 0));
	m_monster->setMonsterParent(m_map);
	m_monster->setvecPatrolpoint();
}

void GameScene::onEnter()
{
	Layer::onEnter();
	Director::getInstance()->setProjection(Director::Projection::_2D);
	Director::getInstance()->setDepthTest(true);
}

void GameScene::onExit()
{
	Director::getInstance()->setProjection(Director::Projection::DEFAULT);
	Director::getInstance()->setDepthTest(false);
	Layer::onExit();
}

void GameScene::update(float dt)
{
	auto p = m_player->getPosition();
	p = CC_POINT_POINTS_TO_PIXELS(p);
	m_player->setVertexZ(-((p.y + 64) / 64));

	auto Vec1 = MonsterManager::getInstance()->getMonsterVec();
	//	log("MonSize%d", Vec1.size());
	Vector<Entity*> Vec;
	for (int i = 0; i < Vec1.size(); i++)
	{
		Vec.pushBack(Vec1.at(i));
	}
	for (int i = 0; i < Vec.size(); i++)
	{
		auto monster = Vec.at(i);
		p = monster->getPosition();
		p = CC_POINT_POINTS_TO_PIXELS(p);
		monster->setVertexZ(-((p.y + 64) / 64));
	}

	Vec.pushBack(m_player);
	auto Vec2 = NpcManager::getInstance()->getNpcsVec();
	for (int i = 0; i < Vec2.size(); i++)
	{
		Vec.pushBack(Vec2.at(i));
	}
	sort(Vec.begin(), Vec.end(), comp);
	for (int i = 0; i < Vec.size(); i++)
	{
		Vec.at(i)->setZOrder(4 + i);
	}
	setViewpointCenter(m_player->getPosition());

}

void GameScene::setViewpointCenter(Vec2 Position)
{
	Size visiblesize = Director::getInstance()->getVisibleSize();
	int x = MAX(Position.x, visiblesize.width / 2);
	int y = MAX(Position.y, visiblesize.height / 2);
	x = MIN(x, (m_map->getMapSize().width * m_map->getTileSize().width) - visiblesize.width / 2);
	y = MIN(y, (m_map->getMapSize().height * m_map->getTileSize().height) - visiblesize.height / 2);
	Vec2 pointA = Vec2(visiblesize.width / 2, visiblesize.height / 2);
	Vec2 pointB = Vec2(x, y);

	Vec2 offset = pointA - pointB;
	this->m_map->setPosition(offset);
}


void GameScene::MonHP_MPBar_Update(float dt)
{
	auto Vec = MonsterManager::getInstance()->getMonsterVec();
	for (int i = 0; i < Vec.size(); i++)
	{
		auto monster = Vec.at(i);
		int Id = monster->monsterIdForBar;
		auto monbar = BarManager::getInstance()->getBars(Id);
		auto monbarLabel = BarManager::getInstance()->getBarsLabel(Id);
		if (monbar != NULL)
		{
			BarManager::getInstance()->setPercent(monbar, monster->monMaxHp, monster->monsdata.hp);
			BarManager::getInstance()->setBarLabel(monbarLabel, monster->monsdata.hp, monster->monMaxHp);
		}
	}
}

void GameScene::firstEnterTalk()
{
	auto enterDlgs = TalkManager::getInstance()->getDataFromEnterSceneDlgsData(sceneId);
	if (enterDlgs && !enterDlgs->enterSceneDlgs.empty() && enterDlgs->enterSceneDlgs.at(0) != "NULL" 
		&& enterDlgs->enterSceneDlgs.at(0) != "" && !enterDlgs->isSaid)
	{
		auto talk = Talk::create(enterDlgs->enterSceneDlgs, enterDlgs->sceneId, Talk_EtScene);
		m_map->addChild(talk, 999);
	}
}

void GameScene::AfterPlayerSetMonsterBindPlayer()
{
	if (m_player)
	{
		for (auto mon : MonsterManager::getInstance()->getMonsterVec())
		{
			mon->bindPlayer(m_player);
			mon->getAnimBase()->setCurDirection(m_player->getPosition());
		}
	}
}
