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
	enum_swordwave, //剑气
	enum_laserskill,//雷电
	enum_fireskill, //火球
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
	int i;//技能编号
	bool b;//单段普通攻击判断完成标志,true代表已判断
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
	std::vector<baseskillstr>& getvecskillstr(); //返回引用
	int getPlayerDir();
	void setPlayerDir(int direction);
	bool playerIsattacked;//主角是否被攻击
	bool IsNot_CollidableTile(Vec2 tieldCoord); //判断barrier层上的瓦片块是否是拥有Collidable属性
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
	int m_playerlevel; //主角等级
	float m_exp; //主角经验
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
	TimeCounter* timecounter_attacked;//用来被攻击的计时
	int PlayerState;
	int PlayerDir;
	Vector<RemoteSkill*> m_swordwave_Arr;
	Vector<RemoteSkill*> m_Using_swordwave_Arr;
	int swordwaveNum;
	TMXTiledMap* m_parrent;
	int m_player_magnification;//玩家精灵放大倍数	
	Color3B m_playerColor;
	SkillControl* skillControl;
	Vector<Monster*> collidedVector;	// 碰撞清单
	void baseskillcollidUpdata(float dt);
	//技能对应消耗mp
	int l_consumemp;
	int k_consumemp;
	int u_consumemp;
	//当前等级hp,mp属性
	float curLevel_Maxhp;
	float curLevel_Maxmp;
	//主角升级时播放动画的精灵
	Sprite* spritelevelup;
};
#endif 
