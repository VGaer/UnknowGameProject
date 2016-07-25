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
#include "AnimationUtil.h"
#include "BossManager.h"

#include "CubeBoss.h"

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

bool comp(Node* a, Node*b)
{
	if (a->getPositionY() > b->getPositionY())
		return true;
	else
		return false;
}

GameScene::GameScene()
{
	m_player = NULL;
	m_spriteRain = NULL;
	m_particleFire = NULL;
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

	/*���浵����������*/
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

	///��Ҫ�ȴ��������addPlayer����Ϊ��addPlayer��ʱ�򣬻ᴴ�����ǿ�ʼʱ�ĶԻ��򣬴�ʱҪ����������ǹ��ﻹû���������Ļ������������ˣ���ɹ�����Զ�
	/*�жϿ�ͷ�����Ƿ���ɣ��ǵĻ�����ӹ���*/
	//�ҵ�ͼidΪ���Ǽ�2
	if (QuestDispatcher::getInstance()->getqData()[4]->status != QuestStatus::finish && GameScene::sceneId == 2)
	{
		auto mapId_Mon = GameData::getInstance()->getDataFromSceneIdToSetMonData(GameScene::sceneId);
		if (mapId_Mon)
		{
			int curScene_MonId = 0;
			for (auto it : mapId_Mon->MonForObjVec)
			{
				for (int i = 1; i <= it.MonNums; i++)
				{
					ValueMap monpos;
					if (objGroup){
						monpos = objGroup->getObject(it.Monname + convertToString(i));
					}
					float monposx = monpos["x"].asFloat();
					float monposy = monpos["y"].asFloat();
					addMonster(it.Monname, Vec2(monposx, monposy));
					m_monster->monsterIdForBar = curScene_MonId;
					curScene_MonId++;
					auto monbar = BarManager::getInstance()->create("UI/Enemy_hp_bar2.png", m_monster->monsterIdForBar, m_monster->monMaxHp);
					//��ӵ����ﾫ��Ѫ��������
					auto monsterbarmanager = MonsterBarManager::getInstance();
					monsterbarmanager->getmonsterBarVec().pushBack(monbar);
					monbar->setAnchorPoint(Vec2(0.5, 1));
					monbar->setPosition(Vec2(size.width / 2 - 10, size.height));
					this->addChild(monbar, this->getChildren().size());

					//////////////////////////////////////////////////////////////////////////
					if (it.Monname == "treemonster")
					{
						auto monLabelname = Label::create(gb2312_to_utf8("����"), "Arial", 25);
						monLabelname->setColor(Color3B::ORANGE);
						monbar->addChild(monLabelname);
						monLabelname->setPosition(Vec2(43, 48));
					}
					else if (it.Monname == "gdragonmonster")
					{
						auto monLabelname = Label::create(gb2312_to_utf8("����"), "Arial", 25);
						monLabelname->setColor(Color3B::ORANGE);
						monbar->addChild(monLabelname);
						monLabelname->setPosition(Vec2(43, 48));
					}
					else if (it.Monname == "bonemonster")
					{
						auto monLabelname = Label::create(gb2312_to_utf8("ʯ��"), "Arial", 25);
						monLabelname->setColor(Color3B::ORANGE);
						monbar->addChild(monLabelname);
						monLabelname->setPosition(Vec2(43, 48));
					}
				}
			}
		}	
	}
	//�����Ǽң�����������
	else if (GameScene::sceneId != 2)
	{
		auto mapId_Mon = GameData::getInstance()->getDataFromSceneIdToSetMonData(GameScene::sceneId);

		if (mapId_Mon)
		{
			int curScene_MonId = 0;
			for (auto it : mapId_Mon->MonForObjVec)
			{
				for (int i = 1; i <= it.MonNums; i++)
				{
					ValueMap monpos;
					if (objGroup){
						monpos = objGroup->getObject(it.Monname + convertToString(i));
					}
					float monposx = monpos["x"].asFloat();
					float monposy = monpos["y"].asFloat();
					addMonster(it.Monname, Vec2(monposx, monposy));
					m_monster->monsterIdForBar = curScene_MonId;
					curScene_MonId++;
					auto monbar = BarManager::getInstance()->create("UI/Enemy_hp_bar2.png", m_monster->monsterIdForBar, m_monster->monMaxHp);
					//��ӵ����ﾫ��Ѫ��������
					auto monsterbarmanager = MonsterBarManager::getInstance();
					monsterbarmanager->getmonsterBarVec().pushBack(monbar);
					monbar->setAnchorPoint(Vec2(0.5, 1));
					monbar->setPosition(Vec2(size.width / 2 - 10, size.height));
					this->addChild(monbar, this->getChildren().size());

					//////////////////////////////////////////////////////////////////////////
					if (it.Monname == "treemonster")
					{
						auto monLabelname = Label::create(gb2312_to_utf8("����"), "Arial", 25);
						monLabelname->setColor(Color3B::ORANGE);
						monbar->addChild(monLabelname);
						monLabelname->setPosition(Vec2(43, 48));
					}
					else if (it.Monname == "gdragonmonster")
					{
						auto monLabelname = Label::create(gb2312_to_utf8("����"), "Arial", 25);
						monLabelname->setColor(Color3B::ORANGE);
						monbar->addChild(monLabelname);
						monLabelname->setPosition(Vec2(43, 48));
					}
					else if (it.Monname == "bonemonster")
					{
						auto monLabelname = Label::create(gb2312_to_utf8("ʯ��"), "Arial", 25);
						monLabelname->setColor(Color3B::ORANGE);
						monbar->addChild(monLabelname);
						monLabelname->setPosition(Vec2(43, 48));
					}
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
	//�ж��Ƿ����defeat����,��������ؿ�ʼ���濪ʼ��Ϸ������QuestDispatcher�Ѿ����ڣ���ʱ�����ж�Ȼ���ֶ�������ʱ����
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

	///��Ҫ�ȴ��������addPlayer����Ϊ��addPlayer��ʱ�򣬻ᴴ�����ǿ�ʼʱ�ĶԻ��򣬴�ʱҪ����������ǹ��ﻹû���������Ļ������������ˣ���ɹ�����Զ�
	/*�жϿ�ͷ�����Ƿ���ɣ��ǵĻ�����ӹ���,������ӹ���*/
	//�ҵ�ͼidΪ���Ǽ�2
	if (QuestDispatcher::getInstance()->getqData()[4]->status != QuestStatus::finish && GameScene::sceneId == 2)
	{
		auto mapId_Mon = GameData::getInstance()->getDataFromSceneIdToSetMonData(GameScene::sceneId);
		if (mapId_Mon)
		{
			int curScene_MonId = 0;
			for (auto it : mapId_Mon->MonForObjVec)
			{
				for (int i = 1; i <= it.MonNums; i++)
				{
					ValueMap monpos;
					if (objGroup){
						monpos = objGroup->getObject(it.Monname + convertToString(i));
					}
					float monposx = monpos["x"].asFloat();
					float monposy = monpos["y"].asFloat();
					addMonster(it.Monname, Vec2(monposx, monposy));
					m_monster->monsterIdForBar = curScene_MonId;
					curScene_MonId++;
					auto monbar = BarManager::getInstance()->create("UI/Enemy_hp_bar2.png", m_monster->monsterIdForBar, m_monster->monMaxHp);
					//��ӵ����ﾫ��Ѫ��������
					auto monsterbarmanager = MonsterBarManager::getInstance();
					monsterbarmanager->getmonsterBarVec().pushBack(monbar);
					monbar->setAnchorPoint(Vec2(0.5, 1));
					monbar->setPosition(Vec2(size.width / 2 - 10, size.height));
					this->addChild(monbar, this->getChildren().size());

					//////////////////////////////////////////////////////////////////////////
					if (it.Monname == "treemonster")
					{
						auto monLabelname = Label::create(gb2312_to_utf8("����"), "Arial", 25);
						monLabelname->setColor(Color3B::ORANGE);
						monbar->addChild(monLabelname);
						monLabelname->setPosition(Vec2(43, 48));
					}
					else if (it.Monname == "gdragonmonster")
					{
						auto monLabelname = Label::create(gb2312_to_utf8("����"), "Arial", 25);
						monLabelname->setColor(Color3B::ORANGE);
						monbar->addChild(monLabelname);
						monLabelname->setPosition(Vec2(43, 48));
					}
					else if (it.Monname == "bonemonster")
					{
						auto monLabelname = Label::create(gb2312_to_utf8("ʯ��"), "Arial", 25);
						monLabelname->setColor(Color3B::ORANGE);
						monbar->addChild(monLabelname);
						monLabelname->setPosition(Vec2(43, 48));
					}
				}
			}
		}		
	}
	//�����Ǽң�����������
	else if (GameScene::sceneId != 2)
	{
		auto mapId_Mon = GameData::getInstance()->getDataFromSceneIdToSetMonData(GameScene::sceneId);

		if (mapId_Mon)
		{
			int curScene_MonId = 0;
			for (auto it : mapId_Mon->MonForObjVec)
			{
				for (int i = 1; i <= it.MonNums; i++)
				{
					ValueMap monpos;
					if (objGroup){
						monpos = objGroup->getObject(it.Monname + convertToString(i));
					}
					float monposx = monpos["x"].asFloat();
					float monposy = monpos["y"].asFloat();
					addMonster(it.Monname, Vec2(monposx, monposy));
					m_monster->monsterIdForBar = curScene_MonId;
					curScene_MonId++;
					auto monbar = BarManager::getInstance()->create("UI/Enemy_hp_bar2.png", m_monster->monsterIdForBar, m_monster->monMaxHp);
					//��ӵ����ﾫ��Ѫ��������
					auto monsterbarmanager = MonsterBarManager::getInstance();
					monsterbarmanager->getmonsterBarVec().pushBack(monbar);
					monbar->setAnchorPoint(Vec2(0.5, 1));
					monbar->setPosition(Vec2(size.width / 2 - 10, size.height));
					this->addChild(monbar, this->getChildren().size());

					//////////////////////////////////////////////////////////////////////////
					if (it.Monname == "treemonster")
					{
						auto monLabelname = Label::create(gb2312_to_utf8("����"), "Arial", 25);
						monLabelname->setColor(Color3B::ORANGE);
						monbar->addChild(monLabelname);
						monLabelname->setPosition(Vec2(43, 48));
					}
					else if (it.Monname == "gdragonmonster")
					{
						auto monLabelname = Label::create(gb2312_to_utf8("����"), "Arial", 25);
						monLabelname->setColor(Color3B::ORANGE);
						monbar->addChild(monLabelname);
						monLabelname->setPosition(Vec2(43, 48));
					}
					else if (it.Monname == "bonemonster")
					{
						auto monLabelname = Label::create(gb2312_to_utf8("ʯ��"), "Arial", 25);
						monLabelname->setColor(Color3B::ORANGE);
						monbar->addChild(monLabelname);
						monLabelname->setPosition(Vec2(43, 48));
					}
				}
			}
		}		
	}

	float playerX = 0.0f;
	float playerY = 0.0f;
	
	/*������������*/
	//�����ʼ��Ϸʱ
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
		//����gamescenedirΪnone
		Player::getInstance()->gamescenedir = "none";
	}

	addPlayer(Vec2(playerX, playerY));

	AfterPlayerSetMonsterBindPlayer();

	/*��������hpmp��*/
	if (BarManager::getInstance()->getParent() != NULL)
	{
		BarManager::getInstance()->removeFromParent();
		this->addChild(BarManager::getInstance());
		auto bar = BarManager::getInstance()->create("UI/PlayerBar_hp.png", "UI/PlayerBar_mp.png", m_player->m_hp, m_player->m_mp);		
		bar->setAnchorPoint(Vec2(0, 1));
		bar->setPosition(0, size.height);
		//�����ж���
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
		//�����ж���
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
	//�ж��Ƿ����defeat���񣬵�û�浲���������󵽿�ʼ���濪ʼ��Ϸ����ͷ�����Ǽ�԰��ʱ������QuestDispatcher�����Ѿ����ڣ����Ե��ֶ�����ʱ����
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

	CubeBoss* cureboss = CubeBoss::create("fc");
	m_map->addChild(cureboss);
	cureboss->setPosition(Vec2(600,500));
	cureboss->setParent(m_map);

	//��boss��ӵ�boss������
	BossManager::getInstance()->getBossVec().pushBack(cureboss);

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

		Vec2 tileCoord = FindOneTiledNoBarrier();
		graph->init(tileCoord);


		objGroup = m_map->getObjectGroup("objects");

		//�����ͼidΪ11�ŵ�Ĺ�ص�ͼ����������֡����
		if (GameScene::sceneId == 11)
		{
			m_spriteRain = Sprite::createWithSpriteFrameName("rain1.png");
			this->addChild(m_spriteRain);
			Size vSize = Director::getInstance()->getVisibleSize();
			m_spriteRain->setPosition(Vec2(vSize.width / 2,vSize.height / 2));
			//������Ļ
			m_spriteRain->setScale(vSize.width / m_spriteRain->getContentSize().width,vSize.height / m_spriteRain->getContentSize().height);
			m_spriteRain->runAction(Animate::create(AnimationUtil::createWithSingleFrameName("rain",0.2f,-1)));
		}
		else
		{
			if (m_spriteRain)
			{
				m_spriteRain->removeFromParent();
				m_spriteRain = NULL;
			}
		}

		if (GameScene::sceneId == 5)
		{
			m_particleFire = ParticleFire::create();
			ValueMap fireobj = objGroup->getObject("fire");
			float x = fireobj["x"].asFloat();
			float y = fireobj["y"].asFloat();
			m_particleFire->setPosition(Vec2(x, y));
			m_particleFire->setZOrder(0);
			m_particleFire->setLife(0.08f);		//����ʱ��̣���ֹ�������ӣ�����һ��BUG�������Ļ����ڵ�ͼ����ʱ������Ź�����	
			m_map->addChild(m_particleFire);
			m_particleFire->setVertexZ(-((y + 64) / 64));
		}
		else
		{
			if (m_particleFire)
			{
				m_particleFire->removeFromParent();
				m_particleFire = NULL;
			}
		}
	}
}

void GameScene::addPlayer(Point pos, int direction)
{
	/*����û��ʱ��ͼ�л�����*/
	Player* player = Player::getInstance();
	if (player->getParent() != NULL)
	{
		player->removeFromParentAndCleanup(false);

		//�л�����ʱ���ǵĳ�ʼ��,����Ҫ��ʼ��������

		player->setEnableAction(true);
		//�������ж�ʱ�� 
		player->setPosition(pos);
		player->setTiledMap(m_map);
		//����K��������
		player->createSwordWave();
		m_player = player;
		return;
	}

	/*�ⲿ��Ϊ��һ�ο�ʼ��Ϸ������������ص���ʼ��Ϸ����Ŀ�ʼ��Ϸ(����û�浵ʱ),��û�浵ʱ���ǳ�����IDΪ2�ŵĵ�ͼ*/
	player->setTiledMap(m_map);
	player->init();
	player->setPosition(pos);

	//���õ�ͼ��ʱ�������id == 2�ĵ�ͼ���Ϳ��Ե������ǿ�ʼʱ�ĶԻ���
	//���ҶԻ����Ǹ�������λ�����õģ�����λ�õ��ȳ�ʼ������
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
	
		//player��init�Ѿ�������direction�ˡ�
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
	/*��ʼ�����mp hp*/
	player->setPlayer_hp(saveData->hp);
	player->setPlayer_mp(saveData->mp);
	/*��ʼ����ҵȼ�*/
	player->m_playerlevel = saveData->level;
	/*��ʼ����Ҿ���*/
	player->m_exp = saveData->exp;
	/*��ʼ����ҵ����HP�����MP*/
	player->getCurMaxHp() = 100 + (saveData->level - 1) * 20;
	player->getCurMaxMp() = 100 + (saveData->level - 1) * 35;

	this->addChild(BarManager::getInstance());
	auto playerbar = BarManager::getInstance()->getPlayerBars();
	//����HP,MPbar
	auto bar = BarManager::getInstance()->create("UI/PlayerBar_hp.png", "UI/PlayerBar_mp.png", player->m_hp, player->m_mp);
	bar->setAnchorPoint(Vec2(0, 1));
	bar->setPosition(0, size.height);
	this->addChild(bar, 100);
	//�����ж���
	auto act = BarManager::getInstance()->create("UI/inventoryBtn.png", "UI/questBtn.png");
	act->setAnchorPoint(Vec2(0.5, 0));
	act->setPosition(size.width / 2 + 30, 0);
	this->addChild(act, 100);
	//����������
	auto exp = BarManager::getInstance()->create("UI/exp.png", 100 * saveData->level);
	exp->setAnchorPoint(Vec2(0.5, 0));
	exp->setPosition(size.width / 2, act->getContentSize().height - 5);
	this->addChild(exp, 100);
	//��ʼ�����Ǿ�����
	//��ʼ������Ѫ����HP��

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

	//���õ�ͼ��ʱ�������id == 2�ĵ�ͼ���Ϳ��Ե������ǿ�ʼʱ�ĶԻ���
	//���ҶԻ����Ǹ�������λ�����õģ�����λ�õ��ȳ�ʼ������
	if (m_map && GameScene::sceneId == 2)
		QuestDispatcher::getInstance()->twoIdSceneTalk(m_map);

	//�浵��PlayerDir
	player->setPlayerDir(saveData->direction);
	/*
	��������
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
	//���ﴴ��
	m_monster = Monster::create(name);
	m_map->addChild(m_monster, (int)m_map->getChildren().size());
	m_monster->setPosition(pos);
	//��ӵ����������
	MonsterManager::getInstance()->getMonsterVec().pushBack(m_monster);
	//����ĸ��ֳ�ʼ��
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
	int tiledSize = 0;
	if (m_map)
	{
		tiledSize = m_map->getTileSize().width;
	}

	auto p = m_player->getPosition();
	p = CC_POINT_POINTS_TO_PIXELS(p);
	m_player->setVertexZ(-((p.y + tiledSize) / tiledSize));

	auto Vec1 = MonsterManager::getInstance()->getMonsterVec();
	//	log("MonSize%d", Vec1.size());
	Vector<Node*> Vec;
	for (int i = 0; i < Vec1.size(); i++)
	{
		Vec.pushBack(Vec1.at(i));
	}
	auto Vecboss = BossManager::getInstance()->getBossVec();
	for (int i = 0; i < Vecboss.size(); i++)
	{
		Vec.pushBack(Vecboss.at(i));
	}

	for (int i = 0; i < Vec.size(); i++)
	{
		auto monsterOrboss = Vec.at(i);
		p = monsterOrboss->getPosition();
		p = CC_POINT_POINTS_TO_PIXELS(p);
		monsterOrboss->setVertexZ(-((p.y + tiledSize) / tiledSize));
	}

	
	Vec.pushBack(m_player);
	auto Vec2 = NpcManager::getInstance()->getNpcsVec();
	for (int i = 0; i < Vec2.size(); i++)
	{
		Vec.pushBack(Vec2.at(i));
	}

	//���������zorder
	if (m_particleFire)
	{
		Vec.pushBack(m_particleFire);
	}

	sort(Vec.begin(), Vec.end(), comp);
	for (int i = 0; i < Vec.size(); i++)
	{
		Vec.at(i)->setZOrder(4 + i);
	}
	
	Vec.clear();

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

cocos2d::Vec2 GameScene::FindOneTiledNoBarrier()
{
	if (m_map)
	{
		int row = m_map->getMapSize().height;
		int col = m_map->getMapSize().width;

		//��ʼ����ͨͼ
		for (int initrow = 0; initrow < row; initrow++)
		{
			for (int initcol = 0; initcol < col; initcol++)
			{
				Vec2 tileCoord = Vec2(initcol, initrow);
				int tileGid = m_map->getLayer("barrier")->getTileGIDAt(tileCoord);
				//ͼ���Gid > 0�����ϰ���ͼ����ܴ���
				if (tileGid > 0)
				{
					Value prop = m_map->getPropertiesForGID(tileGid);
					ValueMap proValueMap = prop.asValueMap();

					if (proValueMap.find("Collidable") != proValueMap.end())
					{
						std::string collision = proValueMap.at("Collidable").asString();
						if (collision == "true")
							continue;
						else
							//barrier�㵱ǰ�����Ƿ��ϰ���ͼ��
							return tileCoord;
					}
					//barrier�㵱ǰ�����Ƿ��ϰ���ͼ��
					else
					{
						return tileCoord;
					}
				}
				else
				{
					//barrier�㵱ǰ����û���ϰ���ͼ�飬
					return tileCoord;
				}
			}
		}
	}
}
