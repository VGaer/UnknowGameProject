#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* HelloWorld::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = HelloWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{

	if (!Layer::init())
	{
		return false;
	}
	/*���ض���*/
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


	/*���ض���*/
	TMXTiledMap* map = TMXTiledMap::create("home.tmx");
	map->getLayer("barrier")->setVisible(false);
	m_map = map;

	Size visiblesize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	map->setPosition(Vec2(0, 0));

	this->addChild(map, 0, 1);

	Player* player = Player::createWithparent(map);
	//bindSprite�����Ǳ��и�ContenSize,��������Ĭ�ϵ�0��0 ContenSize
	player->bindSprite(Sprite::create("player.png"));
	//�Ŵ����Ǿ���
	player->getSprite()->setScale(player->getPlayer_magnification());
	player->getSprite()->setPosition(Vec2(player->getContentSize().width * player->getPlayer_magnification() / 2,
		player->getContentSize().height * player->getPlayer_magnification() / 2));

	//�Ŵ����ǵ�contensize
	player->setContentSize(player->getContentSize() * player->getPlayer_magnification());

	//�������ǵ�ê������ǵľ���һ��Ϊ��0.5��0.5) //��ʱ����Player���Node�ڵ���Player��������ˣ������غ�
	player->setAnchorPoint(Vec2(0.5, 0.5));



	player->setTiledMap(map);
	player->setPosition(Vec2(100, 250));

	Sprite* dian = Sprite::create("dian.jpg");
	dian->setPosition(player->getContentSize().width, 0);
	player->addChild(dian);

	Sprite* dian2 = Sprite::create("dian.jpg");
	player->addChild(dian2);

	//map->addChild(player,(int)map->getChildren().size()); ͨ��Player��createWithparent(map)ִ����	

	m_player = player;

	scheduleUpdate();

	auto drawNode = DrawNode::create();
	drawNode->drawLine(Vec2(0, 0), Vec2(0, 2000), Color4F::BLUE);
	player->addChild(drawNode);

	auto drawNode1 = DrawNode::create();
	drawNode1->drawLine(Vec2(player->getContentSize().width, 0), Vec2(player->getContentSize().width, 2000), Color4F::BLACK);
	player->addChild(drawNode1);

	auto drawNode2 = DrawNode::create();
	drawNode2->drawLine(Vec2(player->getContentSize().width / 2, 0), Vec2(player->getContentSize().width / 2, 2000), Color4F::BLUE);
	player->addChild(drawNode2);

	auto drawNode3 = DrawNode::create();
	drawNode3->drawLine(Vec2(640, 0), Vec2(640, 2000), Color4F::BLACK);
	map->addChild(drawNode3);

	auto drawNode4 = DrawNode::create();
	drawNode4->drawLine(Vec2(682, 0), Vec2(682, 2000), Color4F::BLACK);
	map->addChild(drawNode4);

	//�����������
	GameData* gamedata = GameData::getInstance();
	
	//��ʼ������Ѱ·ͼ 
	initGraph();

	//m_monster = Monster::create("bear");
	//map->addChild(m_monster, (int)map->getChildren().size());
	//m_monster->setMonsterParent(map);
	//m_monster->setvecPatrolpoint();//����Ѳ�ߵ�
	//m_monster->setPosition(32, 384);
	////m_monster->setPosition(32, 32);
	//m_monster->bindPlayer(m_player);
	//m_monster->getAnimBase()->setCurDirection(m_player->getPosition());//��ʼ������������

	//m_monster->getSprite()->setScale(m_monster->getMonster_magnification());//�Ŵ����
	//m_monster->getSprite()->setPosition(Vec2(m_monster->getContentSize().width * 2 / 2,
	//	m_monster->getContentSize().height * 2 / 2));
	//m_monster->setContentSize(m_monster->getContentSize() * 2);
	//Sprite* sprite222 = Sprite::create("dian.jpg");
	//m_monster->addChild(sprite222);
	//sprite222->setPosition(Vec2(m_monster->getContentSize().width / 2,m_monster->getContentSize().height / 2));
	//sprite222->setPosition(Vec2());


	/*�������ֲ���*/
	m_monster = Monster::create("bear");
	map->addChild(m_monster, (int)map->getChildren().size());
	m_monster->setMonsterParent(map);
	m_monster->setvecPatrolpoint();
	m_monster->setPosition(32, 384);
	m_monster->bindPlayer(m_player);
	m_monster->getAnimBase()->setCurDirection(m_player->getPosition());


	return true;
}

void HelloWorld::onEnter()
{
	Layer::onEnter();
	Director::getInstance()->setProjection(Director::Projection::_2D);
	Director::getInstance()->setDepthTest(true);
}

void HelloWorld::onExit()
{
	Director::getInstance()->setProjection(Director::Projection::DEFAULT);
	Director::getInstance()->setDepthTest(false);
	Layer::onExit();
}

void HelloWorld::update(float dt)
{
	auto p = m_player->getPosition();
	p = CC_POINT_POINTS_TO_PIXELS(p);
	m_player->setVertexZ(-((p.y + 64) / 64));

	p = m_monster->getPosition();
	p = CC_POINT_POINTS_TO_PIXELS(p);
	m_monster->setVertexZ(-((p.y + 64) / 64));
	setViewpointCenter(m_player->getPosition());
}

void HelloWorld::setViewpointCenter(Vec2 Position)
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

void HelloWorld::initGraph()
{
	graph = Graph::getInstance();
	graph->setTildMap(m_map);
	graph->init(Vec2(16,20));
	return;
}