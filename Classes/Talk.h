#pragma once
#include "cocos2d.h"
#include "vector"
USING_NS_CC;

using namespace std;

enum TalkContentType
{
	Talk_AcTask,
	Talk_CmTask,
	Talk_FiTask,
	Talk_EtScene
};

class Talk: public Node
{
public:
	static Talk* create(vector<string>strVec, int id, int idType);	
	bool init(vector<string>strVec, int id, int idType);
	void setSceneLockState(bool);
private:
	int id;
	int idType;
	int curPage;
	int curTextLen;	
	bool isShowFinish;
	Label* text;
	Sprite* bg;    // ±³¾°¿ò
	vector<string> strVec;
	EventListenerTouchOneByOne* listener;
private:
	void showFont(float dt);
	void addTouchEventListener();
	void setDlgsIsSaid();
	void setPos();
	void opPage();
	void keyPressed(EventKeyboard::KeyCode keyCode, Event* event);	
};
