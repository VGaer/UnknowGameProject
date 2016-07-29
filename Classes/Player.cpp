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

	//��ʼ������
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
	//�����ǻص�������������Sequence�ģ����ص�����ִ�е��Ƕ�ʱ����ܹ����������ж����ˣ�������init��ʱ����һ��,��������ļ��ؾͻ�����������Ķ���
	//�����ǰ�������Ķ���
	while (!attackedqueue.empty())
		attackedqueue.pop();


	//�ж�Ϊ�ղ���Ӿ��飬��ֹ�ظ����
	if (getSprite() == NULL)
	{
		this->bindSprite(Sprite::create("player.png"));
		m_playerColor = this->getSprite()->getColor();

		//ֻ���һ�����̼������ͺã���ֹ�Ӷ������Ϊ�������󣬻ؿ�ʼ�����㿪ʼ��ϷaddPlayerʱ�ֵ�����һ��init();
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
	//�ڶ�����ͨ��������
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

	/*������������*/
	frameCache->addSpriteFramesWithFile("playerLevelup/playerlevelup.plist", "playerLevelup/playerlevelup.png");

	PlayerState = enum_initNone;//��ʼ��Ϊʲô��û��״̬��һ������Ϸ���û�����ͻ�תΪenum_static,�в���תΪ��Ӧ������walk or run״̬ 
	PlayerDir = em_down;//��ʼ��ʱ

	swordwaveNum = 20;
	//��ɫ���������¿�ʼ��Ϸ�����´���
	createSwordWave();

	m_player_magnification = 2.5;

	m_hp = 100;
	m_mp = 100;

	//��ʱŪ100
	curLevel_Maxhp = 100;
	curLevel_Maxmp = 100;
	
	skillControl = SkillControl::create();
	addChild(skillControl);

	k_consumemp = 4;
	l_consumemp = 7;
	u_consumemp = 6;

	//��ʼ���ȼ�
	m_playerlevel = 1;
	//��ʼ��exp  
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
	/*�����ҳ����˵�ͼ�߽�*/
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


	/////////////////////�������ƶ�
	if (m_Using_swordwave_Arr.size() > 0) {
		RemoteSkill* swordwave;
		for (int i = 0; i < m_Using_swordwave_Arr.size(); i++) { //����for(swordwave:m_Using_swordwave_Arr)�ᱼ��,���������ʽ��ѭ��û��̬�ж�size()
			swordwave = m_Using_swordwave_Arr.at(i);

			//��̬���ý�����vertexz
			auto p = swordwave->getPosition();
			p = CC_POINT_POINTS_TO_PIXELS(p);

			swordwave->setVertexZ(-((p.y + 64) / 64));

			switch (swordwave->getSkillDir())
			{
			case enum_remoteskill_up: {
				//����Ļ��
				//�����ڵ�ͼ��Ե���ӻ�����Ϊ������Ļ��С�����Ƿǵ�ͼ��Ե���ӻ�����Ϊһ����Ļ��С��ȡȫ����֤
				Size visiblesize = Director::getInstance()->getVisibleSize();
				visiblesize.width += 1;
				visiblesize.height += 1;//����Ļ���һ���
				Vec2 pos = (swordwave->getPosition() + Vec2(0, -(swordwave->getContentSize().height / 2))) - this->getPosition();
				//������������Ļ�߶�
				if (abs(pos.x) > visiblesize.width || abs(pos.y) > visiblesize.height) {
					swordwave->hide();//���ؽ���
					m_swordwave_Arr.pushBack(swordwave);//�ѽ�����m_swordwave_Arr����
					m_Using_swordwave_Arr.eraseObject(swordwave);//��m_Using_swordwave���Ƴ�
					i--;//m_Using_swordwave_Arr��Pop��һ����ԭλ�ñ���������һ��Ԫ��ȡ����ѭ��������i++�жϵ�ǰԪ��
				}
				//�����ϰ�����
				else if (IsSussesfulForsetRemoteSkillPos(swordwave->getPosition() + Vec2(0, 6)) == false) {
					swordwave->hide();//���ؽ���
					m_swordwave_Arr.pushBack(swordwave);//�ѽ�����m_swordwave_Arr����
					m_Using_swordwave_Arr.eraseObject(swordwave);//��m_Using_swordwave���Ƴ�
					i--;//m_Using_swordwave_Arr��Pop��һ����ԭλ�ñ���������һ��Ԫ��ȡ����ѭ��������i++�жϵ�ǰԪ��
				}
				//û�����ϰ���
				else {
					swordwave->setSkillMove(Vec2(0, 6));
				}
				break;
			}
			case enum_remoteskill_down: {
				//����Ļ��
				//�����ڵ�ͼ��Ե���ӻ�����Ϊ������Ļ��С�����Ƿǵ�ͼ��Ե���ӻ�����Ϊһ����Ļ��С��ȡȫ����֤
				Size visiblesize = Director::getInstance()->getVisibleSize();
				visiblesize.width += 1;
				visiblesize.height += 1;//����Ļ���һ���
				Vec2 pos = (swordwave->getPosition() + Vec2(0, swordwave->getContentSize().height / 2)) - this->getPosition();
				//������������Ļ�߶�
				if (abs(pos.x) > visiblesize.width || abs(pos.y) > visiblesize.height) {
					swordwave->hide();//���ؽ���
					m_swordwave_Arr.pushBack(swordwave);//�ѽ�����m_swordwave_Arr����
					m_Using_swordwave_Arr.eraseObject(swordwave);//��m_Using_swordwave���Ƴ�
					i--;//m_Using_swordwave_Arr��Pop��һ����ԭλ�ñ���������һ��Ԫ��ȡ����ѭ��������i++�жϵ�ǰԪ��
				}
				//�����ϰ�����
				else if (IsSussesfulForsetRemoteSkillPos(swordwave->getPosition() + Vec2(0, -6)) == false) {
					swordwave->hide();//���ؽ���
					m_swordwave_Arr.pushBack(swordwave);//�ѽ�����m_swordwave_Arr����
					m_Using_swordwave_Arr.eraseObject(swordwave);//��m_Using_swordwave���Ƴ�
					i--;//m_Using_swordwave_Arr��Pop��һ����ԭλ�ñ���������һ��Ԫ��ȡ����ѭ��������i++�жϵ�ǰԪ��
				}
				//û�����ϰ���
				else {
					swordwave->setSkillMove(Vec2(0, -6));
				}
				break;
			}
			case enum_remoteskill_left: {
				//����Ļ��
				//�����ڵ�ͼ��Ե���ӻ�����Ϊ������Ļ��С�����Ƿǵ�ͼ��Ե���ӻ�����Ϊһ����Ļ��С��ȡȫ����֤
				Size visiblesize = Director::getInstance()->getVisibleSize();
				visiblesize.width += 1;
				visiblesize.height += 1;//����Ļ���һ���
				Vec2 pos = (swordwave->getPosition() + Vec2(swordwave->getContentSize().width / 2, 0)) - this->getPosition();
				//������������Ļ�߶�
				if (abs(pos.x) > visiblesize.width || abs(pos.y) > visiblesize.height) {
					swordwave->hide();//���ؽ���
					m_swordwave_Arr.pushBack(swordwave);//�ѽ�����m_swordwave_Arr����
					m_Using_swordwave_Arr.eraseObject(swordwave);//��m_Using_swordwave���Ƴ�
					i--;//m_Using_swordwave_Arr��Pop��һ����ԭλ�ñ���������һ��Ԫ��ȡ����ѭ��������i++�жϵ�ǰԪ��
				}
				//�����ϰ�����
				else if (IsSussesfulForsetRemoteSkillPos(swordwave->getPosition() + Vec2(-6, 0)) == false) {
					swordwave->hide();//���ؽ���
					m_swordwave_Arr.pushBack(swordwave);//�ѽ�����m_swordwave_Arr����
					m_Using_swordwave_Arr.eraseObject(swordwave);//��m_Using_swordwave���Ƴ�
					i--;//m_Using_swordwave_Arr��Pop��һ����ԭλ�ñ���������һ��Ԫ��ȡ����ѭ��������i++�жϵ�ǰԪ��
				}
				//û�����ϰ���
				else {
					swordwave->setSkillMove(Vec2(-6, 0));
				}
				break;
			}
			case enum_remoteskill_right: {
				//����Ļ��
				//�����ڵ�ͼ��Ե���ӻ�����Ϊ������Ļ��С�����Ƿǵ�ͼ��Ե���ӻ�����Ϊһ����Ļ��С��ȡȫ����֤
				Size visiblesize = Director::getInstance()->getVisibleSize();
				visiblesize.width += 1;
				visiblesize.height += 1;//����Ļ���һ���
				Vec2 pos = (swordwave->getPosition() + Vec2(-(swordwave->getContentSize().width / 2), 0)) - this->getPosition();
				//������������Ļ�߶�
				if (abs(pos.x) > visiblesize.width || abs(pos.y) > visiblesize.height) {
					swordwave->hide();//���ؽ���
					m_swordwave_Arr.pushBack(swordwave);//�ѽ�����m_swordwave_Arr����
					m_Using_swordwave_Arr.eraseObject(swordwave);//��m_Using_swordwave���Ƴ�
					i--;//m_Using_swordwave_Arr��Pop��һ����ԭλ�ñ���������һ��Ԫ��ȡ����ѭ��������i++�жϵ�ǰԪ��
				}
				//�����ϰ�����
				else if (IsSussesfulForsetRemoteSkillPos(swordwave->getPosition() + Vec2(6, 0)) == false) {
					swordwave->hide();//���ؽ���
					m_swordwave_Arr.pushBack(swordwave);//�ѽ�����m_swordwave_Arr����
					m_Using_swordwave_Arr.eraseObject(swordwave);//��m_Using_swordwave���Ƴ�
					i--;//m_Using_swordwave_Arr��Pop��һ����ԭλ�ñ���������һ��Ԫ��ȡ����ѭ��������i++�жϵ�ǰԪ��
				}
				//û�����ϰ���
				else {
					swordwave->setSkillMove(Vec2(6, 0));
				}
				break;
			}
			default:
				break;
			}

			//��ʱ�����ж�m_Using_swordwave�Ƿ�Ϊ�գ�Ϊ�յĻ��Ͳ��ܼ��������ˣ���Ϊ�Ѿ����ܴ�m_Using_swordwave��erase��m_Using_swordwaveΪ���ˣ����Ǹ���̬�仯�Ĺ���
			if (m_Using_swordwave_Arr.size() == 0) {
				break;
			}
		}
	}


	//////////////////////////////////////////���Ǳ�����
	if (attackedqueue.size() > 0) {
		//���Ǳ��������˽�ֱʱ��,��pop������
		if (timecounter_attacked->getCurTime() > 0.11f) {
			/*��ֹ���ǻ���Ч��������*/
			this->getSprite()->setAnchorPoint(Vec2(0.5, 0));
			this->getSprite()->setPosition(Vec2(this->getContentSize().width / 2,
				0));
			this->getSprite()->setRotation(0);
			/*��ֹ���ǻ���Ч��������*/
			/*��ֹ����Ⱦɫ���û�ָ�*/
			/*CCTintTo* action = CCTintTo::create(0.01f, m_playerColor);
			this->getSprite()->runAction(action);*/
			this->getSprite()->setColor(m_playerColor);

			attackedqueue.pop();
			//���ü�ʱΪ0������������Ϣ����������Ϣsize�ٴδ���0ʱ���������ϱ�pop�����������µ�����timecouter_attacked->start���ж��Ƿ�pop��
			timecounter_attacked->setstartTimeZeroAndcloseSchedule();
		}

		//��������ֱ���
		if (attackedqueue.size() > 0 && timecounter_attacked->getCurTime() == 0.0f)
		{
			int attack = attackedqueue.front();

			/*���Ǳ���������*/
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
				//�������û�ϰ���ͱ�����
				this->setPlayerPosition(this->getPosition() + Vec2(6, 0));
				//������hp
				m_hp--;

				//�����run״̬�±������ˣ���runת��Ϊwalk
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

				//������״̬���޷��ͷ����Ǽ���
				playerIsattacked = true;

				//�������ǵ�״̬
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
				//�������û�ϰ���ͱ�����
				this->setPlayerPosition(this->getPosition() + Vec2(-6, 0));
				//������hp
				m_hp--;

				//�����run״̬�±������ˣ���runת��Ϊwalk
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

				//������״̬���޷��ͷ����Ǽ���
				playerIsattacked = true;

				//�������ǵ�״̬
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
				//�������û�ϰ���ͱ�����
				this->setPlayerPosition(this->getPosition() + Vec2(0, -6));
				//������hp
				m_hp--;

				//�����run״̬�±������ˣ���runת��Ϊwalk
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

				//������״̬���޷��ͷ����Ǽ���
				playerIsattacked = true;

				//�������ǵ�״̬
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
				//�������û�ϰ���ͱ�����
				this->setPlayerPosition(this->getPosition() + Vec2(0, 6));

				//�����run״̬�±������ˣ���runת��Ϊwalk
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

				//������״̬���޷��ͷ����Ǽ���
				playerIsattacked = true;

				//�������ǵ�״̬
				PlayerState = enum_playerattackedfromup;

				break;
			}
			}
		}

		return;//����Ϊ��ʱ�������ٱ�������,�Ϳ���ִ������Ķ�����
	}


	playerIsattacked = false;//��־���ǲ�Ϊ����״̬

	/*��ֹ���ǻ���Ч��������*/
	this->getSprite()->setAnchorPoint(Vec2(0.5, 0));
	this->getSprite()->setPosition(Vec2(this->getContentSize().width / 2,
		0));
	this->getSprite()->setRotation(0);
	/*��ֹ���ǻ���Ч��������*/
	/*��ֹ����Ⱦɫ���û�ָ�*/
	this->getSprite()->setColor(m_playerColor);
	/*��ֹ����Ⱦɫ���û�ָ�*/

	/////////////////////�����������߻���
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
						swordwave->setPosition(this->getPosition() + Vec2(0, this->getContentSize().height));//�ѽ����������λ�ó�ʼ��
						m_Using_swordwave_Arr.pushBack(swordwave);//�ѽ��������������Ԫ�طŵ�����ǰ�ͷŵĽ�������������
						m_swordwave_Arr.popBack();//pop��
					}

				}

				PlayerState = enum_swordwave;
				return;
			}
			case enum_laserskill: {
				if (PlayerState != enum_laserskill) {
					Animation* animation = AnimationUtil::createWithSingleFrameName("uswordwave", 0.1f, 1);//���׵缼�ܶ���
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
					Animation* animation = AnimationUtil::createWithSingleFrameName("uswordwave", 0.1f, 1);//�Ż����ܶ���
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
						swordwave->setPosition(this->getPosition() + Vec2(0, 0));//�ѽ����������λ�ó�ʼ��
						m_Using_swordwave_Arr.pushBack(swordwave);//�ѽ��������������Ԫ�طŵ�����ǰ�ͷŵĽ�������������
						m_swordwave_Arr.popBack();//pop��
					}
				}

				PlayerState = enum_swordwave;
				return;
			}
			case enum_laserskill: {
				if (PlayerState != enum_laserskill) {
					Animation* animation = AnimationUtil::createWithSingleFrameName("dswordwave", 0.1f, 1);//���׵缼�ܶ���
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
					Animation* animation = AnimationUtil::createWithSingleFrameName("dswordwave", 0.1f, 1);//�Ż����ܶ���
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
						swordwave->setPosition(this->getPosition() + Vec2(-this->getContentSize().width / 2, this->getContentSize().height / 2));//�ѽ����������λ�ó�ʼ��
						m_Using_swordwave_Arr.pushBack(swordwave);//�ѽ��������������Ԫ�طŵ�����ǰ�ͷŵĽ�������������
						m_swordwave_Arr.popBack();//pop��
					}
				}

				PlayerState = enum_swordwave;
				return;
			}
			case enum_laserskill: {
				if (PlayerState != enum_laserskill) {
					Animation* animation = AnimationUtil::createWithSingleFrameName("hswordwave", 0.1f, 1);//���׵缼�ܶ���
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
					Animation* animation = AnimationUtil::createWithSingleFrameName("hswordwave", 0.1f, 1);//�Ż����ܶ���
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
						swordwave->setPosition(this->getPosition() + Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));//�ѽ����������λ�ó�ʼ��
						m_Using_swordwave_Arr.pushBack(swordwave);//�ѽ��������������Ԫ�طŵ�����ǰ�ͷŵĽ�������������
						m_swordwave_Arr.popBack();//pop��
					}
				}

				PlayerState = enum_swordwave;
				return;
			}
			case enum_laserskill: {
				if (PlayerState != enum_laserskill) {
					Animation* animation = AnimationUtil::createWithSingleFrameName("hswordwave", 0.1f, 1);//���׵缼�ܶ���
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
					Animation* animation = AnimationUtil::createWithSingleFrameName("hswordwave", 0.1f, 1);//�Ż����ܶ���
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
	////////////////////�߻���
	else if (vec.size() > 0) {
		if (vec.size() > 1)
		{
			/*��������һ��Ŀ���,,���ڿ���ʱ�ھ�ֹʱ���еģ����Կ�����Զ��vec[0],*/
			if (vec[0] == enum_doubleup && vec.back() == enum_left) {
				// (��������)
				//��ȡ��ͼ��ı�ţ������������߽�����λ�õĵ�ͼ��Ƭ��
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(-this->getContentSize().width / 2 - 2, 0));
				//����������Ҫ��ǰ�ж�һ���ҵ�ͼ���ҵ��󻬹���Ƭ�ı߾ͻ������������ҪvId.x +����
				vId.x = vId.x + 1;
				Vec2 vIdleft;
				vIdleft.x = vId.x - 1; vIdleft.y = vId.y;

				//�������Խ�����λ�õĵ�ͼ��Ƭ
				vId = this->tiledCoordForPosition(this->getPosition() + Vec2(0,2));
				//����������Ҫ��ǰ�ж�һ����Ƭ���ҵ�ͼ���µ��ϻ�����Ƭ�ı߾ͻ��һ��������ҪvId.y + 1;
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
				//�����߻��ϱ����ϰ����ֹ�Խ��ߴ�ǽ�ܹ�ȥ
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
				//��ͼ�±߽�Ĵ��� vIdleft����Χ��,����vIdup�����������ϰ���
				else if (vIdleft.y > yMax && IsNot_CollidableTile(vIdup))
				{
					IsTwoDir = this->setPlayerPosition(this->getPosition() + Vec2(-4, 6));
				}
				
				//���������������ģ���ô���Կ�ǽ(�ϰ���)������
				if (IsTwoDir == false)
				{
					nearBarrierMove = true;
					//��������,���ϰ�������
					if (!this->setPlayerPosition(this->getPosition() + Vec2(0, 6)))
					{
						//�������ߣ��ϰ������Ϸ�
						this->setPlayerPosition(this->getPosition() + Vec2(-4, 0));
					}			
				}
				
				PlayerDir = em_up;
				return;
			}
			else if (vec[0] == enum_doubleup && vec.back() == enum_right) {
				//(���ϡ�����)����
				//��ȡ��ͼ��ı��
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(this->getContentSize().width / 2 + 2, 0));
				//�������һ�����Ƭ��ʱ���1������Ҫ��ǰ�ж�һ������vId.x - 1;
				vId.x = vId.x - 1;
				Vec2 vIdright;
				vIdright.x = vId.x + 1; vIdright.y = vId.y;

				vId = this->tiledCoordForPosition(this->getPosition() + Vec2(0,2));
				//�������ϻ�����Ƭʱ���һ������Ҫ��ǰ�ж�һ������vId.y + 1;
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
				//�����߻��ϱ����ϰ����ֹ�Խ��ߴ�ǽ�ܹ�ȥ
				int xMax = m_map->getMapSize().width - 1;
				int yMax = m_map->getMapSize().height - 1;
				if ((vIdright.x >= 0 && vIdright.x <= xMax && vIdup.x >= 0 && vIdup.x <= xMax) &&
					(vIdright.y >= 0 && vIdright.y <= yMax && vIdup.y >= 0 && vIdup.y <= yMax)) {
					if (barrier->getTileGIDAt(vIdright) == 0 && barrier->getTileGIDAt(vIdup) == 0 ||
						(IsNot_CollidableTile(vIdright) && IsNot_CollidableTile(vIdup))) {
						IsTwoDir = this->setPlayerPosition(this->getPosition() + Vec2(4, 6));
					}
				}
				//��ͼ���±߽�Ĵ���,vIdright����Χ��,����vIdupû����Χ���ǲ����ϰ���
				else if (vIdright.y > yMax && IsNot_CollidableTile(vIdup))
				{
					IsTwoDir = this->setPlayerPosition(this->getPosition() + Vec2(4, 6));
				}
				//�����ͼ�ұ߽糬��Χ���⣬��߽��tiledCoordForPosition���ǳ�����ͼ�߽�ģ�����Ҫ����
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
				//(���¡�����
				//��ȡ��ͼ��ı��
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(-this->getContentSize().width / 2 - 2, 0));
				//���ڴ������󻮹���Ƭ���һ��������ǰ�ж�һ������vId.x + 1;
				vId.x = vId.x + 1;
				Vec2 vIdleft;
				vIdleft.x = vId.x - 1; vIdleft.y = vId.y;

				vId = this->tiledCoordForPosition(this->getPosition() + Vec2(0,-2));
				//���ڴ������»�����Ƭ���1��������ǰ�ж�һ������vId.y - 1;
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
				//�����߻��±����ϰ����ֹ�Խ��ߴ�ǽ�ܹ�ȥ
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
				//(���¡�����)
				//��ȡ��ͼ��ı��
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(this->getContentSize().width / 2 + 2, 0));
				//�����һ�����ͼʱ���1��������ǰ�ж�һ������vId.x - 1;
				vId.x = vId.x - 1;
				Vec2 vIdright;
				vIdright.x = vId.x + 1; vIdright.y = vId.y;

				vId = this->tiledCoordForPosition(this->getPosition() + Vec2(0,-2));
				//���ϵ��»�����Ƭʱ���1��������ǰ�ж�һ������vId.y - 1;
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
				//�����߻��±����ϰ����ֹ�Խ��ߴ�ǽ�ܹ�ȥ
				int xMax = m_map->getMapSize().width - 1;
				int yMax = m_map->getMapSize().height - 1;
				if ((vIdright.x >= 0 && vIdright.x <= xMax && vIddown.x >= 0 && vIddown.x <= xMax) &&
					(vIdright.y >= 0 && vIdright.y <= yMax && vIddown.y >= 0 && vIddown.y <= yMax)) {
					if (barrier->getTileGIDAt(vIdright) == 0 && barrier->getTileGIDAt(vIddown) == 0 ||
						(IsNot_CollidableTile(vIdright) && IsNot_CollidableTile(vIddown))) {
						IsTwoDir = this->setPlayerPosition(this->getPosition() + Vec2(4, -6));
					}
				}
				//�����ͼ�ұ߽糬��Χ���⣬��߽��tiledCoordForPosition���ǳ�����ͼ�߽�ģ�����Ҫ����
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
				// (��������)
				//��ȡ��ͼ��ı�ţ������������߽�����λ�õĵ�ͼ��Ƭ��
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(-this->getContentSize().width / 2 - 2, 0));
				//����������Ҫ��ǰ�ж�һ���ҵ�ͼ���ҵ��󻬹���Ƭ�ı߾ͻ������������ҪvId.x +����
				vId.x = vId.x + 1;
				Vec2 vIdleft;
				vIdleft.x = vId.x - 1; vIdleft.y = vId.y;

				//�������Խ�����λ�õĵ�ͼ��Ƭ
				vId = this->tiledCoordForPosition(this->getPosition() + Vec2(0,2));
				//����������Ҫ��ǰ�ж�һ����Ƭ���ҵ�ͼ���µ��ϻ�����Ƭ�ı߾ͻ��һ��������ҪvId.y + 1;
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
				//�����ͼ���±߽磬vIdleft������Χ,����vIdup�������ǲ����ϰ���
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
				//(���¡�����
				//��ȡ��ͼ��ı��
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(-this->getContentSize().width / 2 - 2, 0));
				//���ڴ������󻮹���Ƭ���һ��������ǰ�ж�һ������vId.x + 1;
				vId.x = vId.x + 1;
				Vec2 vIdleft;
				vIdleft.x = vId.x - 1; vIdleft.y = vId.y;

				vId = this->tiledCoordForPosition(this->getPosition() + Vec2(0,-2));
				//���ڴ������»�����Ƭ���1��������ǰ�ж�һ������vId.y - 1;
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
				//(���ϡ�����)����
				//��ȡ��ͼ��ı��
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(this->getContentSize().width / 2 + 2, 0));
				//�������һ�����Ƭ��ʱ���1������Ҫ��ǰ�ж�һ������vId.x - 1;
				vId.x = vId.x - 1;
				Vec2 vIdright;
				vIdright.x = vId.x + 1; vIdright.y = vId.y;

				vId = this->tiledCoordForPosition(this->getPosition() + Vec2(0,2));
				//�������ϻ�����Ƭʱ���һ������Ҫ��ǰ�ж�һ������vId.y + 1;
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
				//��ͼ���±߽�,vIdright������Χ,����vIdup������Χ�������ϰ���
				else if (vIdright.y > yMax && IsNot_CollidableTile(vIdup))
				{
					IsTwoDir = this->setPlayerPosition(this->getPosition() + Vec2(6, 4));
				}
				//�����ͼ�ұ߽糬��Χ���⣬��߽��tiledCoordForPosition���ǳ�����ͼ�߽�ģ�����Ҫ����
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
				//(���¡�����)
				//��ȡ��ͼ��ı��
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(this->getContentSize().width / 2 + 2, 0));
				//�����һ�����ͼʱ���1��������ǰ�ж�һ������vId.x - 1;
				vId.x = vId.x - 1;
				Vec2 vIdright;
				vIdright.x = vId.x + 1; vIdright.y = vId.y;

				vId = this->tiledCoordForPosition(this->getPosition() + Vec2(0,-2));
				//���ϵ��»�����Ƭʱ���1��������ǰ�ж�һ������vId.y - 1;
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
				//�����ͼ�ұ߽糬��Χ���⣬��߽��tiledCoordForPosition���ǳ�����ͼ�߽�ģ�����Ҫ����
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
			//X or Y������ܣ���������෴���������
			else if ((vec[0] == enum_doubleright || vec[0] == enum_doubleleft || vec[0] == enum_doubleup
				|| vec[0] == enum_doubledown) && (vec.back() == enum_left || vec.back() == enum_right
					|| vec.back() == enum_up || vec.back() == enum_down)) {
				switch (vec[0]) //ת������Ϊ��ͨ����
				{
				case enum_doubleup: vec[0] = enum_up; break;
				case enum_doubledown: vec[0] = enum_down; break;
				case enum_doubleleft: vec[0] = enum_left; break;
				case enum_doubleright: vec[0] = enum_right; break;
				default:
					break;
				}
				//��ת������ͨ�����߷�ʽ��,return���������һ����������߻�run��switch����ִ��
				return;
			}

			/*��������һ�������*/ /*������������������������ĳ�����runһ��ȡ����vec[0]������ȡ���ڵ����ڶ���vec[size - 2]*/
			if (vec.back() == enum_up && vec[vec.size() - 2] == enum_left
				|| vec[vec.size() - 2] == enum_up && vec.back() == enum_left)
			{
				//(��������)��
				//��ȡ��ͼ��ı�ţ������������߽�����λ�õĵ�ͼ��Ƭ��
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(-this->getContentSize().width/2 - 2, 0));
				//����������Ҫ��ǰ�ж�һ���ҵ�ͼ���ҵ��󻬹���Ƭ�ı߾ͻ������������ҪvId.x +����
				vId.x = vId.x + 1;
				Vec2 vIdleft; 
				vIdleft.x = vId.x - 1; vIdleft.y = vId.y;

				//�������Խ�����λ�õĵ�ͼ��Ƭ
				vId = this->tiledCoordForPosition(this->getPosition() + Vec2(0,2));
				//����������Ҫ��ǰ�ж�һ����Ƭ���ҵ�ͼ���µ��ϻ�����Ƭ�ı߾ͻ��һ��������ҪvId.y + 1;
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
				//��ͼ�±߽�Ĵ���vIdleft�᲻���ڵ�ͼ��Χ��,��vIdup�ڵ�ͼ�ڵ��������ϰ���
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
				//(���ϡ�����)
				//��ȡ��ͼ��ı��
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(this->getContentSize().width / 2 + 2, 0));
				//�������һ�����Ƭ��ʱ���1������Ҫ��ǰ�ж�һ������vId.x - 1;
				vId.x = vId.x - 1;
				Vec2 vIdright; 
				vIdright.x = vId.x + 1; vIdright.y = vId.y;
				
				vId = this->tiledCoordForPosition(this->getPosition() + Vec2(0,2));
				//�������ϻ�����Ƭʱ���һ������Ҫ��ǰ�ж�һ������vId.y + 1;
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
				//�����ͼ�±߽����⣬vIdright���ڷ�Χ��,��vIdup�ڷ�Χ�ڲ��Ҳ����ϰ���
				else if (vIdright.y > yMax && IsNot_CollidableTile(vIdup))
				{
					IsTwoDir = this->setPlayerPosition(this->getPosition() + Vec2(4, 4));
				}
				//�����ͼ�ұ߽糬��Χ���⣬��߽��tiledCoordForPosition���ǳ�����ͼ�߽�ģ�����Ҫ����
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
				//(���¡�����
				//��ȡ��ͼ��ı��
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(-this->getContentSize().width / 2 - 2,0));
				//���ڴ������󻮹���Ƭ���һ��������ǰ�ж�һ������vId.x + 1;
				vId.x = vId.x + 1;
				Vec2 vIdleft; 
				vIdleft.x = vId.x - 1; vIdleft.y = vId.y;

				vId = this->tiledCoordForPosition(this->getPosition() + Vec2(0,-2));
				//���ڴ������»�����Ƭ���1��������ǰ�ж�һ������vId.y - 1;
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
				//(���¡�����)
				//��ȡ��ͼ��ı��
				Vec2 vId = this->tiledCoordForPosition(this->getPosition() + Vec2(this->getContentSize().width / 2 + 2,0));
				//�����һ�����ͼʱ���1��������ǰ�ж�һ������vId.x - 1;
				vId.x = vId.x - 1;
				Vec2 vIdright; 
				vIdright.x = vId.x + 1; vIdright.y = vId.y;

				vId = this->tiledCoordForPosition(this->getPosition() + Vec2(0,-2));
				//���ϵ��»�����Ƭʱ���1��������ǰ�ж�һ������vId.y - 1;
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
				//�����ͼ�ұ߽糬��Χ���⣬��߽��tiledCoordForPosition���ǳ�����ͼ�߽�ģ�����Ҫ����
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
		//һ����������߻�run //��ʱ��vecҪ�����0����//һ�������run��walk��������Ҫ��PlayerDir������setPlayerPositionǰ�棬��Ϊ��setPlayerPosition����Ҫ�õ�PlayerDir��
		//��������ľͿ��������ˣ���Ϊ����������Ļ�һ���϶��Ǹ��ӷ��򣬶�������϶�����PlayerDir���������swithc(vec.back())��������
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
			//PlayerDir������setPlayerPositionǰ�棬��Ϊ��setPlayerPosition����Ҫ�õ�PlayerDir
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
			//PlayerDir������setPlayerPositionǰ�棬��Ϊ��setPlayerPosition����Ҫ�õ�PlayerDir
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
			//PlayerDir������setPlayerPositionǰ�棬��Ϊ��setPlayerPosition����Ҫ�õ�PlayerDir
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
			//PlayerDir������setPlayerPositionǰ�棬��Ϊ��setPlayerPosition����Ҫ�õ�PlayerDir
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
			//PlayerDir������setPlayerPositionǰ�棬��Ϊ��setPlayerPosition����Ҫ�õ�PlayerDir
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
			//PlayerDir������setPlayerPositionǰ�棬��Ϊ��setPlayerPosition����Ҫ�õ�PlayerDir
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
			//PlayerDir������setPlayerPositionǰ�棬��Ϊ��setPlayerPosition����Ҫ�õ�PlayerDir
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
			//PlayerDir������setPlayerPositionǰ�棬��Ϊ��setPlayerPosition����Ҫ�õ�PlayerDir
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
	//else vec.size() ���� 0	
	else
	{
		//	//���Ǿ�ֹ
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
		//��ʼ��ʱ
		timecounter_up->start(); //��һ��startʱcurtime��0
								 //�ϳ�,��ֹ״̬�����ϳ幦��
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
		//��ʼ��ʱ
		timecounter_down->start();
		//�³�,��ֹ״̬�����³�
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
		//��ʼ��ʱ
		timecounter_left->start();
		//���,��ֹת̬ʱ�������
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
		//��ʼ��ʱ
		timecounter_right->start();
		//�ҳ�,��ֹ״̬ʱ�����ҳ�
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
		//����J���ܵĹ�����ȴʱ��
		float curtime = timecounter_J->getCurTime();
		//��һ�ΰ�Jʱ����curtime = 0,�˺�ÿ��0.4f����ܰ���һ�μ���
		//���ǷǱ���״̬���ܷż���
		if (curtime == 0 || curtime > 0.3f && (playerIsattacked == false)) {

			timecounter_J->start();//һֱ��ʱ
								   //sizeΪ0������ͨ�Ĺ���
			if (vecskill.size() == 0) {
				//������Ч
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Jswordsound.wav");

				if (PlayerState == enum_doubleup || PlayerState == enum_doubledown
					|| PlayerState == enum_doubleleft || PlayerState == enum_doubleright) {
					vecskill.push_back(enum_basepoke);
					vecskillstr.push_back(baseskillstr(enum_basepoke, false));
					switch (PlayerState)//run״̬��һ֡ʹ��basepoke����ʱ��ʹvec[0](�����run״̬��vec[0]��Զ������run״̬)��Ϊwalk
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
		//����K���ܵĹ�����ȴʱ��//����
		float curtime = timecounter_J->getCurTime();
		//��һ�ΰ�Kʱ����curtime = 0,�˺�ÿ��07f����ܰ���һ�μ���
		//���ǷǱ���״̬���ܷż���
		if (curtime == 0 || curtime > 0.7f && (playerIsattacked == false) && m_mp >= k_consumemp) {
			//��mp
			m_mp -= k_consumemp;
			timecounter_J->start();//һֱ��ʱ
								   //sizeΪ0���н���
			if (vecskill.size() == 0) {
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/Kskill.wav"); 

				if (PlayerState == enum_doubleup || PlayerState == enum_doubledown
					|| PlayerState == enum_doubleleft || PlayerState == enum_doubleright) {
					vecskill.push_back(enum_swordwave);
					switch (PlayerState)//run״̬��һ֡ʹ��swordwave����ʱ��ʹvec[0](�����run״̬��vec[0]��Զ������run״̬)��Ϊwalk
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
					vecskill.push_back(enum_swordwave);//walk״̬Ҳ��swordwave����
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
					switch (PlayerState)//run״̬��һ֡ʹ���׵缼��ʱ��ʹvec[0](�����run״̬��vec[0]��Զ������run״̬)��Ϊwalk
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
					vecskill.push_back(enum_laserskill);//walk״̬Ҳ��swordwave����
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

		//���������˫�����ߵģ���vec.size() > 1������ʱ��ǽ�ߣ���ô�����ǰ�����˫����Ŀ�ǽ�������ߣ�vec.size() > 1�����
		//���⴦����
		if (nearBarrierMove == true)
		{
			nearBarrierMove = false;
			positionVleft = position;
			positionVleft.x -= this->getContentSize().width * this->getAnchorPoint().x;
			positionVright = position;
			positionVright.x += this->getContentSize().width * this->getAnchorPoint().x;
		}

		//run����walk���ӵķ�����ȡ����vec.back(),���Ҹ��ӵķ�����ö��enum_up,enum_left,enum_down,enum_right
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
		//�������Ϸ���walk or run��Ϊ����ê����0.5 0������ֻ�������ߣ�����������Ŀ�ȣ����ԼӶ����ҿ�ȵ��ж�
		else {
			positionVleft = position;
			positionVleft.x -= this->getContentSize().width * this->getAnchorPoint().x;
			positionVright = position;
			positionVright.x += this->getContentSize().width * this->getAnchorPoint().x;
		}

		break;
	}
	case em_down: {

		//���������˫�����ߵģ���vec.size() > 1������ʱ��ǽ�ߣ���ô�����ǰ�����˫����Ŀ�ǽ�������ߣ�vec.size() > 1�����
		//���⴦����
		if (nearBarrierMove == true)
		{
			nearBarrierMove = false;
			positionVleft = position;
			positionVleft.x -= this->getContentSize().width * this->getAnchorPoint().x;
			positionVright = position;
			positionVright.x += this->getContentSize().width * this->getAnchorPoint().x;
		}

		//run����walk���ӵķ�����ȡ����vec.back(),���Ҹ��ӵķ�����ö��enum_up,enum_left,enum_down,enum_right
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
		//�������·���walk or run��Ϊ����ê����0.5 0.5������ֻ�������ߣ�����������Ŀ�ȣ����ԼӶ����ҿ�ȵ��ж�
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
		//run����walk���ӵķ�����ȡ����vec.back(),���Ҹ��ӵķ�����ö��enum_up,enum_left,enum_down,enum_right
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
		position.x += this->getContentSize().width  * this->getAnchorPoint().x;//����������
		 //run����walk���ӵķ�����ȡ����vec.back(),���Ҹ��ӵķ�����ö��enum_up,enum_left,enum_down,enum_right
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

	switch (PlayerDir)//������run or walk��������������Ȳ����㣬�������ϰ���,��return 
	{
	case em_up: {
		Vec2 tileCoord = this->tiledCoordForPosition(positionVleft);

		if (tileCoord.x >= 0 && tileCoord.x < m_map->getMapSize().width //��������Ƭ��ͼ����
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

		if (tileCoord.x >= 0 && tileCoord.x < m_map->getMapSize().width //��������Ƭ��ͼ����
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

		if (tileCoord.x >= 0 && tileCoord.x < m_map->getMapSize().width //��������Ƭ��ͼ����
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

		if (tileCoord.x >= 0 && tileCoord.x < m_map->getMapSize().width //��������Ƭ��ͼ����
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

	Vec2 tileCoord = this->tiledCoordForPosition(position); //����������ê��Ϊ�ж���������run or walk�Ƿ���ײ

	if (tileCoord.x >= 0 && tileCoord.x < m_map->getMapSize().width //��������Ƭ��ͼ����
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

	//��ԭposition
	switch (PlayerDir) {
	case em_up: {
		//position.y -= this->getContentSize().height * this->getAnchorPoint().y;

		//run����walk���ӵķ�����ȡ����vec.back(),���Ҹ��ӵķ�����ö��enum_up,enum_left,enum_down,enum_right
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
		//�������Ϸ���walk or run��Ϊ����ê����0.5 0.5������ֻ�������ߣ�����������Ŀ�ȣ����ԼӶ����ҿ�ȵ��ж�
		else {
			positionVleft = position;
			positionVleft.x += this->getContentSize().width * this->getAnchorPoint().x;
			positionVright = position;
			positionVright.x -= this->getContentSize().width * this->getAnchorPoint().x;
		}

		break;
	}
	case em_down: {

		//run����walk���ӵķ�����ȡ����vec.back(),���Ҹ��ӵķ�����ö��enum_up,enum_left,enum_down,enum_right
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
		//�������·���walk or run��Ϊ����ê����0.5 0.5������ֻ�������ߣ�����������Ŀ�ȣ����ԼӶ����ҿ�ȵ��ж�
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
		//run����walk���ӵķ�����ȡ����vec.back(),���Ҹ��ӵķ�����ö��enum_up,enum_left,enum_down,enum_right
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
		position.x -= this->getContentSize().width  * this->getAnchorPoint().x;//����������
		//run����walk���ӵķ�����ȡ����vec.back(),���Ҹ��ӵķ�����ö��enum_up,enum_left,enum_down,enum_right
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
	if (tileCoord.x >= 0 && tileCoord.x < m_map->getMapSize().width //��������Ƭ��ͼ����
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
		//���ý���swordwave��Node�ڵ��ê��Ϊ0.5 0.5;
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
	if (tileCoord.x >= 0 && tileCoord.x < m_map->getMapSize().width //��������Ƭ��ͼ����
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
	//��ȡ���Ǽ���J��K������������������д��vecskillԪ�����Ϊһ����
	auto vecskill = this->getVecSkill();
	if (PlayerState != enum_baseattack || PlayerState != enum_basepoke)
	{
		collidedVector.clear();
	}

	//��ͨ�����˺�������6
	if (vecskill.size() > 0)
	{
		//�������ͨ��������ǰ�幥��
		if (vecskill.back() == enum_baseattack || vecskill.back() == enum_basepoke)
		{
			auto& vecskillstruct = this->getvecskillstr();//��ȡ������һ������
			if (vecskillstruct.size() > 0)
			{
				if (vecskillstruct.back().b == false)
				{
					vecskillstruct.back().b = true;//��־������ͨ�������ж����;

					// ��ײ���
					Vector<Monster*> monsVec = MonsterManager::getInstance()->getMonsterVec();
					int playerattackRange = 32;
					Vec2 vec;
					//���ǵ���ͨ������ʱд��һ���㡣
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
								/*���Ź��ﱻ��ͨ����������Ч*/
								CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/monhitedbyPlayerBaseskill.wav");

								int i;
								for (i = collidedVector.size() - 1; i >= 0; i--)
								{
									if (collidedVector.at(i) == mons)
										break;
								}
								if (i < 0)
								{
									mons->cmd_hurt(6); //��ͨ�������˺�
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
								/*���Ź��ﱻ��ͨ����������Ч*/
								CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/monhitedbyPlayerBaseskill.wav");

								int i;
								for (i = collidedVector.size() - 1; i >= 0; i--)
								{
									if (collidedVector.at(i) == mons)
										break;
								}
								if (i < 0)
								{
									mons->cmd_hurt(6); //��ͨ�������˺�
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
								/*���Ź��ﱻ��ͨ����������Ч*/
								CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/monhitedbyPlayerBaseskill.wav");

								int i;
								for (i = collidedVector.size() - 1; i >= 0; i--)
								{
									if (collidedVector.at(i) == mons)
										break;
								}
								if (i < 0)
								{
									mons->cmd_hurt(6); //��ͨ�������˺�
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
								/*���Ź��ﱻ��ͨ����������Ч*/
								CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/monhitedbyPlayerBaseskill.wav"); 

								int i;
								for (i = collidedVector.size() - 1; i >= 0; i--)
								{
									if (collidedVector.at(i) == mons)
										break;
								}
								if (i < 0)
								{
									mons->cmd_hurt(6); //��ͨ�������˺�
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
				/*�ж��Ƿ����л���ͼ������*/
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
				/*���K�����ܵĽ�������*/
				clearSwordWave();

				/*�л�����ʱ���ѱ���������ӹ����������Pop����*/
				auto& Vec = MonsterManager::getInstance()->getMonsterVec();
				Vec.clear();
				/*�л�����ʱ���ѱ�����NPC��NPC��������Pop����*/
				auto& Vec2 = NpcManager::getInstance()->getNpcsVec();
				Vec2.clear();
				/*һ��Ҫ��Pop������,�ٴ����³����������Ų�����³�������ҲPop��*/
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
	//����L����U�ļ���Ȼ��ͻȻ�л�ͼ�ˣ�L��U���ܵĶ�ʱ���ᱻ�ص�����getCurtime����cd��Χ�ڣ���ô����֮���һֱ���ܷż���
	//���԰�time��ʼ��Ϊ0
	for (int i = 0; i < MAX_SKILL_NUM; i++)
		skillControl->skillCounter[i]->setstartTimeZeroAndcloseSchedule();
}

void Player::LevelUpdate(float dt)
{
	if (m_exp >= 100 * (m_playerlevel))//��������Ϊ�ȼ����η�����
	{
		m_exp = m_exp - 100 * m_playerlevel;
		//����
		m_playerlevel += 1;

		/*�������hp,mp�������ǵȼ�����*/
		curLevel_Maxhp = 100 + (m_playerlevel - 1) * 20;
		curLevel_Maxmp = 100 + (m_playerlevel - 1) * 35;

		//��������hp,mp
		m_mp = curLevel_Maxmp;
		m_hp = curLevel_Maxhp;

		//������������������
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
		//ֹͣ���ֹͣ����
		this->setEnableAction(false);
		MonsterManager::getInstance()->setMonsEnableAction(false);

		Size size = Director::getInstance()->getVisibleSize();

		//����ʱ������һ����,�̵���������ʱ�Ͳ��ܵ㵽NPC����
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

		//�ؼ���ʱ��
		this->unschedule(schedule_selector(Player::PlayerDiedUpdate));

		//�����ǰ�������Ķ���
		while (!attackedqueue.empty())
			attackedqueue.pop();

		//ͣ����������
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		
		//ͣ�����ǻ�hp mp�Ķ�ʱ��
		void Playerhp_mp_Update(float dt);
		this->unschedule(schedule_selector(Player::recoverHp_Mp));

		//�л���startgamescene����
		//�л�����ǰ�ȴ�����ֵ����͸�������
		if (QuestDispatcher::getInstance()->getParent() != NULL) {
			QuestDispatcher::getInstance()->removeFromParentAndCleanup(false);
		}
		//��QuestDispatcher����������������Ϊ���һ�δ浵��״̬������û�д浵�����
		QuestDispatcher::getInstance()->InitQuestDispathcerForPlayerDied();
		//��TalkManager�ĶԻ�������Ϊ����һ�δ浵��״̬,����û�浵��ʱ��
		TalkManager::getInstance()->InitTalkForPlayerDied();

		/*���K�����ܵĽ�������*/
		clearSwordWave();
		/*�л�����ʱ���ѱ���������ӹ����������Pop����*/
		auto& Vec = MonsterManager::getInstance()->getMonsterVec();
		Vec.clear();
		/*�л�����ʱ���ѱ�����NPC��NPC��������Pop����*/
		auto& Vec2 = NpcManager::getInstance()->getNpcsVec();
		Vec2.clear();

		//������Ϸʧ������
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/gameover.mp3");

		//����ʱ������Ч
		Sprite* gameoversprite = Sprite::create("GameOver/gameover.png");
		if (m_map->getParent() != NULL)
			m_map->getParent()->addChild(gameoversprite);
		gameoversprite->setPosition(size.width / 2, size.height * 1.5);
		gameoversprite->setScale(size.width / gameoversprite->getContentSize().width, size.height / gameoversprite->getContentSize().height);

		MoveTo* moveto = MoveTo::create(3.0f, ccp(size.width / 2, size.height / 2));


		CallFunc* callfunc = CallFunc::create([&](){
			Label* dielabel = Label::create(gb2312_to_utf8("��Ϸ����"), "Arial", 55);
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
