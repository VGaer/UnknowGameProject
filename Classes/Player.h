#ifndef __Player_H__
#define __Player_H__

#include "cocos2d.h"
#include "Entity.h"
#include "TimeCounter.h"
#include "AnimationUtil.h"
#include "SimpleAudioEngine.h"
#include "RemoteSkill.h"

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
	enum_swordwave //剑气
}Skill;

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
	bool b;//单段攻击判断完成标志,true代表已判断
	baseskillstr(int ii, bool bb) : i(ii), b(bb){}
};

class Player : public Entity
{
public:
	//CREATE_FUNC(Player);
	static Player* createWithparent(TMXTiledMap* parent);
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
private:
	bool IsNot_CollidableTile(Vec2 tieldCoord); //判断barrier层上的瓦片块是否是拥有Collidable属性
	TMXTiledMap* m_map;
	std::vector<int> vec;
	std::vector<int> vecskill;
	std::vector<baseskillstr> vecskillstr;
	TimeCounter* timecounter_up;
	TimeCounter* timecounter_down;
	TimeCounter* timecounter_left;
	TimeCounter* timecounter_right;
	TimeCounter* timecounter_J;
	int PlayerState;
	int PlayerDir;
	Vector<RemoteSkill*> m_swordwave_Arr;
	Vector<RemoteSkill*> m_Using_swordwave_Arr;
	void createSwordWave();
	int swordwaveNum;
	TMXTiledMap* m_parrent;
	int m_player_magnification;//玩家精灵放大倍数
};
#endif 
