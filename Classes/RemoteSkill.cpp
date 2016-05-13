#include "RemoteSkill.h"

RemoteSkill::RemoteSkill()
{
	m_skillDir = enum_remoteskill_none;
	this->hide();
}

RemoteSkill::~RemoteSkill()
{

}

bool RemoteSkill::init()
{
	return true;
}

void RemoteSkill::hide()
{
	this->setVisible(false);
}

void RemoteSkill::show()
{
	this->setVisible(true);
}

void RemoteSkill::setSkillMove(Vec2 movespeed)
{
	switch (m_skillDir)
	{
	case enum_remoteskill_up:{
		this->setPosition(this->getPosition() + movespeed);
		break;
	}
	case enum_remoteskill_down:{
		this->setPosition(this->getPosition() + movespeed);
		break;
	}
	case enum_remoteskill_left:{
		this->setPosition(this->getPosition() + movespeed);
		break;
	}
	case enum_remoteskill_right:{
		this->setPosition(this->getPosition() + movespeed);
		break;
	}
	default:
		break;
	}
}

void RemoteSkill::setSkillDir(int dir)
{
	m_skillDir = dir;
}

int RemoteSkill::getSkillDir()
{
	return m_skillDir;
}