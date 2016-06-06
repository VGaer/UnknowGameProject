#include "SaveGameScene.h"
#include "ui\CocosGUI.h"
#include "GameData.h"
#include "G2U.h"
using namespace cocos2d::ui;

Scene* SaveGameScene::scene(RenderTexture* sqr) {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto scene = Scene::create();
	auto layer = SaveGameScene::create();
	Sprite *_spr = Sprite::createWithTexture(sqr->getSprite()->getTexture());
	_spr->setPosition(visibleSize / 2);
	_spr->setFlipY(true);					//UI坐标系与OpenGL坐标系相反
	_spr->setColor(Color3B::GRAY);
	scene->addChild(_spr);
	scene->addChild(layer, 1);
	return scene;
}

bool SaveGameScene::init()
{
	if (!Layer::init()){
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	// 保存按钮
	saveBtn = Label::create(gb2312_to_utf8("存档"), "Arial", 45);
	saveBtn->setColor(Color3B::ORANGE);
	addChild(saveBtn, 1);
	saveBtn->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 + 55));
	addSaveBtnListener(saveBtn);
	// 退出按钮
	quitBtn = Label::create(gb2312_to_utf8("退出"), "Arial", 45);
	quitBtn->setColor(Color3B::ORANGE);
	addChild(quitBtn, 1);
	quitBtn->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - 55));
	addQuitBtnListener(quitBtn);
	return true;
}

void SaveGameScene::addSaveBtnListener(Node* node)
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch* touch, Event* event){
		if (node->getBoundingBox().containsPoint(touch->getLocation()))
			return true;
		return false;
	};
	listener->onTouchEnded = [=](Touch* touch, Event* event){
		log("save");
		auto game = GameData::getInstance();
		//game->writeQuestData();
		game->writePlayerData();
		game->writeQuestData();
		saveBtn->removeFromParent();
		quitBtn->removeFromParent();
		// 成功保存
		Size  vSize = Director::getInstance()->getVisibleSize();
		auto succLabel = Label::create(gb2312_to_utf8("已成功保存游戏"), "Arial", 45);
		succLabel->setColor(Color3B::ORANGE);
		addChild(succLabel, 1);
		succLabel->setPosition(Point(vSize.width / 2, vSize.height / 2));
		auto popDelay = Sequence::create(DelayTime::create(1), CallFunc::create([](){
			Director::getInstance()->popScene();
		}), NULL);
		runAction(popDelay);
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, node);
}

void SaveGameScene::addQuitBtnListener(Node* node)
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch* touch, Event* event){
		if (node->getBoundingBox().containsPoint(touch->getLocation()))
			return true;
		return false;
	};
	listener->onTouchEnded = [=](Touch* touch, Event* event){
		Director::getInstance()->popScene();
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, node);
}