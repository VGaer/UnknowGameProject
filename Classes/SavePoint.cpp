#include "SavePoint.h"
#include "SaveGameScene.h"
#include "AnimationUtil.h"
bool SavePoint::init()
{
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
	auto renderTexture = RenderTexture::create(vSize.width, vSize.height);
	renderTexture->begin();
	getParent()->getParent()->visit();
	renderTexture->end();
	Director::getInstance()->pushScene(SaveGameScene::scene(renderTexture));
}