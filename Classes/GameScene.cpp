#include "GameScene.h"
#include "Monster.h"
#include "NPC.h"
#include "SavePoint.h"
#include "Pop.h"
#include "Graph.h"
#include "GameData.h"
#include "algorithm"
#include "SceneIdManager.h"

int GameScene::sceneId = 2;

bool comp(Entity* a, Entity*b)
{
	if (a->getPositionY() > b->getPositionY())
		return true;
	else
		return false;
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


bool GameScene::init(int sceneId)
{
	setMapInfo(sceneId);
	loadPlistFile();
	addPlayer(Point(100, 250));
	this->addChild(BarManager::getInstance());
	auto bar = BarManager::getInstance()->create("UI/Enemy_hp_bar2.png", 0);
	bar->setAnchorPoint(Vec2(0, 0.5));
	bar->setPosition(400, 400);
	this->addChild(bar, 100);
	BarManager::getInstance()->setPercent(BarManager::getInstance()->getBars(0), 1000, 900);

	auto savePoint = SavePoint::create();
	savePoint->setPosition(Point(500, 350));
	m_map->addChild(savePoint, 100);

	//BarManager::getInstance()->recover(BarType::Enemy, 0);
	//auto bar = BarManager::getInstance()->create("UI/Bar_hp.png", "UI/Bar_mp.png");
	//BarManager::getInstance()->setPercent(BarManager::getInstance()->getPlayerBars()->m_hp, 800, 500);
	//BarManager::getInstance()->setPercent(BarManager::getInstance()->getPlayerBars()->m_mp, 1800, 1000);
	//BarManager::getInstance()->recover(BarType::HP);
	//BarManager::getInstance()->recover(BarType::MP);
	/*bar->setAnchorPoint(Vec2(0, 0.5));
	bar->setPosition(400, 400);
	this->addChild(bar, 100);*/

	//int i = 2;
	//do{
	//	i--;
	//	m_monster = Monster::create("treemonster");
	//	m_map->addChild(m_monster, (int)m_map->getChildren().size());
	//	m_monster->getSprite()->setScale(1.5);
	//	m_monster->setContentSize(m_monster->getContentSize() * 1.5);
	//	m_monster->setAnchorPoint(Vec2(0.5, 0.2));
	//	m_monster->setMonsterParent(m_map);
	//	m_monster->setvecPatrolpoint();
	//	m_monster->setPosition(32 + rand() % 200, 384 + rand() % 200);
	//	m_monster->bindPlayer(m_player);
	//	m_monster->getAnimBase()->setCurDirection(m_player->getPosition());
	//	MonsterManager::getInstance()->getMonsterVec().pushBack(m_monster);
	//} while (i);
	//i = 1;
	//do{
	//	i--;
	//	m_monster = Monster::create("gdragonmonster");
	//	m_map->addChild(m_monster, (int)m_map->getChildren().size());
	//	m_monster->getSprite()->setScale(1.5);
	//	m_monster->setContentSize(m_monster->getContentSize() * 1.5);
	//	m_monster->setAnchorPoint(Vec2(0.5, 0.2));
	//	m_monster->setMonsterParent(m_map);
	//	m_monster->setvecPatrolpoint();
	//	m_monster->setPosition(32 + rand() % 200, 384 + rand() % 200);
	//	m_monster->bindPlayer(m_player);
	//	m_monster->getAnimBase()->setCurDirection(m_player->getPosition());
	//	MonsterManager::getInstance()->getMonsterVec().pushBack(m_monster);
	//} while (i);

	

	{
		m_npc = NPC::create("ÁÎºÆÐÛ");
		m_npc->setAnchorPoint(Vec2(.5f, .5f));
		m_npc->setTiledMap(m_map);
		m_npc->setPosition(Vec2(1000, 400));
		m_npc->setPlayer(m_player);
		m_map->addChild(m_npc, (int)m_map->getChildren().size());
		auto p = m_npc->getPosition();
		p = CC_POINT_POINTS_TO_PIXELS(p);
		m_npc->setVertexZ(-((p.y + 64) / 64));
		auto pop = Pop::create(Vec2(m_npc->getPosition().x, m_npc->getPosition().y - 50));
		this->addChild(pop, 3);
		PopManager::getInstance()->getPopsMap()["ÁÎºÆÐÛ"] = pop;
		NpcManager::getInstance()->getNpcsVec().pushBack(m_npc);
	}

	{
		m_npc = NPC::create("ÓàÓÀŸö");
		m_npc->setAnchorPoint(Vec2(.5f, .5f));
		m_npc->setTiledMap(m_map);
		m_npc->setPosition(Vec2(1000, 700));
		m_npc->setPlayer(m_player);
		m_map->addChild(m_npc, (int)m_map->getChildren().size());
		auto p = m_npc->getPosition();
		p = CC_POINT_POINTS_TO_PIXELS(p);
		m_npc->setVertexZ(-((p.y + 64) / 64));
		auto pop = Pop::create(Vec2(m_npc->getPosition().x, m_npc->getPosition().y - 50));
		this->addChild(pop, 3);
		PopManager::getInstance()->getPopsMap()["ÓàÓÀŸö"] = pop;
		NpcManager::getInstance()->getNpcsVec().pushBack(m_npc);
	}

	scheduleUpdate();
	return true;
}

bool GameScene::init()
{
	auto saveData = GameData::getInstance()->getPlayerData();
	loadPlistFile();
	setMapInfo(saveData->sceneId);
	addPlayer(saveData);

	this->addChild(BarManager::getInstance());
	auto bar = BarManager::getInstance()->create("UI/Enemy_hp_bar2.png", 0);
	bar->setAnchorPoint(Vec2(0, 0.5));
	bar->setPosition(400, 400);
	this->addChild(bar, 100);
	BarManager::getInstance()->setPercent(BarManager::getInstance()->getBars(0), 1000, 900);

	auto savePoint = SavePoint::create();
	savePoint->setPosition(Point(500, 350));
	m_map->addChild(savePoint, 100);

	scheduleUpdate();
	return true;
}

void GameScene::setMapInfo(int id)
{
	//m_map = TMXTiledMap::create("home.tmx");
	if (SceneIdManager::getInstance()->map_sceneIdToname.find(id) != SceneIdManager::getInstance()->map_sceneIdToname.end())
	{
		m_map = TMXTiledMap::create(SceneIdManager::getInstance()->map_sceneIdToname[id]);
		m_map->getLayer("barrier")->setVisible(false);
		addChild(m_map, 0, 1);
		auto graph = Graph::getInstance();
		graph->setTildMap(m_map);
		graph->init(Point(16, 20));
		sceneId = id;
	}
}

void GameScene::addPlayer(Point pos, int direction)
{
	Player* player = Player::getInstance();
	player->setTiledMap(m_map);
	player->init();
	player->getSprite()->setScale(player->getPlayer_magnification());
	player->getSprite()->setPosition(Vec2(player->getContentSize().width * player->getPlayer_magnification() / 2,
		player->getContentSize().height * player->getPlayer_magnification() / 2));
	player->setContentSize(player->getContentSize() * player->getPlayer_magnification());
	player->setAnchorPoint(Vec2(0.5, 0.5));
	player->setPosition(pos);
	player->setPlayerDir(direction);
	Sprite* dian = Sprite::create("dian.jpg");
	dian->setPosition(player->getContentSize().width, 0);
	player->addChild(dian);
	Sprite* dian2 = Sprite::create("dian.jpg");
	player->addChild(dian2);
	m_player = player;
}

void GameScene::addPlayer(PlayerData* saveData)
{
	Player* player = Player::getInstance();
	player->setTiledMap(m_map);
	player->init();
	player->getSprite()->setScale(player->getPlayer_magnification());
	player->getSprite()->setPosition(Vec2(player->getContentSize().width * player->getPlayer_magnification() / 2,
		player->getContentSize().height * player->getPlayer_magnification() / 2));
	player->setContentSize(player->getContentSize() * player->getPlayer_magnification());
	player->setAnchorPoint(Vec2(0.5, 0.5));
	player->setPosition(Point(saveData->posX, saveData->posY));
	player->setPlayerDir(saveData->direction);
	/*
		ÆäËûÊôÐÔ
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
	auto monster = Monster::create(name);
	monster->setPosition(pos);
	m_map->addChild(monster, (int)m_map->getChildren().size());
	MonsterManager::getInstance()->getMonsterVec().pushBack(monster);	// Ìí¼ÓÖÁ¹ÜÀíÆ÷
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
	sort(Vec.begin(), Vec.end(), comp);
	for (int i = 0; i < Vec.size(); i++)
	{
		Vec.at(i)->setZOrder(4 + i);
	}

	
	/*p = m_monster->getPosition();
	p = CC_POINT_POINTS_TO_PIXELS(p);
	m_monster->setVertexZ(-((p.y + 64) / 64));*/
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
	this->setPosition(offset);
}

void GameScene::loadPlistFile()
{
	SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
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
}