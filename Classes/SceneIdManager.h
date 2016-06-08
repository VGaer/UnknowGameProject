#ifndef __SceneIdManager_H__
#define __SceneIdManager_H__

#include <map>
#include <vector>

#define Scene_DATA_PATH "JsonText/SceneId.json"	

struct SceneData
{
	std::string name;
	int Id;
	int Level;//主角要求的等级
	std::vector<int> Misson;//要求的任务完成清单
	std::vector<int> MissonActive;//要求的任务active(接受)清单
};


class SceneIdManager
{
public:
	static SceneIdManager*  getInstance();
	SceneIdManager();
	~SceneIdManager();
public:
	std::map<std::string, SceneData> map_scenenameToSceneData;
	std::map<int, SceneData>  map_sceneIdToSceneData;
public:
	// 加入一条场景数据
	void addDataToSceneData(SceneData scenedata);
private:
	
	// 读取文件数据
	void readSceneDataFile();
};

#endif