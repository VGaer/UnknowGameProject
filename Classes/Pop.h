#pragma once
#include "cocos2d.h"  
#include "ui\UIScale9Sprite.h" 
#include "string"
#include "PopLayer.h"
#include "map"
#include "vector"
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

class Pop : public Node{
public:
	static Pop* create(Vec2 sender);
	bool init(Vec2 sender);
	//��Ӳ˵���
	void addLayer(int tag, Layer* sender, bool isPop);
	pop* getLayerByTag(int tag);
	void releaseLayer(int tag);
	//������־
	bool getPopped(int tag);
	void setPopped(int tag, bool isPop);
private:
	map<int, pop*> layer;
};