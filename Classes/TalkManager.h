#ifndef __TalkManager_H__
#define __TalkManager_H__

#include "GameData.h"

class TalkManager
{
public:
	static TalkManager* getInstance();
	TalkManager();
	map<int, PlayerTaskDlgsData*> m_mapCurPlayerTaskDlgs;
	map<int, EnterSceneDlgsData*> m_mapCurEnterSceneDlgs;
	void initCurPlayerTaskDlgs();
	void initCurEnterSceneDlgs();
	PlayerTaskDlgsData* getDataFromPlayerTaskDlgsData(int id);
	EnterSceneDlgsData* getDataFromEnterSceneDlgsData(int id);
	void InitTalkForPlayerDied();
	
};

#endif