#include "BarManager.h"
#include "QuestDispatcher.h"
#include "G2U.h"
BarManager * BarManager::getInstance()
{
	static BarManager* instance = NULL;
	if (instance == NULL)
		instance = new BarManager();
	return instance;
}

BarManager::BarManager()
{
	playerBar = NULL;
	questView = NULL;
	actBar = NULL;
}

Sprite* BarManager::create(const string& hp, int tag, float maxhp)
{
	EnemyBar* eb = new EnemyBar();
	auto head = Sprite::create("UI/Enemy.png");
	auto m_bar = Sprite::create("UI/Enemy_hp_bar.png");
	auto size = m_bar->getContentSize();
	auto m_inner = Sprite::create(hp);
	auto m_hp = ProgressTimer::create(m_inner);

	auto l_hp = Label::create(to_string((int)maxhp) + " / " + to_string((int)maxhp), "Arial", 13);
	l_hp->setAlignment(CCTextAlignment::CENTER);
	l_hp->setPosition(45, size.height / 2);

	m_hp->setType(ProgressTimer::Type::BAR);
	m_hp->setMidpoint(Vec2(0, 1));
	m_hp->setPercentage(100);
	m_hp->setBarChangeRate(Vec2(1, 0));
	m_hp->setAnchorPoint(Vec2(0, 0.5));
	m_hp->setPosition(9, size.height / 2 + 1);
	m_hp->setScaleX(90.0 / 80.0);
	m_bar->setAnchorPoint(Vec2(0, 0.5));
	m_bar->addChild(m_hp, 1);
	m_bar->addChild(l_hp, 2);
	eb->m_hp = m_hp;
	eb->l_hp = l_hp;
	mbars[tag] = eb;

	m_bar->setPosition(Vec2(head->getContentSize().width - 10, head->getContentSize().height / 2));
	head->addChild(m_bar, -1);
	return head;
}

Sprite* BarManager::create(const string& hp, const string& mp, float maxhp, float maxmp)
{
	if(playerBar == NULL)
		playerBar = new PlayerBar();
	auto head = Sprite::create("UI/Player.png");
	auto m_bar = Sprite::create("UI/Player_bar.png");
	auto size = m_bar->getContentSize();

	playerBar->l_hp = Label::create(to_string((int)maxhp) + " / " + to_string((int)maxhp), "Arial", 13);
	playerBar->l_hp->setAlignment(CCTextAlignment::CENTER);
	playerBar->l_hp->setPosition(45, size.height - 15);

	playerBar->m_hp = ProgressTimer::create(Sprite::create(hp));
	playerBar->m_hp->setType(ProgressTimer::Type::BAR);
	playerBar->m_hp->setMidpoint(Vec2(0, 1));
	playerBar->m_hp->setBarChangeRate(Vec2(1, 0));
	playerBar->m_hp->setAnchorPoint(Vec2(0, 0.5));
	playerBar->m_hp->setPosition(9, size.height - 15);
	playerBar->m_hp->setScaleX(90.0 / 80.0);

	playerBar->l_mp = Label::create(to_string(maxmp) + " / " + to_string(maxmp), "Arial", 13);
	playerBar->l_mp->setAlignment(CCTextAlignment::CENTER);
	playerBar->l_mp->setPosition(45, 15);

	playerBar->m_mp = ProgressTimer::create(Sprite::create(mp));
	playerBar->m_mp->setType(ProgressTimer::Type::BAR);
	playerBar->m_mp->setMidpoint(Vec2(0, 1));
	playerBar->m_mp->setBarChangeRate(Vec2(1, 0));
	playerBar->m_mp->setAnchorPoint(Vec2(0, 0.5));
	playerBar->m_mp->setPosition(9, 15);
	playerBar->m_mp->setScaleX(1.7);


	m_bar->setAnchorPoint(Vec2(0, 0.5));
	m_bar->setPosition(Vec2(head->getContentSize().width - 15, head->getContentSize().height / 2));
	m_bar->addChild(playerBar->l_hp, 2);
	m_bar->addChild(playerBar->l_mp, 2);
	m_bar->addChild(playerBar->m_hp, 1);
	m_bar->addChild(playerBar->m_mp, 1);
	head->addChild(m_bar, -1);
	return head;
}

Sprite * BarManager::create(const string & exp, float lexp)
{
	if (playerBar == NULL)
		playerBar = new PlayerBar();
	auto m_bar = Sprite::create("UI/exp_bar.png");
	auto m_in = Sprite::create("UI/exp_bar2.png");
	auto size = m_bar->getContentSize();
	playerBar->l_exp = Label::create(to_string((int)0) + " / " + to_string((int)lexp), "Arial", 11);
	playerBar->m_exp = ProgressTimer::create(Sprite::create(exp));
	playerBar->m_exp->setType(ProgressTimer::Type::BAR);
	playerBar->m_exp->setMidpoint(Vec2(0, 1));
	playerBar->m_exp->setPercentage(0);
	playerBar->m_exp->setBarChangeRate(Vec2(1, 0));
	playerBar->m_exp->setAnchorPoint(Vec2(0, 0.5));
	playerBar->m_exp->setPosition(9, size.height / 2 - 4.5);
	playerBar->m_exp->setScaleX(2.09);
	playerBar->l_exp->setAlignment(CCTextAlignment::CENTER);
	playerBar->l_exp->setAnchorPoint(Vec2(0.5, 1));
	playerBar->l_exp->setPosition(size.width / 2 + 1, size.height - 6);
	m_in->setAnchorPoint(Vec2(0, 0.5));
	m_in->setPosition(12.5, size.height - 24.501993);
	m_bar->addChild(playerBar->l_exp, 1);
	m_bar->addChild(playerBar->m_exp, 1);
	m_bar->addChild(m_in, 2);
	return m_bar;
}

Sprite * BarManager::create(const string & inv, const string & quest)
{
	questView = NULL;
	
	auto menu = Sprite::create("UI/ActionBar.png");
	auto sub1 = Button::create(inv, inv);
	auto sub2 = Button::create(quest, quest);
	auto size = menu->getContentSize();
	sub1->addTouchEventListener(CC_CALLBACK_2(BarManager::buttonCallback, this));
	sub1->setTag(30);
	sub1->setAnchorPoint(Vec2(0.5, 0.5));
	sub2->addTouchEventListener(CC_CALLBACK_2(BarManager::buttonCallback, this));
	sub2->setTag(31);
	sub2->setAnchorPoint(Vec2(0.5, 0.5));

	sub1->setPosition(Vec2(size.width - 60, size.height / 2));
	sub2->setPosition(Vec2(size.width - 25, size.height / 2));

	menu->addChild(sub1, 20);
	menu->addChild(sub2, 20);
	actBar = menu;
	return menu;
}

ProgressTimer* BarManager::getBars(int tag)
{
	if (mbars.find(tag) != mbars.end())
		return mbars[tag]->m_hp;
	else
		return NULL;
}

Label * BarManager::getBarsLabel(int tag)
{
	if (mbars.find(tag) != mbars.end())
		return mbars[tag]->l_hp;
	else
		return NULL;
}

PlayerBar * BarManager::getPlayerBars()
{
	return playerBar;
}

void BarManager::setPercent(ProgressTimer * pSender, float total, float nowNum)
{
	if (nowNum <= 0) {
		auto action = ProgressFromTo::create(0.2f, pSender->getPercentage(), 0);
		pSender->runAction(action);
	}
	else {
		auto action = ProgressFromTo::create(0.2f, pSender->getPercentage(), nowNum / total * 100);
		pSender->runAction(action);
	}
}

void BarManager::setBarLabel(Label* pSender, float cur, float max)
{
	pSender->setString(to_string((int)cur) + " / " + to_string((int)max));
}

void BarManager::releaseEnemyBar(int tag)
{
	auto temp = mbars[tag];
	mbars.erase(tag);
	delete temp;
}

void BarManager::buttonCallback(Ref * pSender, Widget::TouchEventType type)
{
	Node* node = dynamic_cast<Node*>(pSender);
	auto tag = node->getTag();
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		if (tag == 30)
			log("inventory");
		if (tag == 31) {
			if (questView != NULL) {
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/close.wav");
				questView->removeFromParent();
				questView = NULL;
				break;
			}
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/accept.wav");
			popQuest();
		}
		break;
	default:
		break;
	}
}

void BarManager::popQuest()
{
	// 定义一个弹出层，传入一张背景图  
	QuestList* ql = QuestList::create("UI/UI_QuestPanel.png");

	// ContentSize 是可选的设置，可以不设置，如果设置把它当作 9 图缩放  
	//ql->setContentSize(Size(300, 150));

	// 设置回调函数，回调传回一个 CCNode 以获取 tag 判断点击的按钮  
	ql->setCallbackFunc(this, callfuncN_selector(BarManager::ItemCallback));
	auto bg = ql->getSpriteBackGround();
	// 添加按钮，设置图片，文字，tag, 颜色(颜色默认是白色（Color3B）), 字体信息 
	ql->addButton("UI/UI_QuestPanel_close.png", "UI_QuestPanel_close.png", Vec2(bg->getContentSize().width + 6, bg->getContentSize().height / 2), 20);
	auto quests = QuestDispatcher::getInstance()->getqData();
	for (auto i : quests) {
		if (i.second->status == QuestStatus::active || i.second->status == QuestStatus::commit)
			ql->addItem("UI/UI_QuestPanel_cont.png", "UI/UI_Quest_10.png", gb2312_to_utf8(i.second->title), gb2312_to_utf8(i.second->instruct), i.second->id, "Arial", Color3B(255, 230, 0));
	}
	ql->setPosition(actBar->getContentSize().width, actBar->getContentSize().height);
	// 添加到Menu层  
	actBar->addChild(ql);
	questView = ql;
}

void BarManager::ItemCallback(Node * pNode)
{
	auto btn = pNode->getTag();
	//关闭任务列表
	if (btn == 20) {
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/close.wav");
		questView->removeFromParent();
		questView = NULL;
	}
}
