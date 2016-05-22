#ifndef _GameData_H_
#define _GameData_H_

#include "string"
#include "map"
#include "vector"
using namespace std;

#define MONS_DATA_PATH "JsonText/monattribute.json"	// ���������ļ�·��
#define NPCS_DATA_PATH "JsonText/NpcDlgs.json"	// NPC�����ļ�·��
#define QUESTS_DATA_PATH "JsonText/QuestList.json"	// ���������ļ�·��
#define QDLGS_DATA_PATH "JsonText/QuestDlgs.json"	// ����Ի������ļ�·��

struct NpcsData {
	int id;				// id
	vector<int> quest_id;		//����id
	string name;		// ����
	vector<string> dlgs;		// �Ի�s
	string imagePath;	// ͼƬ·��
};

struct MonsData
{
	string name;	   // ����
	float hp;		   // ����ֵ
	float damage;	   // ������
	float moveSpeed;   // ����һ���ߵ�ʱ��
	float attackInter; // �������
	float attackRange; // ������Χ
	float eyeRange;	   // ��Ұ�뾶��
	float patrolRange; // Ѳ�߰뾶��
	float perceptionRange; //��֪�뾶
	float attackedrestoretimes; //�������ָ�ʱ�� //�ָ�ʱ��Լ��˵��ӲֱԽ��
	float beforeattacktimes; //ǰҡʱ��
	float attackAnimatetimePer; //������������ʱ��
	string imagePath;  // ͼƬ·��
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
	const MonsData* getDataFromMonsData(const string& name);
	// ����һ��NPC����
	void addDataToNpcsData(NpcsData* data);
	// ��ȡһ��NPC����
	const NpcsData* getDataFromNpcsData(const string& name);
	// ����һ����������
	void addDataToQuestsData(QuestListData* data);
	// ��ȡһ����������
	const QuestListData* getDataFromQuestsData(const int id);
	// ����һ������Ի�����
	void addDataToQuestDlgsData(QuestDlgsData* data);
	// ��ȡһ������Ի�����
	const QuestDlgsData* getDataFromQuestDlgsData(const int id);
private:
	// ��ȡ�ļ�����
	void readMonsDataFile();
	void readNpcsDataFile();
	void readQuestsDataFile();
	void readQuestDlgsDataFile();
};

#endif