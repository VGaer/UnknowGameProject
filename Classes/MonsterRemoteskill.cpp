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

////MonsterRemoteskillDur

MonsterRemoteskillDur* MonsterRemoteskillDur::createWithName_width_height_damage_duration_speed_diretion_attackNums_attackNumsInter_distance(std::string projectile, std::string projectileAnimate, float width, float height, float damage, float duration, float speed, int direction, int attackNums, float attckNumsInter,float distance)
{
	MonsterRemoteskillDur* monsterremoteskilldur = new MonsterRemoteskillDur();
	if (monsterremoteskilldur && monsterremoteskilldur->init(projectile, projectileAnimate, width, height, damage, duration, speed, direction,attackNums,attckNumsInter,distance))
	{
		monsterremoteskilldur->autorelease();
		return monsterremoteskilldur;
	}
	else
	{
		delete monsterremoteskilldur;
		monsterremoteskilldur = NULL;
		return NULL;
	}
}

bool MonsterRemoteskillDur::init(std::string skill, std::string projectileAnimate, float width, float height, float damage, float duration, float speed, int direction, int attackNums, float attckNumsInter,float distance)
{
	name = projectileAnimate;
	Sprite* sprite = Sprite::createWithSpriteFrameName(skill);
	Animation* animation = AnimationUtil::createWithSingleFrameName(projectileAnimate.c_str(), 0.1f, -1);
	sprite->runAction(Animate::create(animation));
	sprite->setScale(width / sprite->getContentSize().width, height / sprite->getContentSize().height);
	sprite->setContentSize(Size(width, height));
	bindSprite(sprite);
	this->setAnchorPoint(Vec2(0.5, 0.5));
	m_damage = damage;
	m_duration = duration;
	m_movespeed = speed;
	m_direction = direction;
	m_attckNums = attackNums;
	m_attckNumsInter = attckNumsInter;
	m_player = Player::getInstance();
	m_dt = 0;

	m_attackCounter = TimeCounter::create();
	this->addChild(m_attackCounter);

	switch (direction)
	{
	case Dir_up:{
		//先测试下
		MoveBy* move = MoveBy::create(distance / (m_movespeed  / (1 / 60.0f)),Vec2(0,distance));
		this->runAction(move);
		break;
	}
	case Dir_down:{
		MoveBy* move = MoveBy::create(distance / (m_movespeed / (1 / 60.0f)), Vec2(0, -distance));
		this->runAction(move);
		break;
	}
	case Dir_left:{
		MoveBy* move = MoveBy::create(distance / (m_movespeed / (1 / 60.0f)), Vec2(-distance, 0));
		this->runAction(move);
		break;
	}
	case Dir_right:{
		MoveBy* move = MoveBy::create(distance / (m_movespeed / (1 / 60.0f)), Vec2(distance, 0));
		this->runAction(move);
		break;
	}
	case Dir_upleft:{
		MoveBy* move = MoveBy::create(distance / (m_movespeed / (1 / 60.0f)), Vec2(0, distance));
		this->runAction(move);
		break;
	}
	case Dir_upright:{
		MoveBy* move = MoveBy::create(distance / (m_movespeed / (1 / 60.0f)), Vec2(0, distance));
		this->runAction(move);
		break;
	}
	case Dir_downleft:{
		MoveBy* move = MoveBy::create(distance / (m_movespeed / (1 / 60.0f)), Vec2(0, -distance));
		this->runAction(move);
		break;
	}
	case Dir_downright:{
		MoveBy* move = MoveBy::create(distance / (m_movespeed / (1 / 60.0f)), Vec2(0, -distance));
		this->runAction(move);
		break;
	}
	case Dir_leftup:{
		MoveBy* move = MoveBy::create(distance / (m_movespeed / (1 / 60.0f)), Vec2(-distance, 0));
		this->runAction(move);
		break;
	}
	case Dir_rightup:{
		MoveBy* move = MoveBy::create(distance / (m_movespeed / (1 / 60.0f)), Vec2(distance, 0));
		this->runAction(move);
		break;
	}
	case Dir_leftdown:{
		MoveBy* move = MoveBy::create(distance / (m_movespeed / (1 / 60.0f)), Vec2(-distance, 0));
		this->runAction(move);
		break;
	}
	case Dir_rightdown:{
		MoveBy* move = MoveBy::create(distance / (m_movespeed / (1 / 60.0f)), Vec2(distance, 0));
		this->runAction(move);
		break;
	}
	}

	this->scheduleUpdate();
	m_attackCounter->start();

	return true;
}

void MonsterRemoteskillDur::update(float dt)
{
	m_dt += dt;
	if (m_player != NULL)
	{
		if (m_player->getBoundingBox().intersectsRect(getBoundingBox()))
		{
			if (m_attackCounter->getCurTime() == 0.0f || m_attackCounter->getCurTime() > m_attckNumsInter)
			{
				m_attackCounter->start();
				//扣主角hp
				if (m_player->m_hp > 0)
				{
					m_player->m_hp = m_player->m_hp - m_damage;
				}
			}			
		}
	}

	if(m_dt > m_duration)
		this->removeFromParent();
}

MonsterRemoteskillDur_NoMove* MonsterRemoteskillDur_NoMove::createWithName_width_height_damage_duration_diretion_attackNums_attackNumsInter(std::string projectile, std::string projectileAnimate, float width, float height, float damage, float duration, std::string direction, int attackNums, float attckNumsInter)
{
	MonsterRemoteskillDur_NoMove* monsterremoteskilldur_nomove = new MonsterRemoteskillDur_NoMove();
	if (monsterremoteskilldur_nomove && monsterremoteskilldur_nomove->init(projectile, projectileAnimate, width, height, damage, duration,direction, attackNums, attckNumsInter))
	{
		monsterremoteskilldur_nomove->autorelease();
		return monsterremoteskilldur_nomove;
	}
	else
	{
		delete monsterremoteskilldur_nomove;
		monsterremoteskilldur_nomove = NULL;
		return NULL;
	}
}

bool MonsterRemoteskillDur_NoMove::init(std::string skill, std::string projectileAnimate, float width, float height, float damage, float duration, std::string direction, int attackNums, float attckNumsInter)
{
	name = projectileAnimate;
	Sprite* sprite = Sprite::createWithSpriteFrameName(skill);
	Animation* animation = AnimationUtil::createWithSingleFrameName(projectileAnimate.c_str(), 0.1f, -1);
	sprite->runAction(Animate::create(animation));
	sprite->setScale(width / sprite->getContentSize().width, height / sprite->getContentSize().height);
	sprite->setContentSize(Size(width, height));
	bindSprite(sprite);
	//锚点为0 0.5类型
	this->setAnchorPoint(Vec2(0, 0.5));
	m_damage = damage;
	m_duration = duration;

	m_direction = direction;
	m_attckNums = attackNums;
	m_attckNumsInter = attckNumsInter;
	m_player = Player::getInstance();
	m_dt = 0;

	m_attackCounter = TimeCounter::create();
	this->addChild(m_attackCounter);

	if (m_direction == "up")
	{
		this->setRotation(-90);
	}
	else if (m_direction == "down")
	{
		this->setRotation(90);
	}
	else if (m_direction == "left")
	{
		this->setRotation(180);
	}
	else if (m_direction == "right")
	{
		this->setRotation(0);
	}
	else if (m_direction == "upright")
	{
		this->setRotation(-45);
	}
	else if (m_direction == "downleft")
	{
		this->setRotation(135);
	}
	else if (m_direction == "leftup")
	{
		this->setRotation(-135);
	}
	else if (m_direction == "rightdown")
	{
		this->setRotation(45);
	}

	this->scheduleUpdate();

	return true;
}

void MonsterRemoteskillDur_NoMove::update(float dt)
{
	m_dt += dt;
	if (m_player != NULL)
	{
		if (this->m_direction == "up" || this->m_direction == "down" || this->m_direction == "left" || this->m_direction == "right")
		{
			if (m_player->getBoundingBox().intersectsRect(getBoundingBox()))
			{
				if (m_attackCounter->getCurTime() == 0.0f || m_attackCounter->getCurTime() > m_attckNumsInter)
				{
					m_attackCounter->start();
					//扣主角hp
					if (m_player->m_hp > 0)
					{
						m_player->m_hp = m_player->m_hp - m_damage;
					}
				}
			}
		}
		//由于45度角的激光的boundingbox是指能包围起整个激光的boundingbox（坑），而不是激光看起来的矩形。
		//调用自己写的任意旋转的矩形判断
		else
		{
			Rect rect = m_player->getBoundingBox();
			Vec2 v0 = Vec2(rect.getMinX(),rect.getMinY());
			Vec2 v1 = Vec2(rect.getMaxX(), rect.getMinY());
			Vec2 v2 = Vec2(rect.getMinX(), rect.getMaxY());
			Vec2 v3 = Vec2(rect.getMaxX(), rect.getMaxY());

			Size size = this->getContentSize();
			Vec2 pos = this->getPosition();
			double Pi = 3.141592653;
			//由于是对角线的，cos45确定四个顶点
			Vec2 thisv0,thisv1,thisv2,thisv3;
			if (this->m_direction == "upright")
			{
				thisv0 = Vec2(pos.x + size.height / 2 * cos(45 / 180.0f * Pi), pos.y - size.height / 2 * cos(45 / 180.0f * Pi));
				thisv1 = Vec2(thisv0.x + size.width * cos(45 / 180.0f * Pi), thisv0.y + size.width * cos(45 / 180.0f * Pi));
				thisv2 = Vec2(thisv1.x - size.height * cos(45 / 180.0f * Pi), thisv1.y + size.height * cos(45 / 180.0f * Pi));
				thisv3 = Vec2(thisv2.x - size.width * cos(45 / 180.0f * Pi), thisv2.y - size.width * cos(45 / 180.0f * Pi));
			}
			else if (this->m_direction == "leftup")
			{
				thisv0 = Vec2(pos.x + size.height / 2 * cos(45 / 180.0f * Pi), pos.y + size.height / 2 * cos(45 / 180.0f * Pi));
				thisv1 = Vec2(thisv0.x - size.width * cos(45 / 180.0f * Pi), thisv0.y + size.width * cos(45 / 180.0f * Pi));
				thisv2 = Vec2(thisv1.x - size.height * cos(45 / 180.0f * Pi), thisv1.y - size.height * cos(45 / 180.0f * Pi));
				thisv3 = Vec2(thisv2.x + size.width * cos(45 / 180.0f * Pi), thisv2.y - size.width * cos(45 / 180.0f * Pi));
			}
			else if (this->m_direction == "downleft")
			{
				thisv0 = Vec2(pos.x - size.height / 2 * cos(45 / 180.0f * Pi), pos.y + size.height / 2 * cos(45 / 180.0f * Pi));
				thisv1 = Vec2(thisv0.x - size.width * cos(45 / 180.0f * Pi), thisv0.y - size.width * cos(45 / 180.0f * Pi));
				thisv2 = Vec2(thisv1.x + size.height * cos(45 / 180.0f * Pi), thisv1.y - size.height * cos(45 / 180.0f * Pi));
				thisv3 = Vec2(thisv2.x + size.width * cos(45 / 180.0f * Pi), thisv2.y + size.width  * cos(45 / 180.0f * Pi));
			}
			else if (this->m_direction == "rightdown")
			{
				thisv0 = Vec2(pos.x - size.height / 2 * cos(45 / 180.0f * Pi), pos.y - size.height / 2 * cos(45 / 180.0f * Pi));
				thisv1 = Vec2(thisv0.x + size.width * cos(45 / 180.0f * Pi), thisv0.y - size.width * cos(45 / 180.0f * Pi));
				thisv2 = Vec2(thisv1.x + size.height * cos(45 / 180.0f * Pi), thisv1.y + size.height * cos(45 / 180.0f * Pi));
				thisv3 = Vec2(thisv2.x - size.width * cos(45 / 180.0f * Pi), thisv2.y + size.width  * cos(45 / 180.0f * Pi));
			}
			

			if (MyRectColli(thisv0, thisv1, thisv2, thisv3, v0, v1, v2, v3))
			{
				if (m_attackCounter->getCurTime() == 0.0f || m_attackCounter->getCurTime() > m_attckNumsInter)
				{
					m_attackCounter->start();
					//扣主角hp
					if (m_player->m_hp > 0)
					{
						m_player->m_hp = m_player->m_hp - m_damage;
					}
				}
			}
		}
	}

	if (m_dt > m_duration)
		this->removeFromParent();
}

//四点组成一个矩形，规定v0v1 平行 v2v3 ，v1v2平行v3v4
bool MonsterRemoteskillDur_NoMove::MyRectColli(Vec2 rect1_v0, Vec2 rect1_v1, Vec2 rect1_v2, Vec2 rect1_v3, Vec2 rect2_v0, Vec2 rect2_v1, Vec2 rect2_v2, Vec2 rect2_v3)
{
	std::vector<Vec2>FourPointVec;
	FourPointVec.push_back(rect1_v0);
	FourPointVec.push_back(rect1_v1);
	FourPointVec.push_back(rect1_v2);
	FourPointVec.push_back(rect1_v3);

	std::vector<Vec2>FourPointVec2;
	FourPointVec2.push_back(rect2_v0);
	FourPointVec2.push_back(rect2_v1);
	FourPointVec2.push_back(rect2_v2);
	FourPointVec2.push_back(rect2_v3);

	//rect1一平行边作为法线，求出此法线的单位向量
	Vec2 rect1_faxian_1 = rect1_v0 - rect1_v1;
	float length = ccpDistance(rect1_v0, rect1_v1);
	rect1_faxian_1.x /= length;
	rect1_faxian_1.y /= length;

	//rect1另一平行边作为法线,求出此法线的单位向量
	Vec2 rect1_faxian_2 = rect1_v2 - rect1_v1;
	length = ccpDistance(rect1_v2, rect1_v1);
	rect1_faxian_2.x /= length;
	rect1_faxian_2.y /= length;

	//rect2一平行边作为法线，求出此法线的单位向量
	Vec2 rect2_faxian_1 = rect2_v0 - rect2_v1;
	length = ccpDistance(rect2_v0, rect2_v1);
	rect2_faxian_1.x /= length;
	rect2_faxian_1.y /= length;

	//rect2另一平行边作为法线,求出此法线的单位向量
	Vec2 rect2_faxian_2 = rect2_v2 - rect2_v1;
	length = ccpDistance(rect2_v2, rect2_v1);
	rect2_faxian_2.x /= length;
	rect2_faxian_2.y /= length;

	std::vector<Vec2>FaxianVec;
	FaxianVec.push_back(rect1_faxian_1);
	FaxianVec.push_back(rect1_faxian_2);
	FaxianVec.push_back(rect2_faxian_1);
	FaxianVec.push_back(rect2_faxian_2);
	

	//利用点乘单位向量求投影的性质(矩形各个点的起始向量规定为父节点左下角)
	//不用管法线单位向量的正负，正负这是相对的，现在目标就是找出两个矩形在法线上的
	//投影，然后找出各个矩形的投影， 看跟另一个矩形的投影是否有交集
	//rect1投影区间

	for (auto faxian : FaxianVec)
	{
		float rect1min, rect1max;
		bool s = false;
		for (auto it : FourPointVec)
		{
			//点乘
			float point = it.x * faxian.x + it.y * faxian.y;
			if (s == false)
			{
				s = true;
				rect1min = rect1max = point;
			}
			else
			{
				if (rect1min > point)
					rect1min = point;
				if (rect1max < point)
					rect1max = point;
			}
		}


		//rect2投影区间
		float rect2min, rect2max;
		bool s1 = false;
		for (auto it : FourPointVec2)
		{
			//点乘
			float point = it.x * faxian.x + it.y * faxian.y;
			if (s1 == false)
			{
				s1 = true;
				rect2min = rect2max = point;
			}
			else
			{
				if (rect2min > point)
					rect2min = point;
				if (rect2max < point)
					rect2max = point;
			}
		}

		//判断投影是否有交集，没交集马上断定两个矩形不相交，这就是轴分离定理
		//没交集
		if (rect2min > rect1max || rect2max < rect1min ||
			rect1min > rect2max || rect1max < rect2min)
		{
			return false;
		}
	}
	
	//都有交集
	return true;
}
