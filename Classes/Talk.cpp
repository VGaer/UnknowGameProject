#include "Talk.h"
#include "Player.h"
#include "MonsterManager.h"
#include "G2U.h"

Talk* Talk::create(vector<string>strVec, int id, int idType)
{
	auto talk = new Talk();
	if (talk && talk->init(strVec, id, idType))
	{
		talk->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(talk);
	}
	return talk;
}

bool Talk::init(vector<string>strVec, int id, int idType)
{
	curPage = 0;
	curTextLen = 3;
	isShowFinish = false;
	this->id = id;
	this->idType = idType;
	this->strVec = strVec;
	bg = Sprite::create("UI/UI_Quest_10.png");
	bg->setScale(1.5);
	bg->setContentSize(bg->getContentSize() * getScale());
	addChild(bg);
	text = Label::create("", "Arial", 14);
	text->setDimensions(148, 50);
	text->setAlignment(TextHAlignment::LEFT);
	text->setAnchorPoint(Point(0.5, 0.5));
	text->setPosition(bg->getContentSize() / 2);
	text->setColor(Color3B::BLACK);
	bg->addChild(text);
	setSceneLockState(true);
	addTouchEventListener();
	setVertexZ(1);
	setPos();
	schedule(schedule_selector(Talk::showFont, this), 0.1);
	return true;
}

void Talk::addTouchEventListener()
{
	listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch* touch, Event* event){
		return true;
	};
	listener->onTouchEnded = [&](Touch* touch, Event* event){
		if (isShowFinish)
		{
			// 翻页
			curPage++;			
			if (curPage < strVec.size())
			{
				schedule(schedule_selector(Talk::showFont, this), 0.1);
				isShowFinish = false;
			}			
			// 对话结束
			else
			{
				_eventDispatcher->removeEventListener(listener);
				setSceneLockState(false);
				setDlgsIsSaid();
				removeFromParent();
			}				
		}
		// 显示所有文字
		else
		{
			curTextLen = 3;
			unschedule(schedule_selector(Talk::showFont));			
			text->setString(gb2312_to_utf8(strVec.at(curPage)));
			isShowFinish = true;
		}
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void Talk::setSceneLockState(bool s)
{
	auto player = Player::getInstance();
	player->setEnableAction(!s);
	auto monsManager = MonsterManager::getInstance();
	monsManager->setMonsEnableAction(!s);
}

void Talk::showFont(float dt)
{
	string str = strVec.at(curPage).substr(0, curTextLen);
	text->setString(gb2312_to_utf8(str));
	curTextLen = curTextLen + 3;
	if (curTextLen > strVec.at(curPage).length()) {
		unschedule(schedule_selector(Talk::showFont));
		isShowFinish = true;
		curTextLen = 3;
	}
}

void Talk::setPos()
{
	Size vSize = Director::getInstance()->getVisibleSize();
	Size pySize = Player::getInstance()->getContentSize();
	Point pyPos = Player::getInstance()->getPosition();
	Point talkPos;
	if (pyPos.x <= vSize.width / 4) talkPos.x = pyPos.x + 160;
	else talkPos.x = pyPos.x - 160;
	if (pyPos.y <= vSize.height / 4) talkPos.y = pyPos.y + 100;
	else talkPos.y = pyPos.y + 45;
	setPosition(talkPos);
}

void Talk::setDlgsIsSaid()
{
	switch (idType)
	{
	case Talk_AcTask:
		GameData::getInstance()->getDataFromPlayerTaskDlgsData(id)->isSaidAcTsDlgs = true;
		break;
	case Talk_CmTask:
		GameData::getInstance()->getDataFromPlayerTaskDlgsData(id)->isSaidCmTsDlgs = true;
		break;
	case Talk_FiTask:
		GameData::getInstance()->getDataFromPlayerTaskDlgsData(id)->isSaidFiTsDlgs = true;
		break;
	case Talk_EtScene:
		GameData::getInstance()->getDataFromEnterSceneDlgsData(id)->isSaid = true;
		break;
	default:
		break;
	}
}