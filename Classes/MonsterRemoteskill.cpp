#include "MonsterRemoteskill.h"
#include "AnimBase.h"


MonsterRemoteskill* MonsterRemoteskill::createWithName_width_height_damage_duration_speed_diretion(std::string projectile, std::string projectileAnimate, float width, float height, float damage, float duration, float speed, int direction)
{
	MonsterRemoteskill* monsterremoteskill = new MonsterRemoteskill();
	if (monsterremoteskill && monsterremoteskill->init(projectile, projectileAnimate, width, height, damage, duration, speed, direction))
	{
		monsterremoteskill->autorelease();
		return monsterremoteskill;
	}
	else
	{
		delete monsterremoteskill;
		monsterremoteskill = NULL;
		return NULL;
	}
}

bool MonsterRemoteskill::init(std::string projectile, std::string projectileAnimate, float width, float height, float damage, float duration, float speed, int direction)
{
	name = projectileAnimate;
	Sprite* sprite = Sprite::createWithSpriteFrameName(projectile);
	Animation* animation = AnimationUtil::createWithSingleFrameName(projectileAnimate.c_str(),0.1f,-1);
	sprite->runAction(Animate::create(animation));
	sprite->setScale(width / sprite->getContentSize().width, height / sprite->getContentSize().height);
	sprite->setContentSize(Size(width,height));
	bindSprite(sprite);
	this->setAnchorPoint(Vec2(0.5, 0.5));
	m_damage = damage;
	m_duration = duration;
	m_movespeed = speed;
	m_direction = direction;
	m_player = Player::getInstance();
	m_dt = 0;

	switch (direction)
	{
	case Dir_up:{
		this->setRotation(-90);
		break;
	}
	case Dir_down:{
		this->setRotation(90);
		break;
	}
	case Dir_left:{
		this->setRotation(180);
		break;
	}
	case Dir_right:{
	
		break;
	}
	case Dir_upleft:{
		this->setRotation(-90);
		break;
	}
	case Dir_upright:{
		this->setRotation(-90);
		break;
	}
	case Dir_downleft:{
		this->setRotation(90);
		break;
	}
	case Dir_downright:{
		this->setRotation(90);
		break;
	}
	case Dir_leftup:{
		this->setRotation(180);
		break;
	}
	case Dir_rightup:{
		this->setRotation(0);
		break;
	}
	case Dir_leftdown:{
		this->setRotation(180);
		break;
	}
	case Dir_rightdown:{

		break;
	}
	}
	return true;
}
void MonsterRemoteskill::update(float dt)
{
	m_dt += dt;
	if (m_player != NULL)
	{
		if (m_player->getBoundingBox().intersectsRect(getBoundingBox()))
		{
			//如果是树怪的石头技能
			if (name == "treemonproj")
			{
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/hitedbybone.wav");
			}

			//扣主角hp
			if (m_player->m_hp > 0)
			{
				m_player->m_hp = m_player->m_hp - m_damage;
			}

			//把主角被攻击信息放进队列
			switch (m_direction)
			{
			case Dir_up:{
				m_player->attackedqueue.push(enum_playerattackedfromdown);
				break;
			}
			case Dir_down:{
				m_player->attackedqueue.push(enum_playerattackedfromup);
				break;
			}
			case Dir_left:{
				m_player->attackedqueue.push(enum_playerattackedfromright);
				break;
			}
			case Dir_right:{
				m_player->attackedqueue.push(enum_playerattackedfromleft);
				break;
			}
			case Dir_upleft:{
				m_player->attackedqueue.push(enum_playerattackedfromdown);
				break;
			}
			case Dir_upright:{
				m_player->attackedqueue.push(enum_playerattackedfromdown);
				break;
			}
			case Dir_downleft:{
				m_player->attackedqueue.push(enum_playerattackedfromup);
				break;
			}
			case Dir_downright:{
				m_player->attackedqueue.push(enum_playerattackedfromup);
				break;
			}
			case Dir_leftup:{
				m_player->attackedqueue.push(enum_playerattackedfromright);
				break;
			}
			case Dir_rightup:{
				m_player->attackedqueue.push(enum_playerattackedfromleft);
				break;
			}
			case Dir_leftdown:{
				m_player->attackedqueue.push(enum_playerattackedfromright);
				break;
			}
			case Dir_rightdown:{
				m_player->attackedqueue.push(enum_playerattackedfromleft);
				break;
			}
			}
			this->removeFromParent();
			return;
		}

	}
	// 物体移动
	Vec2 move(0, 0);
	switch (m_direction)
	{
	case Dir_up:{
		move.y = m_movespeed;
		break;
	}
	case Dir_down:{
		move.y = -m_movespeed;
		break;
	}
	case Dir_left:{
		move.x = -m_movespeed;
		break;
	}
	case Dir_right:{
		move.x = m_movespeed;
		break;
	}
	case Dir_upleft:{
		move.y = m_movespeed;
		break;
	}
	case Dir_upright:{
		move.y = m_movespeed;
		break;
	}
	case Dir_downleft:{
		move.y = -m_movespeed;
		break;
	}
	case Dir_downright:{
		move.y = -m_movespeed;
		break;
	}
	case Dir_leftup:{
		move.x = -m_movespeed;
		break;
	}
	case Dir_rightup:{
		move.x = m_movespeed;
		break;
	}
	case Dir_leftdown:{
		move.x = -m_movespeed;
		break;
	}
	case Dir_rightdown:{
		move.x = m_movespeed;
		break;
	}
	}
	Point afterPos(getPosition() + move);
	setPosition(afterPos);
	
	if (m_player){
		// 判断是否撞到障碍物
		if (!m_player->IsNot_CollidableTile(m_player->tiledCoordForPosition(afterPos)))
		{
			removeFromParent();
			return;
		}
	}
	
	if (m_dt > m_duration)
		this->removeFromParent();
}
