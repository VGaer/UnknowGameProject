#ifndef __ChangeScenePointManager_H__
#define __ChangeScenePointManager_H__

#include <vector>

class ChangeScenePointManager
{
public:
	static ChangeScenePointManager* getInstance();
	//�������
	bool IsReachCondition(int newSceneId);
private:
	//�����л������������Ƿ�ﵽ�ȼ�Ҫ��
	bool IsReachLevel(int newSceneId);
	//�����л������������Ƿ������������
	bool IsReachMission(int newSceneId);
};

#endif