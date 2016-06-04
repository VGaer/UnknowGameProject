#ifndef __Player_H__
#define __Player_H__

#include "cocos2d.h"
#include "Entity.h"
#include "TimeCounter.h"
#include "AnimationUtil.h"
#include "SimpleAudioEngine.h"
#include "RemoteSkill.h"
#include "SkillControl.h"
#include <queue>
#include "MonsterManager.h"
#include "Monster.h"
#include "BarManager.h"

USING_NS_CC;
typedef enum
{
	enum_up = 100,
	enum_down,
	enum_left,
	enum_right,
	enum_static,
	enum_doubleup,
	enum_doubledown,
	enum_doubleleft,
	enum_doubleright,
	enum_initNone
}MoveDir;

typedef enum
{
	enum_baseattack = 200,
	enum_basepoke,
	enum_swordwave, //����
	enum_laserskill,//�׵�
	enum_fireskill, //����
}playerskill;

typedef enum
{
	em_up = 300,
	em_down,
	em_left,
	em_right
}FourDir;

struct baseskillstr
{
	int i;//���ܱ��
	bool b;//������ͨ�����ж���ɱ�־,true�������ж�
	baseskillstr(int ii, bool bb) : i(ii), b(bb) {}
};

typedef enum
{
	enum_playerattackedfromleft = 1,
	enum_playerattackedfromright,
	enum_playerattackedfromup,
	enum_playerattackedfromdown
};

class SkillControl;
class Monster;

class Player : public Entity
{
public:
	std::queue<int> attackedqueue;
	//CREATE_FUNC(Player);
	static Player* getInstance();
	//static Player* createWithparent(TMXTiledMap* parent);
	virtual bool init();
	virtual void update(float dt);
	void keyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void keyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	void setTiledMap(TMXTiledMap* map);
	Vec2 tiledCoordForPosition(Vec2 pos);
	bool setPlayerPosition(Vec2 position);
	bool IsSussesfulForsetRemoteSkillPos(Vec2 position);
	Sprite* getPlayerSprite();
	void CallBack1();
	void setPlayerparent(TMXTiledMap* layerparent);
	int getPlayer_magnification();
	Vector<RemoteSkill*> getPlayerUsing_swordwave_Arr();
	std::vector<int> getVecSkill();
	std::vector<baseskillstr>& getvecskillstr(); //��������
	int getPlayerDir();
	void setPlayerDir(int direction);
	bool playerIsattacked;//�����Ƿ񱻹���
	bool IsNot_CollidableTile(Vec2 tieldCoord); //�ж�barrier���ϵ���Ƭ���Ƿ���ӵ��Collidable����
	float m_hp;//hp
	float m_mp;//mp;
	void setPlayer_hp(float hp);
	void setPlayer_mp(float mp);
	void Playerhp_mp_Update(float dt);
	void recoverHp_Mp(float dt);
	float getCurMaxHp();
	float getCurMaxMp();
	void ChangSceneIdUpdate(float dt);
	void openAllUpdate();
	void createSwordWave();
	int m_playerlevel; //���ǵȼ�
	float m_exp; //���Ǿ���
	void LevelUpdate(float dt);
private:
	TMXTiledMap* m_map;
	std::vector<int> vec;
	std::vector<int> vecskill;
	std::vector<baseskillstr> vecskillstr;
	TimeCounter* timecounter_up;
	TimeCounter* timecounter_down;
	TimeCounter* timecounter_left;
	TimeCounter* timecounter_right;
	TimeCounter* timecounter_J;
	TimeCounter* timecounter_attacked;//�����������ļ�ʱ
	int PlayerState;
	int PlayerDir;
	Vector<RemoteSkill*> m_swordwave_Arr;
	Vector<RemoteSkill*> m_Using_swordwave_Arr;
	int swordwaveNum;
	TMXTiledMap* m_parrent;
	int m_player_magnification;//��Ҿ���Ŵ���	
	Color3B m_playerColor;
	SkillControl* skillControl;
	Vector<Monster*> collidedVector;	// ��ײ�嵥
	void baseskillcollidUpdata(float dt);
	//���ܶ�Ӧ����mp
	int l_consumemp;
	int k_consumemp;
	int u_consumemp;
	//��ǰ�ȼ�hp,mp����
	float curLevel_Maxhp;
	float curLevel_Maxmp;
	//��������ʱ���Ŷ����ľ���
	Sprite* spritelevelup;
};
#endif 
