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
   
    if ( !Layer::init() )
    {
        return false;
    }    
	
	TMXTiledMap* map = TMXTiledMap::create("home.tmx");
	map->getLayer("barrier")->setVisible(false);
	m_map = map;

	Size visiblesize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	map->setPosition(Vec2(0, 0));

	this->addChild(map,0,1);

	Player* player = Player::create();
	player->setScale(1.5);
	player->setAnchorPoint(Vec2(0.5,0.5));
	player->bindSprite(Sprite::create("player.png"));
	player->setTiledMap(map);
	player->setPosition(Vec2(100, 150));

    map->addChild(player,(int)map->getChildren().size());
	
	//map->addChild(player,0);
	
	m_player = player;
	
	scheduleUpdate();
	
	
	
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
	//m_player->setPositionZ(-((p.y + 64) / 64));
	//m_player->setPositionZ(-((p.y + 64) / 64));
	m_player->setVertexZ(-((p.y + 64) / 64));
	//m_player->setGlobalZOrder(-((p.y + 64) / 64));
	//log("%f", m_player->getVertexZ());
	//log("Z%f", m_player->getPositionZ());
	setViewpointCenter(m_player->getPosition());

	//log("backz%f", m_map->getLayer("background")->getVertexZ());
	//log("backzo%d", m_map->getLayer("background")->getZOrder());
	//log("pz%f",m_player->getVertexZ());
	
	//log("pzo%d", m_player->getZOrder());
	//log("~~~%f", m_map->getLayer("foreground")->getVertexZ());
	//log("~~~%f", m_map->getLayer("foreground")->getPositionZ());
}

void HelloWorld::setViewpointCenter(Vec2 Position)
{
	Size visiblesize = Director::getInstance()->getVisibleSize();
	int x = MAX(Position.x,visiblesize.width / 2);
	int y = MAX(Position.y,visiblesize.height / 2);
	x = MIN(x, (m_map->getMapSize().width * m_map->getTileSize().width) - visiblesize.width / 2);
	y = MIN(y,(m_map->getMapSize().height * m_map->getTileSize().height) - visiblesize.height / 2);
	Vec2 pointA = Vec2(visiblesize.width / 2,visiblesize.height / 2);
	Vec2 pointB = Vec2(x,y);

	Vec2 offset = pointA - pointB;
	this->setPosition(offset);
}