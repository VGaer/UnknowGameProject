#include "GameScene.h"
#include "Monster.h"
#include "Graph.h"
#include "GameData.h"
#include "algorithm"
#include "SceneIdManager.h"
#include "BarManager.h"
#include "SavePoint.h"
#include <sstream>
#include "G2U.h"
#include "MonsterBarManager.h"

int GameScene::sceneId = 2;

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
	auto saveData = GameData::getInstance()->getPlayerData();
	loadPlistFile();
	setMapInfo(saveData->sceneId);
	addPlayer(saveData);
	
	auto savePoint = SavePoint::create();
	savePoint->setPosition(Point(500, 350));
	m_map->addChild(savePoint, 100);

	scheduleUpdate();
	this->schedule(schedule_selector(GameScene::MonHP_MPBar_Update), 0.2f);
	return true;
}

bool GameScene::init(int sceneId)
{
	setMapInfo(sceneId);
	loadPlistFile();
	/*加载主角坐标*/
	ValueMap playerPointMap = objGroup->getObject("PlayerPoint");
	float playerX = playerPointMap["x"].asFloat();
	float playerY = playerPointMap["y"].asFloat();
	addPlayer(Vec2(playerX, playerY));	

	/*创建主角hpmp条*/
	if (BarManager::getInstance()->getParent() != NULL)
	{
		BarManager::getInstance()->removeFromParent();
		this->addChild(BarManager::getInstance());
		auto bar = BarManager::getInstance()->create("UI/PlayerBar_hp.png", "UI/PlayerBar_mp.png");
		bar->setAnchorPoint(Vec2(0, 0));
		bar->setPosition(0, 0);

		auto playerbar = BarManager::getInstance()->getPlayerBars();
		if (playerbar != NULL)
		{
			playerbar->m_hp->setPercentage(m_player->m_hp);
			playerbar->m_mp->setPercentage(m_player->m_mp);
		}
		this->addChild(bar, 100);
	}
	
	
	auto savePoint = SavePoint::create();
	savePoint->setPosition(Point(500, 350));
	m_map->addChild(savePoint, 100);
	
	string monname[3] = { "treemonster", "gdragonmonster", "bonemonster" };
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			ValueMap monpos = objGroup->getObject("Mon" + convertToString(i * 2 + j + 1));
			float monposx = monpos["x"].asFloat();
			float monposy = monpos["y"].asFloat();
			addMonster(monname[i],Vec2(monposx,monposy));
			m_monster->monsterIdForBar = i * 2 + j;
			auto monbar = BarManager::getInstance()->create("UI/Enemy_hp_bar2.png",m_monster->monsterIdForBar);
			//添加到怪物精灵血条管理器
			auto monsterbarmanager = MonsterBarManager::getInstance();
			monsterbarmanager->getmonsterBarVec().pushBack(monbar);
			monbar->setAnchorPoint(Vec2(0,1));
			monbar->setPosition(Vec2(0, Director::getInstance()->getVisibleSize().height));
			this->addChild(monbar, this->getChildren().size());
			if (monname[i] == "treemonster")
			{
				auto monLabelname = Label::create(gb2312_to_utf8("树怪"), "Arial", 25);
				monLabelname->setColor(Color3B::ORANGE);
				monbar->addChild(monLabelname);
				monLabelname->setPosition(Vec2(43,48));
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
	

	{
		/*NPC* npc = NPC::createWithparent(m_map);
		npc->bindSprite(Sprite::create("player11.png"));
		npc->setAnchorPoint(Vec2(.5f, .5f));
		npc->setTiledMap(m_map);
		npc->setPosition(Vec2(1000, 400));
		npc->setPlayer(player);
		npc->initDataWithName("廖浩雄");
		auto p = npc->getPosition();
		p = CC_POINT_POINTS_TO_PIXELS(p);
		npc->setVertexZ(-((p.y + 64) / 64));
		PopManager::getInstance()->setAnchorPoint(Vec2(0.5, 0));
		PopManager::getInstance()->setPosition(Vec2(npc->getPosition().x, npc->getPosition().y - 50));
		this->addChild(PopManager::getInstance(), 3);
		NpcManager::getInstance()->getNpcsVec().pushBack(npc);*/
	}

	scheduleUpdate();
	this->schedule(schedule_selector(GameScene::MonHP_MPBar_Update), 0.2f);
	return true;
}

void GameScene::setMapInfo(int id)
{

	if (SceneIdManager::getInstance()->map_sceneIdToname.find(id) != SceneIdManager::getInstance()->map_sceneIdToname.end())
	{
	
		m_map = TMXTiledMap::create(SceneIdManager::getInstance()->map_sceneIdToname[id]);
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
	Player* player = Player::getInstance();
	if (player->getParent() != NULL)
	{
		player->removeFromParent();
		player->setPosition(pos);
		player->setTiledMap(m_map);
		m_player = player;
		return;
	}

	player->setTiledMap(m_map);
	player->init();
	player->getSprite()->setScale(player->getPlayer_magnification());
	player->getSprite()->setAnchorPoint(Vec2(0.5, 0));
	player->getSprite()->setPosition(Vec2(player->getContentSize().width * player->getPlayer_magnification() / 2,
		0));
	player->setContentSize(player->getContentSize() * player->getPlayer_magnification());
	player->setAnchorPoint(Vec2(0.5, 0));	
	player->setPosition(pos);
	player->setPlayerDir(direction);


	Sprite* dian = Sprite::create("dian.jpg");
	dian->setPosition(player->getContentSize().width, 0);
	player->addChild(dian);
	Sprite* dian2 = Sprite::create("dian.jpg");
	player->addChild(dian2);
	Sprite* dian3 = Sprite::create("dian.jpg");
	player->addChild(dian3);
	dian3->setPosition(Vec2(player->getContentSize().width * player->getAnchorPoint().x,
		player->getContentSize().height * player->getAnchorPoint().y));

	m_player = player;
}

void GameScene::addPlayer(PlayerData* saveData)
{
	Player* player = Player::getInstance();
	player->setTiledMap(m_map);
	player->init();
	/*初始化玩家mp hp*/
	player->setPlayer_hp(saveData->hp);
	player->setPlayer_mp(saveData->mp);

	this->addChild(BarManager::getInstance());
	auto bar = BarManager::getInstance()->create("UI/PlayerBar_hp.png", "UI/PlayerBar_mp.png");
	bar->setAnchorPoint(Vec2(0, 0));
	bar->setPosition(0, 0);
	this->addChild(bar, 100);
	//初始化主角血条和HP条
	auto playerbar = BarManager::getInstance()->getPlayerBars();
	if (playerbar != NULL)
	{
		playerbar->m_hp->setPercentage(saveData->hp / player->getCurMaxHp());
		playerbar->m_mp->setPercentage(saveData->mp / player->getCurMaxMp());
	}
	
	player->getSprite()->setScale(player->getPlayer_magnification());
	player->getSprite()->setAnchorPoint(Vec2(0.5, 0));
	player->getSprite()->setPosition(Vec2(player->getContentSize().width * player->getPlayer_magnification() / 2,
		0));
	player->setContentSize(player->getContentSize() * player->getPlayer_magnification());
	player->setAnchorPoint(Vec2(0.5, 0));
	player->setPosition(Point(saveData->posX, saveData->posY));
	player->setPlayerDir(saveData->direction);
	/*
	其他属性
	*/

	Sprite* dian = Sprite::create("dian.jpg");
	dian->setPosition(player->getContentSize().width, 0);
	player->addChild(dian);
	Sprite* dian2 = Sprite::create("dian.jpg");
	player->addChild(dian2);
	m_player = player;
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
	m_monster->getSprite()->setScale(1.5);
	m_monster->getSprite()->setAnchorPoint(Vec2(0.5, 0));
	m_monster->setContentSize(m_monster->getContentSize() * 1.5);
	m_monster->getSprite()->setPosition(Vec2(m_monster->getContentSize().width / 2, 0));
	m_monster->setAnchorPoint(Vec2(0.5, 0));
	m_monster->setMonsterParent(m_map);
	m_monster->setvecPatrolpoint();
	m_monster->bindPlayer(m_player);
	m_monster->getAnimBase()->setCurDirection(m_player->getPosition());	
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

void GameScene::loadPlistFile()
{
	SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
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
}

void GameScene::MonHP_MPBar_Update(float dt)
{
	auto Vec = MonsterManager::getInstance()->getMonsterVec();
	for (int i = 0; i < Vec.size(); i++)
	{
		auto monster = Vec.at(i);
		int Id = monster->monsterIdForBar;
		auto monbar = BarManager::getInstance()->getBars(Id);
		if (monbar != NULL)
		{
			BarManager::getInstance()->setPercent(monbar, monster->monMaxHp, monster->monsdata.hp);
		}
	}
}
