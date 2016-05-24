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
	// ����һ�������㣬����һ�ű���ͼ  
	PopLayer* pl = PopLayer::create("UI/UI_Dlg.png");

	// ContentSize �ǿ�ѡ�����ã����Բ����ã�������ð������� 9 ͼ����  
	//pl->setContentSize(Size(300, 150));

	// ���ûص��������ص�����һ�� CCNode �Ի�ȡ tag �жϵ���İ�ť  
	pl->setCallbackFunc(this, callfuncN_selector(NPC::buttonCallback));
	// ��Ӱ�ť������ͼƬ�����֣�tag, ��ɫ(��ɫĬ���ǰ�ɫ��Color3B��), ������Ϣ 
	pl->addButton("UI/UI_Quest_06.png", "UI/UI_Quest_06.png", Vec2(19, 3), 2);
	pl->addButton("UI/UI_Quest_04.png", "UI/UI_Quest_04.png", Vec2(136, 4), 0);
	pl->addButton("UI/UI_Quest_03.png", "UI/UI_Quest_03.png", Vec2(252, 3), 1);
	pl->setTitle(gb2312_to_utf8(data->name), Color3B::BLACK, 30, "arial.ttf");
	pl->setContentText(gb2312_to_utf8(data->dlgs[page]), 20, 40, 400, "arial.ttf", Color3B::RED);
	// ��ӵ���ǰ��  
	PopManager::getInstance()->addChild(pl);
	//isPop = true;
	//isPop = pl->getIsPopped();
	PopManager::getInstance()->addLayer(0, pl, true);
}

void NPC::questLayer()
{
	// ����һ�������㣬����һ�ű���ͼ  
	QuestList* ql = QuestList::create("UI/UI_QuestPanel.png");

	// ContentSize �ǿ�ѡ�����ã����Բ����ã�������ð������� 9 ͼ����  
	//ql->setContentSize(Size(300, 150));

	// ���ûص��������ص�����һ�� CCNode �Ի�ȡ tag �жϵ���İ�ť  
	ql->setCallbackFunc(this, callfuncN_selector(NPC::ItemCallback));
	auto bg = ql->getSpriteBackGround();
	// ��Ӱ�ť������ͼƬ�����֣�tag, ��ɫ(��ɫĬ���ǰ�ɫ��Color3B��), ������Ϣ 
	ql->addButton("UI/UI_QuestPanel_close.png", "UI_QuestPanel_close.png", Vec2(4, bg->getContentSize().height / 2), 0);
	
	for (int i = 0; i < data->quest_id.size(); i++) {
		ql->addItem("UI/UI_QuestPanel_cont.png", "UI/UI_Quest_10.png", gb2312_to_utf8(quests[i]->title), gb2312_to_utf8(quests[i]->instruct), quests[i]->id, "arial.ttf");
	}
	// ��ӵ�Menu��  
	PopManager::getInstance()->getLayerByTag(0)->layer->getChildByTag(0)->addChild(ql);
	PopManager::getInstance()->addLayer(1, ql, true);
}

void NPC::buttonCallback(Node * pNode)
{
	auto btnTag = pNode->getTag();
	Sequence* a = Sequence::create(CallFunc::create([&]() {
		//�ص�����
		static_cast<PopLayer*>(PopManager::getInstance()->getLayerByTag(0)->layer)->popBack();
	}), DelayTime::create(0.3), CallFunc::create([&]() {
		PopManager::getInstance()->releaseLayer(0); }), NULL);
	//ͨ����ȡ��ťTAG���ù���
	switch (btnTag)
	{
	case 0:
		log("page:%d", page);
		runAction(a);
		page++;
		if (page >= data->dlgs.size()) {
			page = 0;
		}
		//�������
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

//��ʼ��NPC���ݺ���������
void NPC::initDataWithName(const string & sender)
{
	data = GameData::getInstance()->getDataFromNpcsData(sender);
	QuestDispatcher::getInstance()->initQuest(this);
	//��ȡ������Ϣ
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