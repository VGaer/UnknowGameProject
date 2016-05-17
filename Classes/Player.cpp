#include "Player.h"

Player* Player::createWithparent(TMXTiledMap* parent)
{
	Player* player = new Player();
	if (player){
		parent->addChild(player, (int)parent->getChildren().size());
	}
	if (player && player->init()){
		player->autorelease();
		return player;
	}
	else{
		delete player;
		player = NULL;
		return NULL;
	}
}

bool Player::init()
{
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(Player::keyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(Player::keyReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	this->scheduleUpdate();

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

	PlayerState = enum_initNone;//��ʼ��Ϊʲô��û��״̬��һ������Ϸ���û�����ͻ�תΪenum_static,�в���תΪ��Ӧ������walk or run״̬ 
	PlayerDir = em_down;//��ʼ��ʱ

	swordwaveNum = 20;
	createSwordWave();

	m_player_magnification = 2;

	return true;
}

void Player::update(float dt)
{
	/////////////////////�������ƶ�
	if (m_Using_swordwave_Arr.size() > 0){
		RemoteSkill* swordwave;
		for (int i = 0; i < m_Using_swordwave_Arr.size(); i++){ //����for(swordwave:m_Using_swordwave_Arr)�ᱼ��,���������ʽ��ѭ��û��̬�ж�size()
			swordwave = m_Using_swordwave_Arr.at(i);

			//��̬���ý�����vertexz
			auto p = swordwave->getPosition();
			p = CC_POINT_POINTS_TO_PIXELS(p);

			swordwave->setVertexZ(-((p.y + 64) / 64));

			switch (swordwave->getSkillDir())
			{
			case enum_remoteskill_up:{
				//����Ļ��
				//�����ڵ�ͼ��Ե���ӻ�����Ϊ������Ļ��С�����Ƿǵ�ͼ��Ե���ӻ�����Ϊһ����Ļ��С��ȡȫ����֤
				Size visiblesize = Director::getInstance()->getVisibleSize();
				visiblesize.width += 1;
				visiblesize.height += 1;//����Ļ���һ���
				Vec2 pos = (swordwave->getPosition() + Vec2(0,-(swordwave->getContentSize().height / 2))) - this->getPosition();
				//������������Ļ�߶�
				if (abs(pos.x) > visiblesize.width || abs(pos.y) > visiblesize.height){
					swordwave->hide();//���ؽ���
					m_swordwave_Arr.pushBack(swordwave);//�ѽ�����m_swordwave_Arr����
					m_Using_swordwave_Arr.eraseObject(swordwave);//��m_Using_swordwave���Ƴ�
					i--;//m_Using_swordwave_Arr��Pop��һ����ԭλ�ñ���������һ��Ԫ��ȡ����ѭ��������i++�жϵ�ǰԪ��
				}
				//�����ϰ�����
				else if (IsSussesfulForsetRemoteSkillPos(swordwave->getPosition() + Vec2(0, 6)) == false){
					swordwave->hide();//���ؽ���
					m_swordwave_Arr.pushBack(swordwave);//�ѽ�����m_swordwave_Arr����
					m_Using_swordwave_Arr.eraseObject(swordwave);//��m_Using_swordwave���Ƴ�
					i--;//m_Using_swordwave_Arr��Pop��һ����ԭλ�ñ���������һ��Ԫ��ȡ����ѭ��������i++�жϵ�ǰԪ��
				}
				//û�����ϰ���
				else{
					swordwave->setSkillMove(Vec2(0, 6));
				}
				break;
			}
			case enum_remoteskill_down:{
				//����Ļ��
				//�����ڵ�ͼ��Ե���ӻ�����Ϊ������Ļ��С�����Ƿǵ�ͼ��Ե���ӻ�����Ϊһ����Ļ��С��ȡȫ����֤
				Size visiblesize = Director::getInstance()->getVisibleSize();
				visiblesize.width += 1;
				visiblesize.height += 1;//����Ļ���һ���
				Vec2 pos = (swordwave->getPosition() + Vec2(0,swordwave->getContentSize().height / 2)) - this->getPosition();
				//������������Ļ�߶�
				if (abs(pos.x) > visiblesize.width || abs(pos.y) > visiblesize.height){
					swordwave->hide();//���ؽ���
					m_swordwave_Arr.pushBack(swordwave);//�ѽ�����m_swordwave_Arr����
					m_Using_swordwave_Arr.eraseObject(swordwave);//��m_Using_swordwave���Ƴ�
					i--;//m_Using_swordwave_Arr��Pop��һ����ԭλ�ñ���������һ��Ԫ��ȡ����ѭ��������i++�жϵ�ǰԪ��
				}
				//�����ϰ�����
				else if (IsSussesfulForsetRemoteSkillPos(swordwave->getPosition() + Vec2(0, -6)) == false){
					swordwave->hide();//���ؽ���
					m_swordwave_Arr.pushBack(swordwave);//�ѽ�����m_swordwave_Arr����
					m_Using_swordwave_Arr.eraseObject(swordwave);//��m_Using_swordwave���Ƴ�
					i--;//m_Using_swordwave_Arr��Pop��һ����ԭλ�ñ���������һ��Ԫ��ȡ����ѭ��������i++�жϵ�ǰԪ��
				}
				//û�����ϰ���
				else{
					swordwave->setSkillMove(Vec2(0, -6));
				}
				break;
			}
			case enum_remoteskill_left:{
				//����Ļ��
				//�����ڵ�ͼ��Ե���ӻ�����Ϊ������Ļ��С�����Ƿǵ�ͼ��Ե���ӻ�����Ϊһ����Ļ��С��ȡȫ����֤
				Size visiblesize = Director::getInstance()->getVisibleSize();
				visiblesize.width += 1;
				visiblesize.height += 1;//����Ļ���һ���
				Vec2 pos = (swordwave->getPosition() + Vec2(swordwave->getContentSize().width / 2,0)) - this->getPosition();
				//������������Ļ�߶�
				if (abs(pos.x) > visiblesize.width || abs(pos.y) > visiblesize.height){
					swordwave->hide();//���ؽ���
					m_swordwave_Arr.pushBack(swordwave);//�ѽ�����m_swordwave_Arr����
					m_Using_swordwave_Arr.eraseObject(swordwave);//��m_Using_swordwave���Ƴ�
					i--;//m_Using_swordwave_Arr��Pop��һ����ԭλ�ñ���������һ��Ԫ��ȡ����ѭ��������i++�жϵ�ǰԪ��
				}
				//�����ϰ�����
				else if (IsSussesfulForsetRemoteSkillPos(swordwave->getPosition() + Vec2(-6, 0)) == false){
					swordwave->hide();//���ؽ���
					m_swordwave_Arr.pushBack(swordwave);//�ѽ�����m_swordwave_Arr����
					m_Using_swordwave_Arr.eraseObject(swordwave);//��m_Using_swordwave���Ƴ�
					i--;//m_Using_swordwave_Arr��Pop��һ����ԭλ�ñ���������һ��Ԫ��ȡ����ѭ��������i++�жϵ�ǰԪ��
				}
				//û�����ϰ���
				else{
					swordwave->setSkillMove(Vec2(-6, 0));
				}
				break;
			}
			case enum_remoteskill_right:{
				//����Ļ��
				//�����ڵ�ͼ��Ե���ӻ�����Ϊ������Ļ��С�����Ƿǵ�ͼ��Ե���ӻ�����Ϊһ����Ļ��С��ȡȫ����֤
				Size visiblesize = Director::getInstance()->getVisibleSize();
				visiblesize.width += 1;
				visiblesize.height += 1;//����Ļ���һ���
				Vec2 pos = (swordwave->getPosition() + Vec2(-(swordwave->getContentSize().width / 2),0)) - this->getPosition();
				//������������Ļ�߶�
				if (abs(pos.x) > visiblesize.width || abs(pos.y) > visiblesize.height){
					swordwave->hide();//���ؽ���
					m_swordwave_Arr.pushBack(swordwave);//�ѽ�����m_swordwave_Arr����
					m_Using_swordwave_Arr.eraseObject(swordwave);//��m_Using_swordwave���Ƴ�
					i--;//m_Using_swordwave_Arr��Pop��һ����ԭλ�ñ���������һ��Ԫ��ȡ����ѭ��������i++�жϵ�ǰԪ��
				}
				//�����ϰ�����
				else if (IsSussesfulForsetRemoteSkillPos(swordwave->getPosition() + Vec2(6, 0)) == false){
					swordwave->hide();//���ؽ���
					m_swordwave_Arr.pushBack(swordwave);//�ѽ�����m_swordwave_Arr����
					m_Using_swordwave_Arr.eraseObject(swordwave);//��m_Using_swordwave���Ƴ�
					i--;//m_Using_swordwave_Arr��Pop��һ����ԭλ�ñ���������һ��Ԫ��ȡ����ѭ��������i++�жϵ�ǰԪ��
				}
				//û�����ϰ���
				else{
					swordwave->setSkillMove(Vec2(6, 0));
				}
				break;
			}
			default:
				break;
			}

			//��ʱ�����ж�m_Using_swordwave�Ƿ�Ϊ�գ�Ϊ�յĻ��Ͳ��ܼ��������ˣ���Ϊ�Ѿ����ܴ�m_Using_swordwave��erase��m_Using_swordwaveΪ���ˣ����Ǹ���̬�仯�Ĺ���
			if (m_Using_swordwave_Arr.size() == 0){
				break;
			}
		}
	}

	/////////////////////�����������߻���
	if (vecskill.size() == 1){
		switch (PlayerDir)
		{
		case em_up:{
			switch (vecskill[0])
			{
			case enum_baseattack:{
				switch ((int)(CCRANDOM_0_1() * 10) % 2)
				{
				case 0:{
					Animation* animation = AnimationUtil::createWithSingleFrameName("ubaseattack", 0.1f, 1);
					Animate* animate = Animate::create(animation);
					if (PlayerState != enum_baseattack){
						this->getPlayerSprite()->stopAllActions();
						CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
						this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));

						//CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(k);
						//k = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/swordsound.wav");
					}

					PlayerState = enum_baseattack;
					return;
				}
				case 1:{
					Animation* animation = AnimationUtil::createWithSingleFrameName("fubaseattack", 0.1f, 1);
					Animate* animate = Animate::create(animation);
					if (PlayerState != enum_baseattack){
						this->getPlayerSprite()->stopAllActions();
						CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
						this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));
					}

					PlayerState = enum_baseattack;
					return;
				}
				}
			}
			case enum_basepoke:{
				Animation* animation = AnimationUtil::createWithSingleFrameName("ubasepoke", 0.1f, 1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_basepoke){
					this->getPlayerSprite()->stopAllActions();
					CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
					this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));
				}

				PlayerState = enum_basepoke;
				return;
			}
			case enum_swordwave:{
				Animation* animation = AnimationUtil::createWithSingleFrameName("uswordwave", 0.1f, 1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_swordwave){
					this->getPlayerSprite()->stopAllActions();
					CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
					this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));

					if (m_swordwave_Arr.size() > 0){
						RemoteSkill* swordwave = m_swordwave_Arr.back();
						swordwave->setSkillDir(enum_remoteskill_up);
						swordwave->show();
						swordwave->setPosition(this->getPosition() + Vec2(0,this->getContentSize().height / 2));//�ѽ����������λ�ó�ʼ��
						m_Using_swordwave_Arr.pushBack(swordwave);//�ѽ��������������Ԫ�طŵ�����ǰ�ͷŵĽ�������������
						m_swordwave_Arr.popBack();//pop��
					}

				}

				PlayerState = enum_swordwave;
				return;
			}
			default:
				break;
			}
		}
		case em_down:{
			switch (vecskill[0])
			{
			case enum_baseattack:{
				switch ((int)(CCRANDOM_0_1() * 10) % 2)
				{
				case 0:{
					Animation* animation = AnimationUtil::createWithSingleFrameName("dbaseattack", 0.1f, 1);
					Animate* animate = Animate::create(animation);
					if (PlayerState != enum_baseattack){
						this->getPlayerSprite()->stopAllActions();
						CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
						this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));
					}

					PlayerState = enum_baseattack;
					return;
				}
				case 1:{
					Animation* animation = AnimationUtil::createWithSingleFrameName("fdbaseattack", 0.1f, 1);
					Animate* animate = Animate::create(animation);
					if (PlayerState != enum_baseattack){
						this->getPlayerSprite()->stopAllActions();
						CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
						this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));
					}

					PlayerState = enum_baseattack;
					return;
				}
				}
			}
			case enum_basepoke:{
				Animation* animation = AnimationUtil::createWithSingleFrameName("dbasepoke", 0.1f, 1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_basepoke){
					this->getPlayerSprite()->stopAllActions();
					CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
					this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));
				}

				PlayerState = enum_basepoke;
				return;
			}
			case enum_swordwave:{
				Animation* animation = AnimationUtil::createWithSingleFrameName("dswordwave", 0.1f, 1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_swordwave){
					this->getPlayerSprite()->stopAllActions();
					CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
					this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));

					if (m_swordwave_Arr.size() > 0){
						RemoteSkill* swordwave = m_swordwave_Arr.back();
						swordwave->setSkillDir(enum_remoteskill_down);
						swordwave->show();
						swordwave->setPosition(this->getPosition() + Vec2(0, -this->getContentSize().height / 2));//�ѽ����������λ�ó�ʼ��
						m_Using_swordwave_Arr.pushBack(swordwave);//�ѽ��������������Ԫ�طŵ�����ǰ�ͷŵĽ�������������
						m_swordwave_Arr.popBack();//pop��
					}
				}

				PlayerState = enum_swordwave;
				return;
			}
			default:
				break;
			}
		}
		case em_left:{
			switch (vecskill[0])
			{
			case enum_baseattack:{
				switch ((int)(CCRANDOM_0_1() * 10) % 2)
				{
				case 0:{
					Animation* animation = AnimationUtil::createWithSingleFrameName("hbaseattack", 0.1f, 1);
					Animate* animate = Animate::create(animation);
					if (PlayerState != enum_baseattack){
						this->getPlayerSprite()->stopAllActions();
						this->getPlayerSprite()->setScaleX(m_player_magnification);
						CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
						this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));
					}

					PlayerState = enum_baseattack;
					return;
				}
				case 1:{
					Animation* animation = AnimationUtil::createWithSingleFrameName("fhbaseattack", 0.1f, 1);
					Animate* animate = Animate::create(animation);
					if (PlayerState != enum_baseattack){
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
			case enum_basepoke:{
				Animation* animation = AnimationUtil::createWithSingleFrameName("hbasepoke", 0.1f, 1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_basepoke){
					this->getPlayerSprite()->stopAllActions();
					this->getPlayerSprite()->setScaleX(m_player_magnification);
					CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
					this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));
				}

				PlayerState = enum_basepoke;
				return;
			}
			case enum_swordwave:{
				Animation* animation = AnimationUtil::createWithSingleFrameName("hswordwave", 0.1f, 1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_swordwave){
					this->getPlayerSprite()->stopAllActions();
					this->getPlayerSprite()->setScaleX(m_player_magnification);
					CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
					this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));

					if (m_swordwave_Arr.size() > 0){
						RemoteSkill* swordwave = m_swordwave_Arr.back();
						swordwave->setSkillDir(enum_remoteskill_left);
						swordwave->show();
						swordwave->setPosition(this->getPosition() + Vec2(-this->getContentSize().width / 2, 0));//�ѽ����������λ�ó�ʼ��
						m_Using_swordwave_Arr.pushBack(swordwave);//�ѽ��������������Ԫ�طŵ�����ǰ�ͷŵĽ�������������
						m_swordwave_Arr.popBack();//pop��
					}
				}

				PlayerState = enum_swordwave;
				return;
			}
			default:
				break;
			}
		}
		case em_right:{
			switch (vecskill[0])
			{
			case enum_baseattack:{
				switch ((int)(CCRANDOM_0_1() * 10) % 2)
				{
				case 0:{
					Animation* animation = AnimationUtil::createWithSingleFrameName("hbaseattack", 0.1f, 1);
					Animate* animate = Animate::create(animation);
					if (PlayerState != enum_baseattack){
						this->getPlayerSprite()->stopAllActions();
						this->getPlayerSprite()->setScaleX(-m_player_magnification);
						CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
						this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));
					}

					PlayerState = enum_baseattack;
					return;
				}
				case 1:{
					Animation* animation = AnimationUtil::createWithSingleFrameName("fhbaseattack", 0.1f, 1);
					Animate* animate = Animate::create(animation);
					if (PlayerState != enum_baseattack){
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
			case enum_basepoke:{
				Animation* animation = AnimationUtil::createWithSingleFrameName("hbasepoke", 0.1f, 1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_basepoke){
					this->getPlayerSprite()->stopAllActions();
					this->getPlayerSprite()->setScaleX(-m_player_magnification);
					CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
					this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));
				}

				PlayerState = enum_basepoke;
				return;
			}
			case enum_swordwave:{
				Animation* animation = AnimationUtil::createWithSingleFrameName("hswordwave", 0.1f, 1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_swordwave){
					this->getPlayerSprite()->stopAllActions();
					this->getPlayerSprite()->setScaleX(-m_player_magnification);
					CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Player::CallBack1, this));
					this->getPlayerSprite()->runAction(Sequence::create(animate, callfunc, NULL));

					if (m_swordwave_Arr.size() > 0){
						RemoteSkill* swordwave = m_swordwave_Arr.back();
						swordwave->setSkillDir(enum_remoteskill_right);
						swordwave->show();
						swordwave->setPosition(this->getPosition() + Vec2(this->getContentSize().width / 2, 0));//�ѽ����������λ�ó�ʼ��
						m_Using_swordwave_Arr.pushBack(swordwave);//�ѽ��������������Ԫ�طŵ�����ǰ�ͷŵĽ�������������
						m_swordwave_Arr.popBack();//pop��
					}
				}

				PlayerState = enum_swordwave;
				return;
			}
			default:
				break;
			}
		}
		default:
			break;
		}
	}
	////////////////////�߻���
	else if (vec.size() > 0){
		if (vec.size() > 1)
		{
			/*��������һ��Ŀ���,,���ڿ���ʱ�ھ�ֹʱ���еģ����Կ�����Զ��vec[0],*/
			if (vec[0] == enum_doubleup && vec.back() == enum_left){
				Animation* animation = AnimationUtil::createWithSingleFrameName("urun", 0.15f, -1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_doubleup){
					this->getPlayerSprite()->stopAllActions();
					this->getPlayerSprite()->runAction(animate);
				}
				PlayerState = enum_doubleup;
				this->setPlayerPosition(this->getPosition() + Vec2(-4, 6));

				PlayerDir = em_up;
				return;
			}
			else if (vec[0] == enum_doubleup && vec.back() == enum_right){
				Animation* animation = AnimationUtil::createWithSingleFrameName("urun", 0.15f, -1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_doubleup){
					this->getPlayerSprite()->stopAllActions();
					this->getPlayerSprite()->runAction(animate);
				}
				PlayerState = enum_doubleup;
				this->setPlayerPosition(this->getPosition() + Vec2(4, 6));

				PlayerDir = em_up;
				return;
			}
			else if (vec[0] == enum_doubledown && vec.back() == enum_left){
				Animation* animation = AnimationUtil::createWithSingleFrameName("drun", 0.18f, -1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_doubledown){
					this->getPlayerSprite()->stopAllActions();
					this->getPlayerSprite()->runAction(animate);
				}
				PlayerState = enum_doubledown;
				this->setPlayerPosition(this->getPosition() + Vec2(-4, -6));

				PlayerDir = em_down;
				return;
			}
			else if (vec[0] == enum_doubledown && vec.back() == enum_right){
				Animation* animation = AnimationUtil::createWithSingleFrameName("drun", 0.18f, -1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_doubledown){
					this->getPlayerSprite()->stopAllActions();
					this->getPlayerSprite()->runAction(animate);
				}
				PlayerState = enum_doubledown;
				this->setPlayerPosition(this->getPosition() + Vec2(4, -6));

				PlayerDir = em_down;
				return;
			}
			else if (vec[0] == enum_doubleleft && vec.back() == enum_up){
				Animation* animation = AnimationUtil::createWithSingleFrameName("hrun", 0.2f, -1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_doubleleft){
					this->getPlayerSprite()->stopAllActions();
					this->getPlayerSprite()->setScaleX(m_player_magnification);
					this->getPlayerSprite()->runAction(animate);
				}
				PlayerState = enum_doubleleft;
				this->setPlayerPosition(this->getPosition() + Vec2(-6, 4));

				PlayerDir = em_left;
				return;
			}
			else if (vec[0] == enum_doubleleft && vec.back() == enum_down){
				Animation* animation = AnimationUtil::createWithSingleFrameName("hrun", 0.2f, -1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_doubleleft){
					this->getPlayerSprite()->stopAllActions();
					this->getPlayerSprite()->setScaleX(m_player_magnification);
					this->getPlayerSprite()->runAction(animate);
				}
				PlayerState = enum_doubleleft;
				this->setPlayerPosition(this->getPosition() + Vec2(-6, -4));

				PlayerDir = em_left;
				return;
			}
			else if (vec[0] == enum_doubleright && vec.back() == enum_up){
				Animation* animation = AnimationUtil::createWithSingleFrameName("hrun", 0.2f, -1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_doubleright){
					this->getPlayerSprite()->stopAllActions();
					this->getPlayerSprite()->setScaleX(-m_player_magnification);
					this->getPlayerSprite()->runAction(animate);
				}
				PlayerState = enum_doubleright;
				this->setPlayerPosition(this->getPosition() + Vec2(6, 4));

				PlayerDir = em_right;
				return;
			}
			else if (vec[0] == enum_doubleright && vec.back() == enum_down){
				Animation* animation = AnimationUtil::createWithSingleFrameName("hrun", 0.2f, -1);
				Animate* animate = Animate::create(animation);
				if (PlayerState != enum_doubleright){
					this->getPlayerSprite()->stopAllActions();
					this->getPlayerSprite()->setScaleX(-m_player_magnification);
					this->getPlayerSprite()->runAction(animate);
				}
				PlayerState = enum_doubleright;
				this->setPlayerPosition(this->getPosition() + Vec2(6, -4));

				PlayerDir = em_right;
				return;
			}
			//X or Y������ܣ���������෴���������
			else if ((vec[0] == enum_doubleright || vec[0] == enum_doubleleft || vec[0] == enum_doubleup
				|| vec[0] == enum_doubledown) && (vec.back() == enum_left || vec.back() == enum_right
				|| vec.back() == enum_up || vec.back() == enum_down)){
				switch (vec[0]) //ת������λ��ͨ����
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
				if (vec.back() == enum_up){
					Animation* animation = AnimationUtil::createWithSingleFrameName("hwalk", 0.2f, -1);
					Animate* animate = Animate::create(animation);
					if (PlayerState != enum_left){
						this->getPlayerSprite()->stopAllActions();
						this->getPlayerSprite()->setScaleX(m_player_magnification);
						this->getPlayerSprite()->runAction(animate);
					}
					PlayerState = enum_left;
					PlayerDir = em_left;
				}
				else{
					Animation* animation = AnimationUtil::createWithSingleFrameName("uwalk", 0.2f, -1);
					Animate* animate = Animate::create(animation);
					if (PlayerState != enum_up){
						this->getPlayerSprite()->stopAllActions();
						this->getPlayerSprite()->runAction(animate);
					}
					PlayerState = enum_up;
					PlayerDir = em_up;
				}
				this->setPlayerPosition(this->getPosition() + Vec2(-4, 4));
				return;
			}
			else if (vec.back() == enum_up && vec[vec.size() - 2] == enum_right
				|| vec[vec.size() - 2] == enum_up && vec.back() == enum_right)
			{
				if (vec.back() == enum_up){
					Animation* animation = AnimationUtil::createWithSingleFrameName("hwalk", 0.2f, -1);
					Animate* animate = Animate::create(animation);
					if (PlayerState != enum_right){
						this->getPlayerSprite()->stopAllActions();
						this->getPlayerSprite()->setScaleX(-m_player_magnification);
						this->getPlayerSprite()->runAction(animate);
					}
					PlayerState = enum_right;
					PlayerDir = em_right;
				}
				else{
					Animation* animation = AnimationUtil::createWithSingleFrameName("uwalk", 0.2f, -1);
					Animate* animate = Animate::create(animation);
					if (PlayerState != enum_up){
						this->getPlayerSprite()->stopAllActions();
						this->getPlayerSprite()->runAction(animate);
					}
					PlayerState = enum_up;
					PlayerDir = em_up;
				}
				this->setPlayerPosition(this->getPosition() + Vec2(4, 4));
				return;
			}
			else if (vec.back() == enum_down && vec[vec.size() - 2] == enum_left
				|| vec[vec.size() - 2] == enum_down && vec.back() == enum_left)
			{
				if (vec.back() == enum_down){
					Animation* animation = AnimationUtil::createWithSingleFrameName("hwalk", 0.2f, -1);
					Animate* animate = Animate::create(animation);
					if (PlayerState != enum_left){
						this->getPlayerSprite()->stopAllActions();
						this->getPlayerSprite()->setScaleX(m_player_magnification);
						this->getPlayerSprite()->runAction(animate);
					}
					PlayerState = enum_left;
					PlayerDir = em_left;
				}
				else{
					Animation* animation = AnimationUtil::createWithSingleFrameName("dwalk", 0.2f, -1);
					Animate* animate = Animate::create(animation);
					if (PlayerState != enum_down){
						this->getPlayerSprite()->stopAllActions();
						this->getPlayerSprite()->runAction(animate);
					}
					PlayerState = enum_down;
					PlayerDir = em_down;
				}
				this->setPlayerPosition(this->getPosition() + Vec2(-4, -4));
				return;
			}
			else if ((vec.back() == enum_down && vec[vec.size() - 2] == enum_right
				|| vec[vec.size() - 2] == enum_down && vec.back() == enum_right))
			{
				if (vec.back() == enum_down){
					Animation* animation = AnimationUtil::createWithSingleFrameName("hwalk", 0.2f, -1);
					Animate* animate = Animate::create(animation);
					if (PlayerState != enum_right){
						this->getPlayerSprite()->stopAllActions();
						this->getPlayerSprite()->setScaleX(-m_player_magnification);
						this->getPlayerSprite()->runAction(animate);
					}
					PlayerState = enum_right;
					PlayerDir = em_right;
				}
				else{
					Animation* animation = AnimationUtil::createWithSingleFrameName("dwalk", 0.2f, -1);
					Animate* animate = Animate::create(animation);
					if (PlayerState != enum_down){
						this->getPlayerSprite()->stopAllActions();
						this->getPlayerSprite()->runAction(animate);
					}
					PlayerState = enum_down;
					PlayerDir = em_down;
				}
				this->setPlayerPosition(this->getPosition() + Vec2(4, -4));
				return;
			}
		}
		//һ����������߻�run //��ʱ��vecҪ�����0����//һ�������run��walk��������Ҫ��PlayerDir������setPlayerPositionǰ�棬��Ϊ��setPlayerPosition����Ҫ�õ�PlayerDir��
		//��������ľͿ��������ˣ���Ϊ����������Ļ�һ���϶��Ǹ��ӷ��򣬶�������϶�����PlayerDir���������swithc(vec.back())��������
		switch (vec.back())
		{
		case enum_up:{
			Animation* animation = AnimationUtil::createWithSingleFrameName("uwalk", 0.2f, -1);
			Animate* animate = Animate::create(animation);
			if (PlayerState != enum_up){
				this->getPlayerSprite()->stopAllActions();
				this->getPlayerSprite()->runAction(animate);
			}
			PlayerState = enum_up;
			//PlayerDir������setPlayerPositionǰ�棬��Ϊ��setPlayerPosition����Ҫ�õ�PlayerDir
			PlayerDir = em_up;
			this->setPlayerPosition(this->getPosition() + Vec2(0, 4));
			break;
		}
		case enum_down:{
			Animation* animation = AnimationUtil::createWithSingleFrameName("dwalk", 0.2f, -1);
			Animate* animate = Animate::create(animation);
			if (PlayerState != enum_down){
				this->getPlayerSprite()->stopAllActions();
				this->getPlayerSprite()->runAction(animate);
			}
			PlayerState = enum_down;
			//PlayerDir������setPlayerPositionǰ�棬��Ϊ��setPlayerPosition����Ҫ�õ�PlayerDir
			PlayerDir = em_down;
			this->setPlayerPosition(this->getPosition() + Vec2(0, -4));
			break;
		}
		case enum_left:{
			Animation* animation = AnimationUtil::createWithSingleFrameName("hwalk", 0.2f, -1);
			Animate* animate = Animate::create(animation);
			if (PlayerState != enum_left){
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
		case enum_right:{
			Animation* animation = AnimationUtil::createWithSingleFrameName("hwalk", 0.2f, -1);
			Animate* animate = Animate::create(animation);
			if (PlayerState != enum_right){
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
		case enum_doubleup:{
			Animation* animation = AnimationUtil::createWithSingleFrameName("urun", 0.15f, -1);
			Animate* animate = Animate::create(animation);
			if (PlayerState != enum_doubleup){
				this->getPlayerSprite()->stopAllActions();
				this->getPlayerSprite()->runAction(animate);
			}
			PlayerState = enum_doubleup;
			//PlayerDir������setPlayerPositionǰ�棬��Ϊ��setPlayerPosition����Ҫ�õ�PlayerDir
			PlayerDir = em_up;
			this->setPlayerPosition(this->getPosition() + Vec2(0, 6));
			break;
		}
		case enum_doubledown:{
			Animation* animation = AnimationUtil::createWithSingleFrameName("drun", 0.18f, -1);
			Animate* animate = Animate::create(animation);
			if (PlayerState != enum_doubledown){
				this->getPlayerSprite()->stopAllActions();
				this->getPlayerSprite()->runAction(animate);
			}
			PlayerState = enum_doubledown;
			//PlayerDir������setPlayerPositionǰ�棬��Ϊ��setPlayerPosition����Ҫ�õ�PlayerDir
			PlayerDir = em_down;
			this->setPlayerPosition(this->getPosition() + Vec2(0, -6));
			break;
		}
		case enum_doubleleft:{
			Animation* animation = AnimationUtil::createWithSingleFrameName("hrun", 0.2f, -1);
			Animate* animate = Animate::create(animation);
			if (PlayerState != enum_doubleleft){
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
		case enum_doubleright:{
			Animation* animation = AnimationUtil::createWithSingleFrameName("hrun", 0.2f, -1);
			Animate* animate = Animate::create(animation);
			if (PlayerState != enum_doubleright){
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
		case em_up:{
			Animation* animation = AnimationUtil::createWithSingleFrameName("ustatic", 0.2f, -1);
			Animate* animate = Animate::create(animation);
			if (PlayerState != enum_static){
				this->getPlayerSprite()->stopAllActions();
				this->getPlayerSprite()->runAction(animate);
			}
			break;
		}
		case em_down:{
			Animation* animation = AnimationUtil::createWithSingleFrameName("dstatic", 0.2f, -1);
			Animate* animate = Animate::create(animation);
			if (PlayerState != enum_static){
				this->getPlayerSprite()->stopAllActions();
				this->getPlayerSprite()->runAction(animate);
			}
			break;
		}
		case em_left:{
			Animation* animation = AnimationUtil::createWithSingleFrameName("hstatic", 0.2f, -1);
			Animate* animate = Animate::create(animation);
			if (PlayerState != enum_static){
				this->getPlayerSprite()->stopAllActions();
				this->getPlayerSprite()->setScaleX(m_player_magnification);
				this->getPlayerSprite()->runAction(animate);
			}
			break;
		}
		case em_right:{
			Animation* animation = AnimationUtil::createWithSingleFrameName("hstatic", 0.2f, -1);
			Animate* animate = Animate::create(animation);
			if (PlayerState != enum_static){
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
		//��ʼ��ʱ
		timecounter_up->start(); //��һ��startʱcurtime��0
		//�ϳ�,��ֹ״̬�����ϳ幦��
		if (curtime > 0 && curtime - 0 < 0.4f && PlayerState == enum_static){
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
		if (curtime > 0 && curtime - 0 < 0.4f && PlayerState == enum_static){
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
		if (curtime > 0 && curtime - 0 < 0.4f && PlayerState == enum_static){
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
		if (curtime > 0 && curtime - 0 < 0.4f && PlayerState == enum_static){
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
		//����J���ܵĹ�����ȴʱ��
		float curtime = timecounter_J->getCurTime();
		//��һ�ΰ�Jʱ����curtime = 0,�˺�ÿ��0.3f����ܰ���һ�μ���
		if (curtime == 0 || curtime > 0.3f){
			timecounter_J->start();//һֱ��ʱ
			//sizeΪ0������ͨ�Ĺ���
			if (vecskill.size() == 0){
				if (PlayerState == enum_doubleup || PlayerState == enum_doubledown
					|| PlayerState == enum_doubleleft || PlayerState == enum_doubleright){
					vecskill.push_back(enum_basepoke);
					switch (PlayerState)//run״̬��һ֡ʹ��basepoke����ʱ��ʹvec[0](�����run״̬��vec[0]��Զ������run״̬)��Ϊwalk
					{
					case enum_doubleup:{
						vec[0] = enum_up;
						break;
					}
					case enum_doubledown:{
						vec[0] = enum_down;
						break;
					}
					case enum_doubleleft:{
						vec[0] = enum_left;
						break;
					}
					case enum_doubleright:{
						vec[0] = enum_right;
						break;
					}
					default:
						break;
					}
				}
				else{
					vecskill.push_back(enum_baseattack);
				}
			}
		}
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_K)
	{
		//����K���ܵĹ�����ȴʱ��//����
		float curtime = timecounter_J->getCurTime();
		//��һ�ΰ�Jʱ����curtime = 0,�˺�ÿ��0.5f����ܰ���һ�μ���
		if (curtime == 0 || curtime > 0.5f){
			timecounter_J->start();//һֱ��ʱ
			//sizeΪ0���н���
			if (vecskill.size() == 0){
				if (PlayerState == enum_doubleup || PlayerState == enum_doubledown
					|| PlayerState == enum_doubleleft || PlayerState == enum_doubleright){
					vecskill.push_back(enum_swordwave);
					switch (PlayerState)//run״̬��һ֡ʹ��swordwave����ʱ��ʹvec[0](�����run״̬��vec[0]��Զ������run״̬)��Ϊwalk
					{
					case enum_doubleup:{
						vec[0] = enum_up;
						break;
					}
					case enum_doubledown:{
						vec[0] = enum_down;
						break;
					}
					case enum_doubleleft:{
						vec[0] = enum_left;
						break;
					}
					case enum_doubleright:{
						vec[0] = enum_right;
						break;
					}
					default:
						break;
					}
				}
				else{
					vecskill.push_back(enum_swordwave);//walk״̬Ҳ��swordwave����
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
	switch (PlayerDir){
	case em_up:{
		position.y += 2;//���ǽŵĴ�С

		//run����walk���ӵķ�����ȡ����vec.back(),���Ҹ��ӵķ�����ö��enum_up,enum_left,enum_down,enum_right
		if (vec.size() > 1){
			switch (vec.back())
			{
			case enum_left:{
				position.x -= this->getContentSize().width / 2;
				break;
			}
			case enum_right:{
				position.x += this->getContentSize().width / 2;
				break;
			}
			default:{
				break;
			}
			}
		}
		//�������Ϸ���walk or run��Ϊ����ê����0.5 0.5������ֻ�������ߣ�����������Ŀ�ȣ����ԼӶ����ҿ�ȵ��ж�
		else{
			positionVleft = position;
			positionVleft.x -= this->getContentSize().width / 2;
			positionVright = position;
			positionVright.x += this->getContentSize().width / 2;
		}

		break;
	}
	case em_down:{

		//run����walk���ӵķ�����ȡ����vec.back(),���Ҹ��ӵķ�����ö��enum_up,enum_left,enum_down,enum_right
		if (vec.size() > 1){
			switch (vec.back())
			{
			case enum_left:{
				position.x -= this->getContentSize().width / 2;
				break;
			}
			case enum_right:{
				position.x += this->getContentSize().width / 2;
				break;
			}
			default:{
				break;
			}
			}
		}
		//�������Ϸ���walk or run��Ϊ����ê����0.5 0.5������ֻ�������ߣ�����������Ŀ�ȣ����ԼӶ����ҿ�ȵ��ж�
		else{
			positionVleft = position;
			positionVleft.x -= this->getContentSize().width / 2;
			positionVright = position;
			positionVright.x += this->getContentSize().width / 2;
		}

		break;
	}
	case em_left:{
		position.x -= this->getContentSize().width / 2;
		//run����walk���ӵķ�����ȡ����vec.back(),���Ҹ��ӵķ�����ö��enum_up,enum_left,enum_down,enum_right
		if (vec.size() > 1){
			switch (vec.back())
			{
			case enum_up:{
				position.y += 2;//��������Ь�ӵĴ�С
				break;
			}
			case enum_down:{

				break;
			}
			default:
				break;
			}
		}
		break;
	}
	case em_right:{
		position.x += this->getContentSize().width / 2;//����������
		//run����walk���ӵķ�����ȡ����vec.back(),���Ҹ��ӵķ�����ö��enum_up,enum_left,enum_down,enum_right
		if (vec.size() > 1){
			switch (vec.back())
			{
			case enum_up:{
				position.y += 2;//��������Ь�ӵĴ�С
				break;
			}
			case enum_down:{

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
	case em_up:{
		Vec2 tileCoord = this->tiledCoordForPosition(positionVleft);

		if (tileCoord.x >= 0 && tileCoord.x < m_map->getMapSize().width //��������Ƭ��ͼ����
			&& tileCoord.y >= 0 && tileCoord.y < m_map->getMapSize().height){
			int tileGid = m_map->getLayer("barrier")->getTileGIDAt(tileCoord);
			if (tileGid > 0){
				Value prop = m_map->getPropertiesForGID(tileGid);
				ValueMap proValueMap = prop.asValueMap();

				if (proValueMap.find("Collidable") != proValueMap.end()){
					std::string collision = proValueMap.at("Collidable").asString();
					if (collision == "true"){
						return false;
					}
				}
			}
		}
		break;
	}
	case em_down:{
		Vec2 tileCoord = this->tiledCoordForPosition(positionVright);

		if (tileCoord.x >= 0 && tileCoord.x < m_map->getMapSize().width //��������Ƭ��ͼ����
			&& tileCoord.y >= 0 && tileCoord.y < m_map->getMapSize().height){
			int tileGid = m_map->getLayer("barrier")->getTileGIDAt(tileCoord);
			if (tileGid > 0){
				Value prop = m_map->getPropertiesForGID(tileGid);
				ValueMap proValueMap = prop.asValueMap();

				if (proValueMap.find("Collidable") != proValueMap.end()){
					std::string collision = proValueMap.at("Collidable").asString();
					if (collision == "true"){
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
		&& tileCoord.y >= 0 && tileCoord.y < m_map->getMapSize().height){
		int tileGid = m_map->getLayer("barrier")->getTileGIDAt(tileCoord);
		if (tileGid > 0){
			Value prop = m_map->getPropertiesForGID(tileGid);
			ValueMap proValueMap = prop.asValueMap();

			if (proValueMap.find("Collidable") != proValueMap.end()){
				std::string collision = proValueMap.at("Collidable").asString();
				if (collision == "true"){
					return false;
				}
			}
		}
	}

	//��ԭposition
	switch (PlayerDir){
	case em_up:{
		position.y -= 2;//���ǽŵĴ�С

		//run����walk���ӵķ�����ȡ����vec.back(),���Ҹ��ӵķ�����ö��enum_up,enum_left,enum_down,enum_right
		if (vec.size() > 1){
			switch (vec.back())
			{
			case enum_left:{
				position.x += this->getContentSize().width / 2;
				break;
			}
			case enum_right:{
				position.x -= this->getContentSize().width / 2;
				break;
			}
			default:
				break;
			}
		}
		break;
	}
	case em_down:{

		//run����walk���ӵķ�����ȡ����vec.back(),���Ҹ��ӵķ�����ö��enum_up,enum_left,enum_down,enum_right
		if (vec.size() > 1){
			switch (vec.back())
			{
			case enum_left:{
				position.x += this->getContentSize().width / 2;
				break;
			}
			case enum_right:{
				position.x -= this->getContentSize().width / 2;
				break;
			}
			default:
				break;
			}
		}
		break;
	}
	case em_left:{
		position.x += this->getContentSize().width / 2;
		//run����walk���ӵķ�����ȡ����vec.back(),���Ҹ��ӵķ�����ö��enum_up,enum_left,enum_down,enum_right
		if (vec.size() > 1){
			switch (vec.back())
			{
			case enum_up:{
				position.y -= 2;//��������Ь�ӵĴ�С
				break;
			}
			case enum_down:{

				break;
			}
			default:
				break;
			}
		}
		break;
	}
	case em_right:{
		position.x -= this->getContentSize().width / 2;//����������
		//run����walk���ӵķ�����ȡ����vec.back(),���Ҹ��ӵķ�����ö��enum_up,enum_left,enum_down,enum_right
		if (vec.size() > 1){
			switch (vec.back())
			{
			case enum_up:{
				position.y -= 2;//��������Ь�ӵĴ�С
				break;
			}
			case enum_down:{

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
		&& tileCoord.y >= 0 && tileCoord.y < m_map->getMapSize().height){
		int tileGid = m_map->getLayer("barrier")->getTileGIDAt(tileCoord);
		if (tileGid > 0){
			Value prop = m_map->getPropertiesForGID(tileGid);
			ValueMap proValueMap = prop.asValueMap();

			if (proValueMap.find("Collidable") != proValueMap.end()){
				std::string collision = proValueMap.at("Collidable").asString();
				if (collision == "true"){
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
	if (vecskill.size() == 1){
		vecskill.erase(vecskill.begin());
	}
}

void Player::createSwordWave()
{
	RemoteSkill* swordwave = NULL;
	for (int i = 0; i < swordwaveNum; i++)
	{
		swordwave = RemoteSkill::create();
		swordwave->bindSprite(Sprite::create("remoteskills/playerskill1.png"));
		//���ý���swordwave��Node�ڵ��ê��Ϊ0.5 0.5;
		swordwave->setAnchorPoint(Vec2(0.5,0.5));
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