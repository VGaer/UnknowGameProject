#include "GameScene.h"
#include "Monster.h"
#include "Graph.h"
#include "GameData.h"

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


bool GameScene::init(int sceneId)
{
	// 先不管id
	setMapInfo(sceneId);
	loadPlistFile();

	// test
	Player* player = Player::getInstance();
	player->setTiledMap(m_map);
	player->init();
	player->getSprite()->setScale(player->getPlayer_magnification());
	player->getSprite()->setPosition(Vec2(player->getContentSize().width * player->getPlayer_magnification() / 2,
		player->getContentSize().height * player->getPlayer_magnification() / 2));
	player->setContentSize(player->getContentSize() * player->getPlayer_magnification());
	player->setAnchorPoint(Vec2(0.5, 0.5));
	player->setPosition(Vec2(100, 250));
	Sprite* dian = Sprite::create("dian.jpg");
	dian->setPosition(player->getContentSize().width, 0);
	player->addChild(dian);
	Sprite* dian2 = Sprite::create("dian.jpg");
	player->addChild(dian2);
	m_player = player;

	m_monster = Monster::create("treemonster");
	m_map->addChild(m_monster, (int)m_map->getChildren().size());
	m_monster->getSprite()->setScale(1.5);
	m_monster->setContentSize(m_monster->getContentSize() * 1.5);
	m_monster->setAnchorPoint(Vec2(0.5, 0.2));
	m_monster->setMonsterParent(m_map);
	m_monster->setvecPatrolpoint();
	m_monster->setPosition(32, 384);
	m_monster->bindPlayer(m_player);
	m_monster->getAnimBase()->setCurDirection(m_player->getPosition());
	MonsterManager::getInstance()->getMonsterVec().pushBack(m_monster);

	scheduleUpdate();
	return true;
}

void GameScene::setMapInfo(int id)
{
	m_map = TMXTiledMap::create("home.tmx");
	m_map->getLayer("barrier")->setVisible(false);
	addChild(m_map, 0, 1);
	auto graph = Graph::getInstance();
	graph->setTildMap(m_map);
	graph->init(Point(16, 20));

}

void GameScene::addMonster(const std::string& name, Point pos)
{
	auto monster = Monster::create(name);
	monster->setPosition(pos);
	m_map->addChild(monster, (int)m_map->getChildren().size());
	MonsterManager::getInstance()->getMonsterVec().pushBack(monster);	// 添加至管理器
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

	p = m_monster->getPosition();
	p = CC_POINT_POINTS_TO_PIXELS(p);
	m_monster->setVertexZ(-((p.y + 64) / 64));
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
}