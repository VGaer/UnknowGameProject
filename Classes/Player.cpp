#include "Player.h"


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
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(Player::keyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(Player::keyReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	this->scheduleUpdate();
	this->schedule(schedule_selector(Player::baseskillcollidUpdata));

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

	PlayerState = enum_initNone;//初始化为什么都没有状态，一运行游戏如果没操作就会转为enum_static,有操作转为对应操作的walk or run状态 
	PlayerDir = em_down;//初始化时

	swordwaveNum = 20;
	createSwordWave();

	m_player_magnification = 2;

	m_hp = 100;

	this->bindSprite(Sprite::create("player.png"));
	m_playerColor = this->getSprite()->getColor();

	skillControl = SkillControl::create();
	addChild(skillControl);

	return true;
}

void Player::update(float dt)
{
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
			attackedqueue.pop();
			//设置计时为0，当被攻击信息队列增加消息size再次大于0时，不会马上被pop掉，而是重新调用了timecouter_attacked->start才判断是否pop掉
			timecounter_attacked->setstartTimeZeroAndcloseSchedule();
		}

		//被攻击僵直间隔
		if (attackedqueue.size() > 0 && timecounter_attacked->getCurTime() == 0.0f)
		{
			int attack = attackedqueue.front();
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
					vec[0] = enum_up;
					break;
				}
				case enum_doubledown: {
					vec[0] = enum_down;
					break;
				}
				case enum_doubleleft: {
					vec[0] = enum_left;
					break;
				}
				case enum_doubleright: {
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
					vec[0] = enum_up;
					break;
				}
				case enum_doubledown: {
					vec[0] = enum_down;
					break;
				}
				case enum_doubleleft: {
					vec[0] = enum_left;
					break;
				}
				case enum_doubleright: {
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
					vec[0] = enum_up;
					break;
				}
				case enum_doubledown: {
					vec[0] = enum_down;
					break;
				}
				case enum_doubleleft: {
					vec[0] = enum_left;
					break;
				}
				case enum_doubleright: {
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
				//扣主角hp
				m_hp--;

				//如果是run状态下被攻击了，把run转化为walk
				switch (PlayerState)
				{
				case enum_doubleup: {
					vec[0] = enum_up;
					break;
				}
				case enum_doubledown: {
					vec[0] = enum_down;
					break;
				}
				case enum_doubleleft: {
					vec[0] = enum_left;
					break;
				}
				case enum_doubleright: {
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
	this->getSprite()->setRotation(0);
	this->getSprite()->setPosition(Vec2(this->getContentSize().width * this->getAnchorPoint().x,this->getContentSize().height * this->getAnchorPoint().y));
	/*防止主角击退效果被打歪*/

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

						//CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(k);
						//k = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/swordsound.wav");
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
						swordwave->setPosition(this->getPosition() + Vec2(0, this->getContentSize().height / 2));//把剑气根据玩家位置初始化
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
						swordwave->setPosition(this->getPosition() + Vec2(0, -this->getContentSize().height / 2));//把剑气根据玩家位置初始化
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
						swordwave->setPosition(this->getPosition() + Vec2(-this->getContentSize().width / 2, 0));//把剑气根据玩家位置初始化
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
						swordwave->setPosition(this->getPosition() + Vec2(this->getContentSize().width / 2, 0));//把剑气根据玩家位置初始化
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
				//上左快跑
				//获取地图块的编号时，主角中心坐标加上图块对应的宽高，因为tiledCoordForPosition从一个瓦片到另一个瓦片只需要碰到边瓦片的坐标就会变化。
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(m_map->getTileSize().width / 2, -m_map->getTileSize().height / 2));
				Vec2 vIdleft; vIdleft.x = vId.x - 1; vIdleft.y = vId.y;
				Vec2 vIdup; vIdup.x = vId.x; vIdup.y = vId.y - 1;

				Animation* animation = AnimationUtil::createWithSingleFrameName("urun", 0.15f, -1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_doubleup) {
					this->getPlayerSprite()->stopAllActions();
					this->getPlayerSprite()->runAction(animate);
				}
				PlayerState = enum_doubleup;
				TMXLayer* barrier = m_map->getLayer("barrier");
				//如果左边或上边有障碍物，防止对角线穿墙跑过去
				int xMax = m_map->getMapSize().width - 1;
				int yMax = m_map->getMapSize().height - 1;
				if ((vIdleft.x >= 0 && vIdleft.x <= xMax && vIdup.x >= 0 && vIdup.x <= xMax) &&
					(vIdleft.y >= 0 && vIdleft.y <= yMax && vIdup.y >= 0 && vIdup.y <= yMax)) {
					if (barrier->getTileGIDAt(vIdleft) == 0 && barrier->getTileGIDAt(vIdup) == 0 ||
						(IsNot_CollidableTile(vIdleft) && IsNot_CollidableTile(vIdup)))
					{
						this->setPlayerPosition(this->getPosition() + Vec2(-4, 6));
					}
				}

				PlayerDir = em_up;
				return;
			}
			else if (vec[0] == enum_doubleup && vec.back() == enum_right) {
				//上右快跑
				//获取地图块的编号时，主角中心坐标加上图块对应的宽高，因为tiledCoordForPosition从一个瓦片到另一个瓦片只需要碰到边瓦片的坐标就会变化。
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(-m_map->getTileSize().width / 2, -m_map->getTileSize().height / 2));
				Vec2 vIdright; vIdright.x = vId.x + 1; vIdright.y = vId.y;
				Vec2 vIdup; vIdup.x = vId.x; vIdup.y = vId.y - 1;

				Animation* animation = AnimationUtil::createWithSingleFrameName("urun", 0.15f, -1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_doubleup) {
					this->getPlayerSprite()->stopAllActions();
					this->getPlayerSprite()->runAction(animate);
				}
				PlayerState = enum_doubleup;
				TMXLayer* barrier = m_map->getLayer("barrier");
				//如果左边或上边有障碍物，防止对角线穿墙跑过去
				int xMax = m_map->getMapSize().width - 1;
				int yMax = m_map->getMapSize().height - 1;
				if ((vIdright.x >= 0 && vIdright.x <= xMax && vIdup.x >= 0 && vIdup.x <= xMax) &&
					(vIdright.y >= 0 && vIdright.y <= yMax && vIdup.y >= 0 && vIdup.y <= yMax)) {
					if (barrier->getTileGIDAt(vIdright) == 0 && barrier->getTileGIDAt(vIdup) == 0 ||
						(IsNot_CollidableTile(vIdright) && IsNot_CollidableTile(vIdup))) {
						this->setPlayerPosition(this->getPosition() + Vec2(4, 6));
					}
				}

				PlayerDir = em_up;
				return;
			}
			else if (vec[0] == enum_doubledown && vec.back() == enum_left) {
				//下左
				//获取地图块的编号时，主角中心坐标加上图块对应的宽高，因为tiledCoordForPosition从一个瓦片到另一个瓦片只需要碰到边瓦片的坐标就会变化。
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(m_map->getTileSize().width / 2, m_map->getTileSize().height / 2));
				Vec2 vIdleft; vIdleft.x = vId.x - 1; vIdleft.y = vId.y;
				Vec2 vIddown; vIddown.x = vId.x; vIddown.y = vId.y + 1;

				Animation* animation = AnimationUtil::createWithSingleFrameName("drun", 0.18f, -1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_doubledown) {
					this->getPlayerSprite()->stopAllActions();
					this->getPlayerSprite()->runAction(animate);
				}
				PlayerState = enum_doubledown;
				TMXLayer* barrier = m_map->getLayer("barrier");
				//如果左边或下边有障碍物，防止对角线穿墙跑过去
				int xMax = m_map->getMapSize().width - 1;
				int yMax = m_map->getMapSize().height - 1;
				if ((vIdleft.x >= 0 && vIdleft.x <= xMax && vIddown.x >= 0 && vIddown.x <= xMax) &&
					(vIdleft.y >= 0 && vIdleft.y <= yMax && vIddown.y >= 0 && vIddown.y <= yMax)) {
					if (barrier->getTileGIDAt(vIddown) == 0 && barrier->getTileGIDAt(vIdleft) == 0 ||
						(IsNot_CollidableTile(vIddown) && IsNot_CollidableTile(vIdleft))) {
						this->setPlayerPosition(this->getPosition() + Vec2(-4, -6));
					}
				}

				PlayerDir = em_down;
				return;
			}
			else if (vec[0] == enum_doubledown && vec.back() == enum_right) {
				//下右
				//获取地图块的编号时，主角中心坐标加上图块对应的宽高，因为tiledCoordForPosition从一个瓦片到另一个瓦片只需要碰到边瓦片的坐标就会变化。
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(-m_map->getTileSize().width / 2, m_map->getTileSize().height / 2));
				Vec2 vIdright; vIdright.x = vId.x + 1; vIdright.y = vId.y;
				Vec2 vIddown; vIddown.x = vId.x; vIddown.y = vId.y + 1;

				Animation* animation = AnimationUtil::createWithSingleFrameName("drun", 0.18f, -1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_doubledown) {
					this->getPlayerSprite()->stopAllActions();
					this->getPlayerSprite()->runAction(animate);
				}
				PlayerState = enum_doubledown;
				TMXLayer* barrier = m_map->getLayer("barrier");
				//如果左边或下边有障碍物，防止对角线穿墙跑过去
				int xMax = m_map->getMapSize().width - 1;
				int yMax = m_map->getMapSize().height - 1;
				if ((vIdright.x >= 0 && vIdright.x <= xMax && vIddown.x >= 0 && vIddown.x <= xMax) &&
					(vIdright.y >= 0 && vIdright.y <= yMax && vIddown.y >= 0 && vIddown.y <= yMax)) {
					if (barrier->getTileGIDAt(vIdright) == 0 && barrier->getTileGIDAt(vIddown) == 0 ||
						(IsNot_CollidableTile(vIdright) && IsNot_CollidableTile(vIddown))) {
						this->setPlayerPosition(this->getPosition() + Vec2(4, -6));
					}
				}

				PlayerDir = em_down;
				return;
			}
			else if (vec[0] == enum_doubleleft && vec.back() == enum_up) {
				//左上快跑
				//获取地图块的编号时，主角中心坐标加上图块对应的宽高，因为tiledCoordForPosition从一个瓦片到另一个瓦片只需要碰到边瓦片的坐标就会变化。
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(m_map->getTileSize().width / 2, -m_map->getTileSize().height / 2));
				Vec2 vIdleft; vIdleft.x = vId.x - 1; vIdleft.y = vId.y;
				Vec2 vIdup; vIdup.x = vId.x; vIdup.y = vId.y - 1;

				Animation* animation = AnimationUtil::createWithSingleFrameName("hrun", 0.2f, -1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_doubleleft) {
					this->getPlayerSprite()->stopAllActions();
					this->getPlayerSprite()->setScaleX(m_player_magnification);
					this->getPlayerSprite()->runAction(animate);
				}
				PlayerState = enum_doubleleft;
				TMXLayer* barrier = m_map->getLayer("barrier");
				int xMax = m_map->getMapSize().width - 1;
				int yMax = m_map->getMapSize().height - 1;
				if ((vIdleft.x >= 0 && vIdleft.x <= xMax && vIdup.x >= 0 && vIdup.x <= xMax) &&
					(vIdleft.y >= 0 && vIdleft.y <= yMax && vIdup.y >= 0 && vIdup.y <= yMax)) {
					if (barrier->getTileGIDAt(vIdleft) == 0 && barrier->getTileGIDAt(vIdup) == 0 ||
						(IsNot_CollidableTile(vIdleft) && IsNot_CollidableTile(vIdup))) {
						this->setPlayerPosition(this->getPosition() + Vec2(-6, 4));
					}
				}

				PlayerDir = em_left;
				return;
			}
			else if (vec[0] == enum_doubleleft && vec.back() == enum_down) {
				//左下
				//获取地图块的编号时，主角中心坐标加上图块对应的宽高，因为tiledCoordForPosition从一个瓦片到另一个瓦片只需要碰到边瓦片的坐标就会变化。
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(m_map->getTileSize().width / 2, m_map->getTileSize().height / 2));
				Vec2 vIdleft; vIdleft.x = vId.x - 1; vIdleft.y = vId.y;
				Vec2 vIddown; vIddown.x = vId.x; vIddown.y = vId.y + 1;

				Animation* animation = AnimationUtil::createWithSingleFrameName("hrun", 0.2f, -1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_doubleleft) {
					this->getPlayerSprite()->stopAllActions();
					this->getPlayerSprite()->setScaleX(m_player_magnification);
					this->getPlayerSprite()->runAction(animate);
				}
				PlayerState = enum_doubleleft;
				TMXLayer* barrier = m_map->getLayer("barrier");
				int xMax = m_map->getMapSize().width - 1;
				int yMax = m_map->getMapSize().height - 1;
				if ((vIdleft.x >= 0 && vIdleft.x <= xMax && vIddown.x >= 0 && vIddown.x <= xMax) &&
					(vIdleft.y >= 0 && vIdleft.y <= yMax && vIddown.y >= 0 && vIddown.y <= yMax)) {
					if (barrier->getTileGIDAt(vIdleft) == 0 && barrier->getTileGIDAt(vIddown) == 0 ||
						(IsNot_CollidableTile(vIdleft) && IsNot_CollidableTile(vIddown))) {
						this->setPlayerPosition(this->getPosition() + Vec2(-6, -4));
					}
				}

				PlayerDir = em_left;
				return;
			}
			else if (vec[0] == enum_doubleright && vec.back() == enum_up) {
				//右上快跑
				//获取地图块的编号时，主角中心坐标加上图块对应的宽高，因为tiledCoordForPosition从一个瓦片到另一个瓦片只需要碰到边瓦片的坐标就会变化。
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(-m_map->getTileSize().width / 2, -m_map->getTileSize().height / 2));
				Vec2 vIdright; vIdright.x = vId.x + 1; vIdright.y = vId.y;
				Vec2 vIdup; vIdup.x = vId.x; vIdup.y = vId.y - 1;

				Animation* animation = AnimationUtil::createWithSingleFrameName("hrun", 0.2f, -1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_doubleright) {
					this->getPlayerSprite()->stopAllActions();
					this->getPlayerSprite()->setScaleX(-m_player_magnification);
					this->getPlayerSprite()->runAction(animate);
				}
				PlayerState = enum_doubleright;
				TMXLayer* barrier = m_map->getLayer("barrier");
				int xMax = m_map->getMapSize().width - 1;
				int yMax = m_map->getMapSize().height - 1;
				if ((vIdright.x >= 0 && vIdright.x <= xMax && vIdup.x >= 0 && vIdup.x <= xMax) &&
					(vIdright.y >= 0 && vIdright.y <= yMax && vIdup.y >= 0 && vIdup.y <= yMax)) {
					if (barrier->getTileGIDAt(vIdright) == 0 && barrier->getTileGIDAt(vIdup) == 0 ||
						(IsNot_CollidableTile(vIdright) && IsNot_CollidableTile(vIdup))) {
						this->setPlayerPosition(this->getPosition() + Vec2(6, 4));
					}
				}

				PlayerDir = em_right;
				return;
			}
			else if (vec[0] == enum_doubleright && vec.back() == enum_down) {
				//右下
				//获取地图块的编号时，主角中心坐标加上图块对应的宽高，因为tiledCoordForPosition从一个瓦片到另一个瓦片只需要碰到边瓦片的坐标就会变化。
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(-m_map->getTileSize().width / 2, m_map->getTileSize().height / 2));
				Vec2 vIdright; vIdright.x = vId.x + 1; vIdright.y = vId.y;
				Vec2 vIddown; vIddown.x = vId.x; vIddown.y = vId.y + 1;

				Animation* animation = AnimationUtil::createWithSingleFrameName("hrun", 0.2f, -1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_doubleright) {
					this->getPlayerSprite()->stopAllActions();
					this->getPlayerSprite()->setScaleX(-m_player_magnification);
					this->getPlayerSprite()->runAction(animate);
				}
				PlayerState = enum_doubleright;
				TMXLayer* barrier = m_map->getLayer("barrier");
				int xMax = m_map->getMapSize().width - 1;
				int yMax = m_map->getMapSize().height - 1;
				if ((vIdright.x >= 0 && vIdright.x <= xMax && vIddown.x >= 0 && vIddown.x <= xMax) &&
					(vIdright.y >= 0 && vIdright.y <= yMax && vIddown.y >= 0 && vIddown.y <= yMax)) {
					if (barrier->getTileGIDAt(vIdright) == 0 && barrier->getTileGIDAt(vIddown) == 0 ||
						(IsNot_CollidableTile(vIdright) && IsNot_CollidableTile(vIddown))) {
						this->setPlayerPosition(this->getPosition() + Vec2(6, -4));
					}
				}

				PlayerDir = em_right;
				return;
			}
			//X or Y方向快跑，如果按了相反键的情况。
			else if ((vec[0] == enum_doubleright || vec[0] == enum_doubleleft || vec[0] == enum_doubleup
				|| vec[0] == enum_doubledown) && (vec.back() == enum_left || vec.back() == enum_right
					|| vec.back() == enum_up || vec.back() == enum_down)) {
				switch (vec[0]) //转换快跑位普通行走
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
				//获取地图块的编号时，主角中心坐标加上图块对应的宽高，因为tiledCoordForPosition从一个瓦片到另一个瓦片只需要碰到边瓦片的坐标就会变化。
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(m_map->getTileSize().width / 2, -m_map->getTileSize().height / 2));
				Vec2 vIdleft; vIdleft.x = vId.x - 1; vIdleft.y = vId.y;
				Vec2 vIdup; vIdup.x = vId.x; vIdup.y = vId.y - 1;

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
				int xMax = m_map->getMapSize().width - 1;
				int yMax = m_map->getMapSize().height - 1;
				if ((vIdleft.x >= 0 && vIdleft.x <= xMax && vIdup.x >= 0 && vIdup.x <= xMax) &&
					(vIdleft.y >= 0 && vIdleft.y <= yMax && vIdup.y >= 0 && vIdup.y <= yMax)) {
					if (barrier->getTileGIDAt(vIdleft) == 0 && barrier->getTileGIDAt(vIdup) == 0 /*||
																								 (IsNot_CollidableTile(vIdleft) && IsNot_CollidableTile(vIdup))*/) {
						this->setPlayerPosition(this->getPosition() + Vec2(-4, 4));
					}
				}

				return;
			}
			else if (vec.back() == enum_up && vec[vec.size() - 2] == enum_right
				|| vec[vec.size() - 2] == enum_up && vec.back() == enum_right)
			{
				//(右上、上右)快跑
				//获取地图块的编号时，主角中心坐标加上图块对应的宽高，因为tiledCoordForPosition从一个瓦片到另一个瓦片只需要碰到边瓦片的坐标就会变化。
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(-m_map->getTileSize().width / 2, -m_map->getTileSize().height / 2));
				Vec2 vIdright; vIdright.x = vId.x + 1; vIdright.y = vId.y;
				Vec2 vIdup; vIdup.x = vId.x; vIdup.y = vId.y - 1;

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
				int xMax = m_map->getMapSize().width - 1;
				int yMax = m_map->getMapSize().height - 1;
				if ((vIdright.x >= 0 && vIdright.x <= xMax && vIdup.x >= 0 && vIdup.x <= xMax) &&
					(vIdright.y >= 0 && vIdright.y <= yMax && vIdup.y >= 0 && vIdup.y <= yMax)) {
					if (barrier->getTileGIDAt(vIdright) == 0 && barrier->getTileGIDAt(vIdup) == 0 /*||
																								  (IsNot_CollidableTile(vIdright) && IsNot_CollidableTile(vIdup))*/) {
						this->setPlayerPosition(this->getPosition() + Vec2(4, 4));
					}
				}

				return;
			}
			else if (vec.back() == enum_down && vec[vec.size() - 2] == enum_left
				|| vec[vec.size() - 2] == enum_down && vec.back() == enum_left)
			{
				//(左下、下左）
				//获取地图块的编号时，主角中心坐标加上图块对应的宽高，因为tiledCoordForPosition从一个瓦片到另一个瓦片只需要碰到边瓦片的坐标就会变化。
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(m_map->getTileSize().width / 2, m_map->getTileSize().height / 2));
				Vec2 vIdleft; vIdleft.x = vId.x - 1; vIdleft.y = vId.y;
				Vec2 vIddown; vIddown.x = vId.x; vIddown.y = vId.y + 1;

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
				int xMax = m_map->getMapSize().width - 1;
				int yMax = m_map->getMapSize().height - 1;
				if ((vIdleft.x >= 0 && vIdleft.x <= xMax && vIddown.x >= 0 && vIddown.x <= xMax) &&
					(vIdleft.y >= 0 && vIdleft.y <= yMax && vIddown.y >= 0 && vIddown.y <= yMax)) {
					if (barrier->getTileGIDAt(vIddown) == 0 && barrier->getTileGIDAt(vIdleft) == 0 /*||
																								   (IsNot_CollidableTile(vIddown) && IsNot_CollidableTile(vIdleft))*/) {
						this->setPlayerPosition(this->getPosition() + Vec2(-4, -4));
					}
				}

				return;
			}
			else if ((vec.back() == enum_down && vec[vec.size() - 2] == enum_right
				|| vec[vec.size() - 2] == enum_down && vec.back() == enum_right))
			{
				//(右下、下右)
				//获取地图块的编号时，主角中心坐标加上图块对应的宽高，因为tiledCoordForPosition从一个瓦片到另一个瓦片只需要碰到边瓦片的坐标就会变化。
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(-m_map->getTileSize().width / 2, m_map->getTileSize().height / 2));
				Vec2 vIdright; vIdright.x = vId.x + 1; vIdright.y = vId.y;
				Vec2 vIddown; vIddown.x = vId.x; vIddown.y = vId.y + 1;

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
				int xMax = m_map->getMapSize().width - 1;
				int yMax = m_map->getMapSize().height - 1;
				if ((vIdright.x >= 0 && vIdright.x <= xMax && vIddown.x >= 0 && vIddown.x <= xMax) &&
					(vIdright.y >= 0 && vIdright.y <= yMax && vIddown.y >= 0 && vIddown.y <= yMax)) {
					if (barrier->getTileGIDAt(vIddown) == 0 && barrier->getTileGIDAt(vIdright) == 0 ||
						(IsNot_CollidableTile(vIddown) && IsNot_CollidableTile(vIdright))) {
						this->setPlayerPosition(this->getPosition() + Vec2(4, -4));
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
			vec.push_back(enum_doubleright);
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
		//第一次按J时才有curtime = 0,此后每隔0.5f秒才能按放一次技能
		//主角非被击状态才能放技能
		if (curtime == 0 || curtime > 0.5f && (playerIsattacked == false)) {

			timecounter_J->start();//一直计时
								   //size为0才有普通的攻击
			if (vecskill.size() == 0) {
				if (PlayerState == enum_doubleup || PlayerState == enum_doubledown
					|| PlayerState == enum_doubleleft || PlayerState == enum_doubleright) {
					vecskill.push_back(enum_basepoke);
					vecskillstr.push_back(baseskillstr(enum_basepoke, false));
					switch (PlayerState)//run状态下一帧使用basepoke技能时，使vec[0](如果有run状态，vec[0]永远代表着run状态)变为walk
					{
					case enum_doubleup: {
						vec[0] = enum_up;
						break;
					}
					case enum_doubledown: {
						vec[0] = enum_down;
						break;
					}
					case enum_doubleleft: {
						vec[0] = enum_left;
						break;
					}
					case enum_doubleright: {
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
		if (curtime == 0 || curtime > 0.7f && (playerIsattacked == false)) {
			timecounter_J->start();//一直计时
								   //size为0才有剑气
			if (vecskill.size() == 0) {
				if (PlayerState == enum_doubleup || PlayerState == enum_doubledown
					|| PlayerState == enum_doubleleft || PlayerState == enum_doubleright) {
					vecskill.push_back(enum_swordwave);
					switch (PlayerState)//run状态下一帧使用swordwave技能时，使vec[0](如果有run状态，vec[0]永远代表着run状态)变为walk
					{
					case enum_doubleup: {
						vec[0] = enum_up;
						break;
					}
					case enum_doubledown: {
						vec[0] = enum_down;
						break;
					}
					case enum_doubleleft: {
						vec[0] = enum_left;
						break;
					}
					case enum_doubleright: {
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
		if (playerIsattacked == false)
		{
			if (vecskill.size() == 0 && skillControl->skill_laser())
			{
				if (PlayerState == enum_doubleup || PlayerState == enum_doubledown
					|| PlayerState == enum_doubleleft || PlayerState == enum_doubleright) {
					vecskill.push_back(enum_laserskill);
					switch (PlayerState)//run状态下一帧使用雷电技能时，使vec[0](如果有run状态，vec[0]永远代表着run状态)变为walk
					{
					case enum_doubleup: {
						vec[0] = enum_up;
						break;
					}
					case enum_doubledown: {
						vec[0] = enum_down;
						break;
					}
					case enum_doubleleft: {
						vec[0] = enum_left;
						break;
					}
					case enum_doubleright: {
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
		if (playerIsattacked == false)
		{
			if (vecskill.size() == 0 && skillControl->skill_fire())
			{
				if (PlayerState == enum_doubleup || PlayerState == enum_doubledown
					|| PlayerState == enum_doubleleft || PlayerState == enum_doubleright) {
					vecskill.push_back(enum_fireskill);
					switch (PlayerState)
					{
					case enum_doubleup: {
						vec[0] = enum_up;
						break;
					}
					case enum_doubledown: {
						vec[0] = enum_down;
						break;
					}
					case enum_doubleleft: {
						vec[0] = enum_left;
						break;
					}
					case enum_doubleright: {
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
	Vec2 positionVleft;
	Vec2 positionVright;
	switch (PlayerDir) {
	case em_up: {
		position.y += 2;//主角脚的大小

						//run或者walk附加的方向都是取决于vec.back(),而且附加的方向都是枚举enum_up,enum_left,enum_down,enum_right
		if (vec.size() > 1) {
			switch (vec.back())
			{
			case enum_left: {
				position.x -= this->getContentSize().width / 2;
				break;
			}
			case enum_right: {
				position.x += this->getContentSize().width / 2;
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
			positionVleft.x -= this->getContentSize().width / 2;
			positionVright = position;
			positionVright.x += this->getContentSize().width / 2;
		}

		break;
	}
	case em_down: {

		//run或者walk附加的方向都是取决于vec.back(),而且附加的方向都是枚举enum_up,enum_left,enum_down,enum_right
		if (vec.size() > 1) {
			switch (vec.back())
			{
			case enum_left: {
				position.x -= this->getContentSize().width / 2;
				break;
			}
			case enum_right: {
				position.x += this->getContentSize().width / 2;
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
			positionVleft.x -= this->getContentSize().width / 2;
			positionVright = position;
			positionVright.x += this->getContentSize().width / 2;
		}

		break;
	}
	case em_left: {
		position.x -= this->getContentSize().width / 2;
		//run或者walk附加的方向都是取决于vec.back(),而且附加的方向都是枚举enum_up,enum_left,enum_down,enum_right
		if (vec.size() > 1) {
			switch (vec.back())
			{
			case enum_up: {
				position.y += 2;//加上主角鞋子的大小
				break;
			}
			case enum_down: {

				break;
			}
			default:
				break;
			}
		}
		break;
	}
	case em_right: {
		position.x += this->getContentSize().width / 2;//主角身体宽度
													   //run或者walk附加的方向都是取决于vec.back(),而且附加的方向都是枚举enum_up,enum_left,enum_down,enum_right
		if (vec.size() > 1) {
			switch (vec.back())
			{
			case enum_up: {
				position.y += 2;//加上主角鞋子的大小
				break;
			}
			case enum_down: {

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
		break;
	}
	case em_down: {
		Vec2 tileCoord = this->tiledCoordForPosition(positionVright);

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
		position.y -= 2;//主角脚的大小

						//run或者walk附加的方向都是取决于vec.back(),而且附加的方向都是枚举enum_up,enum_left,enum_down,enum_right
		if (vec.size() > 1) {
			switch (vec.back())
			{
			case enum_left: {
				position.x += this->getContentSize().width / 2;
				break;
			}
			case enum_right: {
				position.x -= this->getContentSize().width / 2;
				break;
			}
			default:
				break;
			}
		}
		break;
	}
	case em_down: {

		//run或者walk附加的方向都是取决于vec.back(),而且附加的方向都是枚举enum_up,enum_left,enum_down,enum_right
		if (vec.size() > 1) {
			switch (vec.back())
			{
			case enum_left: {
				position.x += this->getContentSize().width / 2;
				break;
			}
			case enum_right: {
				position.x -= this->getContentSize().width / 2;
				break;
			}
			default:
				break;
			}
		}
		break;
	}
	case em_left: {
		position.x += this->getContentSize().width / 2;
		//run或者walk附加的方向都是取决于vec.back(),而且附加的方向都是枚举enum_up,enum_left,enum_down,enum_right
		if (vec.size() > 1) {
			switch (vec.back())
			{
			case enum_up: {
				position.y -= 2;//加上主角鞋子的大小
				break;
			}
			case enum_down: {

				break;
			}
			default:
				break;
			}
		}
		break;
	}
	case em_right: {
		position.x -= this->getContentSize().width / 2;//主角身体宽度
													   //run或者walk附加的方向都是取决于vec.back(),而且附加的方向都是枚举enum_up,enum_left,enum_down,enum_right
		if (vec.size() > 1) {
			switch (vec.back())
			{
			case enum_up: {
				position.y -= 2;//加上主角鞋子的大小
				break;
			}
			case enum_down: {

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
		swordwave->bindSprite(Sprite::create("remoteskills/playerskill1.png"));
		//设置剑气swordwave的Node节点的锚点为0.5 0.5;
		swordwave->setAnchorPoint(Vec2(0.5, 0.5));
		this->getParent()->addChild(swordwave, (int)this->getParent()->getChildren().size());
		Animation* animation = AnimationUtil::createWithSingleFrameName("playerskill", 0.1f, -1);
		Animate* animate = Animate::create(animation);
		swordwave->getSprite()->runAction(animate);
		m_swordwave_Arr.pushBack(swordwave);
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
							vec.y += playerattackRange;
							if (mons->getBoundingBox().containsPoint(vec))
							{
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
							vec.y -= playerattackRange;
							if (mons->getBoundingBox().containsPoint(vec))
							{
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
							vec.x -= playerattackRange;
							if (mons->getBoundingBox().containsPoint(vec))
							{
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
							vec.x += playerattackRange;
							if (mons->getBoundingBox().containsPoint(vec))
							{
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
	