#include "gdShow.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"

Scene* gdShow::createscene()
{
	auto scene = Scene::create();
	auto layer = gdShow::create();
	scene->addChild(layer);
	return scene;
}

bool gdShow::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visiblesize = Director::getInstance()->getVisibleSize();
	Sprite* sprite = NULL;
	float scalX;
	float scalY;
	for (int i = 1; i <= 4; i++)
	{
		sprite = Sprite::create("gdShow/gdshow" + Value(i).asString() + ".png");
		scalX = visiblesize.width / sprite->getContentSize().width;
		scalY = visiblesize.height / sprite->getContentSize().height;
		sprite->setScaleX(scalX);
		sprite->setScaleY(scalY);
		this->addChild(sprite,0,i);
		sprite->setPosition(visiblesize.width / 2 + (i - 1) * visiblesize.width, visiblesize.height / 2);
	}

	//next按钮菜单
	nextMenuItem = MenuItemImage::create(
		"UI/btn_pass_next_n.png",
		"UI/btn_pass_next_p.png",
		CC_CALLBACK_1(gdShow::ButonCallback, this));
	nextMenuItem->setPosition(Vec2(visiblesize.width - nextMenuItem->getContentSize().width / 2,nextMenuItem->getContentSize().height / 2));
	Menu* menu = Menu::create(nextMenuItem, NULL);
	menu->setPosition(Vec2(0, 0));
	this->addChild(menu);

	return true;
}

void gdShow::ButonCallback(Ref* resender)
{
	//如果当前是最后一张图片,点按钮就直接开始游戏
	if (this->getChildByTag(4)->getPositionX() == Director::getInstance()->getVisibleSize().width / 2)
	{
		auto sc = GameScene::createSceneWithId(2);
		auto reScene = TransitionJumpZoom::create(0.0f, sc);
		Director::getInstance()->replaceScene(sc);
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Blip.wav");
		auto m = (MenuItemSprite*)resender;
		m->setEnabled(false);
		return;
	}


	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Blip.wav");
	MenuItemImage* m = (MenuItemImage*)resender;
	m->setEnabled(false);
	CallFunc* call = CallFunc::create([&](){nextMenuItem->setEnabled(true); });
	MoveBy* moveby1 = MoveBy::create(0.4f, Vec2(-Director::getInstance()->getVisibleSize().width, 0));
	MoveBy* moveby2 = MoveBy::create(0.4f, Vec2(-Director::getInstance()->getVisibleSize().width, 0));
	MoveBy* moveby3 = MoveBy::create(0.4f, Vec2(-Director::getInstance()->getVisibleSize().width, 0));
	MoveBy* moveby4 = MoveBy::create(0.4f, Vec2(-Director::getInstance()->getVisibleSize().width, 0));
	this->getChildByTag(2)->runAction(EaseSineInOut::create(moveby2));
	this->getChildByTag(3)->runAction(EaseSineInOut::create(moveby3));
	this->getChildByTag(4)->runAction(EaseSineInOut::create(moveby4));
	this->getChildByTag(1)->runAction(EaseSineInOut::create(Sequence::create(moveby1, call, NULL)));
}

void gdShow::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/gdshow.mp3",true);
}

void gdShow::onExit()
{
	Layer::onExit();
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic("sound/gdshow.mp3");
}