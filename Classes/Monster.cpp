#include "Monster.h"
#include "GameData.h"
#define Pi 3.141592653


Monster* Monster::create(const std::string& name)
{
	auto unit = new Monster();
	if (unit && unit->init(name)){
		unit->autorelease();
	}
	else{
		CC_SAFE_DELETE(unit);
		unit = NULL;
	}
	return unit;
}

Monster::~Monster()
{
	CC_SAFE_DELETE(findPath);
	CC_SAFE_DELETE(animBase);
}

bool Monster::init(const std::string& name)
{
	auto data = GameData::getInstance()->getDataFromMonsData(name);
	this->name = data->name;
	hp = data->hp;
	damage = data->damage;
	moveSpeed = data->moveSpeed;
	attackInter = data->attackInter;
	attackRange = data->attackRange;
	eyeRange = data->eyeRange;
	patrolRange = data->patrolRange;
	perceptionRange = data->perceptionRange;
	attackedrestoretimes = data->attackedrestoretimes;
	bindSprite(Sprite::create(data->imagePath));
	//记录怪物颜色
	m_monstercolor = this->getSprite()->getColor();

	findPath = new FindPath();//findPath需要绑定monster
	findPath->bindMonster(this);

	stateMachine = new StateMachine(this);

	animBase = new AnimBase(this);//设置控制器,控制器用到了monster精灵。



	//设置定时器
	m_timecounter = TimeCounter::create();
	this->addChild(m_timecounter);

	//每个Node都设置为0.5 0.5的锚点
	this->setAnchorPoint(Vec2(0.5, 0.5));

	//怪物的初态都弄为悠闲吧
	stateMachine->SetCurrState(new Idle());
	stateMachine->CurrentState()->Enter(this);

	IspatrolEnd = true;//初始化为true，巡逻状态时才可以第一次寻路。

	this->scheduleUpdate();

	m_magnification = 1;//精灵放大倍数

	Is_firstFindplayer_Track = true;

	return true;
}

AnimBase* Monster::getAnimBase()
{
	return animBase;
}

StateMachine* Monster::getStateMachine()
{
	return stateMachine;
}

Player* Monster::getPlayer()
{
	return player;
}

void Monster::bindPlayer(Player* player)
{
	this->player = player;
}

//移动怪物，分为追踪的移动和巡逻的移动
void Monster::cmd_moveTo(Point tarPos)
{
	//追踪状态
	if (player != NULL && this->m_CurMachineState == enum_MonsterTrack)
	{
		Vec2 start = player->tiledCoordForPosition(this->getPosition());
		Vec2 end = player->tiledCoordForPosition(tarPos);//玩家位置
		findPath->run(start,end);
	}
	//巡逻状态
	else
	{
		Vec2 start = player->tiledCoordForPosition(this->getPosition());
		Vec2 end = tarPos;//巡逻目标位置(巡逻的时候直接传了瓦片的坐标进来了)
		if (end.x >= 0 && end.x < m_parrent->getMapSize().width && end.y >= 0 && end.y < m_parrent->getMapSize().height){
				//肯定可以寻路的，因为巡逻地点在barrier层中已经设置到非障碍物瓦片的地方
				findPath->run(start, end);
				//每寻路完一步，看是否到达目的块,
				//!!!因为寻路算法中不会走到终点块，所以条件成立只能判断是否走到终点瓦片的四周
				Vec2 tiledCoorddif = player->tiledCoordForPosition(this->getPosition()) - end;
				if (!(tiledCoorddif.getLength() > sqrt(2.0))){
					IspatrolEnd = true;
				}
				else{
					IspatrolEnd = false;
				}
			}
	}	
}

void Monster::cmd_stop()
{
	animBase->playStanAnim();
}

bool Monster::cmd_hurt(float damage)
{
	hp -= damage;
	if (hp <= 0)
		return true;
	return false;
}

void Monster::cmd_attack()
{
	//进入攻击范围时，是以怪物前面判定的，此时怪物控制器的方向已经面向主角了
	animBase->playAttaAnim();
}

bool Monster::checkInEyeRange()
{
	//如果是首次发现主角,//采用包含怪物有视觉盲区且主角在怪物后方不可见的方式判断
	if (Is_firstFindplayer_Track == true){
		switch (animBase->getCurDirection())
		{
		case Dir_up:{
			if (Dir_up == checkInSectorRange())	return true;
		}
		case Dir_down:{
			if (Dir_down == checkInSectorRange()) return true;
		}
		case Dir_left:{
			if (Dir_left == checkInSectorRange()) return true;
		}
		case Dir_right:{
			if (Dir_right == checkInSectorRange()) return true;
		}
		case Dir_upleft:{
			if (Dir_up == checkInSectorRange()) return true;
		}
		case Dir_upright:{
			if (Dir_up == checkInSectorRange()) return true;
		}
		case Dir_downleft:{
			if (Dir_down == checkInSectorRange()) return true;
		}
		case Dir_downright:{
			if (Dir_down == checkInSectorRange()) return true;
		}
		case Dir_leftup:{
			if (Dir_left == checkInSectorRange()) return true;
		}
		case Dir_leftdown:{
			if (Dir_left == checkInSectorRange()) return true;
		}
		case Dir_rightup:{
			if (Dir_right == checkInSectorRange()) return true;
		}
		case Dir_rightdown:{
			if (Dir_right == checkInSectorRange()) return true;
		}
		}
		return false;
	}
	//非首次发现主角，以怪物为中点，一个圆的区域去追踪主角，除非主角跑出圆区域，否则走不掉
	else{
		Vec2 distanceVec = player->getPosition() - this->getPosition();
		if (distanceVec.getLength() < eyeRange)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool Monster::checkInPerceptionRange()
{
	Vec2 distanceVec = player->getPosition() - this->getPosition();
	if (distanceVec.getLength() < perceptionRange)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//攻击范围写为怪物前面，（攻击的之后改成矩形的，长宽在文件搞定)
bool Monster::checkInAttaRange()
{
	if (player != NULL)
	{
		Vec2 posdif = player->getPosition() - getPosition();
		float distance = posdif.getLength();
		if (distance <= attackRange){
			switch (animBase->getCurDirection())
			{
			case Dir_up:{
				if (posdif.y > 0) return true;
			}
				break;
			case Dir_down:{
				if (posdif.y < 0) return true;
			}
				break;
			case Dir_left:
				if (posdif.x < 0) return true;
				break;
			case Dir_right:
				if (posdif.x > 0) return true;
				break;
			case Dir_upleft:
				if (posdif.y < 0) return true;
				break;
			case Dir_upright:
				if (posdif.y > 0) return true;
				break;
			case Dir_downleft:
				if (posdif.y < 0) return true;
				break;
			case Dir_downright:
				if (posdif.y > 0) return true;
				break;
			case Dir_leftup:
				if (posdif.x < 0) return true;
			case Dir_leftdown:
				if (posdif.x < 0) return true;
			case Dir_rightup:
				if (posdif.x > 0) return true;
			case Dir_rightdown:
				if (posdif.x > 0) return true;
			default:
				break;
			}
		}
	}
	return false;
}

void Monster::update(float dt)
{
	stateMachine->Update();
}

void Monster::setMachineState(int i)
{
	m_CurMachineState = i;
}

//判断主角是否在以怪物为中心点的扇形区域,在的话返回哪个扇形
int Monster::checkInSectorRange()
{
	if (player != NULL)
	{
		float distance = (player->getPosition() - getPosition()).getLength();
		//在圆内
		if (distance < eyeRange){
			//判断在哪个扇形区域
			double vecX = player->getPositionX() - this->getPositionX();
			double vecY = player->getPositionY() - this->getPositionY();
			double tanNum = tan(vecY / vecX);
			double arctanNum = atan(tanNum);//-pi/2 pi/2
			//95度半边，总170度的扇形
			//只要弧度大于5度对应弧度，就不在四个象限中，怪物上、下、左、右视觉的盲区中
			if (abs(arctanNum) > abs(atan(tan(Pi / 180 * 5))))
			{
				switch (animBase->getCurDirection())
				{
				case Dir_up:{
					//主角不在盲区时，也不能在怪物的背面,在背面return -1
					if (vecY > 0)
						return Dir_up;
					else
						return -1;
				}
				case Dir_down:{
					if (vecY < 0)
						return Dir_down;
					else
						return -1;
				}
				case Dir_left:{
					if (vecX < 0)
						return Dir_left;
					else
						return -1;
				}
				case Dir_right:{
					if (vecX > 0)
						return Dir_right;
					else
						return -1;
				}
				case Dir_upleft:{
					if (vecY < 0)
						return Dir_up;
					else
						return -1;
				}
				case Dir_upright:{
					if (vecY > 0)
						return Dir_up;
					else
						return -1;
				}
				case Dir_downleft:{
					if (vecY < 0)
						return Dir_down;
					else
						return -1;
				}
				case Dir_downright:{
					if (vecY > 0)
						return Dir_down;
					else
						return -1;
				}
				case Dir_leftdown:{
					if (vecX < 0)
						return Dir_left;
					else
						return -1;
				}
				case Dir_rightdown:{
					if (vecX > 0)
						return Dir_right;
					else
						return -1;
				}
				case Dir_leftup:{
					if (vecX < 0)
						return Dir_upleft;
					else
						return -1;
				}
				case Dir_rightup:{
					if (vecX > 0)
						return Dir_right;
					else
						return -1;
				}
				}
			}
			return -1;
		}
	}
}
int Monster::getCurMachineState()
{
	return m_CurMachineState;
}

void Monster::setMonsterParent(TMXTiledMap* parent)
{
	m_parrent = parent;
}

int Monster::getMonster_magnification()
{
	return m_magnification;
}

void Monster::setvecPatrolpoint()
{
	if (m_parrent != NULL)
	{
		for (int i = 0; i < m_parrent->getMapSize().width; i++)
		{
			for (int j = 0; j < m_parrent->getMapSize().height; j++)
			{
				int tileGid = m_parrent->getLayer("barrier")->getTileGIDAt(Vec2(i, j));
				if (tileGid > 0)
				{
					Value prop = m_parrent->getPropertiesForGID(tileGid);
					ValueMap proValueMap = prop.asValueMap();

					if (proValueMap.find("patrolpoint") != proValueMap.end()){
						std::string patrolpoint = proValueMap.at("patrolpoint").asString();
						if (patrolpoint == "true"){
							vecPatrolpoint.push_back(Vec2(i,j));
						}
					}
				}
			}
		}
	}
}

std::vector<Vec2> Monster::getPatrolpointvec()
{
	return vecPatrolpoint;
}

bool Monster::IsattackedByPlayer()
{
	//获取主角剑气
	auto vec = player->getPlayerUsing_swordwave_Arr();
	if (vec.size() > 0){
		for (int i = 0; i < vec.size(); i++){
			if (vec.at(i)->isVisible())
			{
				auto swordwave = vec.at(i);
				Rect rect;
				rect.setRect(this->getPositionX()- this->getContentSize().width / 2,
					this->getPositionY() - this->getContentSize().height / 2,
					this->getContentSize().width,
					this->getContentSize().height);
				if (rect.containsPoint(swordwave->getPosition())){
					swordwave->hide();
					return true;
				}
			}
		}
	}
	//获取主角技能J、K按键容器（在主角类写了vecskill元素最多为一个）
	auto vecskill = player->getVecSkill();
	if (vecskill.size() > 0){
		//如果是普通攻击或者前冲攻击
		if (vecskill.back() == enum_baseattack || vecskill.back() == enum_basepoke){
			//先写死测试下，到时主角攻击范围也设置在json文件里
			Rect rect;
			rect.setRect(this->getPositionX() - this->getContentSize().width / 2,
				this->getPositionY() - this->getContentSize().height / 2,
				this->getContentSize().width,
				this->getContentSize().height);
			int playerattackRange = 32;
			Vec2 vec;
		
			//主角的普通攻击暂时写成一个点。
			switch (player->getPlayerDir())
			{
			case em_up:{
				vec = player->getPosition();
				vec.y += playerattackRange;
				if (rect.containsPoint(vec))
					return true;
				break;
			}
			case em_down:{
				vec = player->getPosition();
				vec.y -= playerattackRange;
				if (rect.containsPoint(vec))
					return true;
				break;
			}
			case em_left:{
				vec = player->getPosition();
				vec.x -= playerattackRange;
				if (rect.containsPoint(vec))
					return true;
				break;
			}
			case em_right:{
				vec = player->getPosition();
				vec.x += playerattackRange;
				if (rect.containsPoint(vec))
					return true;
				break;
			}
			}
		}
	}
	return false;
}