#include "BarManager.h"

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
