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
	bool playerIsattacked;//�����Ƿ񱻹���
	bool IsNot_CollidableTile(Vec2 tieldCoord); //�ж�barrier���ϵ���Ƭ���Ƿ���ӵ��Collidable����
	int m_hp;
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
	void createSwordWave();
	int swordwaveNum;
	TMXTiledMap* m_parrent;
	int m_player_magnification;//��Ҿ���Ŵ���	
	Color3B m_playerColor;
	SkillControl* skillControl;
};
#endif 
