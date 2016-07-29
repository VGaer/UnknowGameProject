#include "Player.h"
#include "GameScene.h"
#include "ChangeScenePointManager.h"
#include "Talk.h"
#include "G2U.h"
#include "StartGameScene.h"
#include "TalkManager.h"

Player::Player()
{
	gamescenedir = "none";

	//初始化父类
	Entity::init();

	IscanMove = true;

	m_map = NULL;
}

Player* Player::getInstance()
{
	static Player* instance = NULL;
	if (instance == NULL)
	{
		instance = new Player();
	}
	return instance;
}


bool Player::init()
{
	//由于是回调函数，而且是Sequence的，到回调函数执行的那段时间可能攻击队列又有东西了，所以在init的时候清一次,主角死后的加载就会清掉被攻击的队列
	//清掉当前被攻击的队列
	while (!attackedqueue.empty())
		attackedqueue.pop();


	//判断为空才添加精灵，防止重复添加
	if (getSprite() == NULL)
	{
		this->bindSprite(Sprite::create("player.png"));
		m_playerColor = this->getSprite()->getColor();

		//只添加一个键盘监听器就好，防止加多个，因为主角死后，回开始场景点开始游戏addPlayer时又调用了一次init();
		auto keyboardListener = EventListenerKeyboard::create();
		keyboardListener->onKeyPressed = CC_CALLBACK_2(Player::keyPressed, this);
		keyboardListener->onKeyReleased = CC_CALLBACK_2(Player::keyReleased, this);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);
	}

	this->scheduleUpdate();
	this->schedule(schedule_selector(Player::baseskillcollidUpdata));
	this->schedule(schedule_selector(Player::Playerhp_mp_Update),0.2f);
	this->schedule(schedule_selector(Player::recoverHp_Mp), 1.0f);
	this->schedule(schedule_selector(Player::ChangSceneIdUpdate));
	this->schedule(schedule_selector(Player::LevelUpdate));
	this->schedule(schedule_selector(Player::PlayerDiedUpdate));

	timecounter_up = TimeCounter::create();
	this->addChild(timecounter_up);
	timecounter_down = TimeCounter::create();
	this->addChild(timecounter_down);
	timecounter_left = TimeCounter::create();
	this->addChild(timecounter_left);
	timecounter_right = TimeCounter::create();
	this->addChild(timecounter_right);
	timecounter_J = TimeCounter::create();
	this->addChild(timecounter_J);
	timecounter_attacked = TimeCounter::create();
	this->addChild(timecounter_attacked);

	SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
	frameCache->addSpriteFramesWithFile("dwalk/dwalk.plist", "dwalk/dwalk.png");
	frameCache->addSpriteFramesWithFile("uwalk/uwalk.plist", "uwalk/uwalk.png");
	frameCache->addSpriteFramesWithFile("hwalk/hwalk.plist", "hwalk/hwalk.png");

	frameCache->addSpriteFramesWithFile("dstatic/dstatic.plist", "dstatic/dstatic.png");
	frameCache->addSpriteFramesWithFile("ustatic/ustatic.plist", "ustatic/ustatic.png");
	frameCache->addSpriteFramesWithFile("hstatic/hstatic.plist", "hstatic/hstatic.png");

	frameCache->addSpriteFramesWithFile("drun/drun.plist", "drun/drun.png");
	frameCache->addSpriteFramesWithFile("urun/urun.plist", "urun/urun.png");
	frameCache->addSpriteFramesWithFile("hrun/hrun.plist", "hrun/hrun.png");

	frameCache->addSpriteFramesWithFile("dbaseattack/dbaseattack.plist", "dbaseattack/dbaseattack.png");
	frameCache->addSpriteFramesWithFile("ubaseattack/ubaseattack.plist", "ubaseattack/ubaseattack.png");
	frameCache->addSpriteFramesWithFile("hbaseattack/hbaseattack.plist", "hbaseattack/hbaseattack.png");
	//第二种普通攻击动画
	frameCache->addSpriteFramesWithFile("fdbaseattack/fdbaseattack.plist", "fdbaseattack/fdbaseattack.png");
	frameCache->addSpriteFramesWithFile("fubaseattack/fubaseattack.plist", "fubaseattack/fubaseattack.png");
	frameCache->addSpriteFramesWithFile("fhbaseattack/fhbaseattack.plist", "fhbaseattack/fhbaseattack.png");

	frameCache->addSpriteFramesWithFile("dbasepoke/dbasepoke.plist", "dbasepoke/dbasepoke.png");
	frameCache->addSpriteFramesWithFile("ubasepoke/ubasepoke.plist", "ubasepoke/ubasepoke.png");
	frameCache->addSpriteFramesWithFile("hbasepoke/hbasepoke.plist", "hbasepoke/hbasepoke.png");

	frameCache->addSpriteFramesWithFile("dswordwave/dswordwave.plist", "dswordwave/dswordwave.png");
	frameCache->addSpriteFramesWithFile("uswordwave/uswordwave.plist", "uswordwave/uswordwave.png");
	frameCache->addSpriteFramesWithFile("hswordwave/hswordwave.plist", "hswordwave/hswordwave.png");

	frameCache->addSpriteFramesWithFile("remoteskills/playerskill.plist", "remoteskills/playerskill.png");
	frameCache->addSpriteFramesWithFile("player_skill/laser.plist", "player_skill/laser.png");
	frameCache->addSpriteFramesWithFile("player_skill/fire.plist", "player_skill/fire.png");
	frameCache->addSpriteFramesWithFile("player_skill/bomb.plist", "player_skill/bomb.png");

	/*主角升级动画*/
	frameCache->addSpriteFramesWithFile("playerLevelup/playerlevelup.plist", "playerLevelup/playerlevelup.png");

	PlayerState = enum_initNone;//初始化为什么都没有状态，一运行游戏如果没操作就会转为enum_static,有操作转为对应操作的walk or run状态 
	PlayerDir = em_down;//初始化时

	swordwaveNum = 20;
	//角色死亡后重新开始游戏会重新创建
	createSwordWave();

	m_player_magnification = 2.5;

	m_hp = 100;
	m_mp = 100;

	//暂时弄100
	curLevel_Maxhp = 100;
	curLevel_Maxmp = 100;
	
	skillControl = SkillControl::create();
	addChild(skillControl);

	k_consumemp = 4;
	l_consumemp = 7;
	u_consumemp = 6;

	//初始化等级
	m_playerlevel = 1;
	//初始化exp  
	m_exp = 0;

	spritelevelup = NULL;

	gamescenedir = "none";

	isAcceptInput = true;
	isInChangeScenePoint = false;

	nearBarrierMove = false;
	return true;
}

void Player::update(float dt)
{
	if (vec.size() == 1)
		log("%d",vec[0]);

	if (!isAcceptInput)
		return;
	/*如果玩家超出了地图边界*/
	if (this->getPositionX() - getContentSize().width / 2 <= 0)
	{
		this->setPositionX(getContentSize().width / 2);
	}
	if (this->getPositionX() + getContentSize().width / 2 >= m_map->getMapSize().width * m_map->getTileSize().width)
	{
		this->setPositionX(m_map->getMapSize().width * m_map->getTileSize().width - getContentSize().width / 2);
	}
	if (this->getPositionY() <= 0)
	{
		this->setPositionY(0 + 2);
	}
	if (this->getPositionY() + getContentSize().height / 2 >= m_map->getMapSize().height * m_map->getTileSize().height)
	{
		this->setPositionY(m_map->getMapSize().height * m_map->getTileSize().height - getContentSize().height / 2);
	}


	/////////////////////剑气的移动
	if (m_Using_swordwave_Arr.size() > 0) {
		RemoteSkill* swordwave;
		for (int i = 0; i < m_Using_swordwave_Arr.size(); i++) { //换成for(swordwave:m_Using_swordwave_Arr)会奔溃,估计这个样式的循环没动态判断size()
			swordwave = m_Using_swordwave_Arr.at(i);

			//动态设置剑气的vertexz
			auto p = swordwave->getPosition();
			p = CC_POINT_POINTS_TO_PIXELS(p);

			swordwave->setVertexZ(-((p.y + 64) / 64));

			switch (swordwave->getSkillDir())
			{
			case enum_remoteskill_up: {
				//超屏幕了
				//主角在地图边缘可视化剑气为整个屏幕大小，主角非地图边缘可视化剑气为一半屏幕大小，取全屏保证
				Size visiblesize = Director::getInstance()->getVisibleSize();
				visiblesize.width += 1;
				visiblesize.height += 1;//比屏幕变大一点点
				Vec2 pos = (swordwave->getPosition() + Vec2(0, -(swordwave->getContentSize().height / 2))) - this->getPosition();
				//剑气超出了屏幕高度
				if (abs(pos.x) > visiblesize.width || abs(pos.y) > visiblesize.height) {
					swordwave->hide();//隐藏剑气
					m_swordwave_Arr.pushBack(swordwave);//把剑气给m_swordwave_Arr容器
					m_Using_swordwave_Arr.eraseObject(swordwave);//从m_Using_swordwave中移除
					i--;//m_Using_swordwave_Arr被Pop掉一个后，原位置被容器中另一个元素取代，循环结束后i++判断当前元素
				}
				//碰到障碍物了
				else if (IsSussesfulForsetRemoteSkillPos(swordwave->getPosition() + Vec2(0, 6)) == false) {
					swordwave->hide();//隐藏剑气
					m_swordwave_Arr.pushBack(swordwave);//把剑气给m_swordwave_Arr容器
					m_Using_swordwave_Arr.eraseObject(swordwave);//从m_Using_swordwave中移除
					i--;//m_Using_swordwave_Arr被Pop掉一个后，原位置被容器中另一个元素取代，循环结束后i++判断当前元素
				}
				//没碰到障碍物
				else {
					swordwave->setSkillMove(Vec2(0, 6));
				}
				break;
			}
			case enum_remoteskill_down: {
				//超屏幕了
				//主角在地图边缘可视化剑气为整个屏幕大小，主角非地图边缘可视化剑气为一半屏幕大小，取全屏保证
				Size visiblesize = Director::getInstance()->getVisibleSize();
				visiblesize.width += 1;
				visiblesize.height += 1;//比屏幕变大一点点
				Vec2 pos = (swordwave->getPosition() + Vec2(0, swordwave->getContentSize().height / 2)) - this->getPosition();
				//剑气超出了屏幕高度
				if (abs(pos.x) > visiblesize.width || abs(pos.y) > visiblesize.height) {
					swordwave->hide();//隐藏剑气
					m_swordwave_Arr.pushBack(swordwave);//把剑气给m_swordwave_Arr容器
					m_Using_swordwave_Arr.eraseObject(swordwave);//从m_Using_swordwave中移除
					i--;//m_Using_swordwave_Arr被Pop掉一个后，原位置被容器中另一个元素取代，循环结束后i++判断当前元素
				}
				//碰到障碍物了
				else if (IsSussesfulForsetRemoteSkillPos(swordwave->getPosition() + Vec2(0, -6)) == false) {
					swordwave->hide();//隐藏剑气
					m_swordwave_Arr.pushBack(swordwave);//把剑气给m_swordwave_Arr容器
					m_Using_swordwave_Arr.eraseObject(swordwave);//从m_Using_swordwave中移除
					i--;//m_Using_swordwave_Arr被Pop掉一个后，原位置被容器中另一个元素取代，循环结束后i++判断当前元素
				}
				//没碰到障碍物
				else {
					swordwave->setSkillMove(Vec2(0, -6));
				}
				break;
			}
			case enum_remoteskill_left: {
				//超屏幕了
				//主角在地图边缘可视化剑气为整个屏幕大小，主角非地图边缘可视化剑气为一半屏幕大小，取全屏保证
				Size visiblesize = Director::getInstance()->getVisibleSize();
				visiblesize.width += 1;
				visiblesize.height += 1;//比屏幕变大一点点
				Vec2 pos = (swordwave->getPosition() + Vec2(swordwave->getContentSize().width / 2, 0)) - this->getPosition();
				//剑气超出了屏幕高度
				if (abs(pos.x) > visiblesize.width || abs(pos.y) > visiblesize.height) {
					swordwave->hide();//隐藏剑气
					m_swordwave_Arr.pushBack(swordwave);//把剑气给m_swordwave_Arr容器
					m_Using_swordwave_Arr.eraseObject(swordwave);//从m_Using_swordwave中移除
					i--;//m_Using_swordwave_Arr被Pop掉一个后，原位置被容器中另一个元素取代，循环结束后i++判断当前元素
				}
				//碰到障碍物了
				else if (IsSussesfulForsetRemoteSkillPos(swordwave->getPosition() + Vec2(-6, 0)) == false) {
					swordwave->hide();//隐藏剑气
					m_swordwave_Arr.pushBack(swordwave);//把剑气给m_swordwave_Arr容器
					m_Using_swordwave_Arr.eraseObject(swordwave);//从m_Using_swordwave中移除
					i--;//m_Using_swordwave_Arr被Pop掉一个后，原位置被容器中另一个元素取代，循环结束后i++判断当前元素
				}
				//没碰到障碍物
				else {
					swordwave->setSkillMove(Vec2(-6, 0));
				}
				break;
			}
			case enum_remoteskill_right: {
				//超屏幕了
				//主角在地图边缘可视化剑气为整个屏幕大小，主角非地图边缘可视化剑气为一半屏幕大小，取全屏保证
				Size visiblesize = Director::getInstance()->getVisibleSize();
				visiblesize.width += 1;
				visiblesize.height += 1;//比屏幕变大一点点
				Vec2 pos = (swordwave->getPosition() + Vec2(-(swordwave->getContentSize().width / 2), 0)) - this->getPosition();
				//剑气超出了屏幕高度
				if (abs(pos.x) > visiblesize.width || abs(pos.y) > visiblesize.height) {
					swordwave->hide();//隐藏剑气
					m_swordwave_Arr.pushBack(swordwave);//把剑气给m_swordwave_Arr容器
					m_Using_swordwave_Arr.eraseObject(swordwave);//从m_Using_swordwave中移除
					i--;//m_Using_swordwave_Arr被Pop掉一个后，原位置被容器中另一个元素取代，循环结束后i++判断当前元素
				}
				//碰到障碍物了
				else if (IsSussesfulForsetRemoteSkillPos(swordwave->getPosition() + Vec2(6, 0)) == false) {
					swordwave->hide();//隐藏剑气
					m_swordwave_Arr.pushBack(swordwave);//把剑气给m_swordwave_Arr容器
					m_Using_swordwave_Arr.eraseObject(swordwave);//从m_Using_swordwave中移除
					i--;//m_Using_swordwave_Arr被Pop掉一个后，原位置被容器中另一个元素取代，循环结束后i++判断当前元素
				}
				//没碰到障碍物
				else {
					swordwave->setSkillMove(Vec2(6, 0));
				}
				break;
			}
			default:
				break;
			}

			//此时必须判断m_Using_swordwave是否为空，为空的话就不能继续遍历了，因为已经可能从m_Using_swordwave中erase到m_Using_swordwave为空了，这是个动态变化的过程
			if (m_Using_swordwave_Arr.size() == 0) {
				break;
			}
		}
	}


	//////////////////////////////////////////主角被攻击
	if (attackedqueue.size() > 0) {
		//主角被攻击过了僵直时间,才pop掉队列
		if (timecounter_attacked->getCurTime() > 0.11f) {
			/*防止主角击退效果被打歪*/
			this->getSprite()->setAnchorPoint(Vec2(0.5, 0));
			this->getSprite()->setPosition(Vec2(this->getContentSize().width / 2,
				0));
			this->getSprite()->setRotation(0);
			/*防止主角击退效果被打歪*/
			/*防止主角染色变红没恢复*/
			/*CCTintTo* action = CCTintTo::create(0.01f, m_playerColor);
			this->getSprite()->runAction(action);*/
			this->getSprite()->setColor(m_playerColor);

			attackedqueue.pop();
			//设置计时为0，当被攻击信息队列增加消息size再次大于0时，不会马上被pop掉，而是重新调用了timecouter_attacked->start才判断是否pop掉
			timecounter_attacked->setstartTimeZeroAndcloseSchedule();
		}

		//被攻击僵直间隔
		if (attackedqueue.size() > 0 && timecounter_attacked->getCurTime() == 0.0f)
		{
			int attack = attackedqueue.front();

			/*主角被击中声音*/
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/playerhited.wav");

			switch (attack)
			{
			case enum_playerattackedfromleft: {
				auto backMove = MoveBy::create(0.05f, Vec2(20, 0));
				auto forwardMove = MoveBy::create(0.05f, Vec2(-20, 0));
				auto backRotate = RotateBy::create(0.05f, 5, 0);
				auto forwardRotate = RotateBy::create(0.05f, -5, 0);
				CCTintTo* action1 = CCTintTo::create(0.05f, 255, 0, 0);
				CCTintTo* action2 = CCTintTo::create(0.05f, m_playerColor);
				auto backActions = Spawn::create(backMove, backRotate, action1, NULL);
				auto forwardActions = Spawn::create(forwardMove, forwardRotate, action2, NULL);
				auto actions = Sequence::create(backActions, forwardActions, NULL);
				this->stopAllActions();
				this->getSprite()->stopAllActions();
				this->getSprite()->runAction(actions);
				timecounter_attacked->start();
				//如果后面没障碍物，就被击退
				this->setPlayerPosition(this->getPosition() + Vec2(6, 0));
				//扣主角hp
				m_hp--;

				//如果是run状态下被攻击了，把run转化为walk
				switch (PlayerState)
				{
				case enum_doubleup: {
					if (vec.size() == 1)
						vec[0] = enum_up;
					break;
				}
				case enum_doubledown: {
					if (vec.size() == 1)
						vec[0] = enum_down;
					break;
				}
				case enum_doubleleft: {
					if (vec.size() == 1)
						vec[0] = enum_left;
					break;
				}
				case enum_doubleright: {
					if (vec.size() == 1)
						vec[0] = enum_right;
					break;
				}
				default:
					break;
				}

				//被攻击状态下无法释放主角技能
				playerIsattacked = true;

				//设置主角的状态
				PlayerState = enum_playerattackedfromleft;

				break;
			}
			case enum_playerattackedfromright: {
				auto backMove = MoveBy::create(0.05f, Vec2(-20, 0));
				auto forwardMove = MoveBy::create(0.05f, Vec2(20, 0));
				auto backRotate = RotateBy::create(0.05f, -5, 0);
				auto forwardRotate = RotateBy::create(0.05f, 5, 0);
				CCTintTo* action1 = CCTintTo::create(0.05f, 255, 0, 0);
				CCTintTo* action2 = CCTintTo::create(0.05f, m_playerColor);
				auto backActions = Spawn::create(backMove, backRotate, action1, NULL);
				auto forwardActions = Spawn::create(forwardMove, forwardRotate, action2, NULL);
				auto actions = Sequence::create(backActions, forwardActions, NULL);
				this->stopAllActions();
				this->getSprite()->stopAllActions();
				this->getSprite()->runAction(actions);
				timecounter_attacked->start();
				//如果后面没障碍物，就被击退
				this->setPlayerPosition(this->getPosition() + Vec2(-6, 0));
				//扣主角hp
				m_hp--;

				//如果是run状态下被攻击了，把run转化为walk
				switch (PlayerState)
				{
				case enum_doubleup: {
					if (vec.size() == 1)
						vec[0] = enum_up;
					break;
				}
				case enum_doubledown: {
					if (vec.size() == 1)
						vec[0] = enum_down;
					break;
				}
				case enum_doubleleft: {
					if (vec.size() == 1)
						vec[0] = enum_left;
					break;
				}
				case enum_doubleright: {
					if (vec.size() == 1)
						vec[0] = enum_right;
					break;
				}
				default:
					break;
				}

				//被攻击状态下无法释放主角技能
				playerIsattacked = true;

				//设置主角的状态
				PlayerState = enum_playerattackedfromright;

				break;
			}
			case enum_playerattackedfromup: {
				auto backMove = MoveBy::create(0.05f, Vec2(0, -20));
				auto forwardMove = MoveBy::create(0.05f, Vec2(0, 20));
				auto backRotate = RotateBy::create(0.05f, 0, -5);
				auto forwardRotate = RotateBy::create(0.05f, 0, 5);
				CCTintTo* action1 = CCTintTo::create(0.05f, 255, 0, 0);
				CCTintTo* action2 = CCTintTo::create(0.05f, m_playerColor);
				auto backActions = Spawn::create(backMove, backRotate, action1, NULL);
				auto forwardActions = Spawn::create(forwardMove, forwardRotate, action2, NULL);
				auto actions = Sequence::create(backActions, forwardActions, NULL);
				this->stopAllActions();
				this->getSprite()->stopAllActions();
				this->getSprite()->runAction(actions);
				timecounter_attacked->start();
				//如果后面没障碍物，就被击退
				this->setPlayerPosition(this->getPosition() + Vec2(0, -6));
				//扣主角hp
				m_hp--;

				//如果是run状态下被攻击了，把run转化为walk
				switch (PlayerState)
				{
				case enum_doubleup: {
					if (vec.size() == 1)
						vec[0] = enum_up;
					break;
				}
				case enum_doubledown: {
					if (vec.size() == 1)
						vec[0] = enum_down;
					break;
				}
				case enum_doubleleft: {
					if (vec.size() == 1)
						vec[0] = enum_left;
					break;
				}
				case enum_doubleright: {
					if (vec.size() == 1)
						vec[0] = enum_right;
					break;
				}
				default:
					break;
				}

				//被攻击状态下无法释放主角技能
				playerIsattacked = true;

				//设置主角的状态
				PlayerState = enum_playerattackedfromup;
				break;
			}
			case enum_playerattackedfromdown: {
				auto backMove = MoveBy::create(0.05f, Vec2(0, 20));
				auto forwardMove = MoveBy::create(0.05f, Vec2(0, -20));
				auto backRotate = RotateBy::create(0.05f, 0, 5);
				auto forwardRotate = RotateBy::create(0.05f, 0, -5);
				CCTintTo* action1 = CCTintTo::create(0.05f, 255, 0, 0);
				CCTintTo* action2 = CCTintTo::create(0.05f, m_playerColor);
				auto backActions = Spawn::create(backMove, backRotate, action1, NULL);
				auto forwardActions = Spawn::create(forwardMove, forwardRotate, action2, NULL);
				auto actions = Sequence::create(backActions, forwardActions, NULL);
				this->stopAllActions();
				this->getSprite()->stopAllActions();
				this->getSprite()->runAction(actions);
				timecounter_attacked->start();
				//如果后面没障碍物，就被击退
				this->setPlayerPosition(this->getPosition() + Vec2(0, 6));

				//如果是run状态下被攻击了，把run转化为walk
				switch (PlayerState)
				{
				case enum_doubleup: {
					if (vec.size() == 1)
						vec[0] = enum_up;
					break;
				}
				case enum_doubledown: {
					if (vec.size() == 1)
						vec[0] = enum_down;
					break;
				}
				case enum_doubleleft: {
					if (vec.size() == 1)
						vec[0] = enum_left;
					break;
				}
				case enum_doubleright: {
					if (vec.size() == 1)
						vec[0] = enum_right;
					break;
				}
				default:
					break;
				}

				//被攻击状态下无法释放主角技能
				playerIsattacked = true;

				//设置主角的状态
				PlayerState = enum_playerattackedfromup;

				break;
			}
			}
		}

		return;//队列为空时，即不再被攻击了,就可以执行下面的动作了
	}


	playerIsattacked = false;//标志主角不为被击状态

	/*防止主角击退效果被打歪*/
	this->getSprite()->setAnchorPoint(Vec2(0.5, 0));
	this->getSprite()->setPosition(Vec2(this->getContentSize().width / 2,
		0));
	this->getSprite()->setRotation(0);
	/*防止主角击退效果被打歪*/
	/*防止主角染色变红没恢复*/
	this->getSprite()->setColor(m_playerColor);
	/*防止主角染色变红没恢复*/

	/////////////////////攻击优先于走或跑
	if (vecskill.size() == 1) {
		switch (PlayerDir)
		{
		case em_up: {
			switch (vecskill[0])
			{
			case enum_baseattack: {
				switch ((int)(CCRANDOM_0_1() * 10) % 2)
				{
				case 0: {
					Animation* animation = AnimationUtil::createWithSingleFrameName("ubaseattack", 0.1f, 1);
					Animate* animate = Animate::create(animation);
					if (PlayerState != enum_baseattack) {
						this->getPlayerSprite()->stopAllActions();
						CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
						this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));

					}

					PlayerState = enum_baseattack;
					return;
				}
				case 1: {
					Animation* animation = AnimationUtil::createWithSingleFrameName("fubaseattack", 0.1f, 1);
					Animate* animate = Animate::create(animation);
					if (PlayerState != enum_baseattack) {
						this->getPlayerSprite()->stopAllActions();
						CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
						this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));
					}

					PlayerState = enum_baseattack;
					return;
				}
				}
			}
			case enum_basepoke: {
				Animation* animation = AnimationUtil::createWithSingleFrameName("ubasepoke", 0.1f, 1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_basepoke) {
					this->getPlayerSprite()->stopAllActions();
					CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
					this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));
				}

				PlayerState = enum_basepoke;
				return;
			}
			case enum_swordwave: {
				Animation* animation = AnimationUtil::createWithSingleFrameName("uswordwave", 0.1f, 1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_swordwave) {
					this->getPlayerSprite()->stopAllActions();
					CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
					this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));

					if (m_swordwave_Arr.size() > 0) {
						RemoteSkill* swordwave = m_swordwave_Arr.back();
						swordwave->setSkillDir(enum_remoteskill_up);
						swordwave->show();
						swordwave->setPosition(this->getPosition() + Vec2(0, this->getContentSize().height));//把剑气根据玩家位置初始化
						m_Using_swordwave_Arr.pushBack(swordwave);//把剑气向量容器最后元素放到代表当前释放的剑气向量容器内
						m_swordwave_Arr.popBack();//pop掉
					}

				}

				PlayerState = enum_swordwave;
				return;
			}
			case enum_laserskill: {
				if (PlayerState != enum_laserskill) {
					Animation* animation = AnimationUtil::createWithSingleFrameName("uswordwave", 0.1f, 1);//放雷电技能动作
					Animate* animate = Animate::create(animation);
					this->getPlayerSprite()->stopAllActions();
					CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
					this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));
				}

				PlayerState = enum_laserskill;
				return;
			}
			default:
				break;
			}
			case enum_fireskill: {
				if (PlayerState != enum_fireskill) {
					Animation* animation = AnimationUtil::createWithSingleFrameName("uswordwave", 0.1f, 1);//放火球技能动作
					Animate* animate = Animate::create(animation);
					this->getPlayerSprite()->stopAllActions();
					CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
					this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));
				}

				PlayerState = enum_fireskill;
				return;
			}
			break;
		}
		case em_down: {
			switch (vecskill[0])
			{
			case enum_baseattack: {
				switch ((int)(CCRANDOM_0_1() * 10) % 2)
				{
				case 0: {
					Animation* animation = AnimationUtil::createWithSingleFrameName("dbaseattack", 0.1f, 1);
					Animate* animate = Animate::create(animation);
					if (PlayerState != enum_baseattack) {
						this->getPlayerSprite()->stopAllActions();
						CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
						this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));
					}

					PlayerState = enum_baseattack;
					return;
				}
				case 1: {
					Animation* animation = AnimationUtil::createWithSingleFrameName("fdbaseattack", 0.1f, 1);
					Animate* animate = Animate::create(animation);
					if (PlayerState != enum_baseattack) {
						this->getPlayerSprite()->stopAllActions();
						CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
						this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));
					}

					PlayerState = enum_baseattack;
					return;
				}
				}
			}
			case enum_basepoke: {
				Animation* animation = AnimationUtil::createWithSingleFrameName("dbasepoke", 0.1f, 1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_basepoke) {
					this->getPlayerSprite()->stopAllActions();
					CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
					this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));
				}

				PlayerState = enum_basepoke;
				return;
			}
			case enum_swordwave: {
				Animation* animation = AnimationUtil::createWithSingleFrameName("dswordwave", 0.1f, 1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_swordwave) {
					this->getPlayerSprite()->stopAllActions();
					CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
					this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));

					if (m_swordwave_Arr.size() > 0) {
						RemoteSkill* swordwave = m_swordwave_Arr.back();
						swordwave->setSkillDir(enum_remoteskill_down);
						swordwave->show();
						swordwave->setPosition(this->getPosition() + Vec2(0, 0));//把剑气根据玩家位置初始化
						m_Using_swordwave_Arr.pushBack(swordwave);//把剑气向量容器最后元素放到代表当前释放的剑气向量容器内
						m_swordwave_Arr.popBack();//pop掉
					}
				}

				PlayerState = enum_swordwave;
				return;
			}
			case enum_laserskill: {
				if (PlayerState != enum_laserskill) {
					Animation* animation = AnimationUtil::createWithSingleFrameName("dswordwave", 0.1f, 1);//放雷电技能动作
					Animate* animate = Animate::create(animation);
					this->getPlayerSprite()->stopAllActions();
					CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
					this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));
				}

				PlayerState = enum_laserskill;
				return;
			}
			case enum_fireskill: {
				if (PlayerState != enum_fireskill) {
					Animation* animation = AnimationUtil::createWithSingleFrameName("dswordwave", 0.1f, 1);//放火球技能动作
					Animate* animate = Animate::create(animation);
					this->getPlayerSprite()->stopAllActions();
					CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
					this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));
				}

				PlayerState = enum_fireskill;
				return;
			}
			default:
				break;
			}

			break;
		}
		case em_left: {
			switch (vecskill[0])
			{
			case enum_baseattack: {
				switch ((int)(CCRANDOM_0_1() * 10) % 2)
				{
				case 0: {
					Animation* animation = AnimationUtil::createWithSingleFrameName("hbaseattack", 0.1f, 1);
					Animate* animate = Animate::create(animation);
					if (PlayerState != enum_baseattack) {
						this->getPlayerSprite()->stopAllActions();
						this->getPlayerSprite()->setScaleX(m_player_magnification);
						CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
						this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));
					}

					PlayerState = enum_baseattack;
					return;
				}
				case 1: {
					Animation* animation = AnimationUtil::createWithSingleFrameName("fhbaseattack", 0.1f, 1);
					Animate* animate = Animate::create(animation);
					if (PlayerState != enum_baseattack) {
						this->getPlayerSprite()->stopAllActions();
						this->getPlayerSprite()->setScaleX(m_player_magnification);
						CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
						this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));
					}

					PlayerState = enum_baseattack;
					return;
				}
				}
			}
			case enum_basepoke: {
				Animation* animation = AnimationUtil::createWithSingleFrameName("hbasepoke", 0.1f, 1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_basepoke) {
					this->getPlayerSprite()->stopAllActions();
					this->getPlayerSprite()->setScaleX(m_player_magnification);
					CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
					this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));
				}

				PlayerState = enum_basepoke;
				return;
			}
			case enum_swordwave: {
				Animation* animation = AnimationUtil::createWithSingleFrameName("hswordwave", 0.1f, 1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_swordwave) {
					this->getPlayerSprite()->stopAllActions();
					this->getPlayerSprite()->setScaleX(m_player_magnification);
					CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
					this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));

					if (m_swordwave_Arr.size() > 0) {
						RemoteSkill* swordwave = m_swordwave_Arr.back();
						swordwave->setSkillDir(enum_remoteskill_left);
						swordwave->show();
						swordwave->setPosition(this->getPosition() + Vec2(-this->getContentSize().width / 2, this->getContentSize().height / 2));//把剑气根据玩家位置初始化
						m_Using_swordwave_Arr.pushBack(swordwave);//把剑气向量容器最后元素放到代表当前释放的剑气向量容器内
						m_swordwave_Arr.popBack();//pop掉
					}
				}

				PlayerState = enum_swordwave;
				return;
			}
			case enum_laserskill: {
				if (PlayerState != enum_laserskill) {
					Animation* animation = AnimationUtil::createWithSingleFrameName("hswordwave", 0.1f, 1);//放雷电技能动作
					Animate* animate = Animate::create(animation);
					this->getPlayerSprite()->stopAllActions();
					CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
					this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));
				}

				PlayerState = enum_laserskill;
				return;
			}
			case enum_fireskill: {
				if (PlayerState != enum_fireskill) {
					Animation* animation = AnimationUtil::createWithSingleFrameName("hswordwave", 0.1f, 1);//放火球技能动作
					Animate* animate = Animate::create(animation);
					this->getPlayerSprite()->stopAllActions();
					CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
					this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));
				}

				PlayerState = enum_fireskill;
				return;
			}
			default:
				break;
			}

			break;
		}
		case em_right: {
			switch (vecskill[0])
			{
			case enum_baseattack: {
				switch ((int)(CCRANDOM_0_1() * 10) % 2)
				{
				case 0: {
					Animation* animation = AnimationUtil::createWithSingleFrameName("hbaseattack", 0.1f, 1);
					Animate* animate = Animate::create(animation);
					if (PlayerState != enum_baseattack) {
						this->getPlayerSprite()->stopAllActions();
						this->getPlayerSprite()->setScaleX(-m_player_magnification);
						CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
						this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));
					}

					PlayerState = enum_baseattack;
					return;
				}
				case 1: {
					Animation* animation = AnimationUtil::createWithSingleFrameName("fhbaseattack", 0.1f, 1);
					Animate* animate = Animate::create(animation);
					if (PlayerState != enum_baseattack) {
						this->getPlayerSprite()->stopAllActions();
						this->getPlayerSprite()->setScaleX(-m_player_magnification);
						CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
						this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));
					}

					PlayerState = enum_baseattack;
					return;
				}
				}
			}
			case enum_basepoke: {
				Animation* animation = AnimationUtil::createWithSingleFrameName("hbasepoke", 0.1f, 1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_basepoke) {
					this->getPlayerSprite()->stopAllActions();
					this->getPlayerSprite()->setScaleX(-m_player_magnification);
					CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
					this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));
				}

				PlayerState = enum_basepoke;
				return;
			}
			case enum_swordwave: {
				Animation* animation = AnimationUtil::createWithSingleFrameName("hswordwave", 0.1f, 1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_swordwave) {
					this->getPlayerSprite()->stopAllActions();
					this->getPlayerSprite()->setScaleX(-m_player_magnification);
					CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
					this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));

					if (m_swordwave_Arr.size() > 0) {
						RemoteSkill* swordwave = m_swordwave_Arr.back();
						swordwave->setSkillDir(enum_remoteskill_right);
						swordwave->show();
						swordwave->setPosition(this->getPosition() + Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));//把剑气根据玩家位置初始化
						m_Using_swordwave_Arr.pushBack(swordwave);//把剑气向量容器最后元素放到代表当前释放的剑气向量容器内
						m_swordwave_Arr.popBack();//pop掉
					}
				}

				PlayerState = enum_swordwave;
				return;
			}
			case enum_laserskill: {
				if (PlayerState != enum_laserskill) {
					Animation* animation = AnimationUtil::createWithSingleFrameName("hswordwave", 0.1f, 1);//放雷电技能动作
					Animate* animate = Animate::create(animation);
					this->getPlayerSprite()->stopAllActions();
					CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
					this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));
				}

				PlayerState = enum_laserskill;
				return;
			}
			case enum_fireskill: {
				if (PlayerState != enum_fireskill) {
					Animation* animation = AnimationUtil::createWithSingleFrameName("hswordwave", 0.1f, 1);//放火球技能动作
					Animate* animate = Animate::create(animation);
					this->getPlayerSprite()->stopAllActions();
					CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
					this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));
				}

				PlayerState = enum_fireskill;
				return;
			}
			default:
				break;
			}

			break;
		}
		default:
			break;
		}
	}
	////////////////////走或跑
	else if (vec.size() > 0) {
		if (vec.size() > 1)
		{
			/*两个方向一起的快走,,由于快走时在静止时才有的，所以快走永远是vec[0],*/
			if (vec[0] == enum_doubleup && vec.back() == enum_left) {
				// (上左、左上)
				//获取地图块的编号，向左走左侧身边界所在位置的地图瓦片号
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(-this->getContentSize().width / 2 - 2, 0));
				//向左走由于要超前判断一格，且地图从右到左滑过瓦片的边就会减１，所以需要vId.x +１；
				vId.x = vId.x + 1;
				Vec2 vIdleft;
				vIdleft.x = vId.x - 1; vIdleft.y = vId.y;

				//向上走以脚所在位置的地图瓦片
				vId = this->tiledCoordForPosition(this->getPosition() + Vec2(0,2));
				//向上走由于要超前判断一个瓦片，且地图从下到上滑过瓦片的边就会减一，所以需要vId.y + 1;
				vId.y = vId.y + 1;
				Vec2 vIdup;
				vIdup.x = vId.x; vIdup.y = vId.y - 1;

				Animation* animation = AnimationUtil::createWithSingleFrameName("urun", 0.15f, -1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_doubleup) {
					this->getPlayerSprite()->stopAllActions();
					this->getPlayerSprite()->runAction(animate);
				}
				PlayerState = enum_doubleup;
				TMXLayer* barrier = m_map->getLayer("barrier");
				bool IsTwoDir = false;
				//如果左边或上边有障碍物，防止对角线穿墙跑过去
				int xMax = m_map->getMapSize().width - 1;
				int yMax = m_map->getMapSize().height - 1;
				if ((vIdleft.x >= 0 && vIdleft.x <= xMax && vIdup.x >= 0 && vIdup.x <= xMax) &&
					(vIdleft.y >= 0 && vIdleft.y <= yMax && vIdup.y >= 0 && vIdup.y <= yMax)) {
					if (barrier->getTileGIDAt(vIdleft) == 0 && barrier->getTileGIDAt(vIdup) == 0 ||
						(IsNot_CollidableTile(vIdleft) && IsNot_CollidableTile(vIdup)))
					{
						IsTwoDir = this->setPlayerPosition(this->getPosition() + Vec2(-4, 6));	
					}
				}
				//地图下边界的处理 vIdleft超范围了,并且vIdup不超但不是障碍物
				else if (vIdleft.y > yMax && IsNot_CollidableTile(vIdup))
				{
					IsTwoDir = this->setPlayerPosition(this->getPosition() + Vec2(-4, 6));
				}
				
				//如果不能两个方向的，那么尝试靠墙(障碍物)单方向
				if (IsTwoDir == false)
				{
					nearBarrierMove = true;
					//尝试上走,即障碍物在左方
					if (!this->setPlayerPosition(this->getPosition() + Vec2(0, 6)))
					{
						//尝试左走，障碍物在上方
						this->setPlayerPosition(this->getPosition() + Vec2(-4, 0));
					}			
				}
				
				PlayerDir = em_up;
				return;
			}
			else if (vec[0] == enum_doubleup && vec.back() == enum_right) {
				//(右上、上右)快跑
				//获取地图块的编号
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(this->getContentSize().width / 2 + 2, 0));
				//从左向右划过瓦片边时会加1，由于要提前判断一格，所以vId.x - 1;
				vId.x = vId.x - 1;
				Vec2 vIdright;
				vIdright.x = vId.x + 1; vIdright.y = vId.y;

				vId = this->tiledCoordForPosition(this->getPosition() + Vec2(0,2));
				//从下向上划过瓦片时会减一，由于要提前判断一格，所以vId.y + 1;
				vId.y = vId.y + 1;
				Vec2 vIdup;
				vIdup.x = vId.x; vIdup.y = vId.y - 1;

				Animation* animation = AnimationUtil::createWithSingleFrameName("urun", 0.15f, -1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_doubleup) {
					this->getPlayerSprite()->stopAllActions();
					this->getPlayerSprite()->runAction(animate);
				}
				PlayerState = enum_doubleup;
				TMXLayer* barrier = m_map->getLayer("barrier");
				bool IsTwoDir = false;
				//如果左边或上边有障碍物，防止对角线穿墙跑过去
				int xMax = m_map->getMapSize().width - 1;
				int yMax = m_map->getMapSize().height - 1;
				if ((vIdright.x >= 0 && vIdright.x <= xMax && vIdup.x >= 0 && vIdup.x <= xMax) &&
					(vIdright.y >= 0 && vIdright.y <= yMax && vIdup.y >= 0 && vIdup.y <= yMax)) {
					if (barrier->getTileGIDAt(vIdright) == 0 && barrier->getTileGIDAt(vIdup) == 0 ||
						(IsNot_CollidableTile(vIdright) && IsNot_CollidableTile(vIdup))) {
						IsTwoDir = this->setPlayerPosition(this->getPosition() + Vec2(4, 6));
					}
				}
				//地图最下边界的处理,vIdright超范围了,并且vIdup没超范围但是不是障碍物
				else if (vIdright.y > yMax && IsNot_CollidableTile(vIdup))
				{
					IsTwoDir = this->setPlayerPosition(this->getPosition() + Vec2(4, 6));
				}
				//处理地图右边界超范围问题，左边界的tiledCoordForPosition不是超过地图边界的，不需要处理
				else if (vIdright.x > xMax && IsNot_CollidableTile(vIdup))
				{
					IsTwoDir = this->setPlayerPosition(this->getPosition() + Vec2(4, 6));
				}

				if (IsTwoDir == false)
				{
					nearBarrierMove = true;
					if (!this->setPlayerPosition(this->getPosition() + Vec2(0, 6)))
					{
						this->setPlayerPosition(this->getPosition() + Vec2(4, 0));
					}
				}

				PlayerDir = em_up;
				return;
			}
			else if (vec[0] == enum_doubledown && vec.back() == enum_left) {
				//(左下、下左）
				//获取地图块的编号
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(-this->getContentSize().width / 2 - 2, 0));
				//由于从右向左划过瓦片会减一，由于提前判断一格，所以vId.x + 1;
				vId.x = vId.x + 1;
				Vec2 vIdleft;
				vIdleft.x = vId.x - 1; vIdleft.y = vId.y;

				vId = this->tiledCoordForPosition(this->getPosition() + Vec2(0,-2));
				//由于从上往下划过瓦片会加1，由于提前判断一格，所以vId.y - 1;
				vId.y = vId.y - 1;
				Vec2 vIddown;
				vIddown.x = vId.x; 	vIddown.y = vId.y + 1;

				Animation* animation = AnimationUtil::createWithSingleFrameName("drun", 0.18f, -1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_doubledown) {
					this->getPlayerSprite()->stopAllActions();
					this->getPlayerSprite()->runAction(animate);
				}
				PlayerState = enum_doubledown;
				TMXLayer* barrier = m_map->getLayer("barrier");
				bool IsTwoDir = false;
				//如果左边或下边有障碍物，防止对角线穿墙跑过去
				int xMax = m_map->getMapSize().width - 1;
				int yMax = m_map->getMapSize().height - 1;
				if ((vIdleft.x >= 0 && vIdleft.x <= xMax && vIddown.x >= 0 && vIddown.x <= xMax) &&
					(vIdleft.y >= 0 && vIdleft.y <= yMax && vIddown.y >= 0 && vIddown.y <= yMax)) {
					if (barrier->getTileGIDAt(vIddown) == 0 && barrier->getTileGIDAt(vIdleft) == 0 ||
						(IsNot_CollidableTile(vIddown) && IsNot_CollidableTile(vIdleft))) {
						IsTwoDir = this->setPlayerPosition(this->getPosition() + Vec2(-4, -6));
					}
				}

				if (IsTwoDir == false)
				{
					nearBarrierMove = true;
					if (!this->setPlayerPosition(this->getPosition() + Vec2(0, -6)))
					{
						this->setPlayerPosition(this->getPosition() + Vec2(-4, 0));
					}
				}

				PlayerDir = em_down;
				return;
			}
			else if (vec[0] == enum_doubledown && vec.back() == enum_right) {
				//(右下、下右)
				//获取地图块的编号
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(this->getContentSize().width / 2 + 2, 0));
				//从左到右划过地图时会加1，由于提前判断一格，所以vId.x - 1;
				vId.x = vId.x - 1;
				Vec2 vIdright;
				vIdright.x = vId.x + 1; vIdright.y = vId.y;

				vId = this->tiledCoordForPosition(this->getPosition() + Vec2(0,-2));
				//从上到下划过瓦片时会加1，由于提前判断一格，所以vId.y - 1;
				vId.y = vId.y - 1;
				Vec2 vIddown;
				vIddown.x = vId.x; vIddown.y = vId.y + 1;

				Animation* animation = AnimationUtil::createWithSingleFrameName("drun", 0.18f, -1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_doubledown) {
					this->getPlayerSprite()->stopAllActions();
					this->getPlayerSprite()->runAction(animate);
				}
				PlayerState = enum_doubledown;
				TMXLayer* barrier = m_map->getLayer("barrier");
				bool IsTwoDir = false;
				//如果左边或下边有障碍物，防止对角线穿墙跑过去
				int xMax = m_map->getMapSize().width - 1;
				int yMax = m_map->getMapSize().height - 1;
				if ((vIdright.x >= 0 && vIdright.x <= xMax && vIddown.x >= 0 && vIddown.x <= xMax) &&
					(vIdright.y >= 0 && vIdright.y <= yMax && vIddown.y >= 0 && vIddown.y <= yMax)) {
					if (barrier->getTileGIDAt(vIdright) == 0 && barrier->getTileGIDAt(vIddown) == 0 ||
						(IsNot_CollidableTile(vIdright) && IsNot_CollidableTile(vIddown))) {
						IsTwoDir = this->setPlayerPosition(this->getPosition() + Vec2(4, -6));
					}
				}
				//处理地图右边界超范围问题，左边界的tiledCoordForPosition不是超过地图边界的，不需要处理
				else if (vIdright.x > xMax && IsNot_CollidableTile(vIddown))
				{
					IsTwoDir = this->setPlayerPosition(this->getPosition() + Vec2(4, -6));
				}

				if (IsTwoDir == false)
				{
					nearBarrierMove = true;
					if (!this->setPlayerPosition(this->getPosition() + Vec2(0, -6)))
					{
						this->setPlayerPosition(this->getPosition() + Vec2(4, 0));
					}
				}

				PlayerDir = em_down;
				return;
			}
			else if (vec[0] == enum_doubleleft && vec.back() == enum_up) {
				// (上左、左上)
				//获取地图块的编号，向左走左侧身边界所在位置的地图瓦片号
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(-this->getContentSize().width / 2 - 2, 0));
				//向左走由于要超前判断一格，且地图从右到左滑过瓦片的边就会减１，所以需要vId.x +１；
				vId.x = vId.x + 1;
				Vec2 vIdleft;
				vIdleft.x = vId.x - 1; vIdleft.y = vId.y;

				//向上走以脚所在位置的地图瓦片
				vId = this->tiledCoordForPosition(this->getPosition() + Vec2(0,2));
				//向上走由于要超前判断一个瓦片，且地图从下到上滑过瓦片的边就会减一，所以需要vId.y + 1;
				vId.y = vId.y + 1;
				Vec2 vIdup;
				vIdup.x = vId.x; vIdup.y = vId.y - 1;

				Animation* animation = AnimationUtil::createWithSingleFrameName("hrun", 0.2f, -1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_doubleleft) {
					this->getPlayerSprite()->stopAllActions();
					this->getPlayerSprite()->setScaleX(m_player_magnification);
					this->getPlayerSprite()->runAction(animate);
				}
				PlayerState = enum_doubleleft;
				TMXLayer* barrier = m_map->getLayer("barrier");
				bool IsTwoDir = false;
				int xMax = m_map->getMapSize().width - 1;
				int yMax = m_map->getMapSize().height - 1;
				if ((vIdleft.x >= 0 && vIdleft.x <= xMax && vIdup.x >= 0 && vIdup.x <= xMax) &&
					(vIdleft.y >= 0 && vIdleft.y <= yMax && vIdup.y >= 0 && vIdup.y <= yMax)) {
					if (barrier->getTileGIDAt(vIdleft) == 0 && barrier->getTileGIDAt(vIdup) == 0 ||
						(IsNot_CollidableTile(vIdleft) && IsNot_CollidableTile(vIdup))) {
						IsTwoDir = this->setPlayerPosition(this->getPosition() + Vec2(-6, 4));
					}
				}
				//处理地图最下边界，vIdleft超过范围,并且vIdup不超但是不是障碍物
				else if (vIdleft.y > yMax && IsNot_CollidableTile(vIdup))
				{
					IsTwoDir = this->setPlayerPosition(this->getPosition() + Vec2(-6, 4));
				}

				if (IsTwoDir == false)
				{
					nearBarrierMove = true;
					if (!this->setPlayerPosition(this->getPosition() + Vec2(-6, 0)))
					{
						this->setPlayerPosition(this->getPosition() + Vec2(0, 4));
					}
				}

				PlayerDir = em_left;
				return;
			}
			else if (vec[0] == enum_doubleleft && vec.back() == enum_down) {
				//(左下、下左）
				//获取地图块的编号
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(-this->getContentSize().width / 2 - 2, 0));
				//由于从右向左划过瓦片会减一，由于提前判断一格，所以vId.x + 1;
				vId.x = vId.x + 1;
				Vec2 vIdleft;
				vIdleft.x = vId.x - 1; vIdleft.y = vId.y;

				vId = this->tiledCoordForPosition(this->getPosition() + Vec2(0,-2));
				//由于从上往下划过瓦片会加1，由于提前判断一格，所以vId.y - 1;
				vId.y = vId.y - 1;
				Vec2 vIddown;
				vIddown.x = vId.x; 	vIddown.y = vId.y + 1;

				Animation* animation = AnimationUtil::createWithSingleFrameName("hrun", 0.2f, -1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_doubleleft) {
					this->getPlayerSprite()->stopAllActions();
					this->getPlayerSprite()->setScaleX(m_player_magnification);
					this->getPlayerSprite()->runAction(animate);
				}
				PlayerState = enum_doubleleft;
				TMXLayer* barrier = m_map->getLayer("barrier");
				bool IsTwoDir = false;
				int xMax = m_map->getMapSize().width - 1;
				int yMax = m_map->getMapSize().height - 1;
				if ((vIdleft.x >= 0 && vIdleft.x <= xMax && vIddown.x >= 0 && vIddown.x <= xMax) &&
					(vIdleft.y >= 0 && vIdleft.y <= yMax && vIddown.y >= 0 && vIddown.y <= yMax)) {
					if (barrier->getTileGIDAt(vIdleft) == 0 && barrier->getTileGIDAt(vIddown) == 0 ||
						(IsNot_CollidableTile(vIdleft) && IsNot_CollidableTile(vIddown))) {
						IsTwoDir = this->setPlayerPosition(this->getPosition() + Vec2(-6, -4));
					}
				}

				if (IsTwoDir == false)
				{
					nearBarrierMove = true;
					if (!this->setPlayerPosition(this->getPosition() + Vec2(-6, 0)))
					{
						this->setPlayerPosition(this->getPosition() + Vec2(0, -4));
					}
				}

				PlayerDir = em_left;
				return;
			}
			else if (vec[0] == enum_doubleright && vec.back() == enum_up) {
				//(右上、上右)快跑
				//获取地图块的编号
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(this->getContentSize().width / 2 + 2, 0));
				//从左向右划过瓦片边时会加1，由于要提前判断一格，所以vId.x - 1;
				vId.x = vId.x - 1;
				Vec2 vIdright;
				vIdright.x = vId.x + 1; vIdright.y = vId.y;

				vId = this->tiledCoordForPosition(this->getPosition() + Vec2(0,2));
				//从下向上划过瓦片时会减一，由于要提前判断一格，所以vId.y + 1;
				vId.y = vId.y + 1;
				Vec2 vIdup;
				vIdup.x = vId.x; vIdup.y = vId.y - 1;

				Animation* animation = AnimationUtil::createWithSingleFrameName("hrun", 0.2f, -1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_doubleright) {
					this->getPlayerSprite()->stopAllActions();
					this->getPlayerSprite()->setScaleX(-m_player_magnification);
					this->getPlayerSprite()->runAction(animate);
				}
				PlayerState = enum_doubleright;
				TMXLayer* barrier = m_map->getLayer("barrier");
				bool IsTwoDir = false;
				int xMax = m_map->getMapSize().width - 1;
				int yMax = m_map->getMapSize().height - 1;
				if ((vIdright.x >= 0 && vIdright.x <= xMax && vIdup.x >= 0 && vIdup.x <= xMax) &&
					(vIdright.y >= 0 && vIdright.y <= yMax && vIdup.y >= 0 && vIdup.y <= yMax)) {
					if (barrier->getTileGIDAt(vIdright) == 0 && barrier->getTileGIDAt(vIdup) == 0 ||
						(IsNot_CollidableTile(vIdright) && IsNot_CollidableTile(vIdup))) {
						IsTwoDir = this->setPlayerPosition(this->getPosition() + Vec2(6, 4));
					}
				}
				//地图最下边界,vIdright超出范围,并且vIdup不超范围但不是障碍物
				else if (vIdright.y > yMax && IsNot_CollidableTile(vIdup))
				{
					IsTwoDir = this->setPlayerPosition(this->getPosition() + Vec2(6, 4));
				}
				//处理地图右边界超范围问题，左边界的tiledCoordForPosition不是超过地图边界的，不需要处理
				else if (vIdright.x > xMax && IsNot_CollidableTile(vIdup))
				{
					IsTwoDir = this->setPlayerPosition(this->getPosition() + Vec2(6, 4));
				}

				if (IsTwoDir == false)
				{
					nearBarrierMove = true;
					if (!this->setPlayerPosition(this->getPosition() + Vec2(6, 0)))
					{
						this->setPlayerPosition(this->getPosition() + Vec2(0, 4));
					}
				}

				PlayerDir = em_right;
				return;
			}
			else if (vec[0] == enum_doubleright && vec.back() == enum_down) {
				//(右下、下右)
				//获取地图块的编号
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(this->getContentSize().width / 2 + 2, 0));
				//从左到右划过地图时会加1，由于提前判断一格，所以vId.x - 1;
				vId.x = vId.x - 1;
				Vec2 vIdright;
				vIdright.x = vId.x + 1; vIdright.y = vId.y;

				vId = this->tiledCoordForPosition(this->getPosition() + Vec2(0,-2));
				//从上到下划过瓦片时会加1，由于提前判断一格，所以vId.y - 1;
				vId.y = vId.y - 1;
				Vec2 vIddown;
				vIddown.x = vId.x; vIddown.y = vId.y + 1;

				Animation* animation = AnimationUtil::createWithSingleFrameName("hrun", 0.2f, -1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_doubleright) {
					this->getPlayerSprite()->stopAllActions();
					this->getPlayerSprite()->setScaleX(-m_player_magnification);
					this->getPlayerSprite()->runAction(animate);
				}
				PlayerState = enum_doubleright;
				TMXLayer* barrier = m_map->getLayer("barrier");
				bool IsTwoDir = false;
				int xMax = m_map->getMapSize().width - 1;
				int yMax = m_map->getMapSize().height - 1;
				if ((vIdright.x >= 0 && vIdright.x <= xMax && vIddown.x >= 0 && vIddown.x <= xMax) &&
					(vIdright.y >= 0 && vIdright.y <= yMax && vIddown.y >= 0 && vIddown.y <= yMax)) {
					if (barrier->getTileGIDAt(vIdright) == 0 && barrier->getTileGIDAt(vIddown) == 0 ||
						(IsNot_CollidableTile(vIdright) && IsNot_CollidableTile(vIddown))) {
						IsTwoDir = this->setPlayerPosition(this->getPosition() + Vec2(6, -4));
					}
				}
				//处理地图右边界超范围问题，左边界的tiledCoordForPosition不是超过地图边界的，不需要处理
				else if (vIdright.x > xMax && IsNot_CollidableTile(vIddown))
				{
					IsTwoDir = this->setPlayerPosition(this->getPosition() + Vec2(6, -4));
				}

				if (IsTwoDir == false)
				{
					nearBarrierMove = true;
					if (!this->setPlayerPosition(this->getPosition() + Vec2(6, 0)))
					{
						this->setPlayerPosition(this->getPosition() + Vec2(0, -4));
					}
				}

				PlayerDir = em_right;
				return;
			}
			//X or Y方向快跑，如果按了相反键的情况。
			else if ((vec[0] == enum_doubleright || vec[0] == enum_doubleleft || vec[0] == enum_doubleup
				|| vec[0] == enum_doubledown) && (vec.back() == enum_left || vec.back() == enum_right
					|| vec.back() == enum_up || vec.back() == enum_down)) {
				switch (vec[0]) //转换快跑为普通行走
				{
				case enum_doubleup: vec[0] = enum_up; break;
				case enum_doubledown: vec[0] = enum_down; break;
				case enum_doubleleft: vec[0] = enum_left; break;
				case enum_doubleright: vec[0] = enum_right; break;
				default:
					break;
				}
				//即转换成普通的行走方式了,return后在下面的一个方向的行走或run的switch语句会执行
				return;
			}

			/*两个方向一起的行走*/ /*两个方向的行走由于主角脸的朝向不像run一样取决于vec[0]，而是取决于倒数第二个vec[size - 2]*/
			if (vec.back() == enum_up && vec[vec.size() - 2] == enum_left
				|| vec[vec.size() - 2] == enum_up && vec.back() == enum_left)
			{
				//(上左、左上)走
				//获取地图块的编号，向左走左侧身边界所在位置的地图瓦片号
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(-this->getContentSize().width/2 - 2, 0));
				//向左走由于要超前判断一格，且地图从右到左滑过瓦片的边就会减１，所以需要vId.x +１；
				vId.x = vId.x + 1;
				Vec2 vIdleft; 
				vIdleft.x = vId.x - 1; vIdleft.y = vId.y;

				//向上走以脚所在位置的地图瓦片
				vId = this->tiledCoordForPosition(this->getPosition() + Vec2(0,2));
				//向上走由于要超前判断一个瓦片，且地图从下到上滑过瓦片的边就会减一，所以需要vId.y + 1;
				vId.y = vId.y + 1;
				Vec2 vIdup; 
				vIdup.x = vId.x; vIdup.y = vId.y - 1;

				if (vec.back() == enum_up) {
					Animation* animation = AnimationUtil::createWithSingleFrameName("hwalk", 0.2f, -1);
					Animate* animate = Animate::create(animation);
					if (PlayerState != enum_left) {
						this->getPlayerSprite()->stopAllActions();
						this->getPlayerSprite()->setScaleX(m_player_magnification);
						this->getPlayerSprite()->runAction(animate);
					}
					PlayerState = enum_left;
					PlayerDir = em_left;
				}
				else {
					Animation* animation = AnimationUtil::createWithSingleFrameName("uwalk", 0.2f, -1);
					Animate* animate = Animate::create(animation);
					if (PlayerState != enum_up) {
						this->getPlayerSprite()->stopAllActions();
						this->getPlayerSprite()->runAction(animate);
					}
					PlayerState = enum_up;
					PlayerDir = em_up;
				}
				TMXLayer* barrier = m_map->getLayer("barrier");
				bool IsTwoDir = false;
				int xMax = m_map->getMapSize().width - 1;
				int yMax = m_map->getMapSize().height - 1;
				if ((vIdleft.x >= 0 && vIdleft.x <= xMax && vIdup.x >= 0 && vIdup.x <= xMax) &&
					(vIdleft.y >= 0 && vIdleft.y <= yMax && vIdup.y >= 0 && vIdup.y <= yMax)) {
					if (barrier->getTileGIDAt(vIdleft) == 0 && barrier->getTileGIDAt(vIdup) == 0 ||
						(IsNot_CollidableTile(vIdleft) && IsNot_CollidableTile(vIdup))) {
						IsTwoDir = this->setPlayerPosition(this->getPosition() + Vec2(-4, 4));
					}
				}
				//地图下边界的处理，vIdleft会不存在地图范围内,且vIdup在地图内但不存在障碍物
				else if (vIdleft.y > yMax && IsNot_CollidableTile(vIdup))
				{
					IsTwoDir = this->setPlayerPosition(this->getPosition() + Vec2(-4, 4));
				}

				if (IsTwoDir == false)
				{
					nearBarrierMove = true;
					if (PlayerState == enum_left)
					{
						if (!this->setPlayerPosition(this->getPosition() + Vec2(-4, 0)))
						{
							this->setPlayerPosition(this->getPosition() + Vec2(0, 4));
						}
					}
					else if (PlayerState == enum_up)
					{
						if (!this->setPlayerPosition(this->getPosition() + Vec2(0, 4)))
						{
							this->setPlayerPosition(this->getPosition() + Vec2(-4, 0));
						}
					}
					
				}

				return;
			}
			else if (vec.back() == enum_up && vec[vec.size() - 2] == enum_right
				|| vec[vec.size() - 2] == enum_up && vec.back() == enum_right)
			{
				//(右上、上右)
				//获取地图块的编号
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(this->getContentSize().width / 2 + 2, 0));
				//从左向右划过瓦片边时会加1，由于要提前判断一格，所以vId.x - 1;
				vId.x = vId.x - 1;
				Vec2 vIdright; 
				vIdright.x = vId.x + 1; vIdright.y = vId.y;
				
				vId = this->tiledCoordForPosition(this->getPosition() + Vec2(0,2));
				//从下向上划过瓦片时会减一，由于要提前判断一格，所以vId.y + 1;
				vId.y = vId.y + 1;
				Vec2 vIdup; 
				vIdup.x = vId.x; vIdup.y = vId.y - 1;

				if (vec.back() == enum_up) {
					Animation* animation = AnimationUtil::createWithSingleFrameName("hwalk", 0.2f, -1);
					Animate* animate = Animate::create(animation);
					if (PlayerState != enum_right) {
						this->getPlayerSprite()->stopAllActions();
						this->getPlayerSprite()->setScaleX(-m_player_magnification);
						this->getPlayerSprite()->runAction(animate);
					}
					PlayerState = enum_right;
					PlayerDir = em_right;
				}
				else {
					Animation* animation = AnimationUtil::createWithSingleFrameName("uwalk", 0.2f, -1);
					Animate* animate = Animate::create(animation);
					if (PlayerState != enum_up) {
						this->getPlayerSprite()->stopAllActions();
						this->getPlayerSprite()->runAction(animate);
					}
					PlayerState = enum_up;
					PlayerDir = em_up;
				}
				TMXLayer* barrier = m_map->getLayer("barrier");
				bool IsTwoDir = false;
				int xMax = m_map->getMapSize().width - 1;
				int yMax = m_map->getMapSize().height - 1;
				if ((vIdright.x >= 0 && vIdright.x <= xMax && vIdup.x >= 0 && vIdup.x <= xMax) &&
					(vIdright.y >= 0 && vIdright.y <= yMax && vIdup.y >= 0 && vIdup.y <= yMax)) {
					if (barrier->getTileGIDAt(vIdright) == 0 && barrier->getTileGIDAt(vIdup) == 0 ||
						(IsNot_CollidableTile(vIdright) && IsNot_CollidableTile(vIdup))) {
						IsTwoDir = this->setPlayerPosition(this->getPosition() + Vec2(4, 4));
					}
				}
				//处理地图下边界问题，vIdright不在范围内,且vIdup在范围内并且不是障碍物
				else if (vIdright.y > yMax && IsNot_CollidableTile(vIdup))
				{
					IsTwoDir = this->setPlayerPosition(this->getPosition() + Vec2(4, 4));
				}
				//处理地图右边界超范围问题，左边界的tiledCoordForPosition不是超过地图边界的，不需要处理
				else if (vIdright.x > xMax && IsNot_CollidableTile(vIdup))
				{
					IsTwoDir = this->setPlayerPosition(this->getPosition() + Vec2(4, 4));
				}

				if (IsTwoDir == false)
				{
					nearBarrierMove = true;
					if (PlayerState == enum_right)
					{
						if (!this->setPlayerPosition(this->getPosition() + Vec2(4, 0)))
						{
							this->setPlayerPosition(this->getPosition() + Vec2(0, 4));
						}
					}
					else if (PlayerState == enum_up)
					{
						if (!this->setPlayerPosition(this->getPosition() + Vec2(0, 4)))
						{
							this->setPlayerPosition(this->getPosition() + Vec2(4, 0));
						}
					}

				}

				return;
			}
			else if (vec.back() == enum_down && vec[vec.size() - 2] == enum_left
				|| vec[vec.size() - 2] == enum_down && vec.back() == enum_left)
			{
				//(左下、下左）
				//获取地图块的编号
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(-this->getContentSize().width / 2 - 2,0));
				//由于从右向左划过瓦片会减一，由于提前判断一格，所以vId.x + 1;
				vId.x = vId.x + 1;
				Vec2 vIdleft; 
				vIdleft.x = vId.x - 1; vIdleft.y = vId.y;

				vId = this->tiledCoordForPosition(this->getPosition() + Vec2(0,-2));
				//由于从上往下划过瓦片会加1，由于提前判断一格，所以vId.y - 1;
				vId.y = vId.y - 1;
				Vec2 vIddown; 
				vIddown.x = vId.x; 	vIddown.y = vId.y + 1;

				if (vec.back() == enum_down) {
					Animation* animation = AnimationUtil::createWithSingleFrameName("hwalk", 0.2f, -1);
					Animate* animate = Animate::create(animation);
					if (PlayerState != enum_left) {
						this->getPlayerSprite()->stopAllActions();
						this->getPlayerSprite()->setScaleX(m_player_magnification);
						this->getPlayerSprite()->runAction(animate);
					}
					PlayerState = enum_left;
					PlayerDir = em_left;
				}
				else {
					Animation* animation = AnimationUtil::createWithSingleFrameName("dwalk", 0.2f, -1);
					Animate* animate = Animate::create(animation);
					if (PlayerState != enum_down) {
						this->getPlayerSprite()->stopAllActions();
						this->getPlayerSprite()->runAction(animate);
					}
					PlayerState = enum_down;
					PlayerDir = em_down;
				}
				TMXLayer* barrier = m_map->getLayer("barrier");
				bool IsTwoDir = false;
				int xMax = m_map->getMapSize().width - 1;
				int yMax = m_map->getMapSize().height - 1;
				if ((vIdleft.x >= 0 && vIdleft.x <= xMax && vIddown.x >= 0 && vIddown.x <= xMax) &&
					(vIdleft.y >= 0 && vIdleft.y <= yMax && vIddown.y >= 0 && vIddown.y <= yMax)) {
					if (barrier->getTileGIDAt(vIddown) == 0 && barrier->getTileGIDAt(vIdleft) == 0 ||
						(IsNot_CollidableTile(vIddown) && IsNot_CollidableTile(vIdleft))) {
						IsTwoDir = this->setPlayerPosition(this->getPosition() + Vec2(-4, -4));
					}
				}

				if (IsTwoDir == false)
				{
					nearBarrierMove = true;
					if (PlayerState == enum_left)
					{
						if (!this->setPlayerPosition(this->getPosition() + Vec2(-4, 0)))
						{
							this->setPlayerPosition(this->getPosition() + Vec2(0, -4));
						}
					}
					else if (PlayerState == enum_down)
					{
						if (!this->setPlayerPosition(this->getPosition() + Vec2(0, -4)))
						{
							this->setPlayerPosition(this->getPosition() + Vec2(-4, 0));
						}
					}
				}

				return;
			}
			else if ((vec.back() == enum_down && vec[vec.size() - 2] == enum_right
				|| vec[vec.size() - 2] == enum_down && vec.back() == enum_right))
			{
				//(右下、下右)
				//获取地图块的编号
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(this->getContentSize().width / 2 + 2,0));
				//从左到右划过地图时会加1，由于提前判断一格，所以vId.x - 1;
				vId.x = vId.x - 1;
				Vec2 vIdright; 
				vIdright.x = vId.x + 1; vIdright.y = vId.y;

				vId = this->tiledCoordForPosition(this->getPosition() + Vec2(0,-2));
				//从上到下划过瓦片时会加1，由于提前判断一格，所以vId.y - 1;
				vId.y = vId.y - 1;
				Vec2 vIddown; 
				vIddown.x = vId.x; vIddown.y = vId.y + 1;

				if (vec.back() == enum_down) {
					Animation* animation = AnimationUtil::createWithSingleFrameName("hwalk", 0.2f, -1);
					Animate* animate = Animate::create(animation);
					if (PlayerState != enum_right) {
						this->getPlayerSprite()->stopAllActions();
						this->getPlayerSprite()->setScaleX(-m_player_magnification);
						this->getPlayerSprite()->runAction(animate);
					}
					PlayerState = enum_right;
					PlayerDir = em_right;
				}
				else {
					Animation* animation = AnimationUtil::createWithSingleFrameName("dwalk", 0.2f, -1);
					Animate* animate = Animate::create(animation);
					if (PlayerState != enum_down) {
						this->getPlayerSprite()->stopAllActions();
						this->getPlayerSprite()->runAction(animate);
					}
					PlayerState = enum_down;
					PlayerDir = em_down;
				}
				TMXLayer* barrier = m_map->getLayer("barrier");
				bool IsTwoDir = false;
				int xMax = m_map->getMapSize().width - 1;
				int yMax = m_map->getMapSize().height - 1;
				if ((vIdright.x >= 0 && vIdright.x <= xMax && vIddown.x >= 0 && vIddown.x <= xMax) &&
					(vIdright.y >= 0 && vIdright.y <= yMax && vIddown.y >= 0 && vIddown.y <= yMax)) {
					if (barrier->getTileGIDAt(vIddown) == 0 && barrier->getTileGIDAt(vIdright) == 0 ||
						(IsNot_CollidableTile(vIddown) && IsNot_CollidableTile(vIdright))) {
						IsTwoDir = this->setPlayerPosition(this->getPosition() + Vec2(4, -4));
					}
				}
				//处理地图右边界超范围问题，左边界的tiledCoordForPosition不是超过地图边界的，不需要处理
				else if (vIdright.x > xMax && IsNot_CollidableTile(vIddown))
				{
					IsTwoDir = this->setPlayerPosition(this->getPosition() + Vec2(4, -4));
				}

				if (IsTwoDir == false)
				{
					nearBarrierMove = true;
					if (PlayerState == enum_right)
					{
						if (!this->setPlayerPosition(this->getPosition() + Vec2(4, 0)))
						{
							this->setPlayerPosition(this->getPosition() + Vec2(0, -4));
						}
					}
					else if (PlayerState == enum_down)
					{
						if (!this->setPlayerPosition(this->getPosition() + Vec2(0, -4)))
						{
							this->setPlayerPosition(this->getPosition() + Vec2(4, 0));
						}
					}
				}

				return;
			}
		}
		//一个方向的行走或run //此时的vec要求大于0即可//一个方向的run或walk必须有这要求（PlayerDir必须在setPlayerPosition前面，因为在setPlayerPosition中需要用到PlayerDir）
		//两个方向的就可以随意了，因为有两个方向的话一个肯定是附加方向，而主方向肯定就是PlayerDir在下面这个swithc(vec.back())决定的了
		switch (vec.back())
		{
		case enum_up: {
			Animation* animation = AnimationUtil::createWithSingleFrameName("uwalk", 0.2f, -1);
			Animate* animate = Animate::create(animation);
			if (PlayerState != enum_up) {
				this->getPlayerSprite()->stopAllActions();
				this->getPlayerSprite()->runAction(animate);
			}
			PlayerState = enum_up;
			//PlayerDir必须在setPlayerPosition前面，因为在setPlayerPosition中需要用到PlayerDir
			PlayerDir = em_up;
			this->setPlayerPosition(this->getPosition() + Vec2(0, 4));
			break;
		}
		case enum_down: {
			Animation* animation = AnimationUtil::createWithSingleFrameName("dwalk", 0.2f, -1);
			Animate* animate = Animate::create(animation);
			if (PlayerState != enum_down) {
				this->getPlayerSprite()->stopAllActions();
				this->getPlayerSprite()->runAction(animate);
			}
			PlayerState = enum_down;
			//PlayerDir必须在setPlayerPosition前面，因为在setPlayerPosition中需要用到PlayerDir
			PlayerDir = em_down;
			this->setPlayerPosition(this->getPosition() + Vec2(0, -4));
			break;
		}
		case enum_left: {
			Animation* animation = AnimationUtil::createWithSingleFrameName("hwalk", 0.2f, -1);
			Animate* animate = Animate::create(animation);
			if (PlayerState != enum_left) {
				this->getPlayerSprite()->stopAllActions();
				this->getPlayerSprite()->setScaleX(m_player_magnification);
				this->getPlayerSprite()->runAction(animate);
			}
			PlayerState = enum_left;
			//PlayerDir必须在setPlayerPosition前面，因为在setPlayerPosition中需要用到PlayerDir
			PlayerDir = em_left;
			this->setPlayerPosition(this->getPosition() + Vec2(-4, 0));
			break;
		}
		case enum_right: {
			Animation* animation = AnimationUtil::createWithSingleFrameName("hwalk", 0.2f, -1);
			Animate* animate = Animate::create(animation);
			if (PlayerState != enum_right) {
				this->getPlayerSprite()->stopAllActions();
				this->getPlayerSprite()->setScaleX(-m_player_magnification);
				this->getPlayerSprite()->runAction(animate);
			}
			PlayerState = enum_right;
			//PlayerDir必须在setPlayerPosition前面，因为在setPlayerPosition中需要用到PlayerDir
			PlayerDir = em_right;
			this->setPlayerPosition(this->getPosition() + Vec2(4, 0));
			break;
		}
		case enum_doubleup: {
			Animation* animation = AnimationUtil::createWithSingleFrameName("urun", 0.15f, -1);
			Animate* animate = Animate::create(animation);
			if (PlayerState != enum_doubleup) {
				this->getPlayerSprite()->stopAllActions();
				this->getPlayerSprite()->runAction(animate);
			}
			PlayerState = enum_doubleup;
			//PlayerDir必须在setPlayerPosition前面，因为在setPlayerPosition中需要用到PlayerDir
			PlayerDir = em_up;
			this->setPlayerPosition(this->getPosition() + Vec2(0, 6));
			break;
		}
		case enum_doubledown: {
			Animation* animation = AnimationUtil::createWithSingleFrameName("drun", 0.18f, -1);
			Animate* animate = Animate::create(animation);
			if (PlayerState != enum_doubledown) {
				this->getPlayerSprite()->stopAllActions();
				this->getPlayerSprite()->runAction(animate);
			}
			PlayerState = enum_doubledown;
			//PlayerDir必须在setPlayerPosition前面，因为在setPlayerPosition中需要用到PlayerDir
			PlayerDir = em_down;
			this->setPlayerPosition(this->getPosition() + Vec2(0, -6));
			break;
		}
		case enum_doubleleft: {
			Animation* animation = AnimationUtil::createWithSingleFrameName("hrun", 0.2f, -1);
			Animate* animate = Animate::create(animation);
			if (PlayerState != enum_doubleleft) {
				this->getPlayerSprite()->stopAllActions();
				this->getPlayerSprite()->setScaleX(m_player_magnification);
				this->getPlayerSprite()->runAction(animate);
			}
			PlayerState = enum_doubleleft;
			//PlayerDir必须在setPlayerPosition前面，因为在setPlayerPosition中需要用到PlayerDir
			PlayerDir = em_left;
			this->setPlayerPosition(this->getPosition() + Vec2(-6, 0));
			break;
		}
		case enum_doubleright: {
			Animation* animation = AnimationUtil::createWithSingleFrameName("hrun", 0.2f, -1);
			Animate* animate = Animate::create(animation);
			if (PlayerState != enum_doubleright) {
				this->getPlayerSprite()->stopAllActions();
				this->getPlayerSprite()->setScaleX(-m_player_magnification);
				this->getPlayerSprite()->runAction(animate);
			}
			PlayerState = enum_doubleright;
			//PlayerDir必须在setPlayerPosition前面，因为在setPlayerPosition中需要用到PlayerDir
			PlayerDir = em_right;
			this->setPlayerPosition(this->getPosition() + Vec2(6, 0));
			break;
		}
		case enum_static:
			break;
		default:
			break;
		}
	}
	//else vec.size() 等于 0	
	else
	{
		//	//主角静止
		switch (PlayerDir)
		{
		case em_up: {
			Animation* animation = AnimationUtil::createWithSingleFrameName("ustatic", 0.2f, -1);
			Animate* animate = Animate::create(animation);
			if (PlayerState != enum_static) {
				this->getPlayerSprite()->stopAllActions();
				this->getPlayerSprite()->runAction(animate);
			}
			break;
		}
		case em_down: {
			Animation* animation = AnimationUtil::createWithSingleFrameName("dstatic", 0.2f, -1);
			Animate* animate = Animate::create(animation);
			if (PlayerState != enum_static) {
				this->getPlayerSprite()->stopAllActions();
				this->getPlayerSprite()->runAction(animate);
			}
			break;
		}
		case em_left: {
			Animation* animation = AnimationUtil::createWithSingleFrameName("hstatic", 0.2f, -1);
			Animate* animate = Animate::create(animation);
			if (PlayerState != enum_static) {
				this->getPlayerSprite()->stopAllActions();
				this->getPlayerSprite()->setScaleX(m_player_magnification);
				this->getPlayerSprite()->runAction(animate);
			}
			break;
		}
		case em_right: {
			Animation* animation = AnimationUtil::createWithSingleFrameName("hstatic", 0.2f, -1);
			Animate* animate = Animate::create(animation);
			if (PlayerState != enum_static) {
				this->getPlayerSprite()->stopAllActions();
				this->getPlayerSprite()->setScaleX(-m_player_magnification);
				this->getPlayerSprite()->runAction(animate);
			}
			break;
		}
		default:
			break;
		}
		PlayerState = enum_static;
	}
}

void Player::keyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (!isAcceptInput)
		return;

	if (keyCode == EventKeyboard::KeyCode::KEY_W)
	{
		float curtime = timecounter_up->getCurTime();
		//开始计时
		timecounter_up->start(); //第一次start时curtime是0
								 //上冲,静止状态才有上冲功能
		if (curtime > 0 && curtime - 0 < 0.4f && PlayerState == enum_static) {
			vec.push_back(enum_doubleup);
			return;
		}
		bool s = true;
		for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); it++)
		{
			if ((*it) == enum_up)
			{
				s = false;
				break;
			}
		}
		if (s == true)
			vec.push_back(enum_up);
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_S)
	{
		float curtime = timecounter_down->getCurTime();
		//开始计时
		timecounter_down->start();
		//下冲,静止状态才有下冲
		if (curtime > 0 && curtime - 0 < 0.4f && PlayerState == enum_static) {
			vec.push_back(enum_doubledown);
			return;
		}
		bool s = true;
		for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); it++)
		{
			if ((*it) == enum_down)
			{
				s = false;
				break;
			}
		}
		if (s == true)
			vec.push_back(enum_down);
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_A)
	{
		float curtime = timecounter_left->getCurTime();
		//开始计时
		timecounter_left->start();
		//左冲,静止转态时才有左冲
		if (curtime > 0 && curtime - 0 < 0.4f && PlayerState == enum_static) {
			vec.push_back(enum_doubleleft);
			return;
		}
		bool s = true;
		for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); it++)
		{
			if ((*it) == enum_left)
			{
				s = false;
				break;
			}
		}
		if (s == true)
			vec.push_back(enum_left);
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_D)
	{
		float curtime = timecounter_right->getCurTime();
		//开始计时
		timecounter_right->start();
		//右冲,静止状态时才有右冲
		if (curtime > 0 && curtime - 0 < 0.4f && PlayerState == enum_static) {
			log("/////////");
			
			vec.push_back(enum_doubleright);
			log("%d", vec[0]);
			return;
		}
		bool s = true;
		for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); it++)
		{
			if ((*it) == enum_right)
			{
				s = false;
				break;
			}
		}
		if (s == true)
			vec.push_back(enum_right);
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_J)
	{
		//设置J技能的攻击冷却时间
		float curtime = timecounter_J->getCurTime();
		//第一次按J时才有curtime = 0,此后每隔0.4f秒才能按放一次技能
		//主角非被击状态才能放技能
		if (curtime == 0 || curtime > 0.3f && (playerIsattacked == false)) {

			timecounter_J->start();//一直计时
								   //size为0才有普通的攻击
			if (vecskill.size() == 0) {
				//播放音效
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Jswordsound.wav");

				if (PlayerState == enum_doubleup || PlayerState == enum_doubledown
					|| PlayerState == enum_doubleleft || PlayerState == enum_doubleright) {
					vecskill.push_back(enum_basepoke);
					vecskillstr.push_back(baseskillstr(enum_basepoke, false));
					switch (PlayerState)//run状态下一帧使用basepoke技能时，使vec[0](如果有run状态，vec[0]永远代表着run状态)变为walk
					{
					case enum_doubleup: {
						if (vec.size() == 1)
							vec[0] = enum_up;
						break;
					}
					case enum_doubledown: {
						if (vec.size() == 1)
							vec[0] = enum_down;
						break;
					}
					case enum_doubleleft: {
						if (vec.size() == 1)
							vec[0] = enum_left;
						break;
					}
					case enum_doubleright: {
						if (vec.size() == 1)
							vec[0] = enum_right;
						break;
					}
					default:
						break;
					}
				}
				else {
					vecskill.push_back(enum_baseattack);
					vecskillstr.push_back(baseskillstr(enum_baseattack, false));
				}
			}
		}
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_K)
	{
		//设置K技能的攻击冷却时间//剑气
		float curtime = timecounter_J->getCurTime();
		//第一次按K时才有curtime = 0,此后每隔07f秒才能按放一次技能
		//主角非被击状态才能放技能
		if (curtime == 0 || curtime > 0.7f && (playerIsattacked == false) && m_mp >= k_consumemp) {
			//扣mp
			m_mp -= k_consumemp;
			timecounter_J->start();//一直计时
								   //size为0才有剑气
			if (vecskill.size() == 0) {
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Kskill.wav"); 

				if (PlayerState == enum_doubleup || PlayerState == enum_doubledown
					|| PlayerState == enum_doubleleft || PlayerState == enum_doubleright) {
					vecskill.push_back(enum_swordwave);
					switch (PlayerState)//run状态下一帧使用swordwave技能时，使vec[0](如果有run状态，vec[0]永远代表着run状态)变为walk
					{
					case enum_doubleup: {
						if (vec.size() == 1)
							vec[0] = enum_up;
						break;
					}
					case enum_doubledown: {
						if (vec.size() == 1)
							vec[0] = enum_down;
						break;
					}
					case enum_doubleleft: {
						if (vec.size() == 1)
							vec[0] = enum_left;
						break;
					}
					case enum_doubleright: {
						if (vec.size() == 1)
							vec[0] = enum_right;
						break;
					}
					default:
						break;
					}
				}
				else {
					vecskill.push_back(enum_swordwave);//walk状态也是swordwave技能
				}
			}
		}
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_L)
	{
		if (playerIsattacked == false && m_mp >= l_consumemp)
		{
			if (vecskill.size() == 0 && skillControl->skill_laser())
			{
				m_mp -= l_consumemp;
				if (PlayerState == enum_doubleup || PlayerState == enum_doubledown
					|| PlayerState == enum_doubleleft || PlayerState == enum_doubleright) {
					vecskill.push_back(enum_laserskill);
					switch (PlayerState)//run状态下一帧使用雷电技能时，使vec[0](如果有run状态，vec[0]永远代表着run状态)变为walk
					{
					case enum_doubleup: {
						if (vec.size() == 1)
							vec[0] = enum_up;
						break;
					}
					case enum_doubledown: {
						if (vec.size() == 1)
							vec[0] = enum_down;
						break;
					}
					case enum_doubleleft: {
						if (vec.size() == 1)
							vec[0] = enum_left;
						break;
					}
					case enum_doubleright: {
						if (vec.size() == 1)
							vec[0] = enum_right;
						break;
					}
					default:
						break;
					}
				}
				else {
					vecskill.push_back(enum_laserskill);//walk状态也是swordwave技能
				}
			}
		}
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_U)
	{
		if (playerIsattacked == false && m_mp >= u_consumemp)
		{
			if (vecskill.size() == 0 && skillControl->skill_fire())
			{
				m_mp -= u_consumemp;
				if (PlayerState == enum_doubleup || PlayerState == enum_doubledown
					|| PlayerState == enum_doubleleft || PlayerState == enum_doubleright) {
					vecskill.push_back(enum_fireskill);
					switch (PlayerState)
					{
					case enum_doubleup: {
						if (vec.size() == 1)
							vec[0] = enum_up;
						break;
					}
					case enum_doubledown: {
						if (vec.size() == 1)
							vec[0] = enum_down;
						break;
					}
					case enum_doubleleft: {
						if (vec.size() == 1)
							vec[0] = enum_left;
						break;
					}
					case enum_doubleright: {
						if (vec.size() == 1)
							vec[0] = enum_right;
						break;
					}
					default:
						break;
					}
				}
				else {
					vecskill.push_back(enum_fireskill);
				}
			}
		}
	}
}

void Player::keyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_W)
	{
		for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); it++)
		{
			if ((*it) == enum_up)
			{
				vec.erase(it);
				break;
			}
		}
		for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); it++)
		{
			if ((*it) == enum_doubleup)
			{
				vec.erase(it);
				break;
			}
		}
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_S)
	{
		for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); it++)
		{
			if ((*it) == enum_down)
			{
				vec.erase(it);
				break;
			}
		}
		for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); it++)
		{
			if ((*it) == enum_doubledown)
			{
				vec.erase(it);
				break;
			}
		}
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_A)
	{
		for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); it++)
		{
			if ((*it) == enum_left)
			{
				vec.erase(it);
				break;
			}
		}
		for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); it++)
		{
			if ((*it) == enum_doubleleft)
			{
				vec.erase(it);
				break;
			}
		}
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_D)
	{
		for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); it++)
		{
			if ((*it) == enum_right)
			{
				vec.erase(it);
				break;
			}
		}
		for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); it++)
		{
			if ((*it) == enum_doubleright)
			{
				vec.erase(it);
				break;
			}
		}
	}
}

void Player::setTiledMap(TMXTiledMap* map)
{
	m_map = map;
	m_map->addChild(this, m_map->getChildren().size());
}

TMXTiledMap* Player::getTiledMap()
{
	return m_map;
}

Vec2 Player::tiledCoordForPosition(Vec2 pos)
{
	Size mapTiledNum = m_map->getMapSize();
	Size tiledSize = m_map->getTileSize();
	Size visiblesize = Director::getInstance()->getVisibleSize();

	int x = (int)(pos.x / tiledSize.width);
	int y = (int)(m_map->getMapSize().height * m_map->getTileSize().height - pos.y) / m_map->getTileSize().height;

	return Vec2(x, y);
}

bool Player::setPlayerPosition(Vec2 position)
{
	if (!IscanMove)
	{
		return false;
	}


	Vec2 positionVleft;
	Vec2 positionVright;
	switch (PlayerDir) {
	case em_up: {
		//position.y += this->getContentSize().height * this->getAnchorPoint().y;

		//如果主角是双方向走的，即vec.size() > 1，但此时靠墙走，那么依旧是按键盘双方向的靠墙单方向走，vec.size() > 1的情况
		//特殊处理下
		if (nearBarrierMove == true)
		{
			nearBarrierMove = false;
			positionVleft = position;
			positionVleft.x -= this->getContentSize().width * this->getAnchorPoint().x;
			positionVright = position;
			positionVright.x += this->getContentSize().width * this->getAnchorPoint().x;
		}

		//run或者walk附加的方向都是取决于vec.back(),而且附加的方向都是枚举enum_up,enum_left,enum_down,enum_right
		if (vec.size() > 1) {
			switch (vec.back())
			{
			case enum_left: {
				position.x -= this->getContentSize().width * this->getAnchorPoint().x;
				break;
			}
			case enum_right: {
				position.x += this->getContentSize().width * this->getAnchorPoint().x;
				break;
			}
			default: {
				break;
			}
			}
		}
		//仅仅单上方向walk or run因为主角锚点是0.5 0，主角只是向上走，主角有身体的宽度，所以加多左右宽度的判断
		else {
			positionVleft = position;
			positionVleft.x -= this->getContentSize().width * this->getAnchorPoint().x;
			positionVright = position;
			positionVright.x += this->getContentSize().width * this->getAnchorPoint().x;
		}

		break;
	}
	case em_down: {

		//如果主角是双方向走的，即vec.size() > 1，但此时靠墙走，那么依旧是按键盘双方向的靠墙单方向走，vec.size() > 1的情况
		//特殊处理下
		if (nearBarrierMove == true)
		{
			nearBarrierMove = false;
			positionVleft = position;
			positionVleft.x -= this->getContentSize().width * this->getAnchorPoint().x;
			positionVright = position;
			positionVright.x += this->getContentSize().width * this->getAnchorPoint().x;
		}

		//run或者walk附加的方向都是取决于vec.back(),而且附加的方向都是枚举enum_up,enum_left,enum_down,enum_right
		if (vec.size() > 1) {
			switch (vec.back())
			{
			case enum_left: {
				position.x -= this->getContentSize().width * this->getAnchorPoint().x;
				break;
			}
			case enum_right: {
				position.x += this->getContentSize().width * this->getAnchorPoint().x;
				break;
			}
			default: {
				break;
			}
			}
		}
		//仅仅单下方向walk or run因为主角锚点是0.5 0.5，主角只是向上走，主角有身体的宽度，所以加多左右宽度的判断
		else {
			positionVleft = position;
			positionVleft.x -= this->getContentSize().width * this->getAnchorPoint().x;
			positionVright = position;
			positionVright.x += this->getContentSize().width * this->getAnchorPoint().x;
		}

		break;
	}
	case em_left: {
		position.x -= this->getContentSize().width  * this->getAnchorPoint().x;
		//run或者walk附加的方向都是取决于vec.back(),而且附加的方向都是枚举enum_up,enum_left,enum_down,enum_right
		if (vec.size() > 1) {
			switch (vec.back())
			{
			case enum_up: {
				//position.y += this->getContentSize().height * this->getAnchorPoint().y;
				break;
			}
			case enum_down: {
				//position.y -= this->getContentSize().height * this->getAnchorPoint().y;
				break;
			}
			default:
				break;
			}
		}
		break;
	}
	case em_right: {
		position.x += this->getContentSize().width  * this->getAnchorPoint().x;//主角身体宽度
		 //run或者walk附加的方向都是取决于vec.back(),而且附加的方向都是枚举enum_up,enum_left,enum_down,enum_right
		if (vec.size() > 1) {
			switch (vec.back())
			{
			case enum_up: {
				//position.y += this->getContentSize().height * this->getAnchorPoint().y;
				break;
			}
			case enum_down: {
				//position.y -= this->getContentSize().height * this->getAnchorPoint().y;
				break;
			}
			default:
				break;
			}
		}
		break;
	}
	}

	switch (PlayerDir)//仅上下run or walk，如果主角身体宽度不满足，即碰到障碍物,就return 
	{
	case em_up: {
		Vec2 tileCoord = this->tiledCoordForPosition(positionVleft);

		if (tileCoord.x >= 0 && tileCoord.x < m_map->getMapSize().width //不超出瓦片地图坐标
			&& tileCoord.y >= 0 && tileCoord.y < m_map->getMapSize().height) {
			int tileGid = m_map->getLayer("barrier")->getTileGIDAt(tileCoord);
			if (tileGid > 0) {
				Value prop = m_map->getPropertiesForGID(tileGid);
				ValueMap proValueMap = prop.asValueMap();

				if (proValueMap.find("Collidable") != proValueMap.end()) {
					std::string collision = proValueMap.at("Collidable").asString();
					if (collision == "true") {
						return false;
					}
				}
			}
		}

		tileCoord = this->tiledCoordForPosition(positionVright);

		if (tileCoord.x >= 0 && tileCoord.x < m_map->getMapSize().width //不超出瓦片地图坐标
			&& tileCoord.y >= 0 && tileCoord.y < m_map->getMapSize().height) {
			int tileGid = m_map->getLayer("barrier")->getTileGIDAt(tileCoord);
			if (tileGid > 0) {
				Value prop = m_map->getPropertiesForGID(tileGid);
				ValueMap proValueMap = prop.asValueMap();

				if (proValueMap.find("Collidable") != proValueMap.end()) {
					std::string collision = proValueMap.at("Collidable").asString();
					if (collision == "true") {
						return false;
					}
				}
			}
		}
		break;
	}
	case em_down: {
		Vec2 tileCoord = this->tiledCoordForPosition(positionVleft);

		if (tileCoord.x >= 0 && tileCoord.x < m_map->getMapSize().width //不超出瓦片地图坐标
			&& tileCoord.y >= 0 && tileCoord.y < m_map->getMapSize().height) {
			int tileGid = m_map->getLayer("barrier")->getTileGIDAt(tileCoord);
			if (tileGid > 0) {
				Value prop = m_map->getPropertiesForGID(tileGid);
				ValueMap proValueMap = prop.asValueMap();

				if (proValueMap.find("Collidable") != proValueMap.end()) {
					std::string collision = proValueMap.at("Collidable").asString();
					if (collision == "true") {
						return false;
					}
				}
			}
		}

		tileCoord = this->tiledCoordForPosition(positionVright);

		if (tileCoord.x >= 0 && tileCoord.x < m_map->getMapSize().width //不超出瓦片地图坐标
			&& tileCoord.y >= 0 && tileCoord.y < m_map->getMapSize().height) {
			int tileGid = m_map->getLayer("barrier")->getTileGIDAt(tileCoord);
			if (tileGid > 0) {
				Value prop = m_map->getPropertiesForGID(tileGid);
				ValueMap proValueMap = prop.asValueMap();

				if (proValueMap.find("Collidable") != proValueMap.end()) {
					std::string collision = proValueMap.at("Collidable").asString();
					if (collision == "true") {
						return false;
					}
				}
			}
		}
		break;
	}
	default:
		break;
	}

	Vec2 tileCoord = this->tiledCoordForPosition(position); //以主角中心锚点为判断左右上下run or walk是否碰撞

	if (tileCoord.x >= 0 && tileCoord.x < m_map->getMapSize().width //不超出瓦片地图坐标
		&& tileCoord.y >= 0 && tileCoord.y < m_map->getMapSize().height) {
		int tileGid = m_map->getLayer("barrier")->getTileGIDAt(tileCoord);
		if (tileGid > 0) {
			Value prop = m_map->getPropertiesForGID(tileGid);
			ValueMap proValueMap = prop.asValueMap();

			if (proValueMap.find("Collidable") != proValueMap.end()) {
				std::string collision = proValueMap.at("Collidable").asString();
				if (collision == "true") {
					return false;
				}
			}
		}
	}

	//还原position
	switch (PlayerDir) {
	case em_up: {
		//position.y -= this->getContentSize().height * this->getAnchorPoint().y;

		//run或者walk附加的方向都是取决于vec.back(),而且附加的方向都是枚举enum_up,enum_left,enum_down,enum_right
		if (vec.size() > 1) {
			switch (vec.back())
			{
			case enum_left: {
				position.x += this->getContentSize().width * this->getAnchorPoint().x;
				break;
			}
			case enum_right: {
				position.x -= this->getContentSize().width * this->getAnchorPoint().x;
				break;
			}
			default: {
				break;
			}
			}
		}
		//仅仅单上方向walk or run因为主角锚点是0.5 0.5，主角只是向上走，主角有身体的宽度，所以加多左右宽度的判断
		else {
			positionVleft = position;
			positionVleft.x += this->getContentSize().width * this->getAnchorPoint().x;
			positionVright = position;
			positionVright.x -= this->getContentSize().width * this->getAnchorPoint().x;
		}

		break;
	}
	case em_down: {

		//run或者walk附加的方向都是取决于vec.back(),而且附加的方向都是枚举enum_up,enum_left,enum_down,enum_right
		if (vec.size() > 1) {
			switch (vec.back())
			{
			case enum_left: {
				position.x += this->getContentSize().width * this->getAnchorPoint().x;
				break;
			}
			case enum_right: {
				position.x -= this->getContentSize().width * this->getAnchorPoint().x;
				break;
			}
			default: {
				break;
			}
			}
		}
		//仅仅单下方向walk or run因为主角锚点是0.5 0.5，主角只是向上走，主角有身体的宽度，所以加多左右宽度的判断
		else {
			positionVleft = position;
			positionVleft.x += this->getContentSize().width * this->getAnchorPoint().x;
			positionVright = position;
			positionVright.x -= this->getContentSize().width * this->getAnchorPoint().x;
		}

		break;
	}
	case em_left: {
		position.x += this->getContentSize().width  * this->getAnchorPoint().x;
		//run或者walk附加的方向都是取决于vec.back(),而且附加的方向都是枚举enum_up,enum_left,enum_down,enum_right
		if (vec.size() > 1) {
			switch (vec.back())
			{
			case enum_up: {
				//position.y -= this->getContentSize().height * this->getAnchorPoint().y;
				break;
			}
			case enum_down: {
				//position.y += this->getContentSize().height * this->getAnchorPoint().y;
				break;
			}
			default:
				break;
			}
		}
		break;
	}
	case em_right: {
		position.x -= this->getContentSize().width  * this->getAnchorPoint().x;//主角身体宽度
		//run或者walk附加的方向都是取决于vec.back(),而且附加的方向都是枚举enum_up,enum_left,enum_down,enum_right
		if (vec.size() > 1) {
			switch (vec.back())
			{
			case enum_up: {
				//position.y -= this->getContentSize().height * this->getAnchorPoint().y;
				break;
			}
			case enum_down: {
				//position.y += this->getContentSize().height * this->getAnchorPoint().y;
				break;
			}
			default:
				break;
			}
		}
		break;
	}
	}

	this->setPosition(position);

	return true;
}

bool Player::IsSussesfulForsetRemoteSkillPos(Vec2 position)
{
	Vec2 tileCoord = this->tiledCoordForPosition(position);
	if (tileCoord.x >= 0 && tileCoord.x < m_map->getMapSize().width //不超出瓦片地图坐标
		&& tileCoord.y >= 0 && tileCoord.y < m_map->getMapSize().height) {
		int tileGid = m_map->getLayer("barrier")->getTileGIDAt(tileCoord);
		if (tileGid > 0) {
			Value prop = m_map->getPropertiesForGID(tileGid);
			ValueMap proValueMap = prop.asValueMap();

			if (proValueMap.find("Collidable") != proValueMap.end()) {
				std::string collision = proValueMap.at("Collidable").asString();
				if (collision == "true") {
					return false;
				}

			}
		}
	}
	return true;
}

Sprite* Player::getPlayerSprite()
{
	return this->getSprite();
}

void Player::CallBack1()
{
	if (vecskill.size() == 1) {
		vecskill.erase(vecskill.begin());
		if (vecskillstr.size() == 1)
			vecskillstr.erase(vecskillstr.begin());
	}
}

void Player::createSwordWave()
{
	RemoteSkill* swordwave = NULL;
	for (int i = 0; i < swordwaveNum; i++)
	{
		swordwave = RemoteSkill::create();
		swordwave->bindSprite(Sprite::createWithSpriteFrameName("playerskill1.png"));
		//设置剑气swordwave的Node节点的锚点为0.5 0.5;
		swordwave->setAnchorPoint(Vec2(0.5, 0.5));
		if (this->getParent() != NULL)
		{
			this->getParent()->addChild(swordwave, (int)this->getParent()->getChildren().size());
			Animation* animation = AnimationUtil::createWithSingleFrameName("playerskill1", 0.1f, -1);
			Animate* animate = Animate::create(animation);
			swordwave->getSprite()->runAction(animate);
			m_swordwave_Arr.pushBack(swordwave);
		}
	}
}

void Player::setPlayerparent(TMXTiledMap* parent)
{
	m_parrent = parent;
}

int Player::getPlayer_magnification()
{
	return m_player_magnification;
}

bool Player::IsNot_CollidableTile(Vec2 tileCoord)
{
	if (tileCoord.x >= 0 && tileCoord.x < m_map->getMapSize().width //不超出瓦片地图坐标
		&& tileCoord.y >= 0 && tileCoord.y < m_map->getMapSize().height) {
		int tileGid = m_map->getLayer("barrier")->getTileGIDAt(tileCoord);
		if (tileGid > 0) {
			Value prop = m_map->getPropertiesForGID(tileGid);
			ValueMap proValueMap = prop.asValueMap();

			if (proValueMap.find("Collidable") != proValueMap.end()) {
				std::string collision = proValueMap.at("Collidable").asString();
				if (collision == "true") {
					return false;
				}

			}
		}
	}
	return true;
}

Vector<RemoteSkill*> Player::getPlayerUsing_swordwave_Arr()
{
	return m_Using_swordwave_Arr;
}

std::vector<int> Player::getVecSkill()
{
	return vecskill;
}

int Player::getPlayerDir()
{
	return PlayerDir;
}

void Player::setPlayerDir(int direction)
{
	PlayerDir = direction;
}

std::vector<baseskillstr>& Player::getvecskillstr()
{
	return vecskillstr;
}

void Player::baseskillcollidUpdata(float dt)
{
	//获取主角技能J、K按键容器（在主角类写了vecskill元素最多为一个）
	auto vecskill = this->getVecSkill();
	if (PlayerState != enum_baseattack || PlayerState != enum_basepoke)
	{
		collidedVector.clear();
	}

	//普通攻击伤害本来是6
	if (vecskill.size() > 0)
	{
		//如果是普通攻击或者前冲攻击
		if (vecskill.back() == enum_baseattack || vecskill.back() == enum_basepoke)
		{
			auto& vecskillstruct = this->getvecskillstr();//获取到的是一个引用
			if (vecskillstruct.size() > 0)
			{
				if (vecskillstruct.back().b == false)
				{
					vecskillstruct.back().b = true;//标志单段普通攻击已判断完成;

					// 碰撞检测
					Vector<Monster*> monsVec = MonsterManager::getInstance()->getMonsterVec();
					int playerattackRange = 32;
					Vec2 vec;
					//主角的普通攻击暂时写成一个点。
					for (auto mons : monsVec)
					{
						switch (this->getPlayerDir())
						{
						case em_up:{
							vec = this->getPosition();
							vec.y += this->getContentSize().height;
							vec.y += playerattackRange;
							if (mons->getBoundingBox().containsPoint(vec))
							{
								/*播放怪物被普通攻击打中音效*/
								CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/monhitedbyPlayerBaseskill.wav");

								int i;
								for (i = collidedVector.size() - 1; i >= 0; i--)
								{
									if (collidedVector.at(i) == mons)
										break;
								}
								if (i < 0)
								{
									mons->cmd_hurt(6); //普通攻击的伤害
									mons->isAttackedByPlayerBaseskill = true;
									collidedVector.pushBack(mons);
								}
							}

							break;
						}
						case em_down:{
							vec = this->getPosition();
							vec.y += this->getContentSize().height;
							vec.y -= playerattackRange;
							if (mons->getBoundingBox().containsPoint(vec))
							{
								/*播放怪物被普通攻击打中音效*/
								CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/monhitedbyPlayerBaseskill.wav");

								int i;
								for (i = collidedVector.size() - 1; i >= 0; i--)
								{
									if (collidedVector.at(i) == mons)
										break;
								}
								if (i < 0)
								{
									mons->cmd_hurt(6); //普通攻击的伤害
									mons->isAttackedByPlayerBaseskill = true;
									collidedVector.pushBack(mons);
								}
							}

							break;
						}
						case em_left:{
							vec = this->getPosition();
							vec.y += this->getContentSize().height;
							vec.x -= playerattackRange;
							if (mons->getBoundingBox().containsPoint(vec))
							{
								/*播放怪物被普通攻击打中音效*/
								CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/monhitedbyPlayerBaseskill.wav");

								int i;
								for (i = collidedVector.size() - 1; i >= 0; i--)
								{
									if (collidedVector.at(i) == mons)
										break;
								}
								if (i < 0)
								{
									mons->cmd_hurt(6); //普通攻击的伤害
									mons->isAttackedByPlayerBaseskill = true;
									collidedVector.pushBack(mons);
								}
							}

							break;
						}
						case em_right:{
							vec = this->getPosition();
							vec.y += this->getContentSize().height;
							vec.x += playerattackRange;
							if (mons->getBoundingBox().containsPoint(vec))
							{
								/*播放怪物被普通攻击打中音效*/
								CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/monhitedbyPlayerBaseskill.wav"); 

								int i;
								for (i = collidedVector.size() - 1; i >= 0; i--)
								{
									if (collidedVector.at(i) == mons)
										break;
								}
								if (i < 0)
								{
									mons->cmd_hurt(6); //普通攻击的伤害
									mons->isAttackedByPlayerBaseskill = true;
									collidedVector.pushBack(mons);
								}
							}

							break;
						}
						}
					}
				}
			}
		}
	}
}

void Player::setPlayer_hp(float hp)
{
	m_hp = hp;
}

void Player::setPlayer_mp(float mp)
{
	m_mp = mp;
}

void Player::Playerhp_mp_Update(float dt)
{
	auto bar = BarManager::getInstance()->getPlayerBars();
	if (bar != NULL)
	{
		BarManager::getInstance()->setPercent(bar->m_hp, curLevel_Maxhp, m_hp);
		BarManager::getInstance()->setBarLabel(bar->l_hp, m_hp, curLevel_Maxhp);
		BarManager::getInstance()->setPercent(bar->m_mp, curLevel_Maxmp, m_mp);
		BarManager::getInstance()->setBarLabel(bar->l_mp, m_mp, curLevel_Maxmp);
		BarManager::getInstance()->setPercent(bar->m_exp, 100 * m_playerlevel, m_exp);
		BarManager::getInstance()->setBarLabel(bar->l_exp, m_exp, 100 * m_playerlevel);
	}
}

float& Player::getCurMaxHp()
{
	return curLevel_Maxhp;
}

float& Player::getCurMaxMp()
{
	return curLevel_Maxmp;
}

void Player::recoverHp_Mp(float dt)
{
	if (m_mp + curLevel_Maxmp * 1 / 100 >= curLevel_Maxmp)
		m_mp = curLevel_Maxmp;
	else
		m_mp += curLevel_Maxmp * 1 / 100;
	if (m_hp + curLevel_Maxhp * 0.5 / 100 >= curLevel_Maxhp)
		m_hp = curLevel_Maxhp;
	else
		m_hp += curLevel_Maxhp * 0.5 / 100;
}

void Player::ChangSceneIdUpdate(float dt)
{
	Vec2 tiledcoord = tiledCoordForPosition(this->getPosition());
	int xMax = m_map->getMapSize().width - 1;
	int yMax = m_map->getMapSize().height - 1;
	if (tiledcoord.x >= 0 && tiledcoord.x <= xMax && tiledcoord.y >= 0 && tiledcoord.y <= yMax)
	{
		int tileGid = m_map->getLayer("barrier")->getTileGIDAt(tiledcoord);
		if (tileGid > 0) {
			Value prop = m_map->getPropertiesForGID(tileGid);
			ValueMap proValueMap = prop.asValueMap();

			if (proValueMap.find("changescene") != proValueMap.end()) {
				std::string changescene = proValueMap.at("changescene").asString();
				Value SceneId(changescene);
				int Id = SceneId.asInt();
				/*判断是否达成切换地图的条件*/
				if (ChangeScenePointManager::getInstance()->IsReachCondition(Id) == false)
				{
					auto dlgs = TalkManager::getInstance()->getDataFromEnterSceneDlgsData(Id);
					if (!isInChangeScenePoint && dlgs && !dlgs->cannotEnterDlgs.empty())
					{
						auto talk = Talk::create(dlgs->cannotEnterDlgs, dlgs->sceneId, -1);
						m_map->addChild(talk, 999);
						isInChangeScenePoint = true;
					} 
					return;
				}

				setEnableAction(false);

				if (QuestDispatcher::getInstance()->getParent() != NULL) {
					QuestDispatcher::getInstance()->removeFromParentAndCleanup(false);
				}

				if (proValueMap.find("nextscenedir") != proValueMap.end())
				{
					this->gamescenedir = proValueMap.at("nextscenedir").asString();
				}
				/*清除K键技能的剑气容器*/
				clearSwordWave();

				/*切换场景时，把本场景怪物从怪物管理器中Pop出来*/
				auto& Vec = MonsterManager::getInstance()->getMonsterVec();
				Vec.clear();
				/*切换场景时，把本场景NPC从NPC管理器中Pop出来*/
				auto& Vec2 = NpcManager::getInstance()->getNpcsVec();
				Vec2.clear();
				/*一定要先Pop本场景,再创建新场景，这样才不会把新场景怪物也Pop了*/
				Scene* sc = NULL;
				sc = GameScene::createSceneWithId(Id);
				auto reScene = TransitionJumpZoom::create(0.0f, sc);
				Director::getInstance()->replaceScene(sc);
			}
		}
		else
			isInChangeScenePoint = false;
	}
}

void Player::openAllUpdate()
{
	this->scheduleUpdate();
	this->schedule(schedule_selector(Player::baseskillcollidUpdata));
	this->schedule(schedule_selector(Player::Playerhp_mp_Update), 0.2f);
	this->schedule(schedule_selector(Player::recoverHp_Mp), 1.0f);
	this->schedule(schedule_selector(Player::ChangSceneIdUpdate));
	this->schedule(schedule_selector(Player::LevelUpdate));
	//放完L或者U的技能然后突然切换图了，L或U技能的定时器会被关掉，而getCurtime若在cd范围内，那么主角之后就一直不能放技能
	//所以把time初始化为0
	for (int i = 0; i < MAX_SKILL_NUM; i++)
		skillControl->skillCounter[i]->setstartTimeZeroAndcloseSchedule();
}

void Player::LevelUpdate(float dt)
{
	if (m_exp >= 100 * (m_playerlevel))//升级经验为等级二次方函数
	{
		m_exp = m_exp - 100 * m_playerlevel;
		//升级
		m_playerlevel += 1;

		/*主角最大hp,mp根据主角等级来定*/
		curLevel_Maxhp = 100 + (m_playerlevel - 1) * 20;
		curLevel_Maxmp = 100 + (m_playerlevel - 1) * 35;

		//升级完满hp,mp
		m_mp = curLevel_Maxmp;
		m_hp = curLevel_Maxhp;

		//播放升级动画和声音
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/levelsup.wav");
		if (spritelevelup == NULL)
		{
			spritelevelup = Sprite::createWithSpriteFrameName("playerlevelup1.png");
			spritelevelup->setPosition(getContentSize().width / 2, getContentSize().height / 2);
			this->addChild(spritelevelup, 500);
			CallFunc* call = CallFunc::create([&](){spritelevelup->removeFromParent(); spritelevelup = NULL; });
			Animation* animation = AnimationUtil::createWithSingleFrameName("playerlevelup", 0.1f, 1);
			Animate* animate = Animate::create(animation);
			spritelevelup->runAction(Sequence::create(animate, call, NULL));
		}
		return;
	}
}

void Player::setEnableAction(bool isEnable)
{
	if (!isEnable)
		playStaticAnim();
	vec.clear();
	vecskill.clear();
	vecskillstr.clear();
	isAcceptInput = isEnable;
}

void Player::playStaticAnim()
{
	switch (PlayerDir)
	{
	case em_up: {
		Animation* animation = AnimationUtil::createWithSingleFrameName("ustatic", 0.2f, -1);
		Animate* animate = Animate::create(animation);
		if (PlayerState != enum_static) {
			this->getPlayerSprite()->stopAllActions();
			this->getPlayerSprite()->runAction(animate);
		}
		break;
	}
	case em_down: {
		Animation* animation = AnimationUtil::createWithSingleFrameName("dstatic", 0.2f, -1);
		Animate* animate = Animate::create(animation);
		if (PlayerState != enum_static) {
			this->getPlayerSprite()->stopAllActions();
			this->getPlayerSprite()->runAction(animate);
		}
		break;
	}
	case em_left: {
		Animation* animation = AnimationUtil::createWithSingleFrameName("hstatic", 0.2f, -1);
		Animate* animate = Animate::create(animation);
		if (PlayerState != enum_static) {
			this->getPlayerSprite()->stopAllActions();
			this->getPlayerSprite()->setScaleX(m_player_magnification);
			this->getPlayerSprite()->runAction(animate);
		}
		break;
	}
	case em_right: {
		Animation* animation = AnimationUtil::createWithSingleFrameName("hstatic", 0.2f, -1);
		Animate* animate = Animate::create(animation);
		if (PlayerState != enum_static) {
			this->getPlayerSprite()->stopAllActions();
			this->getPlayerSprite()->setScaleX(-m_player_magnification);
			this->getPlayerSprite()->runAction(animate);
		}
		break;
	}
	default:
		break;
	}
	PlayerState = enum_static;
}

void Player::PlayerDiedUpdate(float dt)
{
	if (m_hp <= 0)
	{
		//停止怪物，停止主角
		this->setEnableAction(false);
		MonsterManager::getInstance()->setMonsEnableAction(false);

		Size size = Director::getInstance()->getVisibleSize();

		//死亡时候设置一个层,吞掉触摸，此时就不能点到NPC保存
		Node* node = Node::create();
		node->setContentSize(size);
		auto listener = EventListenerTouchOneByOne::create();
		Node* l = m_map->getParent();
		if (l)
		{
			l->addChild(node,10000);
		}

		listener->onTouchBegan = [](Touch* touch, Event* event){return true; };
		listener->setSwallowTouches(true);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, node);

		//关键定时器
		this->unschedule(schedule_selector(Player::PlayerDiedUpdate));

		//清掉当前被攻击的队列
		while (!attackedqueue.empty())
			attackedqueue.pop();

		//停掉背景音乐
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		
		//停掉主角回hp mp的定时器
		void Playerhp_mp_Update(float dt);
		this->unschedule(schedule_selector(Player::recoverHp_Mp));

		//切换到startgamescene场景
		//切换场景前先处理各种单例和各种容器
		if (QuestDispatcher::getInstance()->getParent() != NULL) {
			QuestDispatcher::getInstance()->removeFromParentAndCleanup(false);
		}
		//把QuestDispatcher的任务内容重设置为最近一次存档的状态。考虑没有存档的情况
		QuestDispatcher::getInstance()->InitQuestDispathcerForPlayerDied();
		//把TalkManager的对话重设置为最新一次存档的状态,包括没存档的时候
		TalkManager::getInstance()->InitTalkForPlayerDied();

		/*清除K键技能的剑气容器*/
		clearSwordWave();
		/*切换场景时，把本场景怪物从怪物管理器中Pop出来*/
		auto& Vec = MonsterManager::getInstance()->getMonsterVec();
		Vec.clear();
		/*切换场景时，把本场景NPC从NPC管理器中Pop出来*/
		auto& Vec2 = NpcManager::getInstance()->getNpcsVec();
		Vec2.clear();

		//播放游戏失败音乐
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/gameover.mp3");

		//死亡时场景特效
		Sprite* gameoversprite = Sprite::create("GameOver/gameover.png");
		if (m_map->getParent() != NULL)
			m_map->getParent()->addChild(gameoversprite);
		gameoversprite->setPosition(size.width / 2, size.height * 1.5);
		gameoversprite->setScale(size.width / gameoversprite->getContentSize().width, size.height / gameoversprite->getContentSize().height);

		MoveTo* moveto = MoveTo::create(3.0f, ccp(size.width / 2, size.height / 2));


		CallFunc* callfunc = CallFunc::create([&](){
			Label* dielabel = Label::create(gb2312_to_utf8("游戏结束"), "Arial", 55);
			if (m_map->getParent() != NULL)
				m_map->getParent()->addChild(dielabel);
			dielabel->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2);
			Blink* blink = Blink::create(1.0f, 3);
			CallFunc* call = CallFunc::create([](){

				Scene* sc = NULL;
				sc = StartGameScene::createScene();
				auto reScene = TransitionJumpZoom::create(0.0f, sc);
				Director::getInstance()->replaceScene(sc);
			}
			);
			auto s = Sequence::create(blink, call, NULL);
			dielabel->runAction(s);
		}
		);

		Sequence* sequence = Sequence::create(moveto, callfunc, NULL);
		gameoversprite->runAction(sequence);
	
	}
}

void Player::clearSwordWave()
{
	m_Using_swordwave_Arr.clear();
	m_swordwave_Arr.clear();
}
