#include "NPC.h"
#include "G2U.h"
#include "QuestList.h"
#include "PopManager.h"
NPC* NPC::createWithparent(TMXTiledMap* parent)
{
	NPC* npc = new NPC();
	if (npc) {
		parent->addChild(npc, (int)parent->getChildren().size());
	}
	if (npc && npc->init()) {
		npc->autorelease();
		return npc;
	}
	else {
		delete npc;
		npc = NULL;
		return NULL;
	}
}

bool NPC::init() {
	isPop = false;
	isPop2 = false;
	page = 0;
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(NPC::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(NPC::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

bool NPC::onTouchBegan(Touch * _touch, Event * _event)
{
	if (!_event->getCurrentTarget()->getBoundingBox().containsPoint(m_map->convertToNodeSpace(_touch->getLocation())))
		return false;
	log("touchbegan");
	return true;
}

void NPC::onTouchEnded(Touch * _touch, Event * _event)
{
	if (PopManager::getInstance()->getPopped(0)) {
		return;
	}
	log("end");
	popLayer();
}

void NPC::popLayer()
{
	// 定义一个弹出层，传入一张背景图  
	PopLayer* pl = PopLayer::create("UI/UI_Dlg.png");

	// ContentSize 是可选的设置，可以不设置，如果设置把它当作 9 图缩放  
	//pl->setContentSize(Size(300, 150));

	// 设置回调函数，回调传回一个 CCNode 以获取 tag 判断点击的按钮  
	pl->setCallbackFunc(this, callfuncN_selector(NPC::buttonCallback));
	// 添加按钮，设置图片，文字，tag, 颜色(颜色默认是白色（Color3B）), 字体信息 
	pl->addButton("UI/UI_Quest_06.png", "UI/UI_Quest_06.png", Vec2(19, 3), 2);
	pl->addButton("UI/UI_Quest_04.png", "UI/UI_Quest_04.png", Vec2(136, 4), 0);
	pl->addButton("UI/UI_Quest_03.png", "UI/UI_Quest_03.png", Vec2(252, 3), 1);
	pl->setTitle(gb2312_to_utf8(data->name), Color3B::BLACK, 30, "arial.ttf");
	pl->setContentText(gb2312_to_utf8(data->dlgs[page]), 20, 40, 400, "arial.ttf", Color3B::RED);
	// 添加到当前层  
	PopManager::getInstance()->addChild(pl);
	//isPop = true;
	//isPop = pl->getIsPopped();
	PopManager::getInstance()->addLayer(0, pl, true);
}

void NPC::questLayer()
{
	// 定义一个弹出层，传入一张背景图  
	QuestList* ql = QuestList::create("UI/UI_QuestPanel.png");

	// ContentSize 是可选的设置，可以不设置，如果设置把它当作 9 图缩放  
	//ql->setContentSize(Size(300, 150));

	// 设置回调函数，回调传回一个 CCNode 以获取 tag 判断点击的按钮  
	ql->setCallbackFunc(this, callfuncN_selector(NPC::ItemCallback));
	auto bg = ql->getSpriteBackGround();
	// 添加按钮，设置图片，文字，tag, 颜色(颜色默认是白色（Color3B）), 字体信息 
	ql->addButton("UI/UI_QuestPanel_close.png", "UI_QuestPanel_close.png", Vec2(4, bg->getContentSize().height / 2), 0);
	
	for (int i = 0; i < data->quest_id.size(); i++) {
		ql->addItem("UI/UI_QuestPanel_cont.png", "UI/UI_Quest_10.png", gb2312_to_utf8(quests[i]->title), gb2312_to_utf8(quests[i]->instruct), quests[i]->id, "arial.ttf");
	}
	// 添加到Menu层  
	PopManager::getInstance()->getLayerByTag(0)->layer->getChildByTag(0)->addChild(ql);
	PopManager::getInstance()->addLayer(1, ql, true);
}

void NPC::buttonCallback(Node * pNode)
{
	auto btnTag = pNode->getTag();
	Sequence* a = Sequence::create(CallFunc::create([&]() {
		//回弹动画
		static_cast<PopLayer*>(PopManager::getInstance()->getLayerByTag(0)->layer)->popBack();
	}), DelayTime::create(0.3), CallFunc::create([&]() {
		PopManager::getInstance()->releaseLayer(0); }), NULL);
	//通过获取按钮TAG调用功能
	switch (btnTag)
	{
	case 0:
		log("page:%d", page);
		runAction(a);
		page++;
		if (page >= data->dlgs.size()) {
			page = 0;
		}
		//弹出标记
		PopManager::getInstance()->setPopped(1, false);
		break;
	case 1:
		break;
	case 2:
		if (PopManager::getInstance()->getPopped(1))	break;
		questLayer();
		break;
	default:
		break;
	}
}

void NPC::ItemCallback(Node * pNode)
{
	log("Item call back. tag: %d", pNode->getTag());
	auto btn = pNode->getTag();
	auto item = static_cast<PopLayer*>(PopManager::getInstance()->getLayerByTag(0)->layer);
	item->setTitle(gb2312_to_utf8(quests[btn]->title), Color3B::BLACK, 30, "arial.ttf");
	item->setContentText(gb2312_to_utf8(quests[btn]->instruct), 20, 40, 400, "arial.ttf", Color3B::RED);
	item->flashLabel(item->getLabelTitle());
	item->flashLabel(item->getLabelContentText());
}

//初始化NPC数据和任务数据
void NPC::initDataWithName(const string & sender)
{
	data = GameData::getInstance()->getDataFromNpcsData(sender);
	QuestDispatcher::getInstance()->initQuest(this);
	//获取任务信息
	quests = QuestDispatcher::getInstance()->getQuest(this)->questData;
}

void NPC::setTiledMap(TMXTiledMap* map)
{
	m_map = map;
}

Vec2 NPC::tiledCoordForPosition(Vec2 pos)
{
	Size mapTiledNum = m_map->getMapSize();
	Size tiledSize = m_map->getTileSize();
	Size visiblesize = Director::getInstance()->getVisibleSize();

	int x = (int)(pos.x / tiledSize.width);
	int y = (int)(m_map->getMapSize().height * m_map->getTileSize().height - pos.y) / m_map->getTileSize().height;

	return Vec2(x, y);
}