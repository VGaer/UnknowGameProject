#ifndef _GameData_H_
#define _GameData_H_

#include "string"
#include "map"
#include "vector"
using namespace std;

#define MONS_DATA_PATH "JsonText/monattribute.json"	// 怪物数据文件路径
#define NPCS_DATA_PATH "JsonText/NpcDlgs.json"	// NPC数据文件路径
#define QUESTS_DATA_PATH "JsonText/QuestList.json"	// 任务数据文件路径
#define QDLGS_DATA_PATH "JsonText/QuestDlgs.json"	// 任务对话数据文件路径

struct NpcsData {
	int id;				// id
	vector<int> quest_id;		//任务id
	string name;		// 名字
	vector<string> dlgs;		// 对话s
	string imagePath;	// 图片路径
};

struct MonsData
{
	string name;	   // 名字
	float hp;		   // 生命值
	float damage;	   // 攻击力
	float moveSpeed;   // 行走一条边的时间
	float attackInter; // 攻击间隔
	float attackRange; // 攻击范围
	float eyeRange;	   // 视野半径，
	float patrolRange; // 巡逻半径，
	float perceptionRange; //感知半径
	float attackedrestoretimes; //被攻击恢复时间 //恢复时间约快说明硬直越高
	float beforeattacktimes; //前摇时间
	float attackAnimatetimePer; //攻击动画播放时间
	string imagePath;  // 图片路径
};


struct QuestListData {
	int id;			//任务id
	string title;	//任务标题
	string instruct; //任务简介
	int type;		//任务类型
	int status;		//激活状态 （0：未接、1：激活、2：完成）
	string mons_id;	//任务目标（如果有）
};

struct QuestDlgsData {
	int id;			//对话编号
	string start;	//任务开始前对话
	string active;	//任务激活后对话
	string finish;	//任务完成后对话
	string answer;	//目标NPC回复（如果有）
};

class GameData
{
public:
	// 数据初始化
	static GameData* getInstance();
	GameData();
	~GameData();
	// 基础数据存放
	map<string, MonsData*> m_mapMonsData;
	map<string, NpcsData*> m_mapNpcsData;
	map<int, QuestListData*> m_mapQuestsData;
	map<int, QuestDlgsData*> m_mapQuestDlgsData;
public:
	// 加入一条怪物数据
	void addDataToMonsData(MonsData* data);
	// 获取一条怪物数据
	const MonsData* getDataFromMonsData(const string& name);
	// 加入一条NPC数据
	void addDataToNpcsData(NpcsData* data);
	// 获取一条NPC数据
	const NpcsData* getDataFromNpcsData(const string& name);
	// 加入一条任务数据
	void addDataToQuestsData(QuestListData* data);
	// 获取一条任务数据
	const QuestListData* getDataFromQuestsData(const int id);
	// 加入一条任务对话数据
	void addDataToQuestDlgsData(QuestDlgsData* data);
	// 获取一条任务对话数据
	const QuestDlgsData* getDataFromQuestDlgsData(const int id);
private:
	// 读取文件数据
	void readMonsDataFile();
	void readNpcsDataFile();
	void readQuestsDataFile();
	void readQuestDlgsDataFile();
};

#endif