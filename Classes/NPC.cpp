#include "NPC.h"
#include "G2U.h"
#include "PopManager.h"
#include "GameScene.h"

NPC* NPC::create(const std::string& name)
{
	auto unit = new NPC();
	if (unit && unit->init(name)) {
		unit->autorelease();
	}
	else {
		CC_SAFE_DELETE(unit);
		unit = NULL;
	}
	return unit;
}

bool NPC::init(const std::string& name)
{
	auto data = GameData::getInstance()->getDataFromNpcsData(name);
	QuestDispatcher::getInstance()->addNpc(data->name, this);
	//初始化NPC数据，
	this->data = data;
	initDataWithName(name);

	bindSprite(Sprite::create("player11.png"));
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
	for (auto& a : QuestDispatcher::getInstance()->getQuestListVec()) {
		if (a->targetNpc == data->name) {
			data->status = NpcStatus::actived;
			activeQuest = a->id;
		}
	}
	return true;
}

void NPC::onTouchEnded(Touch * _touch, Event * _event)
{
	if (PopManager::getInstance()->getPopsMap()[data->name]->getPopped(0)) {
		return;
	}
	log("end");
	popLayer();
}

void NPC::popLayer()
{
	// 定义一个弹出层，传入一张背景图  
	PopLayer* pl = PopLayer::create("UI/UI_Dlg.png");
	items = NULL;
	// ContentSize 是可选的设置，可以不设置，如果设置把它当作 9 图缩放  
	//pl->setContentSize(Size(300, 150));

	// 设置回调函数，回调传回一个 CCNode 以获取 tag 判断点击的按钮  
	pl->setCallbackFunc(this, callfuncN_selector(NPC::buttonCallback));
	// 添加按钮，设置图片，文字，tag, 颜色(颜色默认是白色（Color3B）), 字体信息 
	pl->addButton("UI/UI_Quest_06.png", "UI/UI_Quest_06.png", Vec2(19, 3), 2);
	pl->addButton("UI/UI_Quest_04.png", "UI/UI_Quest_04.png", Vec2(136, 4), 0);
	pl->addButton("UI/UI_Quest_03.png", "UI/UI_Quest_03.png", Vec2(252, 3), 1);
	pl->setTitle(gb2312_to_utf8(data->name), Color3B::BLACK, 30, "arial.ttf");
	if (data->status) {
		auto questDlgs = QuestDispatcher::getInstance()->getQuestDlgs();
		pl->setContentText(gb2312_to_utf8(questDlgs[activeQuest]->answer));
	}
	else {
		pl->setContentText(gb2312_to_utf8(data->dlgs[page]));
	}
	// 添加到当前层  
	PopManager::getInstance()->getPopsMap()[data->name]->addChild(pl);
	//isPop = true;
	//isPop = pl->getIsPopped();
	PopManager::getInstance()->getPopsMap()[data->name]->addLayer(0, pl, true);
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
	ql->addButton("UI/UI_QuestPanel_close.png", "UI_QuestPanel_close.png", Vec2(4, bg->getContentSize().height / 2), 20);

	bool forgeFlag = true;
	for (auto i : quests) {
		forgeFlag = true;
		auto key = i.first;
		for (auto k : i.second->forgeID) {
			if (QuestDispatcher::getInstance()->getQuestStatus(this, k) != QuestStatus::finish) {
				forgeFlag = false;
				break;
			}
			forgeFlag = true;
		}
		if (i.second->status == QuestStatus::finish || forgeFlag == false)	continue;
		ql->addItem("UI/UI_QuestPanel_cont.png", "UI/UI_Quest_10.png", gb2312_to_utf8(quests[key]->title), gb2312_to_utf8(quests[key]->instruct), key, "arial.ttf");
	}
	// 添加到Menu层  
	PopManager::getInstance()->getPopsMap()[data->name]->getLayerByTag(0)->layer->getChildByTag(0)->addChild(ql);
	PopManager::getInstance()->getPopsMap()[data->name]->addLayer(1, ql, true);
}

void NPC::buttonCallback(Node * pNode)
{
	auto btnTag = pNode->getTag();
	auto item = static_cast<PopLayer*>(PopManager::getInstance()->getPopsMap()[data->name]->getLayerByTag(0)->layer);
	auto questDlgs = QuestDispatcher::getInstance()->getQuestDlgs();
	Sequence* a = Sequence::create(CallFunc::create([&]() {
		//回弹动画
		static_cast<PopLayer*>(PopManager::getInstance()->getPopsMap()[data->name]->getLayerByTag(0)->layer)->popBack();
	}), DelayTime::create(0.3), CallFunc::create([&]() {
		PopManager::getInstance()->getPopsMap()[data->name]->releaseLayer(0); }), NULL);
	//通过获取按钮TAG调用功能
	switch (btnTag)
	{
	case 0:
		log("page:%d", page);
		runAction(a);
		if (items)
			//重置任务选项
			items->setQuestTag(NULL);
		//闲聊对话翻页
		page++;
		if (page >= data->dlgs.size()) {
			page = 0;
		}
		//弹出标记
		PopManager::getInstance()->getPopsMap()[data->name]->setPopped(1, false);
		break;
	case 1:
		if (data->status) {
			//改变任务状态
			for (auto& i : QuestDispatcher::getInstance()->getQuestListVec()) {
				if (i->type == QuestTypes::search && i->targetNpc == data->name) {
					i->status = QuestStatus::commit;
				}
			}
			runAction(a);
			PopManager::getInstance()->getPopsMap()[data->name]->setPopped(1, false);
		}
		if (items)
		{
			//选择任务并接受
			if (items->getQuestTag() != NULL  && QuestDispatcher::getInstance()->getQuestStatus(this, items->getQuestTag()) == QuestStatus::start) {
				if (QuestDispatcher::getInstance()->getQuestType(this, items->getQuestTag()) == QuestTypes::search)
				{
					QuestDispatcher::getInstance()->QuestStatusControl(this, QuestControl::accpet, items->getQuestTag());
					item->Talking(gb2312_to_utf8(questDlgs[items->getQuestTag()]->active));
				}
				else if (QuestDispatcher::getInstance()->getQuestType(this, items->getQuestTag()) == QuestTypes::defeat) {
					auto tag = QuestDispatcher::getInstance()->getQuest(this, items->getQuestTag());
					QuestDispatcher::getInstance()->QuestStatusControl(this, QuestControl::accpet, items->getQuestTag());
					QuestDispatcher::getInstance()->openUpdate(tag, "defeat");
					item->Talking(gb2312_to_utf8(questDlgs[items->getQuestTag()]->active));
				}
			}
			//完成任务
			else if (items->getQuestTag() != NULL  && QuestDispatcher::getInstance()->getQuestStatus(this, items->getQuestTag()) == QuestStatus::commit) {
				for (auto& a : QuestDispatcher::getInstance()->getQuestListVec()) {
					if (a->type == QuestTypes::search && a->status == QuestStatus::commit) {
						QuestDispatcher::getInstance()->getNpc(a->targetNpc)->data->status = NpcStatus::normal;
					}
					if (a->type == QuestTypes::defeat && a->status == QuestStatus::commit) {
						QuestDispatcher::getInstance()->unschedule("defeat");
					}
				}
				//移除已完成任务
				items->getMenuItems()->getChildByTag(items->getQuestTag())->removeFromParent();
				items->setBtnPos(0.3);
				QuestDispatcher::getInstance()->QuestStatusControl(this, QuestControl::complete, items->getQuestTag());
			}
		}
		break;
	case 2:
		//弹出任务列表，并设置弹出标志
		if (PopManager::getInstance()->getPopsMap()[data->name]->getPopped(1))	break;
		questLayer();
		PopManager::getInstance()->getPopsMap()[data->name]->setPopped(1, true);
		break;
	default:
		break;
	}
}

void NPC::ItemCallback(Node * pNode)
{
	log("Item call back. tag: %d", pNode->getTag());
	//获取任务ID
	auto btn = pNode->getTag();
	//获取任务列表层
	items = static_cast<QuestList*>(PopManager::getInstance()->getPopsMap()[data->name]->getLayerByTag(1)->layer);
	//获取弹出菜单层
	auto item = static_cast<PopLayer*>(PopManager::getInstance()->getPopsMap()[data->name]->getLayerByTag(0)->layer);
	auto questDlgs = QuestDispatcher::getInstance()->getQuestDlgs();
	//关闭任务列表
	if (btn == 20) {
		items->setQuestTag(NULL);
		Sequence* a = Sequence::create(CallFunc::create([&]() {
			static_cast<PopLayer*>(PopManager::getInstance()->getPopsMap()[data->name]->getLayerByTag(1)->layer)->popBack();
		}), DelayTime::create(0.3), CallFunc::create([&]() {
			PopManager::getInstance()->getPopsMap()[data->name]->setPopped(1, false); }), NULL);
		runAction(a);
		return;
	}
	//按任务状态产生对话
	switch (QuestDispatcher::getInstance()->getQuestStatus(this, quests[btn]->id))
	{
	case QuestStatus::start:
		item->getLabelTitle()->setString(gb2312_to_utf8(quests[btn]->title));
		item->Talking(gb2312_to_utf8(questDlgs[btn]->start));
		break;
	case QuestStatus::active:
		item->getLabelTitle()->setString(gb2312_to_utf8(quests[btn]->title));
		item->Talking(gb2312_to_utf8(questDlgs[btn]->active));
		break;
	case QuestStatus::commit:
		item->getLabelTitle()->setString(gb2312_to_utf8(quests[btn]->title));
		item->Talking(gb2312_to_utf8(questDlgs[btn]->finish));
		break;
	default:
		break;
	}
	items->setQuestTag(quests[btn]->id);
}

//初始化NPC数据和任务数据
void NPC::initDataWithName(const string & sender)
{
	data = GameData::getInstance()->getDataFromNpcsData(sender);
	QuestDispatcher::getInstance()->initQuest(this);
	//获取任务信息
	auto temp = QuestDispatcher::getInstance()->getQuest(this);
	for (auto i : temp) {
		quests[i->id] = i;
	}
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