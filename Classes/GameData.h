#ifndef _GameData_H_
#define _GameData_H_

#include "string"
#include "map"
#include "vector"
using namespace std;

//#define MONS_DATA_PATH "JsonText/monattribute.json"	// ���������ļ�·��
#define MONS_DATA_PATH "JsonText/monsterset.json"	// ����skill�ļ�·��
#define NPCS_DATA_PATH "JsonText/NpcDlgs.json"	// NPC�����ļ�·��
#define QUESTS_DATA_PATH "JsonText/QuestList.json"	// ���������ļ�·��
#define QDLGS_DATA_PATH "JsonText/QuestDlgs.json"	// ����Ի������ļ�·��
#define PLAYER_DATA_PATH "JsonText/PlayerData.json" // ��������ļ�·��

struct NpcsData {
	int id;				// id
	vector<int> quest_id;		//����id
	string name;		// ����
	vector<string> dlgs;		// �Ի�s
	string imagePath;	// ͼƬ·��
};

struct MonSkill
{
	string skilltype; //��������
	float attackRange; //������Χ
	float beforeattacktimes;	//����ǰҡʱ��
	float attackAnimatetimePer; //����ÿ֡����ʱ��
	float attackInter;    //�������
	float attackEnd; //������������ʱ��
	bool Isinter;		  //�Ƿ���ǰҡʱ�����ж�
	float damage;		//������
	int attackNums;   //��������
	float attackNumsInter; //��ι����ļ��ʱ��(���ι�����ʱ���ż��ʱ��Ϊ����ǰҡ����������ʱ��,���ʱʱ�䰲��Ϊ��������ʱ��/����)
	string hAnimateName; //ˮƽ������������
	string dAnimateName; //�¹�����������
	string uAnimateName; //�Ϲ�����������
};

struct MonRemoteSkill
{
	string skilltype; //��������
	string projectileName;
	string projectileAnimateName;
	float skillwidth; //Ͷ�����ܿ��
	float skillheight;//Ͷ�����ܸ߶�
	float projmovespeed;//Ͷ�������ƶ��ٶ�
	float eyeRangeForstartskill; //�������������������
	float beforeattacktimes;	//����ǰҡʱ��
	float attackAnimatetimePer; //����ÿ֡����ʱ��
	float attackInter;    //�������
	float attackEnd; //������������ʱ��
	bool Isinter;		  //�Ƿ���ǰҡʱ�����ж�
	float damage;		//������
	float duration; //�ͷų����ļ��ܳ���ʱ��
	string hAnimateName; //ˮƽ������������
	string dAnimateName; //�¹�����������
	string uAnimateName; //�Ϲ�����������
};

struct MonsData
{
	string name;		//����
	float hp;			//����ֵ
	float moveSpeed;	//����һ���ߵ�ʱ��
	float eyeRange;		//��Ұ�뾶
	float perceptionRange;//��֪�뾶
	float attackedrestoretime;//��ֱʱ��
	string imagePath; //ͼƬ·��
	std::map<string, MonSkill> skillmap; //��������
	std::map<string, MonRemoteSkill> remoteskillmap; //Զ�̼�������
};


struct QuestListData {
	int id;			//����id
	string title;	//�������
	string instruct; //������
	int type;		//��������
	int status;		//����״̬ ��0��δ�ӡ�1�����2����ɣ�
	string mons_id;	//����Ŀ�꣨����У�
};

struct QuestDlgsData {
	int id;			//�Ի����
	string start;	//����ʼǰ�Ի�
	string active;	//���񼤻��Ի�
	string finish;	//������ɺ�Ի�
	string answer;	//Ŀ��NPC�ظ�������У�
};

struct PlayerData
{
	int sceneId;
	int direction;
	float posX;
	float posY;
	float level;
	float hp;
	float mp;
};

class GameData
{
public:
	// ���ݳ�ʼ��
	static GameData* getInstance();
	GameData();
	~GameData();
	// �������ݴ��
	map<string, MonsData*> m_mapMonsData;
	map<string, NpcsData*> m_mapNpcsData;
	map<int, QuestListData*> m_mapQuestsData;
	map<int, QuestDlgsData*> m_mapQuestDlgsData;
public:
	// ����һ����������
	void addDataToMonsData(MonsData* data);
	// ��ȡһ����������
	MonsData* getDataFromMonsData(const string& name);
	// ����һ��NPC����
	void addDataToNpcsData(NpcsData* data);
	// ��ȡһ��NPC����
	NpcsData* getDataFromNpcsData(const string& name);
	// ����һ����������
	void addDataToQuestsData(QuestListData* data);
	// ��ȡһ����������
	QuestListData* getDataFromQuestsData(const int id);
	// ����һ������Ի�����
	void addDataToQuestDlgsData(QuestDlgsData* data);
	// ��ȡһ������Ի�����
	QuestDlgsData* getDataFromQuestDlgsData(const int id);

	// �����������
	void writePlayerData();
	// ��ȡ�������
	PlayerData* getPlayerData();
	// �Ƿ��д浵
	bool isExistSaveDoc();
private:
	// ��ȡ�ļ�����
	void readMonsDataFile();
	void readNpcsDataFile();
	void readQuestsDataFile();
	void readQuestDlgsDataFile();
	void readPlayerDataFile();
private:
	PlayerData* playerData;
};

#endif