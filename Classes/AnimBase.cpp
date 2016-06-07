#include "AnimBase.h"
#include "cocos2d.h"

AnimBase::AnimBase(Entity* target)
{
	m_sprite = target->getSprite();
	m_monster = (Monster*)target;
	for (int i = 0; i < 3; i++)
	{
		m_stanAnimArray[i] = NULL;
		m_moveAnimArray[i] = NULL;
		m_attaAnimArray[i] = NULL;
		m_attaBigAnimArray[i] = NULL;
		m_attaRemoteAnimArray[i] = NULL;
	}
	/*初始化好动画数组*/
	setAction();
}

AnimBase::~AnimBase()
{
	for (int i = 0; i < 3; i++)
	{
		if (m_stanAnimArray[i] != NULL)
			CC_SAFE_RELEASE_NULL(m_stanAnimArray[i]);
		if (m_moveAnimArray[i] != NULL)
			CC_SAFE_RELEASE_NULL(m_moveAnimArray[i]);
		if (m_attaAnimArray[i] != NULL)
			CC_SAFE_RELEASE_NULL(m_attaAnimArray[i]);
		if (m_attaBigAnimArray[i] != NULL)
			CC_SAFE_RELEASE_NULL(m_attaBigAnimArray[i]);
		if (m_attaRemoteAnimArray[i] != NULL)
			CC_SAFE_RELEASE_NULL(m_attaRemoteAnimArray[i]);
	}
}

int AnimBase::getDirectionByTargetPos(Point tarPos)
{
	Vec2 vec = tarPos - m_monster->getPosition();
	/*一个更好的版本，即根据两个方向的，根据x打还是y大决定用两个方向中哪个为主方向，然后怪物的脸就朝向哪个方向*/
	if (vec.y > 0 && vec.x < 0){
		if (abs(vec.y) > abs(vec.x)){
			return Dir_upleft;
		}
		else{
			return Dir_leftup;
		}
	}
	else if (vec.y > 0 && vec.x > 0){
		if (abs(vec.y) > abs(vec.x)){
			return Dir_upright;
		}
		else{
			return Dir_rightup;
		}
	}
	else if (vec.y < 0 && vec.x < 0){
		if (abs(vec.y) > abs(vec.x)){
			return Dir_downleft;
		}
		else{
			return Dir_leftdown;
		}
	}
	else if (vec.y < 0 && vec.x > 0){
		if (abs(vec.y) > abs(vec.x)){
			return Dir_downright;
		}
		else{
			return Dir_rightdown;
		}
	}
	else if (vec.y > 0) return Dir_up;
	else if (vec.y < 0) return Dir_down;
	else if (vec.x > 0) return Dir_right;
	else if (vec.x < 0) return Dir_left;
}

int AnimBase::getCurDirection()
{
	return m_curDirection;
}

void AnimBase::setCurDirection(Point tarPos)
{
	 m_curDirection = (AnimDirection)getDirectionByTargetPos(tarPos);
}

void AnimBase::playStanAnim()
{
	if (m_sprite == NULL)
		return;
	if (m_curDirection == Dir_right || m_curDirection == Dir_rightup || m_curDirection == Dir_rightdown)
	{
		m_sprite->setScaleX(-m_monster->getMonster_magnification());
	}
	else if (m_curDirection == Dir_left || m_curDirection == Dir_leftup || m_curDirection == Dir_leftdown)
		m_sprite->setScaleX(m_monster->getMonster_magnification());
	int direction = -1;
	switch (m_curDirection)
	{
	case Dir_up:
		direction = 1;
		break;
	case Dir_down:
		direction = 2;
		break;
	case Dir_left:
		direction = 0;
		break;
	case Dir_right:
		direction = 0;
		break;
	case Dir_upleft:
		direction = 1;
		break;
	case Dir_upright:
		direction = 1;
		break;
	case Dir_downleft:
		direction = 2;
		break;
	case Dir_downright:
		direction = 2;
		break;
	case Dir_leftup:
		direction = 0;
		break;
	case Dir_leftdown:
		direction = 0;
		break;
	case Dir_rightup:
		direction = 0;
		break;
	case Dir_rightdown:
		direction = 0;
		break;
	default:
		break;
	}
	m_sprite->stopAllActions();
	if (direction >=0 && direction < 3)
		m_sprite->runAction(m_stanAnimArray[direction]);
}

void AnimBase::playMoveAnim()
{
	if (m_sprite == NULL)
		return;
	if (m_curDirection == Dir_right || m_curDirection == Dir_rightup || m_curDirection == Dir_rightdown)
	{
		m_sprite->setScaleX(-m_monster->getMonster_magnification());
	}
	else if (m_curDirection == Dir_left || m_curDirection == Dir_leftup || m_curDirection == Dir_leftdown)
		m_sprite->setScaleX(m_monster->getMonster_magnification());
	int direction = -1;
	switch (m_curDirection)
	{
	case Dir_up:
		direction = 1;
		break;
	case Dir_down:
		direction = 2;
		break;
	case Dir_left:
		direction = 0;
		break;
	case Dir_right:
		direction = 0;
		break;
	case Dir_upleft:
		direction = 1;
		break;
	case Dir_upright:
		direction = 1;
		break;
	case Dir_downleft:
		direction = 2;
		break;
	case Dir_downright:
		direction = 2;
		break;
	case Dir_leftup:
		direction = 0;
		break;
	case Dir_leftdown:
		direction = 0;
		break;
	case Dir_rightup:
		direction = 0;
		break;
	case Dir_rightdown:
		direction = 0;
		break;
	default:
		break;
	}
	m_sprite->stopAllActions();
	if (direction >= 0 && direction < 3)
		m_sprite->runAction(m_moveAnimArray[direction]);
}

void AnimBase::playAttaAnim()
{
	if (m_sprite == NULL)
		return;
	if (m_curDirection == Dir_right || m_curDirection == Dir_rightup || m_curDirection == Dir_rightdown)
	{
		m_sprite->setScaleX(-m_monster->getMonster_magnification());
	}
	else if (m_curDirection == Dir_left || m_curDirection == Dir_leftup || m_curDirection == Dir_leftdown)
		m_sprite->setScaleX(m_monster->getMonster_magnification());
	int direction = -1;
	switch (m_curDirection)
	{
	case Dir_up:
		direction = 1;
		break;
	case Dir_down:
		direction = 2;
		break;
	case Dir_left:
		direction = 0;
		break;
	case Dir_right:
		direction = 0;
		break;
	case Dir_upleft:
		direction = 1;
		break;
	case Dir_upright:
		direction = 1;
		break;
	case Dir_downleft:
		direction = 2;
		break;
	case Dir_downright:
		direction = 2;
		break;
	case Dir_leftup:
		direction = 0;
		break;
	case Dir_leftdown:
		direction = 0;
		break;
	case Dir_rightup:
		direction = 0;
		break;
	case Dir_rightdown:
		direction = 0;
		break;
	default:
		break;
	}
	m_sprite->stopAllActions();
	if (direction >= 0 && direction < 3 && m_monster->m_curskill == "baseskill")
	{
		m_sprite->runAction(m_attaAnimArray[direction]);
	}
	else if (direction >= 0 && direction < 3 && m_monster->m_curskill == "bigskill")
	{
		m_sprite->runAction(m_attaBigAnimArray[direction]);
	}
	else if (direction >= 0 && direction < 3 && m_monster->m_curskill == "remoteskill")
	{
		m_sprite->runAction(m_attaRemoteAnimArray[direction]);
	}
		
}

void AnimBase::setAction()
{
	/*到时在json文件一并处理,现在先测试*/

//	Animation* animation = AnimationUtil::createWithSingleFrameName("treemonsterhstatic", 0.1f, 1);
	Animation* animation = AnimationUtil::createWithSingleFrameName((m_monster->monsdata.name + "hstatic").c_str(),0.1f, -1);
	Animate* animate = Animate::create(animation);
	animate->retain();
	m_stanAnimArray[0] = animate; //静止0

	//animation = AnimationUtil::createWithSingleFrameName("treemonsterustatic", 0.1f, 1);
	animation = AnimationUtil::createWithSingleFrameName((m_monster->monsdata.name + "ustatic").c_str(), 0.1f, -1);
	animate = Animate::create(animation);
	animate->retain();
	m_stanAnimArray[1] = animate; //上1

	//animation = AnimationUtil::createWithSingleFrameName("treemonsterdstatic", 0.1f, 1);
	animation = AnimationUtil::createWithSingleFrameName((m_monster->monsdata.name + "dstatic").c_str(),0.1f,-1);
	animate = Animate::create(animation);
	animate->retain();
	m_stanAnimArray[2] = animate;//下2


	//animation = AnimationUtil::createWithSingleFrameName("treemonsterhrun", 0.1f, 1);
	animation = AnimationUtil::createWithSingleFrameName((m_monster->monsdata.name + "hrun").c_str(),0.1f,-1);
	animate = Animate::create(animation);
	animate->retain();
	m_moveAnimArray[0] = animate;

	//animation = AnimationUtil::createWithSingleFrameName("treemonsterurun", 0.1f, 1);
	animation = AnimationUtil::createWithSingleFrameName((m_monster->monsdata.name + "urun").c_str(), 0.1f, -1);
	animate = Animate::create(animation);
	animate->retain();
	m_moveAnimArray[1] = animate;


	//animation = AnimationUtil::createWithSingleFrameName("treemonsterdrun", 0.1f, 1);
	animation = AnimationUtil::createWithSingleFrameName((m_monster->monsdata.name + "drun").c_str(),0.1f,-1);
	animate = Animate::create(animation);
	animate->retain();
	m_moveAnimArray[2] = animate;
	

	//攻击动画时间安排要跟怪物的每帧时间对应
	if (m_monster->monsdata.skillmap.find("baseskill") != m_monster->monsdata.skillmap.end())
	{
		animation = AnimationUtil::createWithSingleFrameName(m_monster->monsdata.skillmap["baseskill"].hAnimateName.c_str(), m_monster->monsdata.skillmap["baseskill"].attackAnimatetimePer, 1);
		animate = Animate::create(animation);
		animate->retain();
		m_attaAnimArray[0] = animate;

		animation = AnimationUtil::createWithSingleFrameName(m_monster->monsdata.skillmap["baseskill"].uAnimateName.c_str(), m_monster->monsdata.skillmap["baseskill"].attackAnimatetimePer, 1);
		animate = Animate::create(animation);
		animate->retain();
		m_attaAnimArray[1] = animate;


		animation = AnimationUtil::createWithSingleFrameName(m_monster->monsdata.skillmap["baseskill"].dAnimateName.c_str(), m_monster->monsdata.skillmap["baseskill"].attackAnimatetimePer, 1);
		animate = Animate::create(animation);
		animate->retain();
		m_attaAnimArray[2] = animate;
	}
	

	///大技能测试
	if (m_monster->monsdata.skillmap.find("bigskill") != m_monster->monsdata.skillmap.end())
	{
		animation = AnimationUtil::createWithSingleFrameName(m_monster->monsdata.skillmap["bigskill"].hAnimateName.c_str(),m_monster->monsdata.skillmap["bigskill"].attackAnimatetimePer, 1);
		animate = Animate::create(animation);
		animate->retain();
		m_attaBigAnimArray[0] = animate;

		animation = AnimationUtil::createWithSingleFrameName(m_monster->monsdata.skillmap["bigskill"].uAnimateName.c_str(), m_monster->monsdata.skillmap["bigskill"].attackAnimatetimePer, 1);
		animate = Animate::create(animation);
		animate->retain();
		m_attaBigAnimArray[1] = animate;


		animation = AnimationUtil::createWithSingleFrameName(m_monster->monsdata.skillmap["bigskill"].dAnimateName.c_str(), m_monster->monsdata.skillmap["bigskill"].attackAnimatetimePer, 1);
		animate = Animate::create(animation);
		animate->retain();
		m_attaBigAnimArray[2] = animate;
	}

	//远程技能测试
	if (m_monster->monsdata.remoteskillmap.find("remoteskill") != m_monster->monsdata.remoteskillmap.end())
	{
		animation = AnimationUtil::createWithSingleFrameName(m_monster->monsdata.remoteskillmap["remoteskill"].hAnimateName.c_str(), m_monster->monsdata.remoteskillmap["remoteskill"].attackAnimatetimePer, 1);
		animate = Animate::create(animation);
		animate->retain();
		m_attaRemoteAnimArray[0] = animate;

		animation = AnimationUtil::createWithSingleFrameName(m_monster->monsdata.remoteskillmap["remoteskill"].uAnimateName.c_str(), m_monster->monsdata.remoteskillmap["remoteskill"].attackAnimatetimePer, 1);
		animate = Animate::create(animation);
		animate->retain();
		m_attaRemoteAnimArray[1] = animate;

		animation = AnimationUtil::createWithSingleFrameName(m_monster->monsdata.remoteskillmap["remoteskill"].dAnimateName.c_str(), m_monster->monsdata.remoteskillmap["remoteskill"].attackAnimatetimePer, 1);
		animate = Animate::create(animation);
		animate->retain();
		m_attaRemoteAnimArray[2] = animate;

	}
}