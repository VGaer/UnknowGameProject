#include "AnimBase.h"

AnimBase::AnimBase(Entity* target)
{
	m_sprite = target->getSprite();
	m_monster = (Monster*)target;
	/*��ʼ���ö�������*/
	setAction();
}

int AnimBase::getDirectionByTargetPos(Point tarPos)
{
	Vec2 vec = tarPos - m_monster->getPosition();
	/*һ�����õİ汾����������������ģ�����x����y������������������ĸ�Ϊ������Ȼ���������ͳ����ĸ�����*/
	if (vec.y > 0 && vec.x < 0) {
		if (abs(vec.y) > abs(vec.x)) {
			return Dir_upleft;
		}
		else {
			return Dir_leftup;
		}
	}
	else if (vec.y > 0 && vec.x > 0) {
		if (abs(vec.y) > abs(vec.x)) {
			return Dir_upright;
		}
		else {
			return Dir_rightup;
		}
	}
	else if (vec.y < 0 && vec.x < 0) {
		if (abs(vec.y) > abs(vec.x)) {
			return Dir_downleft;
		}
		else {
			return Dir_leftdown;
		}
	}
	else if (vec.y < 0 && vec.x > 0) {
		if (abs(vec.y) > abs(vec.x)) {
			return Dir_downright;
		}
		else {
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
	if (direction >= 0 && direction < 3)
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
	if (direction >= 0 && direction < 3)
		m_sprite->runAction(m_attaAnimArray[direction]);
}

void AnimBase::setAction()
{
	/*��ʱ��json�ļ�һ������,�����Ȳ���*/

	Animation* animation = AnimationUtil::createWithSingleFrameName("treemonsterhstatic", 0.1f, 1);
	Animate* animate = Animate::create(animation);
	animate->retain();
	m_stanAnimArray[0] = animate; //��ֹ0

	animation = AnimationUtil::createWithSingleFrameName("treemonsterustatic", 0.1f, 1);
	animate = Animate::create(animation);
	animate->retain();
	m_stanAnimArray[1] = animate; //��1

	animation = AnimationUtil::createWithSingleFrameName("treemonsterdstatic", 0.1f, 1);
	animate = Animate::create(animation);
	animate->retain();
	m_stanAnimArray[2] = animate;//��2


	animation = AnimationUtil::createWithSingleFrameName("treemonsterhrun", 0.1f, 1);
	animate = Animate::create(animation);
	animate->retain();
	m_moveAnimArray[0] = animate;

	animation = AnimationUtil::createWithSingleFrameName("treemonsterurun", 0.1f, 1);
	animate = Animate::create(animation);
	animate->retain();
	m_moveAnimArray[1] = animate;


	animation = AnimationUtil::createWithSingleFrameName("treemonsterdrun", 0.1f, 1);
	animate = Animate::create(animation);
	animate->retain();
	m_moveAnimArray[2] = animate;


	//��������ʱ�䰲��Ҫ������Ĺ��������Ӧ��
	animation = AnimationUtil::createWithSingleFrameName("treemonsterhattack", m_monster->attackAnimatetimePer, 1);
	animate = Animate::create(animation);
	animate->retain();
	m_attaAnimArray[0] = animate;

	animation = AnimationUtil::createWithSingleFrameName("treemonsteruattack", m_monster->attackAnimatetimePer, 1);
	animate = Animate::create(animation);
	animate->retain();
	m_attaAnimArray[1] = animate;


	animation = AnimationUtil::createWithSingleFrameName("treemonsterdattack", m_monster->attackAnimatetimePer, 1);
	animate = Animate::create(animation);
	animate->retain();
	m_attaAnimArray[2] = animate;
}