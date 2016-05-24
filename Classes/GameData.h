#ifndef _GameData_H_
#define _GameData_H_

#include "string"
#include "map"
#include <vector>
using namespace std;

#define MONS_DATA_PATH "monsterset.json"

struct MonSkill
{
	string skilltype; //技能类型
	float attackRange; //攻击范围
	float beforeattacktimes;	//攻击前摇时间
	float attackAnimatetimePer; //攻击每帧动画时间
	float attackInter;    //攻击间隔
	float attackEnd; //攻击动画结束时间
	bool Isinter;		  //是否在前摇时可以中断
	float damage;		//攻击力
	string hAnimatePath; //水平攻击动画
	string dAnimatePath; //下攻击动画
	string uAnimatePath; //上攻击动画
};

struct MonsData
{
	string name;		//名字
	float hp;			//生命值
	float moveSpeed;	//行走一条边的时间
	float eyeRange;		//视野半径
	float perceptionRange;//感知半径
	float attackedrestoretime;//僵直时间
	string imagePath; //图片路径
	std::map<string,MonSkill> skillmap; //技能容器
};

class GameData
{
public:
	//数据初始化
	static GameData* getInstance();
	GameData();
	~GameData();
	//基础数据存放
	map<string, MonsData*> m_mapMonsData;
public:
	void addDataToMonsData(MonsData* data);
	const MonsData* getDataFromMonsData(const string& name);
private:
	//读取文件数据
	void readMonsDataFile();
};

#endif