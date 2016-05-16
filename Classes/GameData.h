#ifndef _GameData_H_
#define _GameData_H_

#include "string"
#include "map"
using namespace std;

struct MonsData
{
	string name;	   // ����
	float hp;		   // ����ֵ
	float damage;	   // ������
	float moveSpeed;   // ����һ���ߵ�ʱ��
	float attackInter; // �������
	float attackRange; // ������Χ
};

class GameData
{
public:
	// ���ݳ�ʼ��
	GameData();
	~GameData();
	// �������ݴ��
	typedef map<string, MonsData> MonsData_t;
	static MonsData_t m_mapMonsData;
public:
	// ����һ����������
	static void addDataToMonsData(const MonsData& data);
	// ��ȡһ����������
	static const MonsData* getDataFromMonsData(const string& name);
};

#endif