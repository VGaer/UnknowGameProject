#pragma once
#include "cocos2d.h"  
#include "ui\UIScale9Sprite.h" 
#include "string"
#include "PopLayer.h"
#include "map"
#include "QuestList.h"
USING_NS_CC;
using namespace ui;
using namespace std;

struct pop {
	Layer* layer;
	bool isPop = false;
	~pop() {
		layer->removeFromParent();
	}
};

class PopManager : public Node{
public:
	static PopManager* getInstance();
	PopManager();
	~PopManager() {}
	//添加菜单层
	void addLayer(int tag, Layer* sender, bool isPop);
	pop* getLayerByTag(int tag);
	void releaseLayer(int tag);
	//弹出标志
	bool getPopped(int tag);
	void setPopped(int tag, bool isPop);
private:
	map<int, pop*> layer;
};