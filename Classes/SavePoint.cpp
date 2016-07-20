#include "SavePoint.h"
#include "AnimationUtil.h"
#include "MonsterManager.h"
#include "Player.h"
#include "G2U.h"

bool SavePoint::init()
{
	isSaving = false;
	isQuiting = false;
	isSavingEnd = true;
	isQuitingEnd = true;
	auto sprite = Sprite::createWithSpriteFrameName("save0.png");
	sprite->setScale(0.6);
	sprite->setContentSize(sprite->getContentSize() * sprite->getScale());
	bindSprite(sprite);
	addTouchEventListener();
	getSprite()->runAction(Animate::create(AnimationUtil::createWithSingleFrameName("save", 0.1, -1)));
	return true;
}

void SavePoint::addTouchEventListener()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(SavePoint::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(SavePoint::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool SavePoint::onTouchBegan(Touch* touch, Event* event)
{
	Point touchPos = getParent()->convertToNodeSpace(touch->getLocation());
	if (getBoundingBox().containsPoint(touchPos))
	{
		return true;
	}
	return false;
}

void SavePoint::onTouchEnded(Touch* touch, Event* event)
{	
	Size vSize = Director::getInstance()->getVisibleSize();
	setSceneLockState(true);
	layer = Node::create();
	layer->setContentSize(vSize);
	gameLayer = Player::getInstance()->getTiledMap()->getParent();
	gameLayer->addChild(layer, 9999);
	// 保存按钮
	saveBtn = Label::create(gb2312_to_utf8("存档"), "Arial", 45);
	saveBtn->setColor(Color3B::ORANGE);
	layer->addChild(saveBtn, 1000);
	saveBtn->setPosition(vSize.width / 2, vSize.height / 2 + 55);
	// 退出按钮
	quitBtn = Label::create(gb2312_to_utf8("退出"), "Arial", 45);
	quitBtn->setColor(Color3B::ORANGE);
	layer->addChild(quitBtn, 1000);
	quitBtn->setPosition(vSize.width / 2, vSize.height / 2 - 55);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch* touch, Event* event){	
		if (saveBtn == NULL || quitBtn == NULL) 
			return true;
		auto touchPos = touch->getLocation();
		if (saveBtn->getBoundingBox().containsPoint(touchPos) && !isSaving)
		{
			isSaving = true;
			isQuiting = false;
		}
					
		else if (quitBtn->getBoundingBox().containsPoint(touchPos) && !isQuiting)
		{
			isQuiting = true;
			isSaving = false;
		}		
		return true;
	};
	listener->onTouchEnded = [=](Touch* touch, Event* event){
		if (isQuiting)
		{
			if (isQuitingEnd)
			{
				isQuitingEnd = false; 
				quit();
			}
				
		}
			
		else if (isSaving)
		{
			if (isSavingEnd)
			{
				isSavingEnd = false;
				save();
			}
				
		}
			
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, layer);
}

void SavePoint::setSceneLockState(bool isLock)
{
	auto player = Player::getInstance();
	player->setEnableAction(!isLock);
	auto monsManager = MonsterManager::getInstance();
	monsManager->setMonsEnableAction(!isLock);
}

void SavePoint::save()
{
	auto game = GameData::getInstance();
	game->writePlayerData();
	game->writeQuestData();
	if (saveBtn)
		saveBtn->removeFromParent();
	if (quitBtn)
		quitBtn->removeFromParent();
	saveBtn = NULL;
	quitBtn = NULL;
	// 成功保存
	Size  vSize = Director::getInstance()->getVisibleSize();
	auto succLabel = Label::create(gb2312_to_utf8("已成功保存游戏"), "Arial", 45);
	succLabel->setColor(Color3B::ORANGE);
	getParent()->addChild(succLabel, 1000);
	succLabel->setPosition(getParent()->convertToNodeSpace(Point(vSize.width / 2, vSize.height / 2)));
	auto popDelay = Sequence::create(DelayTime::create(1), CallFunc::create([=](){
		setSceneLockState(false);
		if (succLabel)
			succLabel->removeFromParent();
		if (layer)
			layer->removeFromParent();
		layer = NULL;
		isSaving = false;
		isSavingEnd = true;
	}), NULL);
	runAction(popDelay);
}

void SavePoint::quit()
{
	if (saveBtn)
		saveBtn->removeFromParent();
	saveBtn = NULL;
	if (quitBtn)
		quitBtn->removeFromParent();
	quitBtn = NULL;
	if (layer)
		layer->removeFromParent();
	layer = NULL;
	setSceneLockState(false);
	isQuiting = false;
	isQuitingEnd = true; 
}