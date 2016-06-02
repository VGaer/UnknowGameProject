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

Sprite* BarManager::create(const string& hp, int tag)
{
	auto head = Sprite::create("UI/Enemy.png");
	auto m_bar = Sprite::create("UI/Enemy_hp_bar.png");
	auto size = m_bar->getContentSize();
	auto m_inner = Sprite::create(hp);
	auto m_hp = ProgressTimer::create(m_inner);
	m_hp->setType(ProgressTimer::Type::BAR);
	m_hp->setMidpoint(Vec2(0, 1));
	m_hp->setPercentage(100);
	m_hp->setBarChangeRate(Vec2(1, 0));
	m_hp->setAnchorPoint(Vec2(0, 0.5));
	m_hp->setPosition(9, size.height / 2 + 1);
	m_hp->setScaleX(90.0 / 80.0);
	m_bar->setAnchorPoint(Vec2(0, 0.5));
	m_bar->addChild(m_hp, 1, tag);
	mbars[tag] = m_hp;
	m_bar->setPosition(Vec2(head->getContentSize().width - 10, head->getContentSize().height / 2));
	head->addChild(m_bar, -1);
	return head;
}

Sprite* BarManager::create(const string& hp, const string& mp)
{
	playerBar = new PlayerBar();
	auto head = Sprite::create("UI/Player.png");
	auto m_bar = Sprite::create("UI/Player_bar.png");
	auto size = m_bar->getContentSize();
	playerBar->m_hp = ProgressTimer::create(Sprite::create(hp));
	playerBar->m_hp->setType(ProgressTimer::Type::BAR);
	playerBar->m_hp->setMidpoint(Vec2(0, 1));
	playerBar->m_hp->setPercentage(100);
	playerBar->m_hp->setBarChangeRate(Vec2(1, 0));
	playerBar->m_hp->setAnchorPoint(Vec2(0, 0.5));
	playerBar->m_hp->setPosition(9, size.height - 15);
	playerBar->m_hp->setScaleX(90.0 / 80.0);

	playerBar->m_mp = ProgressTimer::create(Sprite::create(mp));
	playerBar->m_mp->setType(ProgressTimer::Type::BAR);
	playerBar->m_mp->setMidpoint(Vec2(0, 1));
	playerBar->m_mp->setPercentage(100);
	playerBar->m_mp->setBarChangeRate(Vec2(1, 0));
	playerBar->m_mp->setAnchorPoint(Vec2(0, 0.5));
	playerBar->m_mp->setPosition(9, 15);
	playerBar->m_mp->setScaleX(1.7);

	m_bar->setAnchorPoint(Vec2(0, 0.5));
	m_bar->setPosition(Vec2(head->getContentSize().width - 15, head->getContentSize().height / 2));
	m_bar->addChild(playerBar->m_hp, 1);
	m_bar->addChild(playerBar->m_mp, 1);
	head->addChild(m_bar, -1);
	return head;
}

ProgressTimer* BarManager::getBars(int tag)
{
	if (mbars.find(tag) != mbars.end())
		return mbars[tag];
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
		auto action = Sequence::create(ProgressFromTo::create(0.2f, pSender->getPercentage(), 0), 
			CallFunc::create([=]() {
		}),NULL);
		pSender->runAction(action);
	}
	else {
		auto action = Sequence::create(ProgressFromTo::create(0.2f, pSender->getPercentage(), nowNum / total * 100),
			CallFunc::create([=]() {
	
		}),  NULL);
		pSender->runAction(action);
	}
}

//void BarManager::recover(BarType sender, int tag)
//{
//	if (sender == BarType::HP) {
//		schedule(schedule_selector(BarManager::recoverHP), 0.2);
//	}
//	if (sender == BarType::MP) {
//		schedule(schedule_selector(BarManager::recoverMP), 0.4);
//	}
//	if (sender == BarType::Enemy) {
//		schedule(CC_CALLBACK_1(BarManager::recoverEnemy, this, tag), 0.3, "enemy");
//	}
//}

//void BarManager::recoverHP(float dt)
//{
//	if (playerBar != NULL)
//	{
//		if (playerBar->m_hp->getPercentage() == 100) {
//		//	unschedule(schedule_selector(BarManager::recoverHP));
//			return;
//		}
//		playerBar->m_hp->setPercentage(playerBar->m_hp->getPercentage() + 0.1);
//	}
//}

//void BarManager::recoverEnemy(float dt, int i)
//{
//	if (getBars(i)->getPercentage() == 100) {
//		unschedule("enemy");
//		return;
//	}
//	auto action = Sequence::create(CallFunc::create([=]() {
//		getBars(i)->setPercentage(getBars(i)->getPercentage() + 0.1);
//		/*
//		改变敌方HP（你自己写）
//		*/
//	}), ProgressFromTo::create(0.2f, getBars(i)->getPercentage(), getBars(i)->getPercentage() + 0.1), NULL);
//	getBars(i)->runAction(action);
//}

//void BarManager::recoverMP(float dt)
//{
//	if (playerBar->m_hp->getPercentage() == 100) {
//		//unschedule(schedule_selector(BarManager::recoverMP));
//		return;
//	}
//	playerBar->m_mp->setPercentage(playerBar->m_mp->getPercentage() + 0.2);	
//}

//void BarManager::SchedulePlayerHp_Mp()
//{
//	schedule(schedule_selector(BarManager::recoverHP));
//	schedule(schedule_selector(BarManager::recoverMP));
//}
