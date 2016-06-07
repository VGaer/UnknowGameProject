#include "MonDiedThings.h"

///////HP物品
bool t_HP::init()
{
	m_player = NULL;
	hpNum = 0;
	Sprite* sprite = Sprite::create("MonThings/hp.png");
	bindSprite(sprite);
	JumpBy* jump = JumpBy::create(0.5f,Vec2(0,0),100,1);
	sprite->runAction(jump);
	scheduleUpdate();
	return true;
}

void t_HP::sethpNum(int h)
{
	hpNum = h;
}

void t_HP::bindPlayer(Player* player)
{
	m_player = player;
}

void t_HP::update(float dt)
{
	if (m_player != NULL)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/getthings.wav");
		//吃到hp
		if (m_player->getBoundingBox().intersectsRect(this->getBoundingBox()))
		{
			//回血
			if (m_player->m_hp + hpNum >= m_player->getCurMaxHp())
				m_player->m_hp = m_player->getCurMaxHp();
			else
				m_player->m_hp += hpNum;

			this->removeFromParent();
		}
	}
}

///////MP物品
bool t_MP::init()
{
	m_player = NULL;
	mpNum = 0;
	Sprite* sprite = Sprite::create("MonThings/mp.png");
	bindSprite(sprite);
	JumpBy* jump = JumpBy::create(0.5f, Vec2(0, 0), 100, 1);
	sprite->runAction(jump);
	scheduleUpdate();
	return true;
}

void t_MP::setmpNum(int m)
{
	mpNum = m;
}

void t_MP::bindPlayer(Player* player)
{
	m_player = player;
}

void t_MP::update(float dt)
{
	if (m_player != NULL)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/getthings.wav");
		if (m_player->getBoundingBox().intersectsRect(getBoundingBox()))
		{
			if (m_player->m_mp + mpNum >= m_player->getCurMaxMp())
				m_player->m_mp = m_player->getCurMaxMp();
			else
				m_player->m_mp += mpNum;

			this->removeFromParent();
		}
	}
}


