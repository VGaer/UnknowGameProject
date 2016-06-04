#ifndef __ChangeScenePointManager_H__
#define __ChangeScenePointManager_H__

#include <vector>

class ChangeScenePointManager
{
public:
	static ChangeScenePointManager* getInstance();
	//达成条件
	bool IsReachCondition(int newSceneId);
private:
	//场景切换触发点主角是否达到等级要求
	bool IsReachLevel(int newSceneId);
	//场景切换触发点主角是否完成所需任务
	bool IsReachMission(int newSceneId);
};

#endif